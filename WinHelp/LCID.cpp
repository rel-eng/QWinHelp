/* Locale identifier class implementation.

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

#include "LCID.h"

#include "Utils/DebugUtils.h"

LCID::LCID() :
    primaryLanguageID(0), subLanguageID(0), sortID(0), sortVersion(0)
{
    PRINT_DBG("LCID default constructor");
}

LCID::LCID(quint16 primaryLanguageID, quint8 subLanguageID, quint8 sortID,
    quint8 sortVersion) :
    primaryLanguageID(primaryLanguageID), subLanguageID(subLanguageID), sortID(
    sortID), sortVersion(sortVersion)
{
    PRINT_DBG("LCID constructor");
}

LCID::LCID(const LCID& rhs) :
    primaryLanguageID(rhs.primaryLanguageID), subLanguageID(rhs.subLanguageID),
    sortID(rhs.sortID), sortVersion(rhs.sortVersion)
{
    PRINT_DBG("LCID copy constructor");
}

LCID::~LCID()
{
    PRINT_DBG("LCID destructor");
}

LCID & LCID::operator=(const LCID & rhs)
{
    PRINT_DBG("LCID assignment operator");
    if (this != &rhs)
    {
        this->primaryLanguageID = rhs.primaryLanguageID;
        this->subLanguageID = rhs.subLanguageID;
        this->sortID = rhs.sortID;
        this->sortVersion = rhs.sortVersion;
    }
    return *this;
}

quint16 LCID::getPrimaryLanguageID() const
{
    return primaryLanguageID;
}

quint8 LCID::getSubLanguageID() const
{
    return subLanguageID;
}

quint8 LCID::getSortID() const
{
    return sortID;
}

quint8 LCID::getSortVersion() const
{
    return sortVersion;
}
