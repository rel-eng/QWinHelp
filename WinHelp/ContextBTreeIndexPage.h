/* Context BTree index page definition.

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

#ifndef CONTEXTBTREEINDEXPAGE_H
#define CONTEXTBTREEINDEXPAGE_H

#include "BTreeIndexHeader.h"

#include "ContextBTreeIndexEntry.h"

#include <QtGlobal>
#include <QFile>
#include <QList>

class ContextBTreeIndexPage
{
private:
    /** Заголовок индексной страницы. */
    BTreeIndexHeader header;
    /** Список элементов индексной страницы */
    QList<ContextBTreeIndexEntry> indexEntries;
    /** Размер страницы в байтах. */
    qint64 sz;
public:
    ContextBTreeIndexPage();
    ContextBTreeIndexPage(const BTreeIndexHeader &header);
    ContextBTreeIndexPage(QFile &file, qint64 off);
    ContextBTreeIndexPage(const ContextBTreeIndexPage& rhs);
    virtual ~ContextBTreeIndexPage();
    ContextBTreeIndexPage & operator=(const ContextBTreeIndexPage & rhs);
    qint64 size() const;
    int getNEntries() const;
    quint16 getPreviousPage() const;
    qint64 getUnused() const;
    quint32 getEntryHash(int index) const;
    quint16 getEntryPageNumber(int index) const;
    void appendEntry(const ContextBTreeIndexEntry &entry);
};

#endif // CONTEXTBTREEINDEXPAGE_H
