/* BTree index entry class definition.

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

#ifndef BTREEINDEXENTRY_H_
#define BTREEINDEXENTRY_H_

#include <QtGlobal>
#include <QFile>
#include <QString>

class BTreeIndexEntry
{
private:
    /** Имя файла, null-termainted строка. */
    QString fileName;
    /** Номер страницы, которая содержит записи с именами файлов лексически начинающихся с текущего имени файла, но лексически меньших следующего имени файла, 2 байта без знака */
    quint16 pageNumber;
    /** Размер записи в байтах. */
    qint64 sz;
public:
    BTreeIndexEntry();
    BTreeIndexEntry(const QString &fileName, quint16 pageNumber);
    BTreeIndexEntry(QFile &file, qint64 off);
    BTreeIndexEntry(const BTreeIndexEntry& rhs);
    virtual ~BTreeIndexEntry();
    BTreeIndexEntry & operator=(const BTreeIndexEntry & rhs);
    QString getFileName() const;
    quint16 getPageNumber() const;
    qint64 size() const;
};

#endif /* BTREEINDEXENTRY_H_ */
