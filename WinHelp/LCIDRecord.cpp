/* Locali identifier record class implementation.

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

#include "LCIDRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

LCIDRecord::LCIDRecord() :
    lcid(), ignoreNonSpacingMarksCaseInsensitive(false),
    ignoreSymbolsCaseInsensitive(false),
    ignoreNonSpacingMarksCaseSensitive(false),
    ignoreSymbolsCaseSensitive(false)
{
    PRINT_DBG("LCID record default constructor");
}

LCIDRecord::LCIDRecord(QFile &file, qint64 off)
{
    PRINT_DBG("Reading LCID record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    if (hdr.getRecordSize() == Q_INT64_C(10))
    {
        quint8 sortFlagsCaseInsensitive = readUnsignedByte(file);
        PRINT_DBG("        LCID record sort flags case insensitive: %d",
            sortFlagsCaseInsensitive);
        quint8 sortFlagsCaseSensitive = readUnsignedByte(file);
        PRINT_DBG("        LCID record sort flags case sensitive: %d",
            sortFlagsCaseSensitive);
        ignoreNonSpacingMarksCaseInsensitive = false;
        if ((sortFlagsCaseInsensitive & 0x02) != 0)
        {
            ignoreNonSpacingMarksCaseInsensitive = true;
            PRINT_DBG(
                "        LCID record ignore non spacing marks case insensitive is set.");
        }
        ignoreSymbolsCaseInsensitive = false;
        if ((sortFlagsCaseInsensitive & 0x04) != 0)
        {
            ignoreSymbolsCaseInsensitive = true;
            PRINT_DBG(
                "        LCID record ignore symbols case insensitive is set.");
        }
        ignoreNonSpacingMarksCaseSensitive = false;
        if ((sortFlagsCaseSensitive & 0x02) != 0)
        {
            ignoreNonSpacingMarksCaseSensitive = true;
            PRINT_DBG(
                "        LCID record ignore non spacing marks case sensitive is set.");
        }
        ignoreSymbolsCaseSensitive = false;
        if ((sortFlagsCaseSensitive & 0x04) != 0)
        {
            ignoreSymbolsCaseSensitive = true;
            PRINT_DBG(
                "        LCID record ignore symbols case sensitive is set.");
        }
        for (qint64 index = Q_INT64_C(2); index < (hdr.getRecordSize()
                - Q_INT64_C(2)); index++)
        {
            quint8 unknown = readUnsignedByte(file);
            PRINT_DBG("        LCID record unknown byte: %d", unknown);
        }
        quint8 lcidRawL = readUnsignedByte(file);
        PRINT_DBG("        LCID record LCID low byte: %d", lcidRawL);
        quint8 lcidRawH = readUnsignedByte(file);
        PRINT_DBG("        LCID record LCID high byte: %d", lcidRawH);
        quint16 primaryLanguageID = static_cast<quint16> (lcidRawL) + 256
            * static_cast<quint16> (lcidRawH & 0x03);
        PRINT_DBG("        LCID record primary language id: %d",
            primaryLanguageID);
        quint8 subLanguageID = (((lcidRawH & 0xFC) >> 2) & 0x3F);
        PRINT_DBG("        LCID record sub language id: %d", subLanguageID);
        this->lcid = LCID(primaryLanguageID, subLanguageID, 0, 0);
    }
    else
    {
        qDebug() << "Strange LCID record, using some workaround.";
        ignoreNonSpacingMarksCaseInsensitive = false;
        ignoreSymbolsCaseInsensitive = false;
        ignoreNonSpacingMarksCaseSensitive = false;
        ignoreSymbolsCaseSensitive = false;
        quint8 lcidRaw0 = readUnsignedByte(file);
        PRINT_DBG("        LCID record LCID byte 0: %d", lcidRaw0);
        quint8 lcidRaw1 = readUnsignedByte(file);
        PRINT_DBG("        LCID record LCID byte 1: %d", lcidRaw1);
        quint8 lcidRaw2 = readUnsignedByte(file);
        PRINT_DBG("        LCID record LCID byte 2: %d", lcidRaw2);
        quint8 lcidRaw3 = readUnsignedByte(file);
        PRINT_DBG("        LCID record LCID byte 3: %d", lcidRaw3);
        quint16 primaryLanguageID = static_cast<quint16> (lcidRaw0) + 256
            * static_cast<quint16> (lcidRaw1 & 0x03);
        PRINT_DBG("        LCID record primary language id: %d",
            primaryLanguageID);
        quint8 subLanguageID = (((lcidRaw1 & 0xFC) >> 2) & 0x3F);
        PRINT_DBG("        LCID record sub language id: %d", subLanguageID);
        quint8 sortID = lcidRaw2 & 0x0F;
        PRINT_DBG("        LCID record sort id: %d", sortID);
        quint8 sortVersion = (((lcidRaw2 & 0xF0) >> 4) & 0x0F);
        PRINT_DBG("        LCID record sort version: %d", sortVersion);
        this->lcid
            = LCID(primaryLanguageID, subLanguageID, sortID, sortVersion);
    }
    PRINT_DBG("LCID record loaded successfully");
}

LCIDRecord::LCIDRecord(const LCIDRecord& rhs) :
    lcid(rhs.lcid), ignoreNonSpacingMarksCaseInsensitive(
    rhs.ignoreNonSpacingMarksCaseInsensitive),
    ignoreSymbolsCaseInsensitive(rhs.ignoreSymbolsCaseInsensitive),
    ignoreNonSpacingMarksCaseSensitive(
    rhs.ignoreNonSpacingMarksCaseSensitive),
    ignoreSymbolsCaseSensitive(rhs.ignoreSymbolsCaseSensitive)
{
    PRINT_DBG("LCID record copy constructor");
}

LCIDRecord::~LCIDRecord()
{
    PRINT_DBG("LCID record destructor");
}

LCIDRecord & LCIDRecord::operator=(const LCIDRecord & rhs)
{
    PRINT_DBG("LCID record assignment operator");
    if (this != &rhs)
    {
        this->lcid = rhs.lcid;
        this->ignoreNonSpacingMarksCaseInsensitive
            = rhs.ignoreNonSpacingMarksCaseInsensitive;
        this->ignoreSymbolsCaseInsensitive = rhs.ignoreSymbolsCaseInsensitive;
        this->ignoreNonSpacingMarksCaseSensitive
            = rhs.ignoreNonSpacingMarksCaseSensitive;
        this->ignoreSymbolsCaseSensitive = rhs.ignoreSymbolsCaseSensitive;
    }
    return *this;
}

LCID LCIDRecord::getLCID() const
{
    return lcid;
}

bool LCIDRecord::isIgnoreNonSpacingMarksCaseInsensitive() const
{
    return ignoreNonSpacingMarksCaseInsensitive;
}

bool LCIDRecord::isIgnoreSymbolsCaseInsensitive() const
{
    return ignoreSymbolsCaseInsensitive;
}

bool LCIDRecord::isIgnoreNonSpacingMarksCaseSensitive() const
{
    return ignoreNonSpacingMarksCaseSensitive;
}

bool LCIDRecord::isIgnoreSymbolsCaseSensitive() const
{
    return ignoreSymbolsCaseSensitive;
}
