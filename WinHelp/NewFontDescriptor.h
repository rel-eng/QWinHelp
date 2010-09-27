/* New font descriptor declaration.

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

#ifndef NEWFONTDESCRIPTOR_H
#define NEWFONTDESCRIPTOR_H

#include <QtGlobal>
#include <QFile>
#include <QColor>

enum NewFontPitch : quint8
{
    NEW_FONT_DEFAULT_PITCH = 0x00,
    NEW_FONT_FIXED_PITCH = 0x01,
    NEW_FONT_VARIABLE_PITCH = 0x02
};

enum NewFontFamily : quint8
{
    NEW_FF_DONTCARE = 0x00,
    NEW_FF_ROMAN = 0x01,
    NEW_FF_SWISS = 0x02,
    NEW_FF_MODERN = 0x03,
    NEW_FF_SCRIPT = 0x04,
    NEW_FF_DECORATIVE = 0x05
};

enum NewFontWeight : quint32
{
    NEW_FW_DONTCARE = 0,
    NEW_FW_THIN = 100,
    NEW_FW_EXTRALIGHT_OR_ULTRALIGHT = 200,
    NEW_FW_LIGHT = 300,
    NEW_FW_NORMAL_OR_REGULAR = 400,
    NEW_FW_MEDIUM = 500,
    NEW_FW_SEMIBOLD_OR_DEMIBOLD = 600,
    NEW_FW_BOLD = 700,
    NEW_FW_EXTRABOLD_OR_ULTRABOLD = 800,
    NEW_FW_HEAVY_OR_BLACK = 900
};

enum NewFontCharset : quint8
{
    NEW_FONT_ANSI_CHARSET = 0,
    NEW_FONT_DEFAULT_CHARSET = 1,
    NEW_FONT_SYMBOL_CHARSET = 2,
    NEW_FONT_SHIFTJIS_CHARSET = 128,
    NEW_FONT_HANGEUL_CHARSET = 129,
    NEW_FONT_HANGUL_CHARSET = 129,
    NEW_FONT_GB2312_CHARSET = 134,
    NEW_FONT_CHINESEBIG5_CHARSET = 136,
    NEW_FONT_OEM_CHARSET = 255,
    NEW_FONT_JOHAB_CHARSET = 130,
    NEW_FONT_HEBREW_CHARSET = 177,
    NEW_FONT_ARABIC_CHARSET = 178,
    NEW_FONT_GREEK_CHARSET = 161,
    NEW_FONT_TURKISH_CHARSET = 162,
    NEW_FONT_VIETNAMESE_CHARSET = 163,
    NEW_FONT_THAI_CHARSET = 222,
    NEW_FONT_EASTEUROPE_CHARSET = 238,
    NEW_FONT_RUSSIAN_CHARSET = 204,
    NEW_FONT_MAC_CHARSET = 77,
    NEW_FONT_BALTIC_CHARSET = 186
};

enum NewFontOutputPrecision : quint8
{
    NEW_FONT_OUT_DEFAULT_PRECISION = 0,
    NEW_FONT_OUT_STRING_PRECISION = 1,
    NEW_FONT_OUT_CHARACTER_PRECISION = 2,
    NEW_FONT_OUT_STROKE_PRECISION = 3,
    NEW_FONT_OUT_TRUETYPE_PRECISION = 4,
    NEW_FONT_OUT_DEVICE_PRECISION = 5,
    NEW_FONT_OUT_RASTER_PRECISION = 6,
    NEW_FONT_OUT_TRUETYPE_ONLY_PRECISION = 7,
    NEW_FONT_OUT_OUTLINE_PRECISION = 8,
    NEW_FONT_OUT_SCREEN_OUTLINE_PRECISION = 9,
    NEW_FONT_OUT_POSTSCRIPT_ONLY_PRECISION = 10
};

enum NewFontClipPrecision : quint8
{
    NEW_FONT_CLIP_DEFAULT_PRECISION = 0,
    NEW_FONT_CLIP_CHARACTER_PRECISION = 1,
    NEW_FONT_CLIP_STROKE_PRECISION = 2,
    NEW_FONT_CLIP_MASK = 15,
    NEW_FONT_CLIP_LEFTHANDED_ANGLES = 16,
    NEW_FONT_CLIP_TRUETYPE_ALWAYS = 32,
    NEW_FONT_CLIP_DEFAULT_FONT_ASSOCIATION_DISABLE = 64,
    NEW_FONT_CLIP_EMBEDDED = 128
};

enum NewFontQuality : quint8
{
    NEW_FONT_DEFAULT_QUALITY = 0,
    NEW_FONT_DRAFT_QUALITY = 1,
    NEW_FONT_PROOF_QUALITY = 2,
    NEW_FONT_NONANTIALIASED_QUALITY = 3,
    NEW_FONT_ANTIALIASED_QUALITY = 4,
    NEW_FONT_CLEARTYPE_QUALITY = 5,
    NEW_FONT_CLEARTYPE_NATURAL_QUALITY = 6
};

class NewFontDescriptor
{
private:
    quint16 facenameIndex;
    QRgb fgColor;
    QRgb bgColor;
    qint32 height;
    qint32 width;
    qint32 escapement;
    qint32 orientation;
    qint32 weight;
    quint8 pitch;
    quint8 family;
    bool italic;
    bool underline;
    bool strikeOut;
    quint8 charset;
    quint8 outPrecision;
    quint8 clipPrecision;
    quint8 quality;
public:
    NewFontDescriptor();
    NewFontDescriptor(QFile &file, qint64 off);
    NewFontDescriptor(const NewFontDescriptor& rhs);
    virtual ~NewFontDescriptor();
    NewFontDescriptor & operator=(const NewFontDescriptor & rhs);
    quint16 getFacenameIndex() const;
    QRgb getFgColor() const;
    QRgb getBgColor() const;
    qint32 getHeight() const;
    qint32 getWidth() const;
    qint32 getEscapement() const;
    qint32 getOrientation() const;
    qint32 getWeight() const;
    quint8 getPitch() const;
    quint8 getFamily() const;
    bool isItalic() const;
    bool isUnderline() const;
    bool isStrikeOut() const;
    quint8 getCharset() const;
    quint8 getOutPrecision() const;
    quint8 getClipPrecision() const;
    quint8 getQuality() const;
};

#endif // NEWFONTDESCRIPTOR_H
