/* System header class definition.

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

#ifndef SYSTEMHEADER_H_
#define SYSTEMHEADER_H_

#include <QtGlobal>
#include <QDateTime>
#include <QFile>

class SystemHeader
{
private:
    /** Значение первых двух байт заголовка. */
    static const quint16 magic = 0x036C;
    /**
     * Минорный номер версии формата файла, 2 байта без знака
     *    15 = HC30 Windows 3.0 help file
     *    21 = HC31 Windows 3.1 help file
     *    27 = WMVC/MMVC media view file
     *    33 = MVC or HCW 4.00 Windows 95
     */
    quint16 minorVersion;
    /** Мажорный номер версии формата файла, как правило, равен 1, 2 байта без знака. */
    quint16 majorVersion;
    /** Дата создания, предположительно в формате UNIX epoch time, или ноль, 4 байта без знака */
    QDateTime genDate;
    /**
     * Флаги, 2 байта без знака
     *    0: not compressed,  TopicBlockSize 4k
     *    4: LZ77 compressed, TopicBlockSize 4k
     *    8: LZ77 compressed, TopicBlockSize 2k
     */
    quint16 flags;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 12;
    static const quint16 hc30_format = 15;
    static const quint16 hc31_format = 21;
    static const quint16 wmvc_format = 27;
    static const quint16 hcw40_format = 33;
    static const quint16 not_compressed_4k_topic_block_flag = 0;
    static const quint16 lz77_compressed_4k_topic_block_flag = 4;
    static const quint16 lz77_compressed_2k_topic_block_flag = 8;
    quint16 getFlags() const;
    QDateTime getGenDate() const;
    quint16 getMajor() const;
    quint16 getMinor() const;
    SystemHeader();
    SystemHeader(quint16 minorVersion,
        quint16 majorVersion,
        QDateTime genDate,
        quint16 flags);
    SystemHeader(QFile &file, qint64 off);
    SystemHeader(const SystemHeader& rhs);
    virtual ~SystemHeader();
    SystemHeader & operator=(const SystemHeader & rhs);
};

#endif /* SYSTEMHEADER_H_ */
