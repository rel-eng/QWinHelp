/* Metafile header definition.

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

#ifndef METAFILEHEADER_H
#define METAFILEHEADER_H

#include <QtGlobal>
#include <QIODevice>

#include <cstddef>

class MetafileHeader
{
private:
    quint16 mappingMode;
    quint16 width;
    quint16 height;
    quint32 decompressedSize;
    quint32 compressedSize;
    quint32 hotspotSize;
    quint32 compressedOffset;
    quint32 hotspotOffset;
    qint64 sz;
public:
    MetafileHeader();
    MetafileHeader(QIODevice &device, qint64 off);
    MetafileHeader(const MetafileHeader &rhs);
    virtual ~MetafileHeader();
    MetafileHeader &operator =(const MetafileHeader &rhs);
    quint16 getMappingMode() const;
    quint16 getWidth() const;
    quint16 getHeight() const;
    quint32 getDecompressedSize() const;
    quint32 getCompressedSize() const;
    quint32 getHotspotSize() const;
    quint32 getCompressedOffset() const;
    quint32 getHotspotOffset() const;
    qint64 size() const;
};

#endif // METAFILEHEADER_H
