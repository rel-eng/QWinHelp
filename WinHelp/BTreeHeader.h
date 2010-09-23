/* BTree header class definition.

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

#ifndef BTREEHEADER_H_
#define BTREEHEADER_H_

#include <QtGlobal>
#include <QFile>
#include <QString>

class BTreeHeader
{
private:
    /** Значение первых двух байта заголовка. */
    static const quint16 magic = 0x293B;
    /** Значение первого неиспользуемого двухбайтового поля, всегда ноль. */
    static const quint16 unused_1 = 0x0000;
    /** Значение второго неиспользуемого двухбайтового поля, всегда -1. */
    static const quint16 unused_2 = 0xFFFF;
    /** В поле флагов бит с маской 0x0002 всегда 1. */
    static const quint16 flags_mask_magic_bit = 0x0002;
    /** В поле флагов бит с маской 0x0400 1 если это каталог. */
    static const quint16 flags_mask_catalog_bit = 0x0400;
    /** Двухбайтовое поле флагов, бит с маской 0x0002 всегда 1, бит с маской 0x0400 1 если это каталог. */
    quint16 flags;
    /** Размер страницы, два байта без знака, 0x0400=1k если это каталог, 0x0800=2k иначе, или 4k.  */
    qint64 pageSize;
    /**
     * Строка, описывающая формат данных, 16 байт
     * 'L' = long (indexed)
     * 'F' = NUL-terminated string (indexed)
     * 'i' = NUL-terminated string (indexed)
     * '2' = short
     * '4' = long
     * 'z' = NUL-terminated string
     * '!' = long count value, count/8 * record
     *  long filenumber
     *  long TopicOffset
     */
    QString structure;
    /** Количество разбиений страниц, которым подверглось B+ дерево, 2 байта без знака. */
    quint16 pageSplits;
    /** Номер корневой страницы B+ дерева, 2 байта без знака. */
    quint16 rootPage;
    /** Количество страниц в B+ дереве, 2 байта без знака. */
    quint16 totalPages;
    /** Количество уровней в B+ дереве, 2 байта без знака. */
    quint16 nLevels;
    /** Количество элементов данных в B+ дереве, 4 байта без знака. */
    quint32 totalBTreeEntries;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 38;
    BTreeHeader();
    BTreeHeader(quint16 flags,
        qint64 pageSize,
        const QString &structure,
        quint16 pageSplits,
        quint16 rootPage,
        quint16 totalPages,
        quint16 nLevels,
        quint32 totalBTreeEntries);
    BTreeHeader(QFile &file, qint64 off);
    BTreeHeader(const BTreeHeader& rhs);
    virtual ~BTreeHeader();
    BTreeHeader & operator=(const BTreeHeader & rhs);
    quint16 getFlags() const;
    quint16 getNLevels() const;
    qint64 getPageSize() const;
    quint16 getPageSplits() const;
    quint16 getRootPage() const;
    QString getStructure() const;
    quint32 getTotalBTreeEntries() const;
    quint16 getTotalPages() const;
};

#endif /* BTREEHEADER_H_ */
