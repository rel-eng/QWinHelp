/* Context BTree node entry implementation.

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

#include "ContextBTreeNodeEntry.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/TextUtils.h"

ContextBTreeNodeEntry::ContextBTreeNodeEntry() :
    hash(0), offset()
{
    PRINT_DBG("Context BTree node entry default constructor");
}

ContextBTreeNodeEntry::ContextBTreeNodeEntry(quint32 hash,
    TopicOffset offset) :
    hash(hash), offset(offset)
{
    PRINT_DBG("Context BTree node entry constructor");
}

ContextBTreeNodeEntry::ContextBTreeNodeEntry(QFile &file, qint64 off)
{
    PRINT_DBG("Reading Context BTree node entry at %lld", off);
    seekFile(file, off);
    this->hash = readUnsignedDWord(file);
    PRINT_DBG("        Hash: %d", this->hash);
    PRINT_DBG("        Offset:");
    this->offset = TopicOffset(file, off + Q_INT64_C(4));
    PRINT_DBG("Context BTree node entry loaded successfully");
}

ContextBTreeNodeEntry::ContextBTreeNodeEntry(const ContextBTreeNodeEntry& rhs)
    :
    hash(rhs.hash), offset(rhs.offset)
{
    PRINT_DBG("Context BTree node entry copy constructor");
}

ContextBTreeNodeEntry::~ContextBTreeNodeEntry()
{
    PRINT_DBG("Context BTree node entry destructor");
}

quint32 ContextBTreeNodeEntry::getHash() const
{
    return hash;
}

TopicOffset ContextBTreeNodeEntry::getOffset() const
{
    return offset;
}

ContextBTreeNodeEntry & ContextBTreeNodeEntry::operator=(
    const ContextBTreeNodeEntry & rhs)
{
    PRINT_DBG("Context BTree node entry assignment operator");
    if (this != &rhs)
    {
        this->hash = rhs.hash;
        this->offset = rhs.offset;
    }
    return *this;
}
