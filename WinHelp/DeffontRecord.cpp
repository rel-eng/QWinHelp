/* Deffont record class implementation.

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

#include "DeffontRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

DeffontRecord::DeffontRecord() :
    heightInPoints(0), charset(0), fontName()
{
    PRINT_DBG("Deffont record default constructor");
}

DeffontRecord::DeffontRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Deffont record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->heightInPoints = readUnsignedByte(file);
    PRINT_DBG("        Font height in points %d", this->heightInPoints);
    this->charset = readUnsignedByte(file);
    PRINT_DBG("        Font charset %d", this->charset);
    this->fontName =
        readFixedLengthString(file, static_cast<uint>(hdr.getRecordSize())
        - Q_INT64_C(2), codec);
    PRINT_DBG("        Font name: %s", this->fontName.toLocal8Bit().data());
    PRINT_DBG("Deffont record loaded successfully");
}

DeffontRecord::DeffontRecord(const DeffontRecord& rhs) :
    heightInPoints(rhs.heightInPoints), charset(rhs.charset), fontName(
    rhs.fontName)
{
    PRINT_DBG("Deffont record copy constructor");
}

DeffontRecord & DeffontRecord::operator=(const DeffontRecord & rhs)
{
    PRINT_DBG("Deffont record assignment operator");
    if (this != &rhs)
    {
        this->heightInPoints = rhs.heightInPoints;
        this->charset = rhs.charset;
        this->fontName = rhs.fontName;
    }
    return *this;
}

DeffontRecord::~DeffontRecord()
{
    PRINT_DBG("Deffont record destructor");
}

QString DeffontRecord::getFontName() const
{
    return this->fontName;
}

quint8 DeffontRecord::getFontCharset() const
{
    return this->charset;
}

quint8 DeffontRecord::getFontHeight() const
{
    return this->heightInPoints;
}
