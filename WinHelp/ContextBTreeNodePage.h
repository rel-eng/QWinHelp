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

#ifndef CONTEXTBTREENODEPAGE_H
#define CONTEXTBTREENODEPAGE_H

#include "BTreeNodeHeader.h"

#include "ContextBTreeNodeEntry.h"

#include <QtGlobal>
#include <QFile>
#include <QList>

class ContextBTreeNodePage
{
private:
    /** Заголовок листовой страницы. */
    BTreeNodeHeader header;
    /** Список элементов листовой страницы */
    QList<ContextBTreeNodeEntry> nodeEntries;
    /** Размер страницы в байтах. */
    qint64 sz;
public:
    ContextBTreeNodePage();
    ContextBTreeNodePage(const BTreeNodeHeader &header);
    ContextBTreeNodePage(QFile &file, qint64 off);
    ContextBTreeNodePage(const ContextBTreeNodePage& rhs);
    virtual ~ContextBTreeNodePage();
    ContextBTreeNodePage & operator=(const ContextBTreeNodePage & rhs);
    qint64 size() const;
    int getNEntries() const;
    quint16 getPreviousPage() const;
    quint16 getNextPage() const;
    qint64 getUnused() const;
    quint32 getEntryHash(int index) const;
    TopicOffset getEntryOffset(int index) const;
    void appendEntry(const ContextBTreeNodeEntry &entry);
};

#endif // CONTEXTBTREENODEPAGE_H
