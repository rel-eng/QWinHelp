/* BTree node page class definition.

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

#ifndef BTREENODEPAGE_H_
#define BTREENODEPAGE_H_

#include "BTreeNodeHeader.h"

#include "BTreeNodeEntry.h"

#include <QtGlobal>
#include <QFile>
#include <QList>

class BTreeNodePage
{
private:
    /** Заголовок листовой страницы. */
    BTreeNodeHeader header;
    /** Список элементов листовой страницы */
    QList<BTreeNodeEntry> nodeEntries;
    /** Размер страницы в байтах. */
    qint64 sz;
public:
    BTreeNodePage();
    BTreeNodePage(const BTreeNodeHeader &header);
    BTreeNodePage(QFile &file, qint64 off);
    BTreeNodePage(const BTreeNodePage& rhs);
    virtual ~BTreeNodePage();
    BTreeNodePage & operator=(const BTreeNodePage & rhs);
    qint64 size() const;
    int getNEntries() const;
    quint16 getPreviousPage() const;
    quint16 getNextPage() const;
    qint64 getUnused() const;
    QString getEntryFileName(int index) const;
    qint64 getEntryOffset(int index) const;
    void appendEntry(const BTreeNodeEntry &entry);
};

#endif /* BTREENODEPAGE_H_ */
