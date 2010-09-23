/* Icon record class implementation.

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

#include "IconRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "SystemRecordHeader.h"

IconRecord::IconRecord() :
    icon()
{
    PRINT_DBG("Icon record default constructor");
}

IconRecord::IconRecord(QFile &file, qint64 off)
{
    PRINT_DBG("Reading Icon record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->icon = WinHelpIcon(file, off + SystemRecordHeader::size);
    PRINT_DBG("Icon record loaded successfully");
}

IconRecord::IconRecord(const IconRecord& rhs) :
    icon(rhs.icon)
{
    PRINT_DBG("Icon record copy constructor");
}

IconRecord & IconRecord::operator=(const IconRecord & rhs)
{
    PRINT_DBG("Icon record assignment operator");
    if (this != &rhs)
    {
        this->icon = rhs.icon;
    }
    return *this;
}

IconRecord::~IconRecord()
{
    PRINT_DBG("Icon record destructor");
}

const WinHelpIcon & IconRecord::getIcon() const
{
    return this->icon;
}
