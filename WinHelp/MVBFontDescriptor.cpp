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

#include "MVBFontDescriptor.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

MVBFontDescriptor::MVBFontDescriptor() : facenameIndex(0), styleNumber(0), fgColor(qRgb(0x00, 0x00, 0x00)), bgColor(qRgb(0xFF, 0xFF, 0xFF)), height(0), width(0), escapement(0), orientation(0), weight(0), pitch(0), family(0), italic(false), underline(false), strikeOut(false), charset(0), outPrecision(0), clipPrecision(0), quality(0)
{
    PRINT_DBG("MVB font descriptor default constructor");
}

MVBFontDescriptor::MVBFontDescriptor(QFile &file, qint64 off)
{
    PRINT_DBG("Reading MVB font descriptor at %lld", off);
    seekFile(file, off);
    this->facenameIndex = readUnsignedWord(file);
    PRINT_DBG("        Facename index: %d", this->facenameIndex);
    this->styleNumber = readUnsignedWord(file);
    PRINT_DBG("        StyleNumber: %d", this->styleNumber);
    quint8 unknownByte1 = readUnsignedByte(file);
    PRINT_DBG("        Unknown byte 1: %d", unknownByte1);
    quint8 unknownByte2 = readUnsignedByte(file);
    PRINT_DBG("        Unknown byte 2: %d", unknownByte2);
    this->fgColor = readRGBBytes(file);
    PRINT_DBG("        Fg Red: %d", qRed(this->fgColor));
    PRINT_DBG("        Fg Green: %d", qGreen(this->fgColor));
    PRINT_DBG("        Fg Blue: %d", qBlue(this->fgColor));
    this->bgColor = readRGBBytes(file);
    PRINT_DBG("        Bg Red: %d", qRed(this->bgColor));
    PRINT_DBG("        Bg Green: %d", qGreen(this->bgColor));
    PRINT_DBG("        Bg Blue: %d", qBlue(this->bgColor));
    this->height = readSignedDWord(file);
    PRINT_DBG("        Height: %d", this->height);
    this->width = readSignedDWord(file);
    PRINT_DBG("        Width: %d", this->width);
    this->escapement = readSignedDWord(file);
    PRINT_DBG("        Escapement: %d", this->escapement);
    this->orientation = readSignedDWord(file);
    PRINT_DBG("        Orientation: %d", this->orientation);
    this->weight = readSignedDWord(file);
    PRINT_DBG("        Weight: %d", this->weight);
    quint8 italicByte = readUnsignedByte(file);
    this->italic = false;
    if(italicByte!=0)
    {
        this->italic = true;
        PRINT_DBG("        Italic font");
    }
    quint8 underlineByte = readUnsignedByte(file);
    this->underline = false;
    if(underlineByte!=0)
    {
        this->underline = true;
        PRINT_DBG("        Underline font");
    }
    quint8 strikeoutByte = readUnsignedByte(file);
    this->strikeOut = false;
    if(strikeoutByte!=0)
    {
        this->strikeOut = true;
        PRINT_DBG("        Strike out font");
    }
    this->charset = readUnsignedByte(file);
    PRINT_DBG("        Charset: %d", this->charset);
    this->outPrecision = readUnsignedByte(file);
    PRINT_DBG("        Out precision: %d", this->outPrecision);
    this->clipPrecision = readUnsignedByte(file);
    PRINT_DBG("        Clip precision: %d", this->clipPrecision);
    this->quality = readUnsignedByte(file);
    PRINT_DBG("        Quality: %d", this->quality);
    quint8 pitchAndFamily = readUnsignedByte(file);
    this->pitch = pitchAndFamily & 0x03;
    PRINT_DBG("        Pitch: %d", this->pitch);
    this->family = (((pitchAndFamily & 0xF0)>>4)&0x0F);
    PRINT_DBG("        Family: %d", this->family);
    quint8 unknownByte3 = readUnsignedByte(file);
    PRINT_DBG("        Unknown byte 3: %d", unknownByte3);
    quint8 unknownByte4 = readUnsignedByte(file);
    PRINT_DBG("        Unknown byte 4: %d", unknownByte4);
    PRINT_DBG("MVB font descriptor loaded successfully");
}


MVBFontDescriptor::MVBFontDescriptor(const MVBFontDescriptor& rhs) : facenameIndex(rhs.facenameIndex), styleNumber(rhs.styleNumber), fgColor(rhs.fgColor), bgColor(rhs.bgColor), height(rhs.height), width(rhs.width), escapement(rhs.escapement), orientation(rhs.orientation), weight(rhs.weight), pitch(rhs.pitch), family(rhs.family), italic(rhs.italic), underline(rhs.underline), strikeOut(rhs.strikeOut), charset(rhs.charset), outPrecision(rhs.outPrecision), clipPrecision(rhs.clipPrecision), quality(rhs.quality)
{
    PRINT_DBG("MVB font descriptor copy constructor");
}

MVBFontDescriptor & MVBFontDescriptor::operator=(const MVBFontDescriptor & rhs)
{
    PRINT_DBG("MVB font descriptor assignment operator");
    if (this != &rhs)
    {
        this->facenameIndex = rhs.facenameIndex;
        this->styleNumber = rhs.styleNumber;
        this->fgColor = rhs.fgColor;
        this->bgColor = rhs.bgColor;
        this->height = rhs.height;
        this->width = rhs.width;
        this->escapement = rhs.escapement;
        this->orientation = rhs.orientation;
        this->weight = rhs.weight;
        this->pitch = rhs.pitch;
        this->family = rhs.family;
        this->italic = rhs.italic;
        this->underline = rhs.underline;
        this->strikeOut = rhs.strikeOut;
        this->charset = rhs.charset;
        this->outPrecision = rhs.outPrecision;
        this->clipPrecision = rhs.clipPrecision;
        this->quality = rhs.quality;
    }
    return *this;
}

MVBFontDescriptor::~MVBFontDescriptor()
{
    PRINT_DBG("MVB font descriptor destructor");
}

quint16 MVBFontDescriptor::getFacenameIndex() const
{
    return this->facenameIndex;
}

quint16 MVBFontDescriptor::getStyleNumber() const
{
    return this->styleNumber;
}

QRgb MVBFontDescriptor::getFgColor() const
{
    return this->fgColor;
}

QRgb MVBFontDescriptor::getBgColor() const
{
    return this->bgColor;
}

qint32 MVBFontDescriptor::getHeight() const
{
    return this->height;
}

qint32 MVBFontDescriptor::getWidth() const
{
    return this->width;
}

qint32 MVBFontDescriptor::getEscapement() const
{
    return this->escapement;
}

qint32 MVBFontDescriptor::getOrientation() const
{
    return this->orientation;
}

qint32 MVBFontDescriptor::getWeight() const
{
    return this->weight;
}

quint8 MVBFontDescriptor::getPitch() const
{
    return this->pitch;
}

quint8 MVBFontDescriptor::getFamily() const
{
    return this->family;
}

bool MVBFontDescriptor::isItalic() const
{
    return this->italic;
}

bool MVBFontDescriptor::isUnderline() const
{
    return this->underline;
}

bool MVBFontDescriptor::isStrikeOut() const
{
    return this->strikeOut;
}

quint8 MVBFontDescriptor::getCharset() const
{
    return this->charset;
}

quint8 MVBFontDescriptor::getOutPrecision() const
{
    return this->outPrecision;
}

quint8 MVBFontDescriptor::getClipPrecision() const
{
    return this->clipPrecision;
}

quint8 MVBFontDescriptor::getQuality() const
{
    return this->quality;
}
