/* BTree index header class implementation.

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

#include "BTreeIndexHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

BTreeIndexHeader::BTreeIndexHeader() :
    unused(0), nEntries(0), previousPage(0)
{
    PRINT_DBG("BTree index header default constructor");
}

BTreeIndexHeader::BTreeIndexHeader(qint64 unused, quint16 nEntries,
    quint16 previousPage) :
    unused(unused), nEntries(nEntries), previousPage(previousPage)
{
    PRINT_DBG("BTree index header constructor");
}

BTreeIndexHeader::BTreeIndexHeader(QFile &file, qint64 off)
{
    PRINT_DBG("Reading BTree index header at %lld", off);
    seekFile(file, off);
    this->unused = static_cast<qint64> (readUnsignedWord(file));
    PRINT_DBG("        Unused: %lld", this->unused);
    this->nEntries = readUnsignedWord(file);
    PRINT_DBG("        nEntries: %d", this->nEntries);
    this->previousPage = readUnsignedWord(file);
    PRINT_DBG("        Previous Page: %d", this->previousPage);
    PRINT_DBG("BTree index header loaded successfully");
}

BTreeIndexHeader::BTreeIndexHeader(const BTreeIndexHeader& rhs) :
    unused(rhs.unused), nEntries(rhs.nEntries), previousPage(rhs.previousPage)
{
    PRINT_DBG("BTree index header copy constructor");
}

BTreeIndexHeader::~BTreeIndexHeader()
{
    PRINT_DBG("BTree index header destructor");
}

quint16 BTreeIndexHeader::getNEntries() const
{
    return nEntries;
}

quint16 BTreeIndexHeader::getPreviousPage() const
{
    return previousPage;
}

qint64 BTreeIndexHeader::getUnused() const
{
    return unused;
}

BTreeIndexHeader & BTreeIndexHeader::operator=(const BTreeIndexHeader & rhs)
{
    PRINT_DBG("BTree index header assignment operator");
    if (this != &rhs)
    {
        this->unused = rhs.unused;
        this->nEntries = rhs.nEntries;
        this->previousPage = rhs.previousPage;
    }
    return *this;
}
