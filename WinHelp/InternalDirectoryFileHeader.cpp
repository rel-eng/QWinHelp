/* Internal directory file header class implementation.

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

#include "InternalDirectoryFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

InternalDirectoryFileHeader::InternalDirectoryFileHeader() :
    reservedSpace(0), usedSpace(0), fileFlags(0)
{
    PRINT_DBG("Internal directory file header default constructor");
}

InternalDirectoryFileHeader::InternalDirectoryFileHeader(qint64 reservedSpace,
    qint64 usedSpace, quint8 fileFlags) :
    reservedSpace(reservedSpace), usedSpace(usedSpace), fileFlags(fileFlags)
{
    PRINT_DBG("Internal directory file header constructor");
}

InternalDirectoryFileHeader::~InternalDirectoryFileHeader()
{
    PRINT_DBG("Internal directory file header destructor");
}

InternalDirectoryFileHeader::InternalDirectoryFileHeader(QFile &file,
    qint64 off)
{
    PRINT_DBG("Reading Internal directory file header at: %lld", off);
    seekFile(file, off);
    this->reservedSpace = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Reserved space: %lld", this->reservedSpace);
    this->usedSpace = static_cast<qint64> (readUnsignedDWord(file));
    PRINT_DBG("        Used space: %lld", this->usedSpace);
    this->fileFlags = readUnsignedByte(file);
    if (fileFlags != 4)
    {
        qDebug() << "File flags != 4";
    }
    PRINT_DBG("        Flags: %d", this->fileFlags);
    PRINT_DBG("Internal directory file header loaded successfully");
}

InternalDirectoryFileHeader::InternalDirectoryFileHeader(
    const InternalDirectoryFileHeader& rhs) :
    reservedSpace(rhs.reservedSpace), usedSpace(rhs.usedSpace), fileFlags(
    rhs.fileFlags)
{
    PRINT_DBG("Internal directory file header copy constructor");
}

quint8 InternalDirectoryFileHeader::getFileFlags() const
{
    return fileFlags;
}

qint64 InternalDirectoryFileHeader::getReservedSpace() const
{
    return reservedSpace;
}

qint64 InternalDirectoryFileHeader::getUsedSpace() const
{
    return usedSpace;
}

InternalDirectoryFileHeader & InternalDirectoryFileHeader::operator=(
    const InternalDirectoryFileHeader & rhs)
{
    PRINT_DBG("Internal directory file header assignment operator");
    if (this != &rhs)
    {
        fileFlags = rhs.fileFlags;
        reservedSpace = rhs.reservedSpace;
        usedSpace = rhs.usedSpace;
    }
    return *this;
}
