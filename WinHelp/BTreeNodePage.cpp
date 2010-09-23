/* BTree node page class implementation.

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

#include "BTreeNodePage.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

#include <stdexcept>

BTreeNodePage::BTreeNodePage() :
    header(), nodeEntries(), sz(0)
{
    PRINT_DBG("BTree node page default constructor");
}

BTreeNodePage::BTreeNodePage(const BTreeNodeHeader &header) :
    header(header), nodeEntries(), sz(BTreeNodeHeader::size)
{
    PRINT_DBG("BTree node page constructor");
}

BTreeNodePage::BTreeNodePage(QFile &file, qint64 off) :
    header(file, off), nodeEntries(), sz(BTreeNodeHeader::size)
{
    PRINT_DBG("Reading BTree node page at %lld", off);
    seekFile(file, off + BTreeNodeHeader::size);
    qint64 pos = off + BTreeNodeHeader::size;
    for (int index = 0; index < header.getNEntries(); index++)
    {
        BTreeNodeEntry entry(file, pos);
        nodeEntries.append(entry);
        pos += entry.size();
        sz += entry.size();
    }
    sz += header.getUnused();
    PRINT_DBG("BTree node page loaded successfully");
}

BTreeNodePage::BTreeNodePage(const BTreeNodePage& rhs) :
    header(rhs.header), nodeEntries(rhs.nodeEntries)
{
    PRINT_DBG("BTree node page copy constructor");
}

BTreeNodePage::~BTreeNodePage()
{
    PRINT_DBG("BTree node page destructor");
}

BTreeNodePage & BTreeNodePage::operator=(const BTreeNodePage & rhs)
{
    PRINT_DBG("BTree node page assignment operator");
    if (this != &rhs)
    {
        this->header = rhs.header;
        this->nodeEntries = rhs.nodeEntries;
    }
    return *this;
}

qint64 BTreeNodePage::size() const
{
    return sz;
}

int BTreeNodePage::getNEntries() const
{
    return nodeEntries.size();
}

quint16 BTreeNodePage::getPreviousPage() const
{
    return header.getPreviousPage();
}

quint16 BTreeNodePage::getNextPage() const
{
    return header.getNextPage();
}

qint64 BTreeNodePage::getUnused() const
{
    return header.getUnused();
}

QString BTreeNodePage::getEntryFileName(int index) const
{
    if ((index < 0) || (index >= nodeEntries.size()))
    {
        throw new std::out_of_range(
            "Index of BTree node page entry is out of range");
    }
    return nodeEntries.at(index).getFileName();
}

qint64 BTreeNodePage::getEntryOffset(int index) const
{
    if ((index < 0) || (index >= nodeEntries.size()))
    {
        throw new std::out_of_range(
            "Index of BTree node page entry is out of range");
    }
    return nodeEntries.at(index).getOffset();
}

void BTreeNodePage::appendEntry(const BTreeNodeEntry & entry)
{
    nodeEntries.append(entry);
    if (header.getUnused() < entry.size())
    {
        qDebug() << "BTree node page overflow";
    }
    header = BTreeNodeHeader(header.getUnused() - entry.size(),
        header.getNEntries() - 1, header.getPreviousPage(),
        header.getNextPage());
}
