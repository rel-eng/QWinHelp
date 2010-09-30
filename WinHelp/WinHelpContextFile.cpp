/* WinHelp Context file implementation.

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

#include "WinHelpContextFile.h"

#include "InternalDirectoryFileHeader.h"
#include "BTreeHeader.h"
#include "ContextBTreeIndexPage.h"
#include "ContextBTreeNodePage.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

WinHelpContextFile::WinHelpContextFile() : hashTable()
{
    PRINT_DBG("WinHelp Context file default constructor");
}

WinHelpContextFile::WinHelpContextFile(QFile &file, qint64 off) : hashTable()
{
    PRINT_DBG("Loading WinHelp Context file at %lld", off);
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    PRINT_DBG("        BTree header:");
    BTreeHeader treeHeader(file, off + InternalDirectoryFileHeader::size);

    qint64 pagesOffset = off + InternalDirectoryFileHeader::size
        + BTreeHeader::size;
    qint64 currentPageNumber = static_cast<qint64> (treeHeader.getRootPage());
    if (treeHeader.getNLevels() > 1)
    {
        ContextBTreeIndexPage currentIndexPage(file, getPageOffset(pagesOffset,
                treeHeader.getPageSize(), currentPageNumber));
        for (quint16 level = 0; level < treeHeader.getNLevels() - 1; level++)
        {
            currentPageNumber
                = static_cast<qint64> (currentIndexPage.getPreviousPage());
            if (level < treeHeader.getNLevels() - 2)
            {
                currentIndexPage = ContextBTreeIndexPage(file, getPageOffset(
                        pagesOffset, treeHeader.getPageSize(),
                        currentPageNumber));
            }
        }
    }
    while (true)
    {
        ContextBTreeNodePage currentNodePage(file, getPageOffset(pagesOffset,
                treeHeader.getPageSize(), currentPageNumber));
        for (int index = 0; index < currentNodePage.getNEntries(); index++)
        {
            hashTable.insert(currentNodePage.getEntryHash(index),
                currentNodePage.getEntryOffset(index));
        }
        if (currentNodePage.getNextPage() != BTreeNodeHeader::noMorePages)
        {
            currentPageNumber
                = static_cast<qint64> (currentNodePage.getNextPage());
        }
        else
        {
            break;
        }
    }

    PRINT_DBG("WinHelp Context file loaded successfully");
}

qint64 WinHelpContextFile::getPageOffset(qint64 pagesOffset,
    qint64 pageSize, qint64 pageNumber)
{
    return pagesOffset + pageSize * pageNumber;
}

WinHelpContextFile::WinHelpContextFile(const WinHelpContextFile &rhs) :
    hashTable(rhs.hashTable)
{
    PRINT_DBG("WinHelp Context file copy constructor");
}

WinHelpContextFile::~WinHelpContextFile()
{
    PRINT_DBG("WinHelp Context file destructor");
}

WinHelpContextFile & WinHelpContextFile::operator=(
    const WinHelpContextFile &rhs)
{
    PRINT_DBG("WinHelp Context file assignment operator");
    if (this != &rhs)
    {
        this->hashTable = rhs.hashTable;
    }
    return *this;
}

bool WinHelpContextFile::isHashExists(quint32 hash) const
{
    return this->hashTable.contains(hash);
}

TopicOffset WinHelpContextFile::getOffset(quint32 hash) const
{
    if (!this->hashTable.contains(hash))
    {
        throw std::invalid_argument("No such hash in Context file");
    }
    return this->hashTable[hash];
}

void WinHelpContextFile::addHash(quint32 hash, const TopicOffset &offset)
{
    if (this->hashTable.contains(hash))
    {
        throw std::invalid_argument("Context file already contains this hash");
    }
    this->hashTable.insert(hash, offset);
}
