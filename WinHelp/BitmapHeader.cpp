/* Bitmap header implementation.

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

#include "BitmapHeader.h"

#include "Utils/DebugUtils.h"
#include "Utils/IOUtils.h"

#include <stdexcept>

BitmapHeader::BitmapHeader() : xdpi(0), ydpi(0), planes(0), bitCount(0),
    width(0), height(0), colorsUsed(0), colorsImportant(0), compressedSize(0),
    hotspotSize(0), compressedOffset(0), hotspotOffset(0), sz(Q_INT64_C(0))
{
    PRINT_DBG("Bitmap header default constructor");
}

BitmapHeader::BitmapHeader(QIODevice &device, qint64 off)
{
    PRINT_DBG("Reading Bitmap header at: %lld", off);
    seekFile(device, off);
    this->xdpi = readCompressedUnsignedDWord(device);
    PRINT_DBG("        XDPI: %d", this->xdpi);
    this->ydpi = readCompressedUnsignedDWord(device);
    PRINT_DBG("        YDPI: %d", this->ydpi);
    this->planes = readCompressedUnsignedWord(device);
    PRINT_DBG("        Planes: %d", this->planes);
    this->bitCount = readCompressedUnsignedWord(device);
    PRINT_DBG("        Bit count: %d", this->bitCount);
    this->width = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Width: %d", this->width);
    this->height = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Height: %d", this->height);
    this->colorsUsed = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Colors used: %d", this->colorsUsed);
    this->colorsImportant = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Colors important: %d", this->colorsImportant);
    this->compressedSize = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Compressed size: %d", this->compressedSize);
    this->hotspotSize = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Hotspot size: %d", this->hotspotSize);
    this->compressedOffset = readUnsignedDWord(device);
    PRINT_DBG("        Compressed offset: %d", this->compressedOffset);
    this->hotspotOffset = readUnsignedDWord(device);
    PRINT_DBG("        Hotspot offset: %d", this->hotspotOffset);
    this->sz = device.pos() - off;
    PRINT_DBG("Bitmap header loaded successfully");
}

BitmapHeader::BitmapHeader(const BitmapHeader &rhs) : xdpi(rhs.xdpi),
    ydpi(rhs.ydpi), planes(rhs.planes), bitCount(rhs.bitCount), width(rhs.width),
    height(rhs.height), colorsUsed(rhs.colorsUsed),
    colorsImportant(rhs.colorsImportant), compressedSize(rhs.compressedSize),
    hotspotSize(rhs.hotspotSize), compressedOffset(rhs.compressedOffset),
    hotspotOffset(rhs.hotspotOffset), sz(rhs.sz)
{
    PRINT_DBG("Bitmap header copy constructor");
}

BitmapHeader::~BitmapHeader()
{
    PRINT_DBG("Bitmap header destructor");
}

BitmapHeader & BitmapHeader::operator=(const BitmapHeader &rhs)
{
    PRINT_DBG("Bitmap header assignment operator");
    if(this != &rhs)
    {
        this->xdpi = rhs.xdpi;
        this->ydpi = rhs.ydpi;
        this->planes = rhs.planes;
        this->bitCount = rhs.bitCount;
        this->width = rhs.width;
        this->height = rhs.height;
        this->colorsUsed = rhs.colorsUsed;
        this->colorsImportant = rhs.colorsImportant;
        this->compressedSize = rhs.compressedSize;
        this->hotspotSize = rhs.hotspotSize;
        this->compressedOffset = rhs.compressedOffset;
        this->hotspotOffset = rhs.hotspotOffset;
        this->sz = rhs.sz;
    }
    return *this;
}

quint32 BitmapHeader::getXdpi() const
{
    return this->xdpi;
}

quint32 BitmapHeader::getYdpi() const
{
    return this->ydpi;
}

quint16 BitmapHeader::getPlanes() const
{
    return this->planes;
}

quint16 BitmapHeader::getBitCount() const
{
    return this->bitCount;
}

quint32 BitmapHeader::getWidth() const
{
    return this->width;
}

quint32 BitmapHeader::getHeight() const
{
    return this->height;
}

quint32 BitmapHeader::getColorsUsed() const
{
    return this->colorsUsed;
}

quint32 BitmapHeader::getColorsImportant() const
{
    return this->colorsImportant;
}

quint32 BitmapHeader::getCompressedSize() const
{
    return this->compressedSize;
}

quint32 BitmapHeader::getHotspotSize() const
{
    return this->hotspotSize;
}

quint32 BitmapHeader::getCompressedOffset() const
{
    return this->compressedOffset;
}

quint32 BitmapHeader::getHotspotOffset() const
{
    return this->hotspotOffset;
}

qint64 BitmapHeader::size() const
{
    return this->sz;
}
