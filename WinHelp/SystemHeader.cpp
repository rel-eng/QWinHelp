/* System header class implementation.

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

#include "SystemHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

#include <stdexcept>

SystemHeader::SystemHeader() :
    minorVersion(0), majorVersion(0), genDate(), flags(0)
{
    PRINT_DBG("System header default constructor");
}

SystemHeader::SystemHeader(quint16 minorVersion, quint16 majorVersion,
    QDateTime genDate, quint16 flags) :
    minorVersion(minorVersion), majorVersion(majorVersion), genDate(genDate),
    flags(flags)
{
    PRINT_DBG("System header constructor");
}

SystemHeader::SystemHeader(QFile &file, qint64 off) :
    genDate()
{
    PRINT_DBG("Reading System header at %lld", off);
    seekFile(file, off);
    quint16 magicIn = readUnsignedWord(file);
    if (magicIn != magic)
    {
        throw std::runtime_error("Not a system header");
    }
    PRINT_DBG("        Magic: %d", magicIn);
    this->minorVersion = readUnsignedWord(file);
    if ((this->minorVersion != hc30_format) && (this->minorVersion
            != hc31_format)
        && (this->minorVersion != SystemHeader::wmvc_format)
        && (this->minorVersion != hcw40_format))
    {
        qDebug() << "Unknown minor version: " << this->minorVersion;
    }
    PRINT_DBG("        Minor: %d", this->minorVersion);
    this->majorVersion = readUnsignedWord(file);
    if (this->majorVersion != 1)
    {
        qDebug() << "Unknown major version: " << this->majorVersion;
    }
    PRINT_DBG("        Major: %d", this->majorVersion);
    quint32 genDateIn = readUnsignedDWord(file);
    this->genDate = QDateTime::fromTime_t(static_cast<uint> (genDateIn));
    if (this->minorVersion == 27)
    {
        qDebug() << "Unable to decode genDate correctly";
    }
    PRINT_DBG("        genDate: %s", genDate.toString().toLocal8Bit().constData());
    PRINT_DBG("        genDate raw %d: ", genDateIn);
    this->flags = readUnsignedWord(file);
    if ((this->flags != not_compressed_4k_topic_block_flag) && (this->flags
            != lz77_compressed_4k_topic_block_flag) && (this->flags
            != lz77_compressed_2k_topic_block_flag))
    {
        qDebug() << "Unknown system header flags: " << this->flags;
    }
    PRINT_DBG("        Flags: %d", this->flags);
    PRINT_DBG("System header loaded successfully");
}

SystemHeader::SystemHeader(const SystemHeader& rhs) :
    minorVersion(rhs.minorVersion), majorVersion(rhs.majorVersion), genDate(
    rhs.genDate), flags(rhs.flags)
{
    PRINT_DBG("System header copy constructor");
}

quint16 SystemHeader::getFlags() const
{
    return flags;
}

QDateTime SystemHeader::getGenDate() const
{
    return genDate;
}

quint16 SystemHeader::getMajor() const
{
    return majorVersion;
}

quint16 SystemHeader::getMinor() const
{
    return minorVersion;
}

SystemHeader & SystemHeader::operator=(const SystemHeader & rhs)
{
    PRINT_DBG("System header assignment operator");
    if (this != &rhs)
    {
        this->minorVersion = rhs.minorVersion;
        this->majorVersion = rhs.majorVersion;
        this->genDate = rhs.genDate;
        this->flags = rhs.flags;
    }
    return *this;
}

SystemHeader::~SystemHeader()
{
    PRINT_DBG("System header destructor");
}
