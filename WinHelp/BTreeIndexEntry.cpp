/* BTree index entry class implementation.

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

#include "BTreeIndexEntry.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/TextUtils.h"

BTreeIndexEntry::BTreeIndexEntry() :
    fileName(""), pageNumber(0), sz(Q_INT64_C(0))
{
    PRINT_DBG("BTree index entry default constructor");
}

BTreeIndexEntry::BTreeIndexEntry(const QString &fileName, quint16 pageNumber) :
    fileName(fileName), pageNumber(pageNumber), sz(fileName.size()
    + Q_INT64_C(3))
{
    PRINT_DBG("BTree index entry constructor");
}

BTreeIndexEntry::BTreeIndexEntry(QFile &file, qint64 off)
{
    PRINT_DBG("Reading BTree index entry at %lld", off);
    seekFile(file, off);
    QTextCodec *codec = getDefaultTextCodec();
    qint64 length = 0;
    this->fileName = readNullTerminatedString(file, codec, length);
    PRINT_DBG("        Filename: %s", this->fileName.toLocal8Bit().data());
    this->pageNumber = readUnsignedWord(file);
    PRINT_DBG("        Page Number: %d", this->pageNumber);
    this->sz = length + Q_INT64_C(2);
    PRINT_DBG("        BTree index entry size: %lld", this->sz);
    PRINT_DBG("BTree index entry loaded successfully");
}

BTreeIndexEntry::BTreeIndexEntry(const BTreeIndexEntry& rhs) :
    fileName(rhs.fileName), pageNumber(rhs.pageNumber), sz(rhs.sz)
{
    PRINT_DBG("BTree index entry copy constructor");
}

BTreeIndexEntry::~BTreeIndexEntry()
{
    PRINT_DBG("BTree index entry destructor");
}

QString BTreeIndexEntry::getFileName() const
{
    return fileName;
}

quint16 BTreeIndexEntry::getPageNumber() const
{
    return pageNumber;
}

BTreeIndexEntry & BTreeIndexEntry::operator=(const BTreeIndexEntry & rhs)
{
    PRINT_DBG("BTree index entry assignment operator");
    if (this != &rhs)
    {
        this->fileName = rhs.fileName;
        this->pageNumber = rhs.pageNumber;
        this->sz = rhs.sz;
    }
    return *this;
}

qint64 BTreeIndexEntry::size() const
{
    return sz;
}
