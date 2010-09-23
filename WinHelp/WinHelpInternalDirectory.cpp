/* WinHelp internal directory class implementation.

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

#include "WinHelpInternalDirectory.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "InternalDirectoryFileHeader.h"
#include "BTreeHeader.h"
#include "BTreeIndexPage.h"
#include "BTreeNodePage.h"

#include <stdexcept>

WinHelpInternalDirectory::WinHelpInternalDirectory() :
    directory()
{
    PRINT_DBG("WinHelp internal directory default constructor");
}

WinHelpInternalDirectory::WinHelpInternalDirectory(QFile &file, qint64 off) :
    directory()
{
    PRINT_DBG("Reading WinHelp internal directory at %lld", off);
    seekFile(file, off);
    InternalDirectoryFileHeader directoryHeader(file, off);
    BTreeHeader treeHeader(file, off + InternalDirectoryFileHeader::size);
    qint64 pagesOffset = off + InternalDirectoryFileHeader::size
        + BTreeHeader::size;
    qint64 currentPageNumber = static_cast<qint64> (treeHeader.getRootPage());
    if (treeHeader.getNLevels() > 1)
    {
        BTreeIndexPage currentIndexPage(file, getPageOffset(pagesOffset,
                treeHeader.getPageSize(), currentPageNumber));
        for (quint16 level = 0; level < treeHeader.getNLevels() - 1; level++)
        {
            currentPageNumber
                = static_cast<qint64> (currentIndexPage.getPreviousPage());
            if (level < treeHeader.getNLevels() - 2)
            {
                currentIndexPage = BTreeIndexPage(file, getPageOffset(
                        pagesOffset, treeHeader.getPageSize(),
                        currentPageNumber));
            }
        }
    }
    while (true)
    {
        BTreeNodePage currentNodePage(file, getPageOffset(pagesOffset,
                treeHeader.getPageSize(), currentPageNumber));
        for (int index = 0; index < currentNodePage.getNEntries(); index++)
        {
            directory.insert(currentNodePage.getEntryFileName(index),
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
    PRINT_DBG("WinHelp internal directory successfully loaded");
}

WinHelpInternalDirectory::WinHelpInternalDirectory(
    const WinHelpInternalDirectory& rhs) :
    directory(rhs.directory)
{
    PRINT_DBG("WinHelp internal directory copy constructor");
}

WinHelpInternalDirectory::~WinHelpInternalDirectory()
{
    PRINT_DBG("WinHelp internal directory destructor");
}

qint64 WinHelpInternalDirectory::getPageOffset(qint64 pagesOffset,
    qint64 pageSize, qint64 pageNumber)
{
    return pagesOffset + pageSize * pageNumber;
}

WinHelpInternalDirectory & WinHelpInternalDirectory::operator=(
    const WinHelpInternalDirectory & rhs)
{
    PRINT_DBG("WinHelp internal directory assignment operator");
    if (this != &rhs)
    {
        this->directory = rhs.directory;
    }
    return *this;
}

bool WinHelpInternalDirectory::isFileExists(QString filename) const
{
    return this->directory.contains(filename);
}

qint64 WinHelpInternalDirectory::getFileOffset(QString filename) const
{
    if (!this->directory.contains(filename))
    {
        throw std::invalid_argument("No such file in directory");
    }
    return this->directory[filename];
}

void WinHelpInternalDirectory::addFile(QString filename, qint64 offset)
{
    if (this->directory.contains(filename))
    {
        throw std::invalid_argument("Directory already contains this file");
    }
    this->directory.insert(filename, offset);
}
