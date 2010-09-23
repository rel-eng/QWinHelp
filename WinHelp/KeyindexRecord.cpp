/* Keyindex record class implementation.

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

#include "KeyindexRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

KeyindexRecord::KeyindexRecord() :
    btreename(), mapname(), dataname(), title()
{
    PRINT_DBG("Keyindex record default constructor");
}

KeyindexRecord::KeyindexRecord(const QString &btreename,
    const QString &mapname, const QString &dataname, const QString &title) :
    btreename(btreename), mapname(mapname), dataname(dataname), title(title)
{
    PRINT_DBG("Keyindex record constructor");
}

KeyindexRecord::KeyindexRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Keyindex record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->btreename = readFixedLengthString(file, 10, codec);
    PRINT_DBG("        BtreeName: %s", this->btreename.toLocal8Bit().data());
    this->mapname = readFixedLengthString(file, 10, codec);
    PRINT_DBG("        MapName: %s", this->mapname.toLocal8Bit().data());
    this->dataname = readFixedLengthString(file, 10, codec);
    PRINT_DBG("        DataName: %s", this->dataname.toLocal8Bit().data());
    this->title = readFixedLengthString(file, 80, codec);
    PRINT_DBG("        Title: %s", this->title.toLocal8Bit().data());
    PRINT_DBG("Keyindex record loaded successfully");
}

KeyindexRecord::KeyindexRecord(const KeyindexRecord& rhs) :
    btreename(rhs.btreename), mapname(rhs.mapname), dataname(rhs.dataname),
    title(rhs.title)
{
    PRINT_DBG("Keyindex record copy constructor");
}

KeyindexRecord & KeyindexRecord::operator=(const KeyindexRecord & rhs)
{
    PRINT_DBG("Keyindex record assignment operator");
    if (this != &rhs)
    {
        this->btreename = rhs.btreename;
        this->mapname = rhs.mapname;
        this->dataname = rhs.dataname;
        this->title = rhs.title;
    }
    return *this;
}

KeyindexRecord::~KeyindexRecord()
{
    PRINT_DBG("Keyindex record destructor");
}

QString KeyindexRecord::getBtreeName() const
{
    return this->btreename;
}

QString KeyindexRecord::getMapName() const
{
    return this->mapname;
}

QString KeyindexRecord::getDataName() const
{
    return this->dataname;
}

QString KeyindexRecord::getTitle() const
{
    return this->title;
}
