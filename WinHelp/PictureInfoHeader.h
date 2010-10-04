/* Picture info header definition.

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

#ifndef PICTUREINFOHEADER_H
#define PICTUREINFOHEADER_H

#include <QtGlobal>
#include <QIODevice>

#include <cstddef>

enum PictureType : quint8
{
    DDB_PICTURE = 5,
    DIB_PICTURE = 6,
    METAFILE_PICTURE = 8
};

enum PicturePackingMethod : quint8
{
    UNCOMPRESSED_PICTURE = 0,
    RUNLEN_COMPRESSED_PICTURE = 1,
    LZ77_COMPRESSED_PICTURE = 2,
    LZ77_AND_RUNLEN_COMPRESSED_PICTURE = 3
};

class PictureInfoHeader
{
private:
    quint8 pictureType;
    quint8 packingMethod;
public:
    static const qint64 size = 2;
    PictureInfoHeader();
    PictureInfoHeader(QIODevice &device, qint64 off);
    PictureInfoHeader(const PictureInfoHeader &rhs);
    virtual ~PictureInfoHeader();
    PictureInfoHeader &operator=(const PictureInfoHeader &rhs);
    quint8 getPictureType() const;
    quint8 getPackingMethod() const;
};

#endif // PICTUREINFOHEADER_H
