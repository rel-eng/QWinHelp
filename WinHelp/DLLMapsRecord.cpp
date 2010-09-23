/* DLL maps record class implementation.

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

#include "DLLMapsRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

DLLMapsRecord::DLLMapsRecord() : win16RetailDLL(), win16DebugDLL(),
    win32RetailDLL(), win32DebugDLL()
{
    PRINT_DBG("DLLMaps record default constructor");
}

DLLMapsRecord::DLLMapsRecord(const QString &win16RetailDLL,
    const QString &win16DebugDLL,
    const QString &win32RetailDLL,
    const QString &win32DebugDLL) : win16RetailDLL(win16RetailDLL),
    win16DebugDLL(win16DebugDLL), win32RetailDLL(win32RetailDLL),
    win32DebugDLL(win32DebugDLL)
{
    PRINT_DBG("DLLMaps record constructor");
}

DLLMapsRecord::DLLMapsRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading DLLMaps record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    qint64 win16RetailDLLLength = 0;
    this->win16RetailDLL = readNullTerminatedString(file,
        codec,
        win16RetailDLLLength);
    PRINT_DBG("        Win16 retail DLL: %s",
        this->win16RetailDLL.toLocal8Bit().data());
    PRINT_DBG("        Win16 retail DLL length: %lld", win16RetailDLLLength);
    qint64 win16DebugDLLLength = 0;
    this->win16DebugDLL = readNullTerminatedString(file,
        codec,
        win16DebugDLLLength);
    PRINT_DBG("        Win16 debug DLL: %s",
        this->win16DebugDLL.toLocal8Bit().data());
    PRINT_DBG("        Win16 debug DLL length: %lld", win16DebugDLLLength);
    qint64 win32RetailDLLLength = 0;
    this->win32RetailDLL = readNullTerminatedString(file,
        codec,
        win32RetailDLLLength);
    PRINT_DBG("        Win32 retail DLL: %s",
        this->win32RetailDLL.toLocal8Bit().data());
    PRINT_DBG("        Win32 retail DLL length: %lld", win32RetailDLLLength);
    qint64 win32DebugDLLLength = 0;
    this->win32DebugDLL = readNullTerminatedString(file,
        codec,
        win32DebugDLLLength);
    PRINT_DBG("        Win32 debug DLL: %s",
        this->win32DebugDLL.toLocal8Bit().data());
    PRINT_DBG("        Win32 debug DLL length: %lld", win32DebugDLLLength);
    PRINT_DBG("DLLMaps record loaded successfully");
}

DLLMapsRecord::DLLMapsRecord(const DLLMapsRecord& rhs) : win16RetailDLL(rhs.
    win16RetailDLL), win16DebugDLL(rhs.win16DebugDLL),
    win32RetailDLL(rhs.win32RetailDLL), win32DebugDLL(rhs.win32DebugDLL)
{
    PRINT_DBG("DLLMaps record copy constructor");
}

DLLMapsRecord & DLLMapsRecord::operator=(const DLLMapsRecord & rhs)
{
    PRINT_DBG("DLLMaps record assignment operator");
    if (this != &rhs)
    {
        this->win16RetailDLL = rhs.win16RetailDLL;
        this->win16DebugDLL = rhs.win16DebugDLL;
        this->win32RetailDLL = rhs.win32RetailDLL;
        this->win32DebugDLL = rhs.win32DebugDLL;
    }
    return *this;
}

DLLMapsRecord::~DLLMapsRecord()
{
    PRINT_DBG("DLLMaps record destructor");
}

QString DLLMapsRecord::getWin16RetailDLL() const
{
    return this->win16RetailDLL;
}

QString DLLMapsRecord::getWin16DebugDLL() const
{
    return this->win16DebugDLL;
}

QString DLLMapsRecord::getWin32RetailDLL() const
{
    return this->win32RetailDLL;
}

QString DLLMapsRecord::getWin32DebugDLL() const
{
    return this->win32DebugDLL;
}
