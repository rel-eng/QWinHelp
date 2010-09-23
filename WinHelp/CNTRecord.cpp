/* CNT record class implementation.

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

#include "CNTRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

CNTRecord::CNTRecord() :
    contentFileName()
{
    PRINT_DBG("CNT record default constructor");
}

CNTRecord::CNTRecord(const QString &contentFileName) :
    contentFileName(contentFileName)
{
    PRINT_DBG("CNT record constructor");
}

CNTRecord::CNTRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading CNT record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->contentFileName =
        readFixedLengthString(file, static_cast<uint>(hdr.getRecordSize()),
        codec);
    PRINT_DBG("        Content file name: %s",
        this->contentFileName.toLocal8Bit().data());
    PRINT_DBG("CNT record loaded successfully");
}

CNTRecord::CNTRecord(const CNTRecord& rhs) :
    contentFileName(rhs.contentFileName)
{
    PRINT_DBG("CNT record copy constructor");
}

CNTRecord & CNTRecord::operator=(const CNTRecord & rhs)
{
    PRINT_DBG("CNT record assignment operator");
    if (this != &rhs)
    {
        this->contentFileName = rhs.contentFileName;
    }
    return *this;
}

CNTRecord::~CNTRecord()
{
    PRINT_DBG("CNT record destructor");
}

QString CNTRecord::getContentFileName() const
{
    return this->contentFileName;
}
