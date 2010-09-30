/* Context BTree node page implementation.

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

#include "ContextBTreeNodePage.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>

#include <stdexcept>

ContextBTreeNodePage::ContextBTreeNodePage() :
    header(), nodeEntries(), sz(0)
{
    PRINT_DBG("Context BTree node page default constructor");
}

ContextBTreeNodePage::ContextBTreeNodePage(const BTreeNodeHeader &header) :
    header(header), nodeEntries(), sz(BTreeNodeHeader::size)
{
    PRINT_DBG("Context BTree node page constructor");
}

ContextBTreeNodePage::ContextBTreeNodePage(QFile &file, qint64 off) :
    header(file, off), nodeEntries(), sz(BTreeNodeHeader::size)
{
    PRINT_DBG("Reading Context BTree node page at %lld", off);
    seekFile(file, off + BTreeNodeHeader::size);
    qint64 pos = off + BTreeNodeHeader::size;
    for (int index = 0; index < header.getNEntries(); index++)
    {
        ContextBTreeNodeEntry entry(file, pos);
        nodeEntries.append(entry);
        pos += ContextBTreeNodeEntry::size;
        sz += ContextBTreeNodeEntry::size;
    }
    sz += header.getUnused();
    PRINT_DBG("Context BTree node page loaded successfully");
}

ContextBTreeNodePage::ContextBTreeNodePage(const ContextBTreeNodePage& rhs) :
    header(rhs.header), nodeEntries(rhs.nodeEntries)
{
    PRINT_DBG("Context BTree node page copy constructor");
}

ContextBTreeNodePage::~ContextBTreeNodePage()
{
    PRINT_DBG("Context BTree node page destructor");
}

ContextBTreeNodePage & ContextBTreeNodePage::operator=(
    const ContextBTreeNodePage & rhs)
{
    PRINT_DBG("Context BTree node page assignment operator");
    if (this != &rhs)
    {
        this->header = rhs.header;
        this->nodeEntries = rhs.nodeEntries;
    }
    return *this;
}

qint64 ContextBTreeNodePage::size() const
{
    return sz;
}

int ContextBTreeNodePage::getNEntries() const
{
    return nodeEntries.size();
}

quint16 ContextBTreeNodePage::getPreviousPage() const
{
    return header.getPreviousPage();
}

quint16 ContextBTreeNodePage::getNextPage() const
{
    return header.getNextPage();
}

qint64 ContextBTreeNodePage::getUnused() const
{
    return header.getUnused();
}

quint32 ContextBTreeNodePage::getEntryHash(int index) const
{
    if ((index < 0) || (index >= nodeEntries.size()))
    {
        throw new std::out_of_range(
            "Index of Context BTree node page entry is out of range");
    }
    return nodeEntries.at(index).getHash();
}

TopicOffset ContextBTreeNodePage::getEntryOffset(int index) const
{
    if ((index < 0) || (index >= nodeEntries.size()))
    {
        throw new std::out_of_range(
            "Index of Context BTree node page entry is out of range");
    }
    return nodeEntries.at(index).getOffset();
}

void ContextBTreeNodePage::appendEntry(const ContextBTreeNodeEntry & entry)
{
    nodeEntries.append(entry);
    if (header.getUnused() < ContextBTreeNodeEntry::size)
    {
        qDebug() << "Context BTree node page overflow";
    }
    header = BTreeNodeHeader(header.getUnused() - ContextBTreeNodeEntry::size,
        header.getNEntries() - 1, header.getPreviousPage(),
        header.getNextPage());
}
