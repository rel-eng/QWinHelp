/* Free header class implementation.

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

#include "FreeHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

FreeHeader::FreeHeader() :
    freeSpace(0), nextFreeBlock(0), hasNext(false)
{
    PRINT_DBG("Free header default constructor");
}

FreeHeader::FreeHeader(qint64 freeSpace, qint64 nextFreeBlock) :
    freeSpace(freeSpace), nextFreeBlock(nextFreeBlock), hasNext(nextFreeBlock
    != noMoreFreeBlocks)
{
    PRINT_DBG("Free header constructor");
}

FreeHeader::FreeHeader(QFile &file, qint64 off)
{
    PRINT_DBG("Reading Free header at %lld", off);
    seekFile(file, off);
    this->freeSpace = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Free space: %lld", this->freeSpace);
    this->nextFreeBlock = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Next free block: %lld", this->nextFreeBlock);
    this->hasNext = (this->nextFreeBlock != noMoreFreeBlocks);
    PRINT_DBG("Free header loaded successfully");
}

FreeHeader::FreeHeader(const FreeHeader& rhs) :
    freeSpace(rhs.freeSpace), nextFreeBlock(rhs.nextFreeBlock), hasNext(
    rhs.hasNext)
{
    PRINT_DBG("Free header copy constructor");
}

FreeHeader & FreeHeader::operator=(const FreeHeader & rhs)
{
    PRINT_DBG("Free header assignment operator");
    if (this != &rhs)
    {
        freeSpace = rhs.freeSpace;
        nextFreeBlock = rhs.nextFreeBlock;
        hasNext = rhs.hasNext;
    }
    return *this;
}

qint64 FreeHeader::getFreeSpace() const
{
    return freeSpace;
}

qint64 FreeHeader::getNextFreeBlock() const
{
    return nextFreeBlock;
}

bool FreeHeader::hasNextBlock() const
{
    return hasNext;
}

FreeHeader::~FreeHeader()
{
    PRINT_DBG("Free header destructor");
}
