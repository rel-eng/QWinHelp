/* Context BTree index entry implementation.

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

#include "ContextBTreeIndexEntry.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

ContextBTreeIndexEntry::ContextBTreeIndexEntry() : hash(0), pageNumber(0)
{
    PRINT_DBG("Context BTree index entry default constructor");
}

ContextBTreeIndexEntry::ContextBTreeIndexEntry(quint32 hash,
    quint16 pageNumber) :
    hash(hash), pageNumber(pageNumber)
{
    PRINT_DBG("Context BTree index entry constructor");
}

ContextBTreeIndexEntry::ContextBTreeIndexEntry(QFile &file, qint64 off)
{
    PRINT_DBG("Reading Context BTree index entry at %lld", off);
    seekFile(file, off);
    this->hash = readUnsignedDWord(file);
    PRINT_DBG("        Hash: %d", this->hash);
    this->pageNumber = readUnsignedWord(file);
    PRINT_DBG("        Page Number: %d", this->pageNumber);
    PRINT_DBG("Context BTree index entry loaded successfully");
}

ContextBTreeIndexEntry::ContextBTreeIndexEntry(
    const ContextBTreeIndexEntry& rhs) :
    hash(rhs.hash), pageNumber(rhs.pageNumber)
{
    PRINT_DBG("Context BTree index entry copy constructor");
}

ContextBTreeIndexEntry::~ContextBTreeIndexEntry()
{
    PRINT_DBG("Context BTree index entry destructor");
}

quint32 ContextBTreeIndexEntry::getHash() const
{
    return hash;
}

quint16 ContextBTreeIndexEntry::getPageNumber() const
{
    return pageNumber;
}

ContextBTreeIndexEntry & ContextBTreeIndexEntry::operator=(
    const ContextBTreeIndexEntry & rhs)
{
    PRINT_DBG("Context BTree index entry assignment operator");
    if (this != &rhs)
    {
        this->hash = rhs.hash;
        this->pageNumber = rhs.pageNumber;
    }
    return *this;
}
