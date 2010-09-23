/* Title record class implementation.

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

#include "TitleRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "SystemRecordHeader.h"

TitleRecord::TitleRecord() :
    title()
{
    PRINT_DBG("Title record default constructor");
}

TitleRecord::TitleRecord(const QString &title) :
    title(title)
{
    PRINT_DBG("Title record constructor");
}

TitleRecord::TitleRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Title record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->title =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        Title: %s", this->title.toLocal8Bit().data());
    PRINT_DBG("Title record loaded successfully");
}

TitleRecord::TitleRecord(const TitleRecord& rhs) :
    title(rhs.title)
{
    PRINT_DBG("Title record copy constructor");
}

TitleRecord & TitleRecord::operator=(const TitleRecord & rhs)
{
    PRINT_DBG("Title record assignment operator");
    if (this != &rhs)
    {
        this->title = rhs.title;
    }
    return *this;
}

TitleRecord::~TitleRecord()
{
    PRINT_DBG("Title record destructor");
}

QString TitleRecord::getTitle() const
{
    return this->title;
}
