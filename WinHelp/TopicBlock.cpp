/* Topic block class implementation.

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

#include "TopicBlock.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/CompressionUtils.h"

#include <cstring>

#include <stdexcept>

TopicBlock::TopicBlock() : header(), unpackedBlock(),
    decompressSize(Q_INT64_C(0)), bufferSize(static_cast<size_t>(0)),
    actualDecompressedDataSize(static_cast<size_t>(0))
{
    PRINT_DBG("Topic block default constructor");
}

TopicBlock::TopicBlock(QFile &file,
    qint64 off,
    qint64 topicBlockSize,
    bool decompressTopicBlock,
    qint64 actualSize,
    bool w95Hint) : header(), unpackedBlock()
{
    PRINT_DBG("Loading Topic block at %lld", off);
    seekFile(file, off);
    if(decompressTopicBlock)
    {
        decompressSize = Q_INT64_C(16384);
    }
    else
    {
        if(!w95Hint)
        {
            if(topicBlockSize < TopicBlockHeader::size)
            {
                throw std::runtime_error("Topic block size is too small");
            }
            decompressSize = topicBlockSize - TopicBlockHeader::size;
        }
        else
        {
            decompressSize = Q_INT64_C(16384);
        }
    }
    this->header = TopicBlockHeader(file, off, decompressSize);
    if(this->decompressSize < Q_INT64_C(0))
    {
        throw std::runtime_error("Topic block decompress size is negative");
    }
    this->bufferSize = static_cast<size_t>(decompressSize);
    QScopedArrayPointer<quint8>(new quint8[bufferSize]).swap(
        this->unpackedBlock);
    memset(reinterpret_cast<void *>(this->unpackedBlock.data()), 0, bufferSize);
    if(decompressTopicBlock)
    {
        this->actualDecompressedDataSize = unpackLZ77(file,
            off + TopicBlockHeader::size,
            actualSize - TopicBlockHeader::size,
            reinterpret_cast<void *>(this->unpackedBlock.data()),
            bufferSize);
    }
    else
    {
        if(actualSize < TopicBlockHeader::size)
        {
            throw std::runtime_error("Topic block actual size is too small");
        }
        if(decompressSize > actualSize - TopicBlockHeader::size)
        {
            this->actualDecompressedDataSize =
                static_cast<size_t>(actualSize - TopicBlockHeader::size);
        }
        else
        {
            this->actualDecompressedDataSize =
                static_cast<size_t>(decompressSize);
        }
        fillBuffer(file,
            actualSize - TopicBlockHeader::size,
            this->unpackedBlock.data(),
            bufferSize);
    }
    PRINT_DBG("Topic block loaded successfully");
}

TopicBlock::TopicBlock(const TopicBlock& rhs) : header(rhs.header),
    unpackedBlock(), decompressSize(rhs.decompressSize),
    bufferSize(rhs.bufferSize),
    actualDecompressedDataSize(rhs.actualDecompressedDataSize)
{
    PRINT_DBG("Topic block copy constructor");
    if((rhs.bufferSize != static_cast<size_t>(0)) && (!rhs.unpackedBlock.isNull()))
    {
        QScopedArrayPointer<quint8>(new quint8[rhs.bufferSize]).swap(
            this->unpackedBlock);
        memcpy(
            reinterpret_cast<void *>(this->unpackedBlock.data()),
            reinterpret_cast<const void *>(rhs.unpackedBlock.data()),
            rhs.bufferSize);
    }
}

TopicBlock::~TopicBlock()
{
    PRINT_DBG("Topic block destructor");
}

TopicBlock & TopicBlock::operator=(const TopicBlock & rhs)
{
    PRINT_DBG("Topic block assignment operator");
    if (this != &rhs)
    {
        this->header = rhs.header;
        this->decompressSize = rhs.decompressSize;
        this->bufferSize = rhs.bufferSize;
        this->actualDecompressedDataSize = rhs.actualDecompressedDataSize;
        if(this->decompressSize < Q_INT64_C(0))
        {
            throw std::runtime_error("Topic block decompress size is negative");
        }
        QScopedArrayPointer<quint8>().swap(this->unpackedBlock);
        if((rhs.bufferSize != static_cast<size_t>(0)) &&
                (!rhs.unpackedBlock.isNull()))
        {
            QScopedArrayPointer<quint8>(new quint8[rhs.bufferSize]).swap(
                this->unpackedBlock);
            memcpy(
                reinterpret_cast<void *>(this->unpackedBlock.data()),
                reinterpret_cast<const void *>(rhs.unpackedBlock.data()),
                rhs.bufferSize);
        }
    }
    return *this;
}

qint64 TopicBlock::getDecompressSize() const
{
    return this->decompressSize;
}

size_t TopicBlock::getActualDecompressedDataSize() const
{
    return this->actualDecompressedDataSize;
}

size_t TopicBlock::CopyDecompressedBytes(void *dest,
    size_t destSize,
    size_t srcOffset,
    size_t srcLength) const
{
    size_t toCopy = static_cast<size_t>(0);
    if((srcOffset + srcLength) <= this->actualDecompressedDataSize)
    {
        toCopy = srcLength;
    }
    else
    {
        if(this->actualDecompressedDataSize <= srcOffset)
        {
            toCopy = static_cast<size_t>(0);
        }
        else
        {
            toCopy = this->actualDecompressedDataSize - srcOffset;
        }
    }
    if(toCopy > destSize)
    {
        toCopy = destSize;
    }
    memcpy(dest,
        reinterpret_cast<const void *>(this->unpackedBlock.data() + srcOffset),
        toCopy);
    return toCopy;
}
