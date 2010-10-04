/* Bitmap header definition.

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

#ifndef BITMAPHEADER_H
#define BITMAPHEADER_H

#include <QtGlobal>
#include <QIODevice>

#include <cstddef>

class BitmapHeader
{
private:
    quint32 xdpi;
    quint32 ydpi;
    quint16 planes;
    quint16 bitCount;
    quint32 width;
    quint32 height;
    quint32 colorsUsed;
    quint32 colorsImportant;
    quint32 compressedSize;
    quint32 hotspotSize;
    quint32 compressedOffset;
    quint32 hotspotOffset;
    qint64 sz;
public:
    BitmapHeader();
    BitmapHeader(QIODevice &device, qint64 off);
    BitmapHeader(const BitmapHeader &rhs);
    virtual ~BitmapHeader();
    BitmapHeader &operator=(const BitmapHeader &rhs);
    quint32 getXdpi() const;
    quint32 getYdpi() const;
    quint16 getPlanes() const;
    quint16 getBitCount() const;
    quint32 getWidth() const;
    quint32 getHeight() const;
    quint32 getColorsUsed() const;
    quint32 getColorsImportant() const;
    quint32 getCompressedSize() const;
    quint32 getHotspotSize() const;
    quint32 getCompressedOffset() const;
    quint32 getHotspotOffset() const;
    qint64 size() const;
};

#endif // BITMAPHEADER_H
