/* WinHelp file header class implementation.

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

#include "WinHelpFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

WinHelpFileHeader::WinHelpFileHeader() :
    directoryStart(0), firstFreeBlock(0), entireFileSize(0)
{
    PRINT_DBG("WinHelp file header default constructor");
}

WinHelpFileHeader::WinHelpFileHeader(qint64 directoryStart,
    qint64 firstFreeBlock, qint64 entireFileSize) :
    directoryStart(directoryStart), firstFreeBlock(firstFreeBlock),
    entireFileSize(entireFileSize)
{
    PRINT_DBG("WinHelp file header constructor");
}

WinHelpFileHeader::WinHelpFileHeader(QFile &file, qint64 off)
{
    PRINT_DBG("Reading WinHelp file header at %lld", off);
    seekFile(file, off);
    quint32 magicIn = readUnsignedDWord(file);
    if (magicIn != magic)
    {
        throw std::runtime_error("Not a WinHelp file header");
    }
    PRINT_DBG("        Magic: %d", magicIn);
    this->directoryStart = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Directory start: %lld", this->directoryStart);
    this->firstFreeBlock = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        FirstFreeBlock: %lld", this->firstFreeBlock);
    this->entireFileSize = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Entire file size: %lld", this->entireFileSize);
    PRINT_DBG("WinHelp file header loaded successfully");
}

WinHelpFileHeader::WinHelpFileHeader(const WinHelpFileHeader& rhs) :
    directoryStart(rhs.directoryStart), firstFreeBlock(rhs.firstFreeBlock),
    entireFileSize(rhs.entireFileSize)
{
    PRINT_DBG("WinHelp file header copy constructor");
}

WinHelpFileHeader & WinHelpFileHeader::operator=(const WinHelpFileHeader & rhs)
{
    PRINT_DBG("WinHelp file header assignment operator");
    if (this != &rhs)
    {
        directoryStart = rhs.directoryStart;
        entireFileSize = rhs.entireFileSize;
        firstFreeBlock = rhs.firstFreeBlock;
    }
    return *this;
}

qint64 WinHelpFileHeader::getDirectoryStart() const
{
    return directoryStart;
}

qint64 WinHelpFileHeader::getEntireFileSize() const
{
    return entireFileSize;
}

qint64 WinHelpFileHeader::getFirstFreeBlock() const
{
    return firstFreeBlock;
}

WinHelpFileHeader::~WinHelpFileHeader()
{
    PRINT_DBG("WinHelp file header destructor");
}
