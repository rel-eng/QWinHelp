/* Old font descriptor implementation.

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

#include "OldFontDescriptor.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

OldFontDescriptor::OldFontDescriptor() : FontDescriptor(OLD_FONT_DESCRIPTOR),
    bold(false), italic(false),
    underline(false), strikeOut(false), doubleUnderline(false), smallCaps(false),
    halfPoints(0), fontFamily(0), facenameIndex(0),
    fgColor(qRgb(0x00, 0x00, 0x00)), bgColor(qRgb(0xFF, 0xFF, 0xFF))
{
    PRINT_DBG("Old font descriptor default constructor");
}

OldFontDescriptor::OldFontDescriptor(QFile &file,
    qint64 off) : FontDescriptor(OLD_FONT_DESCRIPTOR)
{
    PRINT_DBG("Reading Old font descriptor at %lld", off);
    seekFile(file, off);
    quint8 attributes = readUnsignedByte(file);
    PRINT_DBG("        Attributes: %d", attributes);
    this->bold = false;
    if((attributes & 0x01) != 0)
    {
        this->bold = true;
        PRINT_DBG("        Bold font");
    }
    this->italic = false;
    if((attributes & 0x02) != 0)
    {
        this->italic = true;
        PRINT_DBG("        Italic font");
    }
    this->underline = false;
    if((attributes & 0x04) != 0)
    {
        this->underline = true;
        PRINT_DBG("        Underline font");
    }
    this->strikeOut = false;
    if((attributes & 0x08) != 0)
    {
        this->strikeOut = true;
        PRINT_DBG("        Strike out font");
    }
    this->doubleUnderline = false;
    if((attributes & 0x10) != 0)
    {
        this->doubleUnderline = true;
        PRINT_DBG("        Double underline font");
    }
    this->smallCaps = false;
    if((attributes & 0x20) != 0)
    {
        this->smallCaps = true;
        PRINT_DBG("        Small caps font");
    }
    this->halfPoints = readUnsignedByte(file);
    PRINT_DBG("        Half points: %d", this->halfPoints);
    this->fontFamily = readUnsignedByte(file);
    PRINT_DBG("        Font family: %d", this->fontFamily);
    this->facenameIndex = readUnsignedWord(file);
    PRINT_DBG("        Facename index: %d", this->facenameIndex);
    this->fgColor = readRGBBytes(file);
    PRINT_DBG("        Fg Red: %d", qRed(this->fgColor));
    PRINT_DBG("        Fg Green: %d", qGreen(this->fgColor));
    PRINT_DBG("        Fg Blue: %d", qBlue(this->fgColor));
    this->bgColor = readRGBBytes(file);
    PRINT_DBG("        Bg Red: %d", qRed(this->bgColor));
    PRINT_DBG("        Bg Green: %d", qGreen(this->bgColor));
    PRINT_DBG("        Bg Blue: %d", qBlue(this->bgColor));
    PRINT_DBG("Old font descriptor loaded successfully");
}

OldFontDescriptor::OldFontDescriptor(const OldFontDescriptor& rhs) :
    FontDescriptor(rhs), bold(rhs.
    bold), italic(rhs.italic), underline(rhs.underline),
    strikeOut(rhs.strikeOut),
    doubleUnderline(rhs.doubleUnderline), smallCaps(rhs.smallCaps),
    halfPoints(rhs.halfPoints), fontFamily(rhs.fontFamily),
    facenameIndex(rhs.facenameIndex), fgColor(rhs.fgColor), bgColor(rhs.bgColor)
{
    PRINT_DBG("Old font descriptor copy constructor");
}

OldFontDescriptor & OldFontDescriptor::operator=(const OldFontDescriptor & rhs)
{
    PRINT_DBG("Old font descriptor assignment operator");
    if (this != &rhs)
    {
        FontDescriptor::operator =(rhs);
        this->bold = rhs.bold;
        this->italic = rhs.italic;
        this->underline = rhs.underline;
        this->strikeOut = rhs.strikeOut;
        this->doubleUnderline = rhs.doubleUnderline;
        this->smallCaps = rhs.doubleUnderline;
        this->halfPoints = rhs.halfPoints;
        this->fontFamily = rhs.fontFamily;
        this->facenameIndex = rhs.facenameIndex;
        this->fgColor = rhs.fgColor;
        this->bgColor = rhs.bgColor;
    }
    return *this;
}

OldFontDescriptor::~OldFontDescriptor()
{
    PRINT_DBG("Old font descriptor destructor");
}

bool OldFontDescriptor::isBold() const
{
    return this->bold;
}

bool OldFontDescriptor::isItalic() const
{
    return this->italic;
}

bool OldFontDescriptor::isUnderline() const
{
    return this->underline;
}

bool OldFontDescriptor::isStrikeOut() const
{
    return this->strikeOut;
}

bool OldFontDescriptor::isDoubleUnderline() const
{
    return this->doubleUnderline;
}

bool OldFontDescriptor::isSmallCaps() const
{
    return this->smallCaps;
}

quint8 OldFontDescriptor::getHalfPoints() const
{
    return this->halfPoints;
}

quint8 OldFontDescriptor::getFontFamily() const
{
    return this->fontFamily;
}

quint16 OldFontDescriptor::getFacenameIndex() const
{
    return this->facenameIndex;
}

QRgb OldFontDescriptor::getFgColor() const
{
    return this->fgColor;
}

QRgb OldFontDescriptor::getBgColor() const
{
    return this->bgColor;
}
