/* Picture header implementation.

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

#include "PictureHeader.h"

#include "Utils/DebugUtils.h"
#include "Utils/IOUtils.h"

#include <stdexcept>

PictureHeader::PictureHeader() : magic(0), pictureOffsets(), sz(Q_INT64_C(0))
{
    PRINT_DBG("Picture header default constructor");
}

PictureHeader::PictureHeader(QIODevice& device,
    qint64 off) : magic(0), pictureOffsets(), sz(Q_INT64_C(0))
{
    PRINT_DBG("Reading Picture header at: %lld", off);
    seekFile(device, off);
    this->magic = readUnsignedWord(device);
    PRINT_DBG("        Magic: %d", this->magic);
    quint16 numberOfPictures = readUnsignedWord(device);
    PRINT_DBG("        Number of pictures: %d", numberOfPictures);
    sz = Q_INT64_C(4);
    for(quint16 i = 0; i < numberOfPictures; i++)
    {
        this->pictureOffsets.append(readUnsignedDWord(device));
        PRINT_DBG("        Picture offset: %d", pictureOffsets.last());
        sz += Q_INT64_C(4);
    }
    PRINT_DBG("Picture header loaded successfully");
}

PictureHeader::PictureHeader(const PictureHeader &rhs) : magic(rhs.magic),
    pictureOffsets(rhs.pictureOffsets), sz(rhs.sz)
{
    PRINT_DBG("Picture header copy constructor");
}

PictureHeader::~PictureHeader()
{
    PRINT_DBG("Picture header destructor");
}

PictureHeader & PictureHeader::operator=(const PictureHeader &rhs)
{
    PRINT_DBG("Picture header assignment operator");
    if(this != &rhs)
    {
        this->magic = rhs.magic;
        this->pictureOffsets = rhs.pictureOffsets;
        this->sz = rhs.sz;
    }
    return *this;
}

quint16 PictureHeader::getMagic() const
{
    return this->magic;
}

int PictureHeader::getNumberOfPictures() const
{
    return this->pictureOffsets.count();
}

quint32 PictureHeader::getPictureOffset(int index)
{
    if ((index < 0) || (index >= this->pictureOffsets.size()))
    {
        throw new std::out_of_range("Index of picture is out of range");
    }
    return this->pictureOffsets.at(index);
}

qint64 PictureHeader::size() const
{
    return this->sz;
}
