/* WinHelp PhrIndex file class implementation.

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

#include "WinHelpPhrIndexFile.h"

#include "InternalDirectoryFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

#include <stdexcept>

WinHelpPhrIndexFile::WinHelpPhrIndexFile() :
    phrImageSize(0), phrImageCompressedSize(0), phrasesOffset(), phrasesBits()
{
    PRINT_DBG("WinHelp PhrIndex file default constructor");
}

WinHelpPhrIndexFile::WinHelpPhrIndexFile(QFile &file, qint64 off) :
    phrasesOffset(), phrasesBits()
{
    PRINT_DBG("Loading WinHelp PhrIndex file at %lld", off);
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    seekFile(file, off + InternalDirectoryFileHeader::size);
    quint32 magic = readUnsignedDWord(file);
    PRINT_DBG("        Magic: %d", magic);
    if (magic != 1)
    {
        throw std::runtime_error("Not a PhrIndex file");
    }
    quint32 nEntries = readUnsignedDWord(file);
    PRINT_DBG("        nEntries: %d", nEntries);
    quint32 compressedSize = readUnsignedDWord(file);
    PRINT_DBG("        Compressed size: %d", compressedSize);
    this->phrImageSize = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        phrImage size: %lld", this->phrImageSize);
    this->phrImageCompressedSize
        = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        phrImage compressed size: %lld",
        this->phrImageCompressedSize);
    quint32 alwaysZero = readUnsignedDWord(file);
    PRINT_DBG("        Always zero: %d", alwaysZero);
    if (alwaysZero != 0)
    {
        qDebug() << "Always zero phrIndex file field is non zero";
    }
    quint8 bitfieldL = readUnsignedByte(file);
    PRINT_DBG("        Bitfield low byte: %d", bitfieldL);
    quint8 bitfieldH = readUnsignedByte(file);
    PRINT_DBG("        Bitfield high byte: %d", bitfieldH);
    quint8 bitCount = bitfieldL & 0x0F;
    PRINT_DBG("        Bitcount: %d", bitCount);
    quint16 unknownValue = readUnsignedWord(file);
    PRINT_DBG("        Unknown value: %d", unknownValue);
    this->phrasesOffset.resize(nEntries + 1);
    this->phrasesOffset[0] = 0;
    PRINT_DBG("        Phrase offset: %d", this->phrasesOffset[0]);
    IntAllignedCursor cur;
    cur.currentBitIndex = 0;
    cur.currentDwordByte0 = 0;
    cur.currentDwordByte1 = 0;
    cur.currentDwordByte2 = 0;
    cur.currentDwordByte3 = 0;
    for (quint32 i = 0; i < nEntries; i++)
    {
        uint n = 1;
        while (this->getNextBitIntAlligned(file, cur))
        {
            n += static_cast<uint> (0x0001 << bitCount);
        }
        if (this->getNextBitIntAlligned(file, cur))
        {
            n += 1;
        }
        if (bitCount > 1)
        {
            if (this->getNextBitIntAlligned(file, cur))
            {
                n += 2;
            }
        }
        if (bitCount > 2)
        {
            if (this->getNextBitIntAlligned(file, cur))
            {
                n += 4;
            }
        }
        if (bitCount > 3)
        {
            if (this->getNextBitIntAlligned(file, cur))
            {
                n += 8;
            }
        }
        if (bitCount > 4)
        {
            if (this->getNextBitIntAlligned(file, cur))
            {
                n += 16;
            }
        }
        this->phrasesOffset[i + 1] = this->phrasesOffset[i] + n;
        PRINT_DBG("        Phrase offset: %d", this->phrasesOffset[i + 1]);
    }
    this->phrasesBits.resize(nEntries);
    ByteAllignedCursor bCur;
    bCur.currentBitIndex = 0;
    bCur.currentByte = 0;
    for (quint32 index = 0; index < nEntries; index++)
    {
        if (this->getNextBitByteAlligned(file, bCur))
        {
            this->phrasesBits[index] = true;
            PRINT_DBG("        Phrase bit is true");
        }
        else
        {
            this->phrasesBits[index] = false;
            PRINT_DBG("        Phrase bit is false");
        }
    }
    PRINT_DBG("WinHelp PhrIndex file loaded successfully");
}

WinHelpPhrIndexFile::WinHelpPhrIndexFile(const WinHelpPhrIndexFile& rhs) :
    phrImageSize(rhs.phrImageSize), phrImageCompressedSize(
    rhs.phrImageCompressedSize), phrasesOffset(rhs.phrasesOffset),
    phrasesBits(rhs.phrasesBits)
{
    PRINT_DBG("WinHelp PhrIndex file copy constructor");
}

WinHelpPhrIndexFile::~WinHelpPhrIndexFile()
{
    PRINT_DBG("WinHelp PhrIndex file destructor");
}

WinHelpPhrIndexFile & WinHelpPhrIndexFile::operator=(
    const WinHelpPhrIndexFile & rhs)
{
    PRINT_DBG("WinHelp PhrIndex file assignment operator");
    if (this != &rhs)
    {
        this->phrImageSize = rhs.phrImageSize;
        this->phrImageCompressedSize = rhs.phrImageCompressedSize;
        this->phrasesOffset = rhs.phrasesOffset;
        this->phrasesBits = rhs.phrasesBits;
    }
    return *this;
}

bool WinHelpPhrIndexFile::getNextBitIntAlligned(QFile &file,
    IntAllignedCursor &cur)
{
    if (cur.currentBitIndex == 0)
    {
        cur.currentDwordByte0 = readUnsignedByte(file);
        cur.currentDwordByte1 = readUnsignedByte(file);
        cur.currentDwordByte2 = readUnsignedByte(file);
        cur.currentDwordByte3 = readUnsignedByte(file);
    }
    bool result = false;
    quint8 mask = 0x01 << (cur.currentBitIndex % 8);
    switch (cur.currentBitIndex / 8)
    {
    case 0:
        if ((cur.currentDwordByte0 & mask) != 0)
        {
            result = true;
        }
        break;

    case 1:
        if ((cur.currentDwordByte1 & mask) != 0)
        {
            result = true;
        }
        break;

    case 2:
        if ((cur.currentDwordByte2 & mask) != 0)
        {
            result = true;
        }
        break;

    case 3:
        if ((cur.currentDwordByte3 & mask) != 0)
        {
            result = true;
        }
        break;

    default:
        throw std::runtime_error(
            "Unable to unpack bitcompressed phrases offset");
    }
    cur.currentBitIndex++;
    if (cur.currentBitIndex > 31)
    {
        cur.currentBitIndex = 0;
    }
    return result;
}

bool WinHelpPhrIndexFile::getNextBitByteAlligned(QFile &file,
    ByteAllignedCursor &cur)
{
    if (cur.currentBitIndex == 0)
    {
        cur.currentByte = readUnsignedByte(file);
    }
    bool result = false;
    quint8 mask = 0x01 << cur.currentBitIndex;
    if ((cur.currentByte & mask) != 0)
    {
        result = true;
    }
    cur.currentBitIndex++;
    if (cur.currentBitIndex > 7)
    {
        cur.currentBitIndex = 0;
    }
    return result;
}

qint64 WinHelpPhrIndexFile::getPhrImageSize() const
{
    return this->phrImageSize;
}

qint64 WinHelpPhrIndexFile::getPhrImageCompressedSize() const
{
    return this->phrImageCompressedSize;
}

int WinHelpPhrIndexFile::getPhraseOffsetsCount() const
{
    return this->phrasesOffset.count();
}

uint WinHelpPhrIndexFile::getPhraseOffset(int index) const
{
    if ((index < 0) || (index >= phrasesOffset.size()))
    {
        throw new std::out_of_range("Index of phrase offset is out of range");
    }
    return phrasesOffset.at(index);
}

int WinHelpPhrIndexFile::getPhraseBitsCount() const
{
    return this->phrasesBits.count();
}

bool WinHelpPhrIndexFile::getPhraseBit(int index) const
{
    if ((index < 0) || (index >= phrasesBits.size()))
    {
        throw new std::out_of_range("Index of phrase bit is out of range");
    }
    return phrasesBits.at(index);
}
