/* System record header class implementation.

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

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

SystemRecordHeader::SystemRecordHeader() :
    recordType(0), recordSize(0)
{
    PRINT_DBG("System record header default constructor");
}

SystemRecordHeader::SystemRecordHeader(quint16 recordType, qint64 recordSize) :
    recordType(recordType), recordSize(recordSize)
{
    PRINT_DBG("System record header constructor");
}

SystemRecordHeader::SystemRecordHeader(QFile &file, qint64 off)
{
    PRINT_DBG("Reading System record header at %lld", off);
    seekFile(file, off);
    this->recordType = readUnsignedWord(file);
    PRINT_DBG("        RecordType: %d", this->recordType);
    this->recordSize = static_cast<qint64> (readUnsignedWord(file));
    PRINT_DBG("        RecordSize: %lld", this->recordSize);
    PRINT_DBG("System record header loaded successfully");
}

SystemRecordHeader::SystemRecordHeader(const SystemRecordHeader& rhs) :
    recordType(rhs.recordType), recordSize(rhs.recordSize)
{
    PRINT_DBG("System record header copy constructor");
}

SystemRecordHeader & SystemRecordHeader::operator=(
    const SystemRecordHeader & rhs)
{
    PRINT_DBG("System record header assignment operator");
    if (this != &rhs)
    {
        this->recordType = rhs.recordType;
        this->recordSize = rhs.recordSize;
    }
    return *this;
}

SystemRecordHeader::~SystemRecordHeader()
{
    PRINT_DBG("System record header destructor");
}

quint16 SystemRecordHeader::getRecordType() const
{
    return this->recordType;
}

qint64 SystemRecordHeader::getRecordSize() const
{
    return this->recordSize;
}
