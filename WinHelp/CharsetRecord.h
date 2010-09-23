/* Charset record class definition.

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

#ifndef CHARSETRECORD_H_
#define CHARSETRECORD_H_

#include <QtGlobal>
#include <QTextCodec>
#include <QFile>

enum TextCharset : quint8
{
    ANSI_CHARSET = 0,
    DEFAULT_CHARSET = 1,
    SYMBOL_CHARSET = 2,
    MAC_CHARSET = 77,
    SHIFTJIS_CHARSET = 128,
    HANGEUL_OR_HANGUL_CHARSET = 129,
    JOHAB_CHARSET = 130,
    GB2312_CHARSET = 134,
    CHINESEBIG5_CHARSET = 136,
    GREEK_CHARSET = 161,
    TURKISH_CHARSET = 162,
    VIETNAMESE_CHARSET = 163,
    HEBREW_CHARSET = 177,
    ARABIC_SIMPLIFIED_CHARSET = 178,
    ARABIC_TRADITIONAL_CHARSET = 179,
    BALTIC_CHARSET = 186,
    RUSSIAN_CHARSET = 204,
    THAI_CHARSET = 222,
    EASTEUROPE_CHARSET = 238,
    PC437_CHARSET = 254,
    OEM_CHARSET = 255
};

class CharsetRecord
{
private:
    quint8 charset;
    QTextCodec *codec;
public:
    CharsetRecord();
    CharsetRecord(quint8 charset);
    CharsetRecord(QFile &file, qint64 off);
    CharsetRecord(const CharsetRecord& rhs);
    virtual ~CharsetRecord();
    CharsetRecord & operator=(const CharsetRecord & rhs);
    quint8 getCharset() const;
    QTextCodec *getCodec() const;
};

#endif /* CHARSETRECORD_H_ */
