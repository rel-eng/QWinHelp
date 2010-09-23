/* BTree node entry class definition.

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

#ifndef BTREENODEENTRY_H_
#define BTREENODEENTRY_H_

#include <QtGlobal>
#include <QFile>
#include <QString>

class BTreeNodeEntry
{
private:
    /** Имя файла, null-termainted строка. */
    QString fileName;
    /** Смещение внутреннего файла внутри файла WinHelp, 4 байта без знака */
    qint64 offset;
    /** Размер записи в байтах. */
    qint64 sz;
public:
    BTreeNodeEntry();
    BTreeNodeEntry(const QString &fileName, qint64 offset);
    BTreeNodeEntry(QFile &file, qint64 off);
    BTreeNodeEntry(const BTreeNodeEntry& rhs);
    virtual ~BTreeNodeEntry();
    BTreeNodeEntry & operator=(const BTreeNodeEntry & rhs);
    QString getFileName() const;
    qint64 getOffset() const;
    qint64 size() const;
};

#endif /* BTREENODEENTRY_H_ */
