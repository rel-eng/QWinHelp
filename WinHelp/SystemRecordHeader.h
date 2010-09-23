/* System record header class definition.

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

#ifndef SYSTEMRECORDHEADER_H_
#define SYSTEMRECORDHEADER_H_

#include <QtGlobal>
#include <QFile>

enum SystemRecordType : quint16
{
    /** help file title (STRINGZ). */
    TITLE_RECORD = 1,
    /** copyright notice shown in AboutBox (STRINGZ). */
    COPYRIGHT_RECORD = 2,
    /** topic offset of starting topic (TOPICOFFSET). */
    CONTENTS_RECORD = 3,
    /** all macros executed on opening (STRINGZ). */
    CONFIG_RECORD = 4,
    /** Windows *.ICO file (See WIN31WH on icon file format). */
    ICON_RECORD = 5,
    /** Windows defined in the HPJ-file. */
    WINDOW_HPJ_FILE_RECORD = 6,
    /** Viewer 2.0 Windows defined in MVP-file. */
    WINDOW_MVP_FILE_RECORD = 6,
    /** the Citation printed (STRINGZ). */
    CITATION_RECORD = 8,
    /** short LCID[4] language ID, Windows 95 (HCW 4.00). */
    LCID_RECORD = 9,
    /** CNT file name, Windows 95 (HCW 4.00) (STRINGZ). */
    CNT_RECORD = 10,
    /** unsigned short Charset charset, Windows 95 (HCW 4.00)*/
    CHARSET_RECORD = 11,
    /** default dialog font, Windows 95 (HCW 4.00). */
    DEFFONT_RECORD = 12,
    /** Multimedia Help Files dtypes (STRINGZ). */
    FTINDEX_RECORD = 12,
    /** defined GROUPs, Multimedia Help File (STRINGZ). */
    GROUPS_RECORD = 13,
    /** separators, Windows 95 (HCW 4.00) (STRINGZ). */
    INDEX_S_RECORD = 14,
    /** Multimedia Help Files. */
    KEYINDEX_RECORD = 14,
    /** defined language, Multimedia Help Files (STRINGZ). */
    LANGUAGE_RECORD = 18,
    /** defined DLLMAPS, Multimedia Help Files. */
    DLLMAPS_RECORD = 19
};

class SystemRecordHeader
{
private:
    /** Тип записи, 2 байта без знака. */
    quint16 recordType;
    /** Размер данных, 2 байта без знака. */
    qint64 recordSize;
public:
    /** Размер заголовка в байтах. */
    static const qint64 size = 4;
    SystemRecordHeader();
    SystemRecordHeader(quint16 recordType, qint64 recordSize);
    SystemRecordHeader(QFile &file, qint64 off);
    SystemRecordHeader(const SystemRecordHeader& rhs);
    virtual ~SystemRecordHeader();
    SystemRecordHeader & operator=(const SystemRecordHeader & rhs);
    quint16 getRecordType() const;
    qint64 getRecordSize() const;
};

#endif /* SYSTEMRECORDHEADER_H_ */
