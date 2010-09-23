/* Groups record class implementation.

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

#include "GroupsRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

GroupsRecord::GroupsRecord() :
    group()
{
    PRINT_DBG("Groups record default constructor");
}

GroupsRecord::GroupsRecord(const QString &group) :
    group(group)
{
    PRINT_DBG("Groups record constructor");
}

GroupsRecord::GroupsRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Groups record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->group =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        Group: %s", this->group.toLocal8Bit().data());
    PRINT_DBG("Groups record loaded successfully");
}

GroupsRecord::GroupsRecord(const GroupsRecord& rhs) :
    group(rhs.group)
{
    PRINT_DBG("Groups record copy constructor");
}

GroupsRecord & GroupsRecord::operator=(const GroupsRecord & rhs)
{
    PRINT_DBG("Groups record assignment operator");
    if (this != &rhs)
    {
        this->group = rhs.group;
    }
    return *this;
}

GroupsRecord::~GroupsRecord()
{
    PRINT_DBG("Groups record destructor");
}

QString GroupsRecord::getGroup() const
{
    return this->group;
}
