/* QWinHelp main.

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

#ifndef MVBFONTDESCRIPTOR_H
#define MVBFONTDESCRIPTOR_H

#include <QtGlobal>
#include <QFile>
#include <QColor>

enum MVBFontPitch : quint8
{
    MVB_FONT_DEFAULT_PITCH = 0x00,
    MVB_FONT_FIXED_PITCH = 0x01,
    MVB_FONT_VARIABLE_PITCH = 0x02
};

enum MVBFontFamily : quint8
{
    MVB_FF_DONTCARE = 0x00,
    MVB_FF_ROMAN = 0x01,
    MVB_FF_SWISS = 0x02,
    MVB_FF_MODERN = 0x03,
    MVB_FF_SCRIPT = 0x04,
    MVB_FF_DECORATIVE = 0x05
};

enum MVBFontWeight : quint32
{
    MVB_FW_DONTCARE = 0,
    MVB_FW_THIN = 100,
    MVB_FW_EXTRALIGHT_OR_ULTRALIGHT = 200,
    MVB_FW_LIGHT = 300,
    MVB_FW_NORMAL_OR_REGULAR = 400,
    MVB_FW_MEDIUM = 500,
    MVB_FW_SEMIBOLD_OR_DEMIBOLD = 600,
    MVB_FW_BOLD = 700,
    MVB_FW_EXTRABOLD_OR_ULTRABOLD = 800,
    MVB_FW_HEAVY_OR_BLACK = 900
};

enum MVBFontCharset : quint8
{
    MVB_FONT_ANSI_CHARSET = 0,
    MVB_FONT_DEFAULT_CHARSET = 1,
    MVB_FONT_SYMBOL_CHARSET = 2,
    MVB_FONT_SHIFTJIS_CHARSET = 128,
    MVB_FONT_HANGEUL_CHARSET = 129,
    MVB_FONT_HANGUL_CHARSET = 129,
    MVB_FONT_GB2312_CHARSET = 134,
    MVB_FONT_CHINESEBIG5_CHARSET = 136,
    MVB_FONT_OEM_CHARSET = 255,
    MVB_FONT_JOHAB_CHARSET = 130,
    MVB_FONT_HEBREW_CHARSET = 177,
    MVB_FONT_ARABIC_CHARSET = 178,
    MVB_FONT_GREEK_CHARSET = 161,
    MVB_FONT_TURKISH_CHARSET = 162,
    MVB_FONT_VIETNAMESE_CHARSET = 163,
    MVB_FONT_THAI_CHARSET = 222,
    MVB_FONT_EASTEUROPE_CHARSET = 238,
    MVB_FONT_RUSSIAN_CHARSET = 204,
    MVB_FONT_MAC_CHARSET = 77,
    MVB_FONT_BALTIC_CHARSET = 186
};

enum MVBFontOutputPrecision : quint8
{
    MVB_FONT_OUT_DEFAULT_PRECISION = 0,
    MVB_FONT_OUT_STRING_PRECISION = 1,
    MVB_FONT_OUT_CHARACTER_PRECISION = 2,
    MVB_FONT_OUT_STROKE_PRECISION = 3,
    MVB_FONT_OUT_TRUETYPE_PRECISION = 4,
    MVB_FONT_OUT_DEVICE_PRECISION = 5,
    MVB_FONT_OUT_RASTER_PRECISION = 6,
    MVB_FONT_OUT_TRUETYPE_ONLY_PRECISION = 7,
    MVB_FONT_OUT_OUTLINE_PRECISION = 8,
    MVB_FONT_OUT_SCREEN_OUTLINE_PRECISION = 9,
    MVB_FONT_OUT_POSTSCRIPT_ONLY_PRECISION = 10
};

enum MVBFontClipPrecision : quint8
{
    MVB_FONT_CLIP_DEFAULT_PRECISION = 0,
    MVB_FONT_CLIP_CHARACTER_PRECISION = 1,
    MVB_FONT_CLIP_STROKE_PRECISION = 2,
    MVB_FONT_CLIP_MASK = 15,
    MVB_FONT_CLIP_LEFTHANDED_ANGLES = 16,
    MVB_FONT_CLIP_TRUETYPE_ALWAYS = 32,
    MVB_FONT_CLIP_DEFAULT_FONT_ASSOCIATION_DISABLE = 64,
    MVB_FONT_CLIP_EMBEDDED = 128
};

enum MVBFontQuality : quint8
{
    MVB_FONT_DEFAULT_QUALITY = 0,
    MVB_FONT_DRAFT_QUALITY = 1,
    MVB_FONT_PROOF_QUALITY = 2,
    MVB_FONT_NONANTIALIASED_QUALITY = 3,
    MVB_FONT_ANTIALIASED_QUALITY = 4,
    MVB_FONT_CLEARTYPE_QUALITY = 5,
    MVB_FONT_CLEARTYPE_NATURAL_QUALITY = 6
};

class MVBFontDescriptor
{
private:
    quint16 facenameIndex;
    quint16 styleNumber;
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
    MVBFontDescriptor();
    MVBFontDescriptor(QFile &file, qint64 off);
    MVBFontDescriptor(const MVBFontDescriptor& rhs);
    virtual ~MVBFontDescriptor();
    MVBFontDescriptor & operator=(const MVBFontDescriptor & rhs);
    quint16 getFacenameIndex() const;
    quint16 getStyleNumber() const;
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

#endif // MVBFONTDESCRIPTOR_H
