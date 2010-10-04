/* Picture header definition.

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

#ifndef PICTUREHEADER_H
#define PICTUREHEADER_H

#include <QtGlobal>
#include <QIODevice>
#include <QList>

#include <cstddef>

enum PictureMagic : quint16
{
    SHG_PICTURE = 0x506C,
    MRB_PICTURE = 0x706C
};

class PictureHeader
{
private:
    quint16 magic;
    QList<quint32> pictureOffsets;
    qint64 sz;
public:
    PictureHeader();
    PictureHeader(QIODevice& device, qint64 off);
    PictureHeader(const PictureHeader &rhs);
    virtual ~PictureHeader();
    PictureHeader &operator=(const PictureHeader &rhs);
    quint16 getMagic() const;
    int getNumberOfPictures() const;
    quint32 getPictureOffset(int index);
    qint64 size() const;
};

#endif // PICTUREHEADER_H
