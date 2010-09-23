/* Internal directory file header class definition.

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

#ifndef INTERNALDIRECTORYFILEHEADER_H_
#define INTERNALDIRECTORYFILEHEADER_H_

#include <QtGlobal>
#include <QFile>

class InternalDirectoryFileHeader
{
private:
    /** Зарезервированный размер, включая этот заголовок, 4 байта без знака. */
    qint64 reservedSpace;
    /** Размер внутреннего файла в байтах, 4 байта без знака. */
    qint64 usedSpace;
    /** Флаги, как правило равно 4, один байт. */
    quint8 fileFlags;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 9;
    InternalDirectoryFileHeader();
    InternalDirectoryFileHeader(qint64 reservedSpace,
        qint64 usedSpace,
        quint8 fileFlags);
    InternalDirectoryFileHeader(QFile &file, qint64 off);
    InternalDirectoryFileHeader(const InternalDirectoryFileHeader& rhs);
    virtual ~InternalDirectoryFileHeader();
    InternalDirectoryFileHeader & operator=(
        const InternalDirectoryFileHeader & rhs);
    quint8 getFileFlags() const;
    qint64 getReservedSpace() const;
    qint64 getUsedSpace() const;
};

#endif /* INTERNALDIRECTORYFILEHEADER_H_ */
