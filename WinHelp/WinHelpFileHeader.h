/* WinHelp file header class definition.

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

#ifndef WINHELPFILEHEADER_H_
#define WINHELPFILEHEADER_H_

#include <QtGlobal>
#include <QFile>

class WinHelpFileHeader
{
private:
    /** Значение первых четыре байта заголовка. */
    static const quint32 magic = 0x00035F3F;
    /** Смещение заголовка внутреннего каталога относительно начала файла, 4 байта без знака. */
    qint64 directoryStart;
    /** Смещение заголовка списка свободных участков или -1L если он отсутствует, 4 байта без знака. */
    qint64 firstFreeBlock;
    /** Размер файла в байтах, 4 байта без знака. */
    qint64 entireFileSize;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 16;
    WinHelpFileHeader();
    WinHelpFileHeader(qint64 directoryStart,
        qint64 firstFreeBlock,
        qint64 entireFileSize);
    WinHelpFileHeader(QFile &file, qint64 off);
    WinHelpFileHeader(const WinHelpFileHeader& rhs);
    virtual ~WinHelpFileHeader();
    WinHelpFileHeader & operator=(const WinHelpFileHeader & rhs);
    qint64 getDirectoryStart() const;
    qint64 getEntireFileSize() const;
    qint64 getFirstFreeBlock() const;
};

#endif /* WINHELPFILEHEADER_H_ */
