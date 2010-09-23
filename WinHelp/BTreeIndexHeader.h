/* BTree index header class definition.

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

#ifndef BTREEINDEXHEADER_H_
#define BTREEINDEXHEADER_H_

#include <QtGlobal>
#include <QFile>

class BTreeIndexHeader
{
private:
    /** Количество неиспользуемых байт в конце страницы, 2 байта без знака. */
    qint64 unused;
    /** Количество записей в индексной странице, 2 байта без знака. */
    quint16 nEntries;
    /** Номер предыдущей страницы, 2 байта без знака. */
    quint16 previousPage;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 6;
    quint16 getNEntries() const;
    quint16 getPreviousPage() const;
    qint64 getUnused() const;
    BTreeIndexHeader();
    BTreeIndexHeader(qint64 unused, quint16 nEntries, quint16 previousPage);
    BTreeIndexHeader(QFile &file, qint64 off);
    BTreeIndexHeader(const BTreeIndexHeader& rhs);
    virtual ~BTreeIndexHeader();
    BTreeIndexHeader & operator=(const BTreeIndexHeader & rhs);
};

#endif /* BTREEINDEXHEADER_H_ */
