/* Ftindex record class implementation.

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

#include "FtindexRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

FtindexRecord::FtindexRecord() :
    dtype()
{
    PRINT_DBG("Ftindex record default constructor");
}

FtindexRecord::FtindexRecord(const QString &dtype) :
    dtype(dtype)
{
    PRINT_DBG("Ftindex record constructor");
}

FtindexRecord::FtindexRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Ftindex record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->dtype =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        DType: %s", this->dtype.toLocal8Bit().data());
    PRINT_DBG("Ftindex record loaded successfully");
}

FtindexRecord::FtindexRecord(const FtindexRecord& rhs) :
    dtype(rhs.dtype)
{
    PRINT_DBG("Ftindex record copy constructor");
}

FtindexRecord & FtindexRecord::operator=(const FtindexRecord & rhs)
{
    PRINT_DBG("Ftindex record assignment operator");
    if (this != &rhs)
    {
        this->dtype = rhs.dtype;
    }
    return *this;
}

FtindexRecord::~FtindexRecord()
{
    PRINT_DBG("Ftindex record destructor");
}

QString FtindexRecord::getDType() const
{
    return this->dtype;
}
