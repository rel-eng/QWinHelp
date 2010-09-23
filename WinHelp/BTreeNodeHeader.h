/* BTree node header class definition.

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

#ifndef BTREENODEHEADER_H_
#define BTREENODEHEADER_H_

#include <QtGlobal>
#include <QFile>

class BTreeNodeHeader
{
private:
    /** Количество неиспользуемых байт в конце страницы, 2 байта без знака. */
    qint64 unused;
    /** Количество записей в индексной странице, 2 байта без знака. */
    quint16 nEntries;
    /** Номер предыдущей страницы, 0x0000FFFF для первой страницы, 2 байта без знака. */
    quint16 previousPage;
    /** Номер следующей страницы, 0x0000FFFF для последней страницы, 2 байта без знака. */
    quint16 nextPage;
public:
    /** Ссылка на следующую или предыдущую страницу не действительна. */
    static const quint16 noMorePages = 0xFFFF;
    /** Размер заголовка в байтах. */
    static const qint64 size = 8;
    quint16 getNEntries() const;
    quint16 getPreviousPage() const;
    quint16 getNextPage() const;
    qint64 getUnused() const;
    BTreeNodeHeader();
    BTreeNodeHeader(qint64 unused,
        quint16 nEntries,
        quint16 previousPage,
        quint16 nextPage);
    BTreeNodeHeader(QFile &file, qint64 off);
    BTreeNodeHeader(const BTreeNodeHeader& rhs);
    virtual ~BTreeNodeHeader();
    BTreeNodeHeader & operator=(const BTreeNodeHeader & rhs);
};

#endif /* BTREENODEHEADER_H_ */
