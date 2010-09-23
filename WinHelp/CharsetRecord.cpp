/* Charset record class implementation.

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

#include "CharsetRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/TextUtils.h"

#include "SystemRecordHeader.h"

#include <QtDebug>

#include <stdexcept>

CharsetRecord::CharsetRecord() :
    charset(0), codec(getDefaultTextCodec())
{
    PRINT_DBG("Charset record default constructor");
}

CharsetRecord::CharsetRecord(quint8 charset) :
    charset(charset)
{
    PRINT_DBG("Charset record constructor");
    switch (static_cast<TextCharset> (charset))
    {
    case TextCharset::ANSI_CHARSET:
        codec = QTextCodec::codecForName("CP1252");
        break;

    case TextCharset::DEFAULT_CHARSET:
        codec = QTextCodec::codecForName("CP1252");
        break;

    case TextCharset::SYMBOL_CHARSET:
        codec = QTextCodec::codecForName("CP1252");
        qDebug() << "Symbol charset is not really supported";
        break;

    case TextCharset::MAC_CHARSET:
        codec = QTextCodec::codecForName("Apple Roman");
        break;

    case TextCharset::SHIFTJIS_CHARSET:
        codec = QTextCodec::codecForName("Shift-JIS");
        break;

    case TextCharset::HANGEUL_OR_HANGUL_CHARSET:
        codec = QTextCodec::codecForName("eucKR");
        break;

    case TextCharset::JOHAB_CHARSET:
        codec = QTextCodec::codecForName("CP1361");
        qDebug() << "CP1361 may not be really supported by Qt right now";
        break;

    case TextCharset::GB2312_CHARSET:
        codec = QTextCodec::codecForName("GB2312");
        break;

    case TextCharset::CHINESEBIG5_CHARSET:
        codec = QTextCodec::codecForName("Big5-HKSCS");
        break;

    case TextCharset::GREEK_CHARSET:
        codec = QTextCodec::codecForName("CP1253");
        break;

    case TextCharset::TURKISH_CHARSET:
        codec = QTextCodec::codecForName("CP1254");
        break;

    case TextCharset::VIETNAMESE_CHARSET:
        codec = QTextCodec::codecForName("CP1258");
        break;

    case TextCharset::HEBREW_CHARSET:
        codec = QTextCodec::codecForName("CP1255");
        break;

    case TextCharset::ARABIC_SIMPLIFIED_CHARSET:
        codec = QTextCodec::codecForName("CP1256");
        break;

    case TextCharset::ARABIC_TRADITIONAL_CHARSET:
        codec = QTextCodec::codecForName("CP1256");
        qDebug() << "Is Arabic Traditional really a CP1256?";
        break;

    case TextCharset::BALTIC_CHARSET:
        codec = QTextCodec::codecForName("CP1257");
        break;

    case TextCharset::RUSSIAN_CHARSET:
        codec = QTextCodec::codecForName("CP1251");
        break;

    case TextCharset::THAI_CHARSET:
        codec = QTextCodec::codecForName("CP874");
        break;

    case TextCharset::EASTEUROPE_CHARSET:
        codec = QTextCodec::codecForName("CP1250");
        break;

    case TextCharset::PC437_CHARSET:
        codec = QTextCodec::codecForName("CP850");
        qDebug() << "PC437 is not really a CP850";
        break;

    case TextCharset::OEM_CHARSET:
        codec = QTextCodec::codecForName("CP850");
        break;

    default:
        codec = getDefaultTextCodec();
        qDebug() << "Unknown charset, defaulting to CP1252";
    }
    if (codec == NULL)
    {
        codec = getDefaultTextCodec();
        qDebug() << "Defaulting to CP1252";
    }
}

CharsetRecord::CharsetRecord(QFile &file, qint64 off)
{
    PRINT_DBG("Reading charset record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    if (hdr.getRecordSize() < Q_INT64_C(1))
    {
        qDebug() << "Charset record size is too small: " << hdr.getRecordSize();
    }
    this->charset = readUnsignedByte(file);
    PRINT_DBG("        Charset: %d", this->charset);
    switch (static_cast<TextCharset> (charset))
    {
    case TextCharset::ANSI_CHARSET:
        codec = QTextCodec::codecForName("CP1252");
        break;

    case TextCharset::DEFAULT_CHARSET:
        codec = QTextCodec::codecForName("CP1252");
        break;

    case TextCharset::SYMBOL_CHARSET:
        codec = QTextCodec::codecForName("CP1252");
        qDebug() << "Symbol charset is not really supported";
        break;

    case TextCharset::MAC_CHARSET:
        codec = QTextCodec::codecForName("Apple Roman");
        break;

    case TextCharset::SHIFTJIS_CHARSET:
        codec = QTextCodec::codecForName("Shift-JIS");
        break;

    case TextCharset::HANGEUL_OR_HANGUL_CHARSET:
        codec = QTextCodec::codecForName("eucKR");
        break;

    case TextCharset::JOHAB_CHARSET:
        codec = QTextCodec::codecForName("CP1361");
        qDebug() << "CP1361 may not be really supported by Qt right now";
        break;

    case TextCharset::GB2312_CHARSET:
        codec = QTextCodec::codecForName("GB2312");
        break;

    case TextCharset::CHINESEBIG5_CHARSET:
        codec = QTextCodec::codecForName("Big5-HKSCS");
        break;

    case TextCharset::GREEK_CHARSET:
        codec = QTextCodec::codecForName("CP1253");
        break;

    case TextCharset::TURKISH_CHARSET:
        codec = QTextCodec::codecForName("CP1254");
        break;

    case TextCharset::VIETNAMESE_CHARSET:
        codec = QTextCodec::codecForName("CP1258");
        break;

    case TextCharset::HEBREW_CHARSET:
        codec = QTextCodec::codecForName("CP1255");
        break;

    case TextCharset::ARABIC_SIMPLIFIED_CHARSET:
        codec = QTextCodec::codecForName("CP1256");
        break;

    case TextCharset::ARABIC_TRADITIONAL_CHARSET:
        codec = QTextCodec::codecForName("CP1256");
        qDebug() << "Is Arabic Traditional really a CP1256?";
        break;

    case TextCharset::BALTIC_CHARSET:
        codec = QTextCodec::codecForName("CP1257");
        break;

    case TextCharset::RUSSIAN_CHARSET:
        codec = QTextCodec::codecForName("CP1251");
        break;

    case TextCharset::THAI_CHARSET:
        codec = QTextCodec::codecForName("CP874");
        break;

    case TextCharset::EASTEUROPE_CHARSET:
        codec = QTextCodec::codecForName("CP1250");
        break;

    case TextCharset::PC437_CHARSET:
        codec = QTextCodec::codecForName("CP850");
        qDebug() << "PC437 is not really a CP850";
        break;

    case TextCharset::OEM_CHARSET:
        codec = QTextCodec::codecForName("CP850");
        break;

    default:
        codec = getDefaultTextCodec();
        qDebug() << "Unknown charset, defaulting to CP1252";
    }
    if (codec == NULL)
    {
        codec = getDefaultTextCodec();
        qDebug() << "Defaulting to CP1252";
    }
    PRINT_DBG("Charset record loaded successfully");
}

CharsetRecord::CharsetRecord(const CharsetRecord& rhs) :
    charset(rhs.charset), codec(rhs.codec)
{
    PRINT_DBG("Charset record copy constructor");
}

CharsetRecord & CharsetRecord::operator=(const CharsetRecord & rhs)
{
    PRINT_DBG("Charset record assignment operator");
    if (this != &rhs)
    {
        this->charset = rhs.charset;
        this->codec = rhs.codec;
    }
    return *this;
}

CharsetRecord::~CharsetRecord()
{
    PRINT_DBG("Charset record destructor");
}

quint8 CharsetRecord::getCharset() const
{
    return this->charset;
}

QTextCodec *CharsetRecord::getCodec() const
{
    return this->codec;
}
