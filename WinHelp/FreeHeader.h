/* Free header class definition.

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

#ifndef FREEHEADER_H_
#define FREEHEADER_H_

#include <QtGlobal>
#include <QFile>

class FreeHeader
{
private:
    /** Ссылка на следующий свободный блок не действительна. */
    static const qint64 noMoreFreeBlocks = 0x00000000FFFFFFFF;
    /** Количество неиспользуемых байт, включая этот заголовок, 4 байта без знака. */
    qint64 freeSpace;
    /** Смещение следующего заголовка свободного блока или 0xFFFFFFFF если это последний блок, 4 байта без знака. */
    qint64 nextFreeBlock;
    /** Есть ли следующий свободный блок. */
    bool hasNext;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 8;
    FreeHeader();
    FreeHeader(qint64 freeSpace, qint64 nextFreeBlock);
    FreeHeader(QFile &file, qint64 off);
    FreeHeader(const FreeHeader& rhs);
    virtual ~FreeHeader();
    FreeHeader & operator=(const FreeHeader & rhs);
    qint64 getFreeSpace() const;
    qint64 getNextFreeBlock() const;
    bool hasNextBlock() const;
};

#endif /* FREEHEADER_H_ */
