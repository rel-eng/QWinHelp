/* Configuration record class implementation.

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

#include "ConfigRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "SystemRecordHeader.h"

ConfigRecord::ConfigRecord() :
    macro()
{
    PRINT_DBG("Config record default constructor");
}

ConfigRecord::ConfigRecord(const QString &macro) :
    macro(macro)
{
    PRINT_DBG("Config record constructor");
}

ConfigRecord::ConfigRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Config record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->macro =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        Macro: %s", this->macro.toLocal8Bit().data());
    PRINT_DBG("Config record loaded successfully");
}

ConfigRecord::ConfigRecord(const ConfigRecord& rhs) :
    macro(rhs.macro)
{
    PRINT_DBG("Config record copy constructor");
}

ConfigRecord & ConfigRecord::operator=(const ConfigRecord & rhs)
{
    PRINT_DBG("Config record assignment operator");
    if (this != &rhs)
    {
        this->macro = rhs.macro;
    }
    return *this;
}

ConfigRecord::~ConfigRecord()
{
    PRINT_DBG("Config record destructor");
}

QString ConfigRecord::getMacro() const
{
    return this->macro;
}
