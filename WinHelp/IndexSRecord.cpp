/* IndexS record class implementation.

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

#include "IndexSRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

IndexSRecord::IndexSRecord() :
    indexSeparators()
{
    PRINT_DBG("IndexS record default constructor");
}

IndexSRecord::IndexSRecord(const QString &indexSeparators) :
    indexSeparators(indexSeparators)
{
    PRINT_DBG("IndexS record constructor");
}

IndexSRecord::IndexSRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading IndexS record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->indexSeparators =
        readFixedLengthString(file, static_cast<uint>(hdr.getRecordSize()),
        codec);
    PRINT_DBG("        Index separators: %s", this->indexSeparators.toLocal8Bit(
            ).data());
    PRINT_DBG("IndexS record loaded successfully");
}

IndexSRecord::IndexSRecord(const IndexSRecord& rhs) :
    indexSeparators(rhs.indexSeparators)
{
    PRINT_DBG("IndexS record copy constructor");
}

IndexSRecord & IndexSRecord::operator=(const IndexSRecord & rhs)
{
    PRINT_DBG("IndexS record assignment operator");
    if (this != &rhs)
    {
        this->indexSeparators = rhs.indexSeparators;
    }
    return *this;
}

IndexSRecord::~IndexSRecord()
{
    PRINT_DBG("IndexS record destructor");
}

QString IndexSRecord::getIndexSeparators() const
{
    return this->indexSeparators;
}
