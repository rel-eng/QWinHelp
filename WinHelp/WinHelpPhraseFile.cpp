/* WinHelp Phrase file class implementation.

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

#include "WinHelpPhraseFile.h"

#include "InternalDirectoryFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/CompressionUtils.h"

#include <QScopedArrayPointer>

#include <stdexcept>

WinHelpPhraseFile::WinHelpPhraseFile() :
    phrases(), phrasesRaw()
{
    PRINT_DBG("WinHelp phrase file default constructor");
}

WinHelpPhraseFile::WinHelpPhraseFile(QFile &file, qint64 off,
    QTextCodec *codec, bool compressed, bool mvbHint) :
    phrases(), phrasesRaw()
{
    PRINT_DBG("Loading WinHelp phrase file at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    if (compressed)
    {
        if (mvbHint)
        {
            seekFile(file, off + InternalDirectoryFileHeader::size);
            quint16 eightHundr = readUnsignedWord(file);
            quint16 nPhr = readUnsignedWord(file);
            quint16 oneHundr = readUnsignedWord(file);
            if ((eightHundr == 0x0800) && (oneHundr == 0x0100) && (nPhr != 0))
            {
                PRINT_DBG(
                    "Loading compressed WinHelp phrase file, MVB alternative");
                seekFile(file, off + InternalDirectoryFileHeader::size);
                quint16 eightHundred = readUnsignedWord(file);
                PRINT_DBG("        Eight hundred: %d", eightHundred);
                quint16 numPhrases = readUnsignedWord(file);
                PRINT_DBG("        Num phrases: %d", numPhrases);
                quint16 oneHundred = readUnsignedWord(file);
                PRINT_DBG("        One hundred: %d", oneHundred);
                if (oneHundred != 0x0100)
                {
                    throw std::runtime_error("Not a WinHelp phrase file");
                }
                quint32 uncompressedSize = readUnsignedDWord(file);
                PRINT_DBG("        Uncompressed size: %d", uncompressedSize);
                for (int i = 0; i < 30; i++)
                {
                    quint8 unused = readUnsignedByte(file);
                    PRINT_DBG("        Unused: %d", unused);
                }
                QScopedArrayPointer<uint> phraseOffset(
                    new uint[static_cast<size_t> (numPhrases + 1)]);
                PRINT_DBG("Reading phrase offsets at %lld", file.pos());
                for (quint16 index = 0; index < numPhrases + 1; index++)
                {
                    phraseOffset[index] = static_cast<uint> (readUnsignedWord(
                            file));
                    PRINT_DBG("        Phrase offset: %d", phraseOffset[index]);
                }
                qint64 inputLength = off + hdr.getReservedSpace() - file.pos();
                PRINT_DBG("Reading compressed phrases at %lld", file.pos());
                QScopedArrayPointer<quint8>
                uncompressedPhrases(
                    new quint8[static_cast<size_t> (uncompressedSize)]);
                unpackLZ77(file, file.pos(), inputLength,
                    uncompressedPhrases.data(),
                    static_cast<size_t> (uncompressedSize));
                size_t pointer = static_cast<size_t> (0);
                for (quint16 index = 0; index < numPhrases; index++)
                {
                    uint size = phraseOffset[index + 1] - phraseOffset[index];
                    QString phrase = readFixedLengthStringFromBuffer(
                        uncompressedPhrases.data(),
                        static_cast<size_t> (uncompressedSize),
                        pointer, size, codec);
                    this->phrases.append(phrase);
                    QScopedArrayPointer<quint8> phraseRaw(
                        new quint8[static_cast<size_t> (size)]);
                    copyBytesFromBuffer(
                        reinterpret_cast<const void *> (uncompressedPhrases.
                            data()),
                        static_cast<size_t> (uncompressedSize),
                        pointer, static_cast<size_t> (size),
                        reinterpret_cast<void *> (phraseRaw.data()),
                        static_cast<size_t> (size));
                    this->phrasesRaw.append(QByteArray(
                            reinterpret_cast<const char *> (phraseRaw.data()),
                            static_cast<int> (size)));
                    pointer += static_cast<size_t> (size);
                    PRINT_DBG("        Phrase: %s", phrase.toLocal8Bit().data());
                }
            }
            else
            {
                PRINT_DBG("Loading compressed WinHelp phrase file");
                seekFile(file, off + InternalDirectoryFileHeader::size);
                quint16 numPhrases = readUnsignedWord(file);
                PRINT_DBG("        Num phrases: %d", numPhrases);
                quint16 oneHundred = readUnsignedWord(file);
                PRINT_DBG("        One hundred: %d", oneHundred);
                if (oneHundred != 0x0100)
                {
                    throw std::runtime_error("Not a WinHelp phrase file");
                }
                quint32 uncompressedSize = readUnsignedDWord(file);
                PRINT_DBG("        Uncompressed size: %d", uncompressedSize);
                QScopedArrayPointer<uint> phraseOffset(
                    new uint[static_cast<size_t> (numPhrases + 1)]);
                PRINT_DBG("Reading phrase offsets at %lld", file.pos());
                for (quint16 index = 0; index < numPhrases + 1; index++)
                {
                    phraseOffset[index] = static_cast<uint> (readUnsignedWord(
                            file));
                    PRINT_DBG("        Phrase offset: %d", phraseOffset[index]);
                }
                PRINT_DBG("Reading compressed phrases at %lld", file.pos());
                qint64 inputLength = off + hdr.getReservedSpace() - file.pos();
                QScopedArrayPointer<quint8>
                uncompressedPhrases(
                    new quint8[static_cast<size_t> (uncompressedSize)]);
                unpackLZ77(file, file.pos(), inputLength,
                    uncompressedPhrases.data(),
                    static_cast<size_t> (uncompressedSize));
                size_t pointer = static_cast<size_t> (0);
                for (quint16 index = 0; index < numPhrases; index++)
                {
                    uint size = phraseOffset[index + 1] - phraseOffset[index];
                    QString phrase = readFixedLengthStringFromBuffer(
                        uncompressedPhrases.data(),
                        static_cast<size_t> (uncompressedSize),
                        pointer, size, codec);
                    this->phrases.append(phrase);
                    QScopedArrayPointer<quint8> phraseRaw(
                        new quint8[static_cast<size_t> (size)]);
                    copyBytesFromBuffer(
                        reinterpret_cast<const void *> (uncompressedPhrases.
                            data()),
                        static_cast<size_t> (uncompressedSize),
                        pointer, static_cast<size_t> (size),
                        reinterpret_cast<void *> (phraseRaw.data()),
                        static_cast<size_t> (size));
                    this->phrasesRaw.append(QByteArray(
                            reinterpret_cast<const char *> (phraseRaw.data()),
                            static_cast<int> (size)));
                    pointer += static_cast<size_t> (size);
                    PRINT_DBG("        Phrase: %s", phrase.toLocal8Bit().data());
                }
            }
        }
        else
        {
            PRINT_DBG("Loading compressed WinHelp phrase file");
            seekFile(file, off + InternalDirectoryFileHeader::size);
            quint16 numPhrases = readUnsignedWord(file);
            PRINT_DBG("        Num phrases: %d", numPhrases);
            quint16 oneHundred = readUnsignedWord(file);
            PRINT_DBG("        One hundred: %d", oneHundred);
            if (oneHundred != 0x0100)
            {
                throw std::runtime_error("Not a WinHelp phrase file");
            }
            quint32 uncompressedSize = readUnsignedDWord(file);
            PRINT_DBG("        Uncompressed size: %d", uncompressedSize);
            QScopedArrayPointer<uint> phraseOffset(
                new uint[static_cast<size_t> (numPhrases + 1)]);
            PRINT_DBG("Reading phrase offsets at %lld", file.pos());
            for (quint16 index = 0; index < numPhrases + 1; index++)
            {
                phraseOffset[index]
                    = static_cast<uint> (readUnsignedWord(file));
                PRINT_DBG("        Phrase offset: %d", phraseOffset[index]);
            }
            PRINT_DBG("Reading compressed phrases at %lld", file.pos());
            qint64 inputLength = off + hdr.getReservedSpace() - file.pos();
            QScopedArrayPointer<quint8> uncompressedPhrases(
                new quint8[static_cast<size_t> (uncompressedSize)]);
            unpackLZ77(file, file.pos(), inputLength,
                uncompressedPhrases.data(),
                static_cast<size_t> (uncompressedSize));
            size_t pointer = static_cast<size_t> (0);
            for (quint16 index = 0; index < numPhrases; index++)
            {
                uint size = phraseOffset[index + 1] - phraseOffset[index];
                QString phrase = readFixedLengthStringFromBuffer(
                    uncompressedPhrases.data(),
                    static_cast<size_t> (uncompressedSize), pointer,
                    size, codec);
                this->phrases.append(phrase);
                QScopedArrayPointer<quint8> phraseRaw(
                    new quint8[static_cast<size_t> (size)]);
                copyBytesFromBuffer(
                    reinterpret_cast<const void *> (uncompressedPhrases.data()),
                    static_cast<size_t> (uncompressedSize), pointer,
                    static_cast<size_t> (size),
                    reinterpret_cast<void *> (phraseRaw.data()),
                    static_cast<size_t> (size));
                this->phrasesRaw.append(QByteArray(
                        reinterpret_cast<const char *> (phraseRaw.data()),
                        static_cast<int> (size)));
                pointer += static_cast<size_t> (size);
                PRINT_DBG("        Phrase: %s", phrase.toLocal8Bit().data());
            }
        }
    }
    else
    {
        PRINT_DBG("Loading uncompressed WinHelp phrase file");
        seekFile(file, off + InternalDirectoryFileHeader::size);
        quint16 numPhrases = readUnsignedWord(file);
        PRINT_DBG("        Num phrases: %d", numPhrases);
        quint16 oneHundred = readUnsignedWord(file);
        PRINT_DBG("        One hundred: %d", oneHundred);
        if (oneHundred != 0x0100)
        {
            throw std::runtime_error("Not a WinHelp phrase file");
        }
        QScopedArrayPointer<uint> phraseOffset(
            new uint[static_cast<size_t> (numPhrases + 1)]);
        PRINT_DBG("Reading phrase offsets at %lld", file.pos());
        for (quint16 index = 0; index < numPhrases + 1; index++)
        {
            phraseOffset[index] = static_cast<uint> (readUnsignedWord(file));
            PRINT_DBG("        Phrase offset: %d", phraseOffset[index]);
        }
        PRINT_DBG("Reading phrases at %lld", file.pos());
        for (quint16 index = 0; index < numPhrases; index++)
        {
            uint size = phraseOffset[index + 1] - phraseOffset[index];
            qint64 posMem = file.pos();
            QString phrase = readFixedLengthString(file, size, codec);
            this->phrases.append(phrase);
            QScopedArrayPointer<quint8> phraseRaw(
                new quint8[static_cast<size_t> (size)]);
            seekFile(file, posMem);
            fillBuffer(file, static_cast<qint64> (size),
                reinterpret_cast<void *> (phraseRaw.data()),
                static_cast<size_t> (size));
            this->phrasesRaw.append(QByteArray(
                    reinterpret_cast<const char *> (phraseRaw.data()),
                    static_cast<int> (size)));
            PRINT_DBG("        Phrase: %s", phrase.toLocal8Bit().data());
        }
    }
    PRINT_DBG("WinHelp phrase file loaded successfully");
}

WinHelpPhraseFile::WinHelpPhraseFile(const WinHelpPhraseFile& rhs) :
    phrases(rhs.phrases), phrasesRaw(rhs.phrasesRaw)
{
    PRINT_DBG("WinHelp phrase file copy constructor");
}

WinHelpPhraseFile::~WinHelpPhraseFile()
{
    PRINT_DBG("WinHelp phrase file destructor");
}

WinHelpPhraseFile & WinHelpPhraseFile::operator=(const WinHelpPhraseFile & rhs)
{
    PRINT_DBG("WinHelp phrase file assignment operator");
    if (this != &rhs)
    {
        this->phrases = rhs.phrases;
        this->phrasesRaw = rhs.phrasesRaw;
    }
    return *this;
}

int WinHelpPhraseFile::getPhrasesCount() const
{
    return phrases.count();
}

QString WinHelpPhraseFile::getPhrase(int index) const
{
    if ((index < 0) || (index >= phrases.size()))
    {
        throw new std::out_of_range("Index of phrase is out of range");
    }
    return phrases.at(index);
}

QByteArray WinHelpPhraseFile::getRawPhrase(int index) const
{
    if ((index < 0) || (index >= phrases.size()))
    {
        throw new std::out_of_range("Index of phrase is out of range");
    }
    return phrasesRaw.at(index);
}
