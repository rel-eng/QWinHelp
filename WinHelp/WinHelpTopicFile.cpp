/* WinHelp topic file class implementation.

   Copyright (C) 2010 rel-eng

   This file is part of QWinHelp.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "WinHelpTopicFile.h"

#include "InternalDirectoryFileHeader.h"

#include "TopicHeader.h"
#include "TopicHeaderOld.h"
#include "TopicHeaderNew.h"
#include "DisplayableText.h"
#include "DisplayableTextOld.h"
#include "DisplayableTextNew.h"
#include "DisplayableTable.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/CompressionUtils.h"

#include <QScopedArrayPointer>
#include <QScopedPointer>
#include <QtDebug>

#include <stdexcept>

WinHelpTopicFile::WinHelpTopicFile() : linkPointers(), linkBlocks(),
    rawDescriptors()
{
    PRINT_DBG("WinHelp Topic file default constructor");
}

WinHelpTopicFile::WinHelpTopicFile(QFile &file,
    qint64 off,
    QTextCodec *codec,
    quint16 minor,
    quint16 flags) : linkPointers(), linkBlocks(), rawDescriptors()
{
    PRINT_DBG("Loading WinHelp Topic file at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    bool isHC30 = false;
    if(minor <= 16)
    {
        isHC30 = true;
    }
    bool isW95 = false;
    if((minor == 33) || (minor == 21) || (minor == 27))
    {
        isW95 = true;
    }
    bool decompressTopicBlock = false;
    qint64 topicBlockSize = Q_INT64_C(0);
    if(minor <= 16)
    {
        topicBlockSize = Q_INT64_C(2048);
        decompressTopicBlock = false;
    }
    else
    {
        if(flags == 0)
        {
            topicBlockSize = Q_INT64_C(4096);
            decompressTopicBlock = false;
        }
        else
        {
            if(flags == 4)
            {
                topicBlockSize = Q_INT64_C(4096);
                decompressTopicBlock = true;
            }
            else
            {
                if(flags == 8)
                {
                    topicBlockSize = Q_INT64_C(2048);
                    decompressTopicBlock = true;
                }
                else
                {
                    throw std::runtime_error(
                        "Unknown |TOPIC file compression method");
                }
            }
        }
    }
    qint64 blockDecompressSize = Q_INT64_C(0);
    if(decompressTopicBlock)
    {
        blockDecompressSize = Q_INT64_C(16384);
    }
    else
    {
        if(!isW95)
        {
            blockDecompressSize = topicBlockSize - TopicBlockHeader::size;
        }
        else
        {
            blockDecompressSize = Q_INT64_C(16384);
        }
    }
    qint64 firstTopicBlockOffset = off + InternalDirectoryFileHeader::size;
    qint64 maxOffset = firstTopicBlockOffset + hdr.getUsedSpace();
    TopicPos src(Q_INT64_C(0), Q_INT64_C(
            0), TopicBlockHeader::size, blockDecompressSize);
    TopicBlockCache cache(5);
    while(!src.isInvalid())
    {
        TopicLinkHeader topicLinkHeader = this->parseTopicLinkUncompressed(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            TopicBlockHeader::size,
            blockDecompressSize,
            src,
            cache,
            isHC30,
            codec,
            isW95);
        if(isHC30)
        {
            if(!topicLinkHeader.getNextBlock().isInvalid())
            {
                src = src + topicLinkHeader.getNextBlock();
            }
            else
            {
                src = topicLinkHeader.getNextBlock();
            }
        }
        else
        {
            src = topicLinkHeader.getNextBlock();
        }
    }
    for(int i = 0; i < rawDescriptors.count(); i++)
    {
        rawDescriptors.at(i)->transformToHtml(i,
            rawDescriptors.count(), linkPointers);
    }
    PRINT_DBG("WinHelp Topic file loaded successfully");
}

WinHelpTopicFile::WinHelpTopicFile(QFile &file,
    qint64 off,
    QTextCodec *codec,
    const WinHelpPhraseFile &phrases,
    quint16 minor,
    quint16 flags) : linkPointers(), linkBlocks(), rawDescriptors()
{
    PRINT_DBG("Loading WinHelp Topic file at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    bool isHC30 = false;
    if(minor <= 16)
    {
        isHC30 = true;
    }
    bool isW95 = false;
    if((minor == 33) || (minor == 21) || (minor == 27))
    {
        isW95 = true;
    }
    bool decompressTopicBlock = false;
    qint64 topicBlockSize = Q_INT64_C(0);
    if(minor <= 16)
    {
        topicBlockSize = Q_INT64_C(2048);
        decompressTopicBlock = false;
    }
    else
    {
        if(flags == 0)
        {
            topicBlockSize = Q_INT64_C(4096);
            decompressTopicBlock = false;
        }
        else
        {
            if(flags == 4)
            {
                topicBlockSize = Q_INT64_C(4096);
                decompressTopicBlock = true;
            }
            else
            {
                if(flags == 8)
                {
                    topicBlockSize = Q_INT64_C(2048);
                    decompressTopicBlock = true;
                }
                else
                {
                    throw std::runtime_error(
                        "Unknown |TOPIC file compression method");
                }
            }
        }
    }
    qint64 blockDecompressSize = Q_INT64_C(0);
    if(decompressTopicBlock)
    {
        blockDecompressSize = Q_INT64_C(16384);
    }
    else
    {
        if(!isW95)
        {
            blockDecompressSize = topicBlockSize - TopicBlockHeader::size;
        }
        else
        {
            blockDecompressSize = Q_INT64_C(16384);
        }
    }
    qint64 firstTopicBlockOffset = off + InternalDirectoryFileHeader::size;
    qint64 maxOffset = firstTopicBlockOffset + hdr.getUsedSpace();
    TopicPos src(Q_INT64_C(0), Q_INT64_C(
            0), TopicBlockHeader::size, blockDecompressSize);
    TopicBlockCache cache(5);
    while(!src.isInvalid())
    {
        TopicLinkHeader topicLinkHeader =
            this->parseTopicLinkCompressedOldPhrases(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            TopicBlockHeader::size,
            blockDecompressSize,
            src,
            cache,
            phrases,
            isHC30,
            codec,
            isW95);
        if(isHC30)
        {
            if(!topicLinkHeader.getNextBlock().isInvalid())
            {
                src = src + topicLinkHeader.getNextBlock();
            }
            else
            {
                src = topicLinkHeader.getNextBlock();
            }
        }
        else
        {
            src = topicLinkHeader.getNextBlock();
        }
    }
    for(int i = 0; i < rawDescriptors.count(); i++)
    {
        rawDescriptors.at(i)->transformToHtml(i,
            rawDescriptors.count(), linkPointers);
    }
    PRINT_DBG("WinHelp Topic file loaded successfully");
}

WinHelpTopicFile::WinHelpTopicFile(QFile &file,
    qint64 off,
    QTextCodec *codec,
    const WinHelpPhrImageFile &phrases,
    quint16 minor,
    quint16 flags) : linkPointers(), linkBlocks(), rawDescriptors()
{
    PRINT_DBG("Loading WinHelp Topic file at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    bool isHC30 = false;
    if(minor <= 16)
    {
        isHC30 = true;
    }
    bool isW95 = false;
    if((minor == 33) || (minor == 21) || (minor == 27))
    {
        isW95 = true;
    }
    bool decompressTopicBlock = false;
    qint64 topicBlockSize = Q_INT64_C(0);
    if(minor <= 16)
    {
        topicBlockSize = Q_INT64_C(2048);
        decompressTopicBlock = false;
    }
    else
    {
        if(flags == 0)
        {
            topicBlockSize = Q_INT64_C(4096);
            decompressTopicBlock = false;
        }
        else
        {
            if(flags == 4)
            {
                topicBlockSize = Q_INT64_C(4096);
                decompressTopicBlock = true;
            }
            else
            {
                if(flags == 8)
                {
                    topicBlockSize = Q_INT64_C(2048);
                    decompressTopicBlock = true;
                }
                else
                {
                    throw std::runtime_error(
                        "Unknown |TOPIC file compression method");
                }
            }
        }
    }
    qint64 blockDecompressSize = Q_INT64_C(0);
    if(decompressTopicBlock)
    {
        blockDecompressSize = Q_INT64_C(16384);
    }
    else
    {
        if(!isW95)
        {
            blockDecompressSize = topicBlockSize - TopicBlockHeader::size;
        }
        else
        {
            blockDecompressSize = Q_INT64_C(16384);
        }
    }
    qint64 firstTopicBlockOffset = off + InternalDirectoryFileHeader::size;
    qint64 maxOffset = firstTopicBlockOffset + hdr.getUsedSpace();
    TopicPos src(Q_INT64_C(0), Q_INT64_C(
            0), TopicBlockHeader::size, blockDecompressSize);
    TopicBlockCache cache(5);
    while(!src.isInvalid())
    {
        TopicLinkHeader topicLinkHeader =
            this->parseTopicLinkCompressedNewPhrases(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            TopicBlockHeader::size,
            blockDecompressSize,
            src,
            cache,
            phrases,
            isHC30,
            codec,
            isW95);
        if(isHC30)
        {
            if(!topicLinkHeader.getNextBlock().isInvalid())
            {
                src = src + topicLinkHeader.getNextBlock();
            }
            else
            {
                src = topicLinkHeader.getNextBlock();
            }
        }
        else
        {
            src = topicLinkHeader.getNextBlock();
        }
    }
    for(int i = 0; i < rawDescriptors.count(); i++)
    {
        rawDescriptors.at(i)->transformToHtml(i,
            rawDescriptors.count(), linkPointers);
    }
    PRINT_DBG("WinHelp Topic file loaded successfully");
}

WinHelpTopicFile::WinHelpTopicFile(const WinHelpTopicFile& rhs) : linkPointers(
    rhs.linkPointers), linkBlocks(rhs.linkBlocks),
    rawDescriptors(rhs.rawDescriptors)
{
    PRINT_DBG("WinHelp Topic file copy constructor");
}

WinHelpTopicFile::~WinHelpTopicFile()
{
    PRINT_DBG("WinHelp Topic file destructor");
}

WinHelpTopicFile & WinHelpTopicFile::operator=(const WinHelpTopicFile & rhs)
{
    PRINT_DBG("WinHelp Topic file assignment operator");
    if (this != &rhs)
    {
        this->linkPointers = rhs.linkPointers;
        this->linkBlocks = rhs.linkBlocks;
        this->rawDescriptors = rhs.rawDescriptors;
    }
    return *this;
}

size_t WinHelpTopicFile::CopyTopicBytes(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    TopicPos src,
    qint64 srcLength,
    void *dest,
    size_t destSize,
    TopicBlockCache &blockCache,
    bool w95Hint)
{
    memset(dest, 0, static_cast<size_t>(destSize));
    if(srcLength < 0)
    {
        throw std::runtime_error(
            "Unable to copy les then zero bytes from topic");
    }
    size_t toCopy = static_cast<size_t>(srcLength);
    if(toCopy > destSize)
    {
        toCopy = destSize;
    }
    size_t copied = static_cast<size_t>(0);
    qint64 topicBlockActualSize = topicBlockSize;
    if(toCopy > static_cast<size_t>(0))
    {
        qint64 blockOffset = firstTopicBlockOffset +
            src.getTopicBlockNumber() * topicBlockSize;
        if(src.getTopicBlockOffset() < Q_INT64_C(0))
        {
            throw std::runtime_error("Invalid topic position");
        }
        size_t inBlockPtr = static_cast<size_t>(src.getTopicBlockOffset());
        while(copied < toCopy)
        {
            if(blockOffset + topicBlockSize > maxOffset)
            {
                if(maxOffset > blockOffset)
                {
                    topicBlockActualSize = maxOffset - blockOffset;
                }
                else
                {
                    break;
                }
            }
            QScopedPointer<TopicBlock> block(blockCache.take(blockOffset));
            if(block.isNull())
            {
                QScopedPointer<TopicBlock>(new TopicBlock(file, blockOffset,
                        topicBlockSize, decompressTopicBlock,
                        topicBlockActualSize,
                        w95Hint)).swap(block);
            }
            size_t toCopyFromBlock = block->getActualDecompressedDataSize();
            if(toCopy < toCopyFromBlock)
            {
                toCopyFromBlock = toCopy;
            }
            size_t actuallyCopied = block->CopyDecompressedBytes(
                reinterpret_cast<void *>((reinterpret_cast<quint8 *>(dest) +
                        copied)),
                destSize - copied,
                inBlockPtr,
                toCopyFromBlock);
            blockCache.insert(blockOffset, block.take());
            inBlockPtr = static_cast<size_t>(0);
            copied += actuallyCopied;
            blockOffset += topicBlockSize;
        }
    }
    return copied;
}

size_t WinHelpTopicFile::CopyTopicLinkData1(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    TopicPos src,
    void *dest,
    size_t destSize,
    TopicBlockCache &blockCache,
    const TopicLinkHeader &header,
    bool w95Hint)
{
    if(header.getDataLen1() < TopicLinkHeader::size)
    {
        throw std::runtime_error("Topic linkData1 size is too small");
    }
    size_t toCopy =
        static_cast<size_t>(header.getDataLen1() - TopicLinkHeader::size);
    if(destSize < toCopy)
    {
        toCopy = destSize;
    }
    size_t bufSize = static_cast<size_t>(header.getDataLen1());
    QScopedArrayPointer<quint8> buffer(new quint8[bufSize]);
    size_t bytesCopied = this->CopyTopicBytes(file,
        topicBlockSize,
        decompressTopicBlock,
        firstTopicBlockOffset,
        maxOffset,
        src,
        header.getDataLen1(),
        reinterpret_cast<void *>(buffer.data()),
        bufSize,
        blockCache,
        w95Hint);
    if(bytesCopied < bufSize)
    {
        throw std::runtime_error("Unable to read Topic linkData1 header");
    }
    if(toCopy > 0)
    {
        memcpy(dest,
            reinterpret_cast<const void *>(buffer.data() +
                static_cast<size_t>(TopicLinkHeader::size)), toCopy);
    }
    return toCopy;
}

size_t WinHelpTopicFile::CopyTopicLinkData2Uncompressed(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    TopicPos src,
    void *dest,
    size_t destSize,
    TopicBlockCache &blockCache,
    const TopicLinkHeader &header,
    bool w95Hint)
{
    if(header.getBlockSize() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic block size is too small");
    }
    if(header.getDataLen1() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic linkData1 size is too small");
    }
    if(header.getBlockSize() < header.getDataLen1())
    {
        throw std::runtime_error("Topic linkData2 size is too small");
    }
    if(header.getDataLen2() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic linkData2 size is too small");
    }
    size_t toCopy =
        static_cast<size_t>(header.getBlockSize() - header.getDataLen1());
    if(header.getDataLen2() < (header.getBlockSize() - header.getDataLen1()))
    {
        toCopy = static_cast<size_t>(header.getDataLen2());
    }
    if(destSize < toCopy)
    {
        toCopy = destSize;
    }
    size_t bufSize = static_cast<size_t>(header.getBlockSize());
    QScopedArrayPointer<quint8> buffer(new quint8[bufSize]);
    size_t bytesCopied = this->CopyTopicBytes(file,
        topicBlockSize,
        decompressTopicBlock,
        firstTopicBlockOffset,
        maxOffset,
        src,
        header.getBlockSize(),
        reinterpret_cast<void *>(buffer.data()),
        bufSize,
        blockCache,
        w95Hint);
    if(bytesCopied < bufSize)
    {
        throw std::runtime_error("Unable to read uncompressed Topic linkData2");
    }
    if(toCopy > 0)
    {
        memcpy(dest,
            reinterpret_cast<const void *>(buffer.data() +
                static_cast<size_t>(header.getDataLen1())), toCopy);
    }
    return toCopy;
}

size_t WinHelpTopicFile::CopyTopicLinkData2CompressedOldPhrases(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    TopicPos src,
    void *dest,
    size_t destSize,
    TopicBlockCache &blockCache,
    TopicLinkHeader &header,
    const WinHelpPhraseFile &phrases,
    bool w95Hint)
{
    if(header.getBlockSize() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic block size is too small");
    }
    if(header.getDataLen1() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic linkData1 size is too small");
    }
    if(header.getBlockSize() < header.getDataLen1())
    {
        throw std::runtime_error("Topic linkData2 size is too small");
    }
    if(header.getDataLen2() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic linkData2 size is too small");
    }
    if((header.getBlockSize() - header.getDataLen1()) < header.getDataLen2())
    {
        size_t bufSize = static_cast<size_t>(header.getBlockSize());
        QScopedArrayPointer<quint8> buffer(new quint8[bufSize]);
        size_t bytesCopied = this->CopyTopicBytes(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            src,
            header.getBlockSize(),
            reinterpret_cast<void *>(buffer.data()),
            bufSize,
            blockCache,
            w95Hint);
        if(bytesCopied < bufSize)
        {
            throw std::runtime_error(
                "Unable to read phrase compressed Topic linkData2");
        }
        size_t inputPtr = static_cast<size_t>(header.getDataLen1());
        size_t outputPtr = static_cast<size_t>(0);
        size_t toCopy = static_cast<size_t>(header.getDataLen2());
        quint8 *outBuf = reinterpret_cast<quint8 *>(dest);
        if(destSize < toCopy)
        {
            toCopy = destSize;
        }
        while((outputPtr < toCopy) && (inputPtr < bytesCopied))
        {
            if((buffer[inputPtr] == 0) || (buffer[inputPtr] > 15))
            {
                outBuf[outputPtr] = buffer[inputPtr];
                inputPtr++;
                outputPtr++;
            }
            else
            {
                if((inputPtr + static_cast<size_t>(1)) >= bytesCopied)
                {
                    throw std::runtime_error(
                        "Unable to unpack phrase compressed Topic linkData2");
                }
                int index =  static_cast<int>(buffer[inputPtr]) * 256 - 256 +
                    static_cast<int>(buffer[inputPtr + static_cast<size_t>(1)]);
                inputPtr += static_cast<size_t>(2);
                if((index % 2) == 0)
                {
                    int phraseIndex = index / 2;
                    QByteArray phrase = phrases.getRawPhrase(phraseIndex);
                    for(int i = 0; i < phrase.size(); i++)
                    {
                        if(outputPtr >= toCopy)
                        {
                            break;
                        }
                        outBuf[outputPtr] = static_cast<quint8>(phrase.at(i));
                        outputPtr++;
                    }
                }
                else
                {
                    int phraseIndex = index / 2;
                    QByteArray phrase = phrases.getRawPhrase(phraseIndex);
                    for(int i = 0; i < phrase.size(); i++)
                    {
                        if(outputPtr >= toCopy)
                        {
                            break;
                        }
                        outBuf[outputPtr] = static_cast<quint8>(phrase.at(i));
                        outputPtr++;
                    }
                    if(outputPtr < toCopy)
                    {
                        outBuf[outputPtr] = 0x20;
                        outputPtr++;
                    }
                }
            }
        }
        return outputPtr;
    }
    else
    {
        return CopyTopicLinkData2Uncompressed(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            src,
            dest,
            destSize,
            blockCache,
            header,
            w95Hint);
    }
}

size_t WinHelpTopicFile::CopyTopicLinkData2CompressedNewPhrases(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    TopicPos src,
    void *dest,
    size_t destSize,
    TopicBlockCache &blockCache,
    TopicLinkHeader &header,
    const WinHelpPhrImageFile &phrases,
    bool w95Hint)
{
    if(header.getBlockSize() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic block size is too small");
    }
    if(header.getDataLen1() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic linkData1 size is too small");
    }
    if(header.getBlockSize() < header.getDataLen1())
    {
        throw std::runtime_error("Topic linkData2 size is too small");
    }
    if(header.getDataLen2() < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic linkData2 size is too small");
    }
    if((header.getBlockSize() - header.getDataLen1()) < header.getDataLen2())
    {
        size_t bufSize = static_cast<size_t>(header.getBlockSize());
        QScopedArrayPointer<quint8> buffer(new quint8[bufSize]);
        size_t bytesCopied = this->CopyTopicBytes(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            src,
            header.getBlockSize(),
            reinterpret_cast<void *>(buffer.data()),
            bufSize,
            blockCache,
            w95Hint);
        if(bytesCopied < bufSize)
        {
            throw std::runtime_error(
                "Unable to read phrase compressed Topic linkData2");
        }
        size_t inputPtr = static_cast<size_t>(header.getDataLen1());
        size_t outputPtr = static_cast<size_t>(0);
        size_t toCopy = static_cast<size_t>(header.getDataLen2());
        quint8 *outBuf = reinterpret_cast<quint8 *>(dest);
        if(destSize < toCopy)
        {
            toCopy = destSize;
        }
        while((outputPtr < toCopy) && (inputPtr < bytesCopied))
        {
            if((buffer[inputPtr] % 2) == 0)
            {
                int phraseIndex = static_cast<int>(buffer[inputPtr] / 2);
                if(phraseIndex < phrases.getPhrasesCount())
                {
                    QByteArray phrase = phrases.getRawPhrase(phraseIndex);
                    inputPtr++;
                    for(int i = 0; i < phrase.size(); i++)
                    {
                        outBuf[outputPtr] = static_cast<quint8>(phrase.at(i));
                        outputPtr++;
                        if(outputPtr >= toCopy)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    throw std::runtime_error("Phrase is out of range");
                }
            }
            else
            {
                if((buffer[inputPtr] & 0x03) == 1)
                {
                    if((inputPtr + static_cast<size_t>(1)) >= bytesCopied)
                    {
                        throw std::runtime_error(
                            "Unable to unpack phrase compressed Topic linkData2");
                    }
                    int phraseIndex =  static_cast<int>(buffer[inputPtr]) *
                        64 + 64 +
                        static_cast<int>(buffer[inputPtr +
                            static_cast<size_t>(1)]);
                    inputPtr += static_cast<size_t>(2);
                    if(phraseIndex < phrases.getPhrasesCount())
                    {
                        QByteArray phrase = phrases.getRawPhrase(phraseIndex);
                        for(int i = 0; i < phrase.size(); i++)
                        {
                            outBuf[outputPtr] = static_cast<quint8>(phrase.at(i));
                            outputPtr++;
                            if(outputPtr >= toCopy)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        throw std::runtime_error("Phrase is out of range");
                    }
                }
                else
                {
                    if((buffer[inputPtr] & 0x07) == 3)
                    {
                        quint64 charToCopy =
                            static_cast<quint64>(buffer[inputPtr]) / 8 + 1;
                        inputPtr++;
                        for(quint64 i = 0; i < charToCopy; i++)
                        {
                            if(outputPtr >= toCopy)
                            {
                                break;
                            }
                            if(inputPtr >= bytesCopied)
                            {
                                break;
                            }
                            outBuf[outputPtr] = buffer[inputPtr];
                            inputPtr++;
                            outputPtr++;
                        }
                    }
                    else
                    {
                        if((buffer[inputPtr] & 0x0F) == 7)
                        {
                            quint64 spacesCount =
                                static_cast<quint64>(buffer[inputPtr]) / 16 + 1;
                            inputPtr++;
                            for(quint64 i = 0; i < spacesCount; i++)
                            {
                                if(outputPtr >= toCopy)
                                {
                                    break;
                                }
                                outBuf[outputPtr] = 0x20;
                                outputPtr++;
                            }
                        }
                        else
                        {
                            if((buffer[inputPtr] & 0x0F) == 15)
                            {
                                quint64 nullsCount =
                                    static_cast<quint64>(buffer[inputPtr]) /
                                    16 + 1;
                                inputPtr++;
                                for(quint64 i = 0; i < nullsCount; i++)
                                {
                                    if(outputPtr >= toCopy)
                                    {
                                        break;
                                    }
                                    outBuf[outputPtr] = 0;
                                    outputPtr++;
                                }
                            }
                        }
                    }
                }
            }
        }
        return outputPtr;
    }
    else
    {
        return CopyTopicLinkData2Uncompressed(file,
            topicBlockSize,
            decompressTopicBlock,
            firstTopicBlockOffset,
            maxOffset,
            src,
            dest,
            destSize,
            blockCache,
            header,
            w95Hint);
    }
}

TopicLinkHeader WinHelpTopicFile::getTopicLinkHeader(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    qint64 topicBlockHeaderSize,
    qint64 blockDecompressSize,
    TopicPos src,
    TopicBlockCache &blockCache,
    bool w95Hint)
{
    size_t bufSize = static_cast<size_t>(TopicLinkHeader::size);
    QScopedArrayPointer<quint8> buffer(new quint8[bufSize]);
    size_t bytesCopied = this->CopyTopicBytes(file,
        topicBlockSize,
        decompressTopicBlock,
        firstTopicBlockOffset,
        maxOffset,
        src,
        TopicLinkHeader::size,
        reinterpret_cast<void *>(buffer.data()),
        bufSize,
        blockCache,
        w95Hint);
    if(bytesCopied < bufSize)
    {
        if(bytesCopied != 0)
        {
            throw std::runtime_error("Unable to read Topic link header");
        }
        else
        {
            qDebug() << "Topic link header is out of |TOPIC file";
            return TopicLinkHeader();
        }
    }
    return TopicLinkHeader(
        reinterpret_cast<const void *>(buffer.data()), bufSize,
        topicBlockHeaderSize,
        blockDecompressSize);
}

TopicLinkHeader WinHelpTopicFile::parseTopicLinkUncompressed(QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    qint64 topicBlockHeaderSize,
    qint64 blockDecompressSize,
    TopicPos src,
    TopicBlockCache &blockCache,
    bool isHC30,
    QTextCodec *codec,
    bool w95Hint)
{
    TopicLinkHeader topicLinkHeader = this->getTopicLinkHeader(file,
        topicBlockSize,
        decompressTopicBlock,
        firstTopicBlockOffset,
        maxOffset,
        topicBlockHeaderSize,
        blockDecompressSize,
        src,
        blockCache,
        w95Hint);
    if(topicLinkHeader.getRecordType() != 0)
    {
        if((topicLinkHeader.getDataLen1() > TopicLinkHeader::size))
        {
            if(topicLinkHeader.getDataLen2() < Q_INT64_C(0))
            {
                throw std::runtime_error("Unable to load Topic file");
            }
            size_t buf2Size = static_cast<size_t>(topicLinkHeader.getDataLen2());
            QScopedArrayPointer<quint8> data2Buf;
            size_t data2Len = static_cast<size_t>(0);
            if(buf2Size > static_cast<size_t>(0))
            {
                QScopedArrayPointer<quint8>(new quint8[buf2Size]).swap(data2Buf);
                data2Len = CopyTopicLinkData2Uncompressed(file,
                    topicBlockSize,
                    decompressTopicBlock,
                    firstTopicBlockOffset,
                    maxOffset,
                    src,
                    reinterpret_cast<void *>(data2Buf.data()),
                    buf2Size,
                    blockCache,
                    topicLinkHeader,
                    w95Hint);
                if(data2Len < buf2Size)
                {
                    throw std::runtime_error("Unable to load Topic file");
                }
            }
            size_t buf1Size =
                static_cast<size_t>(topicLinkHeader.getDataLen1() -
                TopicLinkHeader::size);
            QScopedArrayPointer<quint8> data1Buf(new quint8[buf1Size]);
            size_t data1Len = CopyTopicLinkData1(file,
                topicBlockSize,
                decompressTopicBlock,
                firstTopicBlockOffset,
                maxOffset,
                src,
                reinterpret_cast<void *>(data1Buf.data()),
                buf1Size,
                blockCache,
                topicLinkHeader,
                w95Hint);
            if(data1Len < buf1Size)
            {
                throw std::runtime_error("Unable to load Topic file");
            }
            switch(topicLinkHeader.getRecordType())
            {
            case 2:
                if(isHC30)
                {
                    QSharedPointer<TopicHeader> topicHdr(
                        dynamic_cast<TopicHeader *>(new TopicHeaderOld(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count(), linkPointers.count())));
                    QSharedPointer<TopicRawDescriptor> newDescriptor(
                        new TopicRawDescriptor(topicHdr));
                    rawDescriptors.append(newDescriptor);
                    linkPointers.append(topicHdr.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                else
                {
                    QSharedPointer<TopicHeader> topicHdr(
                        dynamic_cast<TopicHeader *>(new TopicHeaderNew(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                topicBlockHeaderSize,
                                blockDecompressSize,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count(), linkPointers.count())));
                    QSharedPointer<TopicRawDescriptor> newDescriptor(
                        new TopicRawDescriptor(topicHdr));
                    rawDescriptors.append(newDescriptor);
                    linkPointers.append(topicHdr.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                break;

            case 1:
                if(isHC30)
                {
                    QSharedPointer<DisplayableText> dispTextOld(
                        dynamic_cast<DisplayableText *>(new DisplayableTextOld(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                    if(rawDescriptors.isEmpty())
                    {
                        throw std::runtime_error("Topic without header");
                    }
                    if(rawDescriptors.last().isNull())
                    {
                        throw std::runtime_error("Topic header is NULL");
                    }
                    rawDescriptors.last()->AddDisplayableText(dispTextOld);
                    linkPointers.append(dispTextOld.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                else
                {
                    qDebug() << "Invalid version of displayable text";
                }
                break;

            case 0x20:
            {
                QSharedPointer<DisplayableText> dispTextNew(
                        dynamic_cast<DisplayableText *>(new DisplayableTextNew(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                if(rawDescriptors.isEmpty())
                {
                    throw std::runtime_error("Topic without header");
                }
                if(rawDescriptors.last().isNull())
                {
                    throw std::runtime_error("Topic header is NULL");
                }
                rawDescriptors.last()->AddDisplayableText(dispTextNew);
                linkPointers.append(dispTextNew.dynamicCast<TopicLink>());
                linkBlocks.insert(src.getTopicBlockNumber(),
                        linkPointers.size() - 1);
            }
            break;

            case 0x23:
            {
                QSharedPointer<DisplayableText> dispTable(
                        dynamic_cast<DisplayableText *>(new DisplayableTable(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                if(rawDescriptors.isEmpty())
                {
                    throw std::runtime_error("Topic without header");
                }
                if(rawDescriptors.last().isNull())
                {
                    throw std::runtime_error("Topic header is NULL");
                }
                rawDescriptors.last()->AddDisplayableText(dispTable);
                linkPointers.append(dispTable.dynamicCast<TopicLink>());
                linkBlocks.insert(src.getTopicBlockNumber(),
                        linkPointers.size() - 1);
            }
            break;

            default:
                qDebug() << "Unknown Topic link header record type";
            }
        }
        else
        {
            qDebug() << "Empty topic link data";
        }
    }
    return topicLinkHeader;
}

TopicLinkHeader WinHelpTopicFile::parseTopicLinkCompressedOldPhrases(
    QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    qint64 topicBlockHeaderSize,
    qint64 blockDecompressSize,
    TopicPos src,
    TopicBlockCache &blockCache,
    const WinHelpPhraseFile &phrases,
    bool isHC30,
    QTextCodec *codec,
    bool w95Hint)
{
    TopicLinkHeader topicLinkHeader = this->getTopicLinkHeader(file,
        topicBlockSize,
        decompressTopicBlock,
        firstTopicBlockOffset,
        maxOffset,
        topicBlockHeaderSize,
        blockDecompressSize,
        src,
        blockCache,
        w95Hint);
    if(topicLinkHeader.getRecordType() != 0)
    {
        if((topicLinkHeader.getDataLen1() > TopicLinkHeader::size))
        {
            if(topicLinkHeader.getDataLen2() < Q_INT64_C(0))
            {
                throw std::runtime_error("Unable to load Topic file");
            }
            size_t buf2Size = static_cast<size_t>(topicLinkHeader.getDataLen2());
            QScopedArrayPointer<quint8> data2Buf;
            size_t data2Len = static_cast<size_t>(0);
            if(buf2Size > static_cast<size_t>(0))
            {
                QScopedArrayPointer<quint8>(new quint8[buf2Size]).swap(data2Buf);
                data2Len = CopyTopicLinkData2CompressedOldPhrases(file,
                    topicBlockSize,
                    decompressTopicBlock,
                    firstTopicBlockOffset,
                    maxOffset,
                    src,
                    reinterpret_cast<void *>(data2Buf.data()),
                    buf2Size,
                    blockCache,
                    topicLinkHeader,
                    phrases,
                    w95Hint);
                if(data2Len < buf2Size)
                {
                    throw std::runtime_error("Unable to load Topic file");
                }
            }
            size_t buf1Size =
                static_cast<size_t>(topicLinkHeader.getDataLen1() -
                TopicLinkHeader::size);
            QScopedArrayPointer<quint8> data1Buf(new quint8[buf1Size]);
            size_t data1Len = CopyTopicLinkData1(file,
                topicBlockSize,
                decompressTopicBlock,
                firstTopicBlockOffset,
                maxOffset,
                src,
                reinterpret_cast<void *>(data1Buf.data()),
                buf1Size,
                blockCache,
                topicLinkHeader,
                w95Hint);
            if(data1Len < buf1Size)
            {
                throw std::runtime_error("Unable to load Topic file");
            }
            switch(topicLinkHeader.getRecordType())
            {
            case 2:
                if(isHC30)
                {
                    QSharedPointer<TopicHeader> topicHdr(
                        dynamic_cast<TopicHeader *>(new TopicHeaderOld(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count(), linkPointers.count())));
                    QSharedPointer<TopicRawDescriptor> newDescriptor(
                        new TopicRawDescriptor(topicHdr));
                    rawDescriptors.append(newDescriptor);
                    linkPointers.append(topicHdr.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                else
                {
                    QSharedPointer<TopicHeader> topicHdr(
                        dynamic_cast<TopicHeader *>(new TopicHeaderNew(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                topicBlockHeaderSize,
                                blockDecompressSize,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count(), linkPointers.count())));
                    QSharedPointer<TopicRawDescriptor> newDescriptor(
                        new TopicRawDescriptor(topicHdr));
                    rawDescriptors.append(newDescriptor);
                    linkPointers.append(topicHdr.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                break;

            case 1:
                if(isHC30)
                {
                    QSharedPointer<DisplayableText> dispTextOld(
                        dynamic_cast<DisplayableText *>(new DisplayableTextOld(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                    if(rawDescriptors.isEmpty())
                    {
                        throw std::runtime_error("Topic without header");
                    }
                    if(rawDescriptors.last().isNull())
                    {
                        throw std::runtime_error("Topic header is NULL");
                    }
                    rawDescriptors.last()->AddDisplayableText(dispTextOld);
                    linkPointers.append(dispTextOld.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                else
                {
                    qDebug() << "Invalid version of displayable text";
                }
                break;

            case 0x20:
            {
                QSharedPointer<DisplayableText> dispTextNew(
                        dynamic_cast<DisplayableText *>(new DisplayableTextNew(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                if(rawDescriptors.isEmpty())
                {
                    throw std::runtime_error("Topic without header");
                }
                if(rawDescriptors.last().isNull())
                {
                    throw std::runtime_error("Topic header is NULL");
                }
                rawDescriptors.last()->AddDisplayableText(dispTextNew);
                linkPointers.append(dispTextNew.dynamicCast<TopicLink>());
                linkBlocks.insert(src.getTopicBlockNumber(),
                        linkPointers.size() - 1);
            }
            break;

            case 0x23:
            {
                QSharedPointer<DisplayableText> dispTable(
                        dynamic_cast<DisplayableText *>(new DisplayableTable(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                if(rawDescriptors.isEmpty())
                {
                    throw std::runtime_error("Topic without header");
                }
                if(rawDescriptors.last().isNull())
                {
                    throw std::runtime_error("Topic header is NULL");
                }
                rawDescriptors.last()->AddDisplayableText(dispTable);
                linkPointers.append(dispTable.dynamicCast<TopicLink>());
                linkBlocks.insert(src.getTopicBlockNumber(),
                        linkPointers.size() - 1);
            }
            break;

            default:
                qDebug() << "Unknown Topic link header record type";
            }
        }
        else
        {
            qDebug() << "Empty topic link data";
        }
    }
    return topicLinkHeader;
}

TopicLinkHeader WinHelpTopicFile::parseTopicLinkCompressedNewPhrases(
    QFile &file,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 firstTopicBlockOffset,
    qint64 maxOffset,
    qint64 topicBlockHeaderSize,
    qint64 blockDecompressSize,
    TopicPos src,
    TopicBlockCache &blockCache,
    const WinHelpPhrImageFile &phrases,
    bool isHC30,
    QTextCodec *codec,
    bool w95Hint)
{
    TopicLinkHeader topicLinkHeader = this->getTopicLinkHeader(file,
        topicBlockSize,
        decompressTopicBlock,
        firstTopicBlockOffset,
        maxOffset,
        topicBlockHeaderSize,
        blockDecompressSize,
        src,
        blockCache,
        w95Hint);
    if(topicLinkHeader.getRecordType() != 0)
    {
        if((topicLinkHeader.getDataLen1() > TopicLinkHeader::size))
        {
            if(topicLinkHeader.getDataLen2() < Q_INT64_C(0))
            {
                throw std::runtime_error("Unable to load Topic file");
            }
            size_t buf2Size = static_cast<size_t>(topicLinkHeader.getDataLen2());
            QScopedArrayPointer<quint8> data2Buf;
            size_t data2Len = static_cast<size_t>(0);
            if(buf2Size > static_cast<size_t>(0))
            {
                QScopedArrayPointer<quint8>(new quint8[buf2Size]).swap(data2Buf);
                data2Len = CopyTopicLinkData2CompressedNewPhrases(file,
                    topicBlockSize,
                    decompressTopicBlock,
                    firstTopicBlockOffset,
                    maxOffset,
                    src,
                    reinterpret_cast<void *>(data2Buf.data()),
                    buf2Size,
                    blockCache,
                    topicLinkHeader,
                    phrases,
                    w95Hint);
                if(data2Len < buf2Size)
                {
                    throw std::runtime_error("Unable to load Topic file");
                }
            }
            size_t buf1Size =
                static_cast<size_t>(topicLinkHeader.getDataLen1() -
                TopicLinkHeader::size);
            QScopedArrayPointer<quint8> data1Buf(new quint8[buf1Size]);
            size_t data1Len = CopyTopicLinkData1(file,
                topicBlockSize,
                decompressTopicBlock,
                firstTopicBlockOffset,
                maxOffset,
                src,
                reinterpret_cast<void *>(data1Buf.data()),
                buf1Size,
                blockCache,
                topicLinkHeader,
                w95Hint);
            if(data1Len < buf1Size)
            {
                throw std::runtime_error("Unable to load Topic file");
            }
            switch(topicLinkHeader.getRecordType())
            {
            case 2:
                if(isHC30)
                {
                    QSharedPointer<TopicHeader> topicHdr(
                        dynamic_cast<TopicHeader *>(new TopicHeaderOld(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count(), linkPointers.count())));
                    QSharedPointer<TopicRawDescriptor> newDescriptor(
                        new TopicRawDescriptor(topicHdr));
                    rawDescriptors.append(newDescriptor);
                    linkPointers.append(topicHdr.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                else
                {
                    QSharedPointer<TopicHeader> topicHdr(
                        dynamic_cast<TopicHeader *>(new TopicHeaderNew(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                topicBlockHeaderSize,
                                blockDecompressSize,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count(), linkPointers.count())));
                    QSharedPointer<TopicRawDescriptor> newDescriptor(
                        new TopicRawDescriptor(topicHdr));
                    rawDescriptors.append(newDescriptor);
                    linkPointers.append(topicHdr.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                break;

            case 1:
                if(isHC30)
                {
                    QSharedPointer<DisplayableText> dispTextOld(
                        dynamic_cast<DisplayableText *>(new DisplayableTextOld(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                    if(rawDescriptors.isEmpty())
                    {
                        throw std::runtime_error("Topic without header");
                    }
                    if(rawDescriptors.last().isNull())
                    {
                        throw std::runtime_error("Topic header is NULL");
                    }
                    rawDescriptors.last()->AddDisplayableText(dispTextOld);
                    linkPointers.append(dispTextOld.dynamicCast<TopicLink>());
                    linkBlocks.insert(
                        src.getTopicBlockNumber(), linkPointers.size() - 1);
                }
                else
                {
                    qDebug() << "Invalid version of displayable text";
                }
                break;

            case 0x20:
            {
                QSharedPointer<DisplayableText> dispTextNew(
                        dynamic_cast<DisplayableText *>(new DisplayableTextNew(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                if(rawDescriptors.isEmpty())
                {
                    throw std::runtime_error("Topic without header");
                }
                if(rawDescriptors.last().isNull())
                {
                    throw std::runtime_error("Topic header is NULL");
                }
                rawDescriptors.last()->AddDisplayableText(dispTextNew);
                linkPointers.append(dispTextNew.dynamicCast<TopicLink>());
                linkBlocks.insert(src.getTopicBlockNumber(),
                        linkPointers.size() - 1);
            }
            break;

            case 0x23:
            {
                QSharedPointer<DisplayableText> dispTable(
                        dynamic_cast<DisplayableText *>(new DisplayableTable(
                                reinterpret_cast<const void *>(data1Buf.data()),
                                data1Len,
                                reinterpret_cast<const void *>(data2Buf.data()),
                                data2Len,
                                codec,
                                rawDescriptors.count() - 1, linkPointers.count())));
                if(rawDescriptors.isEmpty())
                {
                    throw std::runtime_error("Topic without header");
                }
                if(rawDescriptors.last().isNull())
                {
                    throw std::runtime_error("Topic header is NULL");
                }
                rawDescriptors.last()->AddDisplayableText(dispTable);
                linkPointers.append(dispTable.dynamicCast<TopicLink>());
                linkBlocks.insert(src.getTopicBlockNumber(),
                        linkPointers.size() - 1);
            }
            break;

            default:
                qDebug() << "Unknown Topic link header record type";
            }
        }
        else
        {
            qDebug() << "Empty topic link data";
        }
    }
    return topicLinkHeader;
}

int WinHelpTopicFile::getTopicsCount() const
{
    return this->rawDescriptors.count();
}

QString WinHelpTopicFile::getTopicCaption(int index) const
{
    if ((index < 0) || (index >= rawDescriptors.size()))
    {
        throw new std::out_of_range("Index of topic is out of range");
    }
    return rawDescriptors.at(index)->getCaption();
}

QString WinHelpTopicFile::getTopicContents(int index) const
{
    if ((index < 0) || (index >= rawDescriptors.size()))
    {
        throw new std::out_of_range("Index of topic is out of range");
    }
    return rawDescriptors.at(index)->getContents();
}

int WinHelpTopicFile::getTopicIndexByTopicOffset(int block,
    int character) const
{
    int result = 0;
    if(this->linkBlocks.contains(static_cast<qint64>(block)))
    {
        QList<int> values = this->linkBlocks.values(static_cast<qint64>(block));
        qSort(values);
        int currentPosition = 0;
        bool found = false;
        for(int i = 0; i < values.count(); i++)
        {
            currentPosition +=
                this->linkPointers.at(values.at(i))->getCharacterCount();
            if(currentPosition > character)
            {
                if(values.at(i) < this->linkPointers.count())
                {
                    result =
                        this->linkPointers.at(values.at(i))->
                        getTopicDescriptorNumber();
                    found = true;
                }
                break;
            }
        }
        if(!found)
        {
            result =
                this->linkPointers.at(values.last())->getTopicDescriptorNumber();
        }
    }
    return result;
}
