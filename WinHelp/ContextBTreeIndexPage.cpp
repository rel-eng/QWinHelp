/* Context BTree index page implementation.

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

#include "ContextBTreeIndexPage.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

#include <stdexcept>

ContextBTreeIndexPage::ContextBTreeIndexPage() :
    header(), indexEntries(), sz(0)
{
    PRINT_DBG("Context BTree index page default constructor");
}

ContextBTreeIndexPage::ContextBTreeIndexPage(const BTreeIndexHeader &header) :
    header(header), indexEntries(), sz(BTreeIndexHeader::size)
{
    PRINT_DBG("Context BTree index page constructor");
}

ContextBTreeIndexPage::ContextBTreeIndexPage(QFile &file, qint64 off) :
    header(file, off), indexEntries(), sz(BTreeIndexHeader::size)
{
    PRINT_DBG("Reading Context BTree index page at %lld", off);
    seekFile(file, off + BTreeIndexHeader::size);
    qint64 pos = off + BTreeIndexHeader::size;
    for (int index = 0; index < header.getNEntries(); index++)
    {
        ContextBTreeIndexEntry entry(file, pos);
        indexEntries.append(entry);
        pos += ContextBTreeIndexEntry::size;
        sz += ContextBTreeIndexEntry::size;
    }
    sz += header.getUnused();
    PRINT_DBG("Context BTree index page successfully loaded");
}

ContextBTreeIndexPage::ContextBTreeIndexPage(const ContextBTreeIndexPage& rhs)
    :
    header(rhs.header), indexEntries(rhs.indexEntries), sz(rhs.sz)
{
    PRINT_DBG("Context BTree index page copy constructor");
}

ContextBTreeIndexPage::~ContextBTreeIndexPage()
{
    PRINT_DBG("Context BTree index page destructor");
}

ContextBTreeIndexPage & ContextBTreeIndexPage::operator=(
    const ContextBTreeIndexPage & rhs)
{
    PRINT_DBG("Context BTree index page assignment operator");
    if (this != &rhs)
    {
        this->header = rhs.header;
        this->indexEntries = rhs.indexEntries;
        this->sz = rhs.sz;
    }
    return *this;
}

qint64 ContextBTreeIndexPage::size() const
{
    return sz;
}

int ContextBTreeIndexPage::getNEntries() const
{
    return indexEntries.size();
}

quint16 ContextBTreeIndexPage::getPreviousPage() const
{
    return header.getPreviousPage();
}

qint64 ContextBTreeIndexPage::getUnused() const
{
    return header.getUnused();
}

quint16 ContextBTreeIndexPage::getEntryPageNumber(int index) const
{
    if ((index < 0) || (index >= indexEntries.size()))
    {
        throw new std::out_of_range(
            "Index of Context BTree index page entry is out of range");
    }
    return indexEntries.at(index).getPageNumber();
}

quint32 ContextBTreeIndexPage::getEntryHash(int index) const
{
    if ((index < 0) || (index >= indexEntries.size()))
    {
        throw new std::out_of_range(
            "Index of Context BTree index page entry is out of range");
    }
    return indexEntries.at(index).getHash();
}

void ContextBTreeIndexPage::appendEntry(const ContextBTreeIndexEntry & entry)
{
    indexEntries.append(entry);
    if (header.getUnused() < ContextBTreeIndexEntry::size)
    {
        qDebug() << "Context BTree index page overflow";
    }
    header = BTreeIndexHeader(header.getUnused() - ContextBTreeIndexEntry::size,
        header.getNEntries() - 1, header.getPreviousPage());
}
