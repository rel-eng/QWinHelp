/* BTree index page class implementation.

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

#include "BTreeIndexPage.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

#include <stdexcept>

BTreeIndexPage::BTreeIndexPage() :
    header(), indexEntries(), sz(0)
{
    PRINT_DBG("BTree index page default constructor");
}

BTreeIndexPage::BTreeIndexPage(const BTreeIndexHeader &header) :
    header(header), indexEntries(), sz(BTreeIndexHeader::size)
{
    PRINT_DBG("BTree index page constructor");
}

BTreeIndexPage::BTreeIndexPage(QFile &file, qint64 off) :
    header(file, off), indexEntries(), sz(BTreeIndexHeader::size)
{
    PRINT_DBG("Reading BTree index page at %lld", off);
    seekFile(file, off + BTreeIndexHeader::size);
    qint64 pos = off + BTreeIndexHeader::size;
    for (int index = 0; index < header.getNEntries(); index++)
    {
        BTreeIndexEntry entry(file, pos);
        indexEntries.append(entry);
        pos += entry.size();
        sz += entry.size();
    }
    sz += header.getUnused();
    PRINT_DBG("BTree index page successfully loaded");
}

BTreeIndexPage::BTreeIndexPage(const BTreeIndexPage& rhs) :
    header(rhs.header), indexEntries(rhs.indexEntries), sz(rhs.sz)
{
    PRINT_DBG("BTree index page copy constructor");
}

BTreeIndexPage::~BTreeIndexPage()
{
    PRINT_DBG("BTree index page destructor");
}

BTreeIndexPage & BTreeIndexPage::operator=(const BTreeIndexPage & rhs)
{
    PRINT_DBG("BTree index page assignment operator");
    if (this != &rhs)
    {
        this->header = rhs.header;
        this->indexEntries = rhs.indexEntries;
        this->sz = rhs.sz;
    }
    return *this;
}

qint64 BTreeIndexPage::size() const
{
    return sz;
}

int BTreeIndexPage::getNEntries() const
{
    return indexEntries.size();
}

quint16 BTreeIndexPage::getPreviousPage() const
{
    return header.getPreviousPage();
}

qint64 BTreeIndexPage::getUnused() const
{
    return header.getUnused();
}

quint16 BTreeIndexPage::getEntryPageNumber(int index) const
{
    if ((index < 0) || (index >= indexEntries.size()))
    {
        throw new std::out_of_range(
            "Index of BTree index page entry is out of range");
    }
    return indexEntries.at(index).getPageNumber();
}

QString BTreeIndexPage::getEntryFilename(int index) const
{
    if ((index < 0) || (index >= indexEntries.size()))
    {
        throw new std::out_of_range(
            "Index of BTree index page entry is out of range");
    }
    return indexEntries.at(index).getFileName();
}

void BTreeIndexPage::appendEntry(const BTreeIndexEntry & entry)
{
    indexEntries.append(entry);
    if (header.getUnused() < entry.size())
    {
        qDebug() << "BTree index page overflow";
    }
    header = BTreeIndexHeader(header.getUnused() - entry.size(),
        header.getNEntries() - 1, header.getPreviousPage());
}
