/* BTree node header class implementation.

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

#include "BTreeNodeHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

BTreeNodeHeader::BTreeNodeHeader() :
    unused(0), nEntries(0), previousPage(0), nextPage(0)
{
    PRINT_DBG("BTree node header default constructor");
}

BTreeNodeHeader::BTreeNodeHeader(qint64 unused, quint16 nEntries,
    quint16 previousPage, quint16 nextPage) :
    unused(unused), nEntries(nEntries), previousPage(previousPage), nextPage(
    nextPage)
{
    PRINT_DBG("BTree node header constructor");
}

BTreeNodeHeader::BTreeNodeHeader(QFile &file, qint64 off)
{
    PRINT_DBG("Reading BTree node header at %lld", off);
    seekFile(file, off);
    this->unused = static_cast<qint64> (readUnsignedWord(file));
    PRINT_DBG("        Unused: %lld", this->unused);
    this->nEntries = readUnsignedWord(file);
    PRINT_DBG("        nEntries: %d", this->nEntries);
    this->previousPage = readUnsignedWord(file);
    PRINT_DBG("        Previous page: %d", this->previousPage);
    this->nextPage = readUnsignedWord(file);
    PRINT_DBG("        Next page: %d", this->nextPage);
    PRINT_DBG("BTree node header successfully loaded");
}

BTreeNodeHeader::BTreeNodeHeader(const BTreeNodeHeader& rhs) :
    unused(rhs.unused), nEntries(rhs.nEntries), previousPage(rhs.previousPage),
    nextPage(rhs.nextPage)
{
    PRINT_DBG("BTree node header copy constructor");
}

BTreeNodeHeader::~BTreeNodeHeader()
{
    PRINT_DBG("BTree node header destructor");
}

quint16 BTreeNodeHeader::getNEntries() const
{
    return nEntries;
}

quint16 BTreeNodeHeader::getPreviousPage() const
{
    return previousPage;
}

quint16 BTreeNodeHeader::getNextPage() const
{
    return nextPage;
}

qint64 BTreeNodeHeader::getUnused() const
{
    return unused;
}

BTreeNodeHeader & BTreeNodeHeader::operator=(const BTreeNodeHeader & rhs)
{
    PRINT_DBG("BTree node header assignment operator");
    if (this != &rhs)
    {
        this->unused = rhs.unused;
        this->nEntries = rhs.nEntries;
        this->previousPage = rhs.previousPage;
        this->nextPage = rhs.nextPage;
    }
    return *this;
}
