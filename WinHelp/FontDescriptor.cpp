/* Font descriptor implementation.

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

#include "FontDescriptor.h"

FontDescriptor::FontDescriptor() : descriptorType(0)
{
}

FontDescriptor::FontDescriptor(quint8 descriptorType) : descriptorType(
    descriptorType)
{
}

FontDescriptor::FontDescriptor(const FontDescriptor &rhs) : descriptorType(rhs.
    descriptorType)
{
}

FontDescriptor::~FontDescriptor()
{
}

FontDescriptor & FontDescriptor::operator=(const FontDescriptor &rhs)
{
    if (this != &rhs)
    {
        this->descriptorType = rhs.descriptorType;
    }
    return *this;
}

quint8 FontDescriptor::getType() const
{
    return this->descriptorType;
}
