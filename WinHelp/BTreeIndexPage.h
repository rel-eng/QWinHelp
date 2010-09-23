/* BTree index page class definition.

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

#ifndef BTREEINDEXPAGE_H_
#define BTREEINDEXPAGE_H_

#include "BTreeIndexHeader.h"

#include "BTreeIndexEntry.h"

#include <QtGlobal>
#include <QFile>
#include <QList>

class BTreeIndexPage
{
private:
    /** Заголовок индексной страницы. */
    BTreeIndexHeader header;
    /** Список элементов индексной страницы */
    QList<BTreeIndexEntry> indexEntries;
    /** Размер страницы в байтах. */
    qint64 sz;
public:
    BTreeIndexPage();
    BTreeIndexPage(const BTreeIndexHeader &header);
    BTreeIndexPage(QFile &file, qint64 off);
    BTreeIndexPage(const BTreeIndexPage& rhs);
    virtual ~BTreeIndexPage();
    BTreeIndexPage & operator=(const BTreeIndexPage & rhs);
    qint64 size() const;
    int getNEntries() const;
    quint16 getPreviousPage() const;
    qint64 getUnused() const;
    QString getEntryFilename(int index) const;
    quint16 getEntryPageNumber(int index) const;
    void appendEntry(const BTreeIndexEntry &entry);
};

#endif /* BTREEINDEXPAGE_H_ */
