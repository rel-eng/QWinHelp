/* Picture info header implementation.

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

#include "PictureInfoHeader.h"

#include "Utils/DebugUtils.h"
#include "Utils/IOUtils.h"

#include <stdexcept>

PictureInfoHeader::PictureInfoHeader() : pictureType(0), packingMethod(0)
{
    PRINT_DBG("Picture info header default constructor");
}

PictureInfoHeader::PictureInfoHeader(QIODevice &device, qint64 off)
{
    PRINT_DBG("Reading Picture info header at: %lld", off);
    seekFile(device, off);
    this->pictureType = readUnsignedByte(device);
    PRINT_DBG("        Picture type: %d", this->pictureType);
    this->packingMethod = readUnsignedByte(device);
    PRINT_DBG("        Packing method: %d", this->packingMethod);
    PRINT_DBG("Picture info header loaded successfully");
}

PictureInfoHeader::PictureInfoHeader(const PictureInfoHeader &rhs) :
    pictureType(rhs.pictureType), packingMethod(rhs.packingMethod)
{
    PRINT_DBG("Picture info header copy constructor");
}

PictureInfoHeader::~PictureInfoHeader()
{
    PRINT_DBG("Picture info header destructor");
}

PictureInfoHeader & PictureInfoHeader::operator=(const PictureInfoHeader &rhs)
{
    PRINT_DBG("Picture info header assignment operator");
    if(this != &rhs)
    {
        this->pictureType = rhs.pictureType;
        this->packingMethod = rhs.packingMethod;
    }
    return *this;
}

quint8 PictureInfoHeader::getPictureType() const
{
    return this->pictureType;
}

quint8 PictureInfoHeader::getPackingMethod() const
{
    return this->packingMethod;
}
