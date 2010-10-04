/* Metafile header implementation.

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

#include "MetafileHeader.h"

#include "Utils/DebugUtils.h"
#include "Utils/IOUtils.h"

#include <stdexcept>

MetafileHeader::MetafileHeader() : mappingMode(0), width(0), height(0),
    decompressedSize(0), compressedSize(0), hotspotSize(0), compressedOffset(0),
    hotspotOffset(0), sz(Q_INT64_C(0))
{
    PRINT_DBG("Metafile header default constructor");
}

MetafileHeader::MetafileHeader(QIODevice &device, qint64 off)
{
    PRINT_DBG("Reading Metafile header at: %lld", off);
    seekFile(device, off);
    this->mappingMode = readCompressedUnsignedWord(device);
    PRINT_DBG("        Mapping mode: %d", this->mappingMode);
    this->width = readUnsignedWord(device);
    PRINT_DBG("        Width: %d", this->width);
    this->height = readUnsignedWord(device);
    PRINT_DBG("        Height: %d", this->height);
    this->decompressedSize = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Decompressed size: %d", this->decompressedSize);
    this->compressedSize = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Compressed size: %d", this->compressedSize);
    this->hotspotSize = readCompressedUnsignedDWord(device);
    PRINT_DBG("        Hotspot size: %d", this->hotspotSize);
    this->compressedOffset = readUnsignedDWord(device);
    PRINT_DBG("        Compressed offset: %d", this->compressedOffset);
    this->hotspotOffset = readUnsignedDWord(device);
    PRINT_DBG("        Hotspot offset: %d", this->hotspotOffset);
    this->sz = device.pos() - off;
    PRINT_DBG("Metafile header loaded successfully");
}

MetafileHeader::MetafileHeader(const MetafileHeader &rhs) : mappingMode(rhs.
    mappingMode), width(rhs.width), height(rhs.height),
    decompressedSize(rhs.decompressedSize), compressedSize(rhs.compressedSize),
    hotspotSize(rhs.hotspotSize), compressedOffset(rhs.compressedOffset),
    hotspotOffset(rhs.hotspotOffset), sz(rhs.sz)
{
    PRINT_DBG("Metafile header copy constructor");
}

MetafileHeader::~MetafileHeader()
{
    PRINT_DBG("Metafile header destructor");
}

MetafileHeader & MetafileHeader::operator =(const MetafileHeader &rhs)
{
    PRINT_DBG("Metafile header assignment operator");
    if(this != &rhs)
    {
        this->mappingMode = rhs.mappingMode;
        this->width = rhs.width;
        this->height = rhs.height;
        this->decompressedSize = rhs.decompressedSize;
        this->compressedSize = rhs.compressedSize;
        this->hotspotSize = rhs.hotspotSize;
        this->compressedOffset = rhs.compressedOffset;
        this->hotspotOffset = rhs.hotspotOffset;
        this->sz = rhs.sz;
    }
    return *this;
}

quint16 MetafileHeader::getMappingMode() const
{
    return this->mappingMode;
}

quint16 MetafileHeader::getWidth() const
{
    return this->width;
}

quint16 MetafileHeader::getHeight() const
{
    return this->height;
}

quint32 MetafileHeader::getDecompressedSize() const
{
    return this->decompressedSize;
}

quint32 MetafileHeader::getCompressedSize() const
{
    return this->compressedSize;
}

quint32 MetafileHeader::getHotspotSize() const
{
    return this->hotspotSize;
}

quint32 MetafileHeader::getCompressedOffset() const
{
    return this->compressedOffset;
}

quint32 MetafileHeader::getHotspotOffset() const
{
    return this->hotspotOffset;
}

qint64 MetafileHeader::size() const
{
    return this->sz;
}
