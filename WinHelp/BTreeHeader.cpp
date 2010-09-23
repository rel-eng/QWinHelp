/* BTree header class implementation.

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

#include "BTreeHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/TextUtils.h"

#include <QtDebug>

#include <stdexcept>

BTreeHeader::BTreeHeader() :
    flags(0), pageSize(0), structure(""), pageSplits(0), rootPage(0),
    totalPages(0), nLevels(0), totalBTreeEntries(0)
{
    PRINT_DBG("BTree header default constructor");
}

BTreeHeader::BTreeHeader(quint16 flags, qint64 pageSize,
    const QString &structure, quint16 pageSplits, quint16 rootPage,
    quint16 totalPages, quint16 nLevels, quint32 totalBTreeEntries) :
    flags(flags), pageSize(pageSize), structure(structure), pageSplits(
    pageSplits), rootPage(rootPage), totalPages(totalPages), nLevels(
    nLevels), totalBTreeEntries(totalBTreeEntries)
{
    PRINT_DBG("BTree header constructor");
}

BTreeHeader::BTreeHeader(QFile &file, qint64 off)
{
    PRINT_DBG("Reading BTree header at %lld", off);
    seekFile(file, off);
    quint16 magicIn = readUnsignedWord(file);
    if (magicIn != magic)
    {
        throw std::runtime_error("Not a BTree header");
    }
    PRINT_DBG("        Magic: %d", magicIn);
    this->flags = readUnsignedWord(file);
    if ((this->flags & this->flags_mask_magic_bit) == 0)
    {
        qDebug() << "Zero bit 0x0002 in  B+ tree header flags.";
    }
    if ((this->flags & this->flags_mask_catalog_bit) == 0)
    {
        qDebug()
        << "Zero bit 0x0400 in  B+ tree header flags (not a directory).";
    }
    PRINT_DBG("        Flags: %d", this->flags);
    this->pageSize = static_cast<qint64> (readUnsignedWord(file));
    PRINT_DBG("        Page size: %lld", this->pageSize);
    QTextCodec *codec = getDefaultTextCodec();
    this->structure = readFixedLengthString(file, 16, codec);
    PRINT_DBG("        Structure: %s", this->structure.toLocal8Bit().data());
    quint16 unused1In = readUnsignedWord(file);
    if (unused1In != unused_1)
    {
        qDebug() << "Nonzero unused field in  B+ tree header.";
    }
    PRINT_DBG("        Unused_1: %d", unused1In);
    this->pageSplits = readUnsignedWord(file);
    PRINT_DBG("        Page splits: %d", this->pageSplits);
    this->rootPage = readUnsignedWord(file);
    PRINT_DBG("        Root page: %d", this->rootPage);
    quint16 unused2In = readUnsignedWord(file);
    if (unused2In != unused_2)
    {
        qDebug() << "Unused field is not equal 0xFFFF in  B+ tree header.";
    }
    PRINT_DBG("        Unused_2: %d", unused2In);
    this->totalPages = readUnsignedWord(file);
    PRINT_DBG("        Total pages: %d", this->totalPages);
    this->nLevels = readUnsignedWord(file);
    PRINT_DBG("        nLevels: %d", this->nLevels);
    this->totalBTreeEntries = readUnsignedDWord(file);
    PRINT_DBG("        Total BTree Entries: %d", this->totalBTreeEntries);
    PRINT_DBG("BTree header loaded successfully");
}

BTreeHeader::BTreeHeader(const BTreeHeader& rhs) :
    flags(rhs.flags), pageSize(rhs.pageSize), structure(rhs.structure),
    pageSplits(rhs.pageSplits), rootPage(rhs.rootPage), totalPages(
    rhs.totalPages), nLevels(rhs.nLevels), totalBTreeEntries(
    rhs.totalBTreeEntries)
{
    PRINT_DBG("BTree header copy constructor");
}

BTreeHeader::~BTreeHeader()
{
    PRINT_DBG("BTree header destructor");
}

quint16 BTreeHeader::getFlags() const
{
    return flags;
}

quint16 BTreeHeader::getNLevels() const
{
    return nLevels;
}

qint64 BTreeHeader::getPageSize() const
{
    return pageSize;
}

quint16 BTreeHeader::getPageSplits() const
{
    return pageSplits;
}

quint16 BTreeHeader::getRootPage() const
{
    return rootPage;
}

QString BTreeHeader::getStructure() const
{
    return structure;
}

quint32 BTreeHeader::getTotalBTreeEntries() const
{
    return totalBTreeEntries;
}

quint16 BTreeHeader::getTotalPages() const
{
    return totalPages;
}

BTreeHeader & BTreeHeader::operator=(const BTreeHeader & rhs)
{
    PRINT_DBG("BTree header assignment operator");
    if (this != &rhs)
    {
        this->flags = rhs.flags;
        this->pageSize = rhs.pageSize;
        this->structure = rhs.structure;
        this->pageSplits = rhs.pageSplits;
        this->rootPage = rhs.rootPage;
        this->totalPages = rhs.totalPages;
        this->nLevels = rhs.nLevels;
        this->totalBTreeEntries = rhs.totalBTreeEntries;
    }
    return *this;
}
