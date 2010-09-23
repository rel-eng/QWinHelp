/* Copyright record class implementation.

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

#include "CopyrightRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

CopyrightRecord::CopyrightRecord() :
    copyright()
{
    PRINT_DBG("Copyright record default constructor");
}

CopyrightRecord::CopyrightRecord(const QString &copyright) :
    copyright(copyright)
{
    PRINT_DBG("Copyright record constructor");
}

CopyrightRecord::CopyrightRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Copyright record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->copyright =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        Copyright: %s", this->copyright.toLocal8Bit().data());
    PRINT_DBG("Copyright record loaded successfully");
}

CopyrightRecord::CopyrightRecord(const CopyrightRecord& rhs) :
    copyright(rhs.copyright)
{
    PRINT_DBG("Copyright record copy constructor");
}

CopyrightRecord & CopyrightRecord::operator=(const CopyrightRecord & rhs)
{
    PRINT_DBG("Copyright record assignment operator");
    if (this != &rhs)
    {
        this->copyright = rhs.copyright;
    }
    return *this;
}

CopyrightRecord::~CopyrightRecord()
{
    PRINT_DBG("Copyright record destructor");
}

QString CopyrightRecord::getCopyright() const
{
    return this->copyright;
}
