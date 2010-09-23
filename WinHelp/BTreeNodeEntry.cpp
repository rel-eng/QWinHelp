/* BTree node entry class implementation.

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

#include "BTreeNodeEntry.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/TextUtils.h"

BTreeNodeEntry::BTreeNodeEntry() :
    fileName(""), offset(0), sz(Q_INT64_C(0))
{
    PRINT_DBG("BTree node entry default constructor");
}

BTreeNodeEntry::BTreeNodeEntry(const QString &fileName, qint64 offset) :
    fileName(fileName), offset(offset), sz(fileName.size() + Q_INT64_C(5))
{
    PRINT_DBG("BTree node entry constructor");
}

BTreeNodeEntry::BTreeNodeEntry(QFile &file, qint64 off)
{
    PRINT_DBG("Reading BTree node entry at %lld", off);
    seekFile(file, off);
    QTextCodec *codec = getDefaultTextCodec();
    qint64 length = 0;
    this->fileName = readNullTerminatedString(file, codec, length);
    PRINT_DBG("        Filename: %s", this->fileName.toLocal8Bit().data());
    this->offset = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Offset: %lld", this->offset);
    this->sz = length + Q_INT64_C(4);
    PRINT_DBG("        BTree node entry size: %lld", this->sz);
    PRINT_DBG("BTree node entry loaded successfully");
}

BTreeNodeEntry::BTreeNodeEntry(const BTreeNodeEntry& rhs) :
    fileName(rhs.fileName), offset(rhs.offset), sz(rhs.sz)
{
    PRINT_DBG("BTree node entry copy constructor");
}

BTreeNodeEntry::~BTreeNodeEntry()
{
    PRINT_DBG("BTree node entry destructor");
}

QString BTreeNodeEntry::getFileName() const
{
    return fileName;
}

qint64 BTreeNodeEntry::getOffset() const
{
    return offset;
}

BTreeNodeEntry & BTreeNodeEntry::operator=(const BTreeNodeEntry & rhs)
{
    PRINT_DBG("BTree node entry assignment operator");
    if (this != &rhs)
    {
        this->fileName = rhs.fileName;
        this->offset = rhs.offset;
        this->sz = rhs.sz;
    }
    return *this;
}

qint64 BTreeNodeEntry::size() const
{
    return sz;
}
