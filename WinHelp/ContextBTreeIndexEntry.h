/* Context BTree index entry definition.

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

#ifndef CONTEXTBTREEINDEXENTRY_H
#define CONTEXTBTREEINDEXENTRY_H

#include <QtGlobal>
#include <QFile>

class ContextBTreeIndexEntry
{
    quint32 hash;
    quint16 pageNumber;
public:
    static const qint64 size = 6;
    ContextBTreeIndexEntry();
    ContextBTreeIndexEntry(quint32 hash, quint16 pageNumber);
    ContextBTreeIndexEntry(QFile &file, qint64 off);
    ContextBTreeIndexEntry(const ContextBTreeIndexEntry& rhs);
    virtual ~ContextBTreeIndexEntry();
    ContextBTreeIndexEntry & operator=(const ContextBTreeIndexEntry & rhs);
    quint32 getHash() const;
    quint16 getPageNumber() const;
};

#endif // CONTEXTBTREEINDEXENTRY_H
