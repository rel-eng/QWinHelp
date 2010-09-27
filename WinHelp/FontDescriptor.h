/* Font descriptor definition.

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

#ifndef FONTDESCRIPTOR_H
#define FONTDESCRIPTOR_H

#include <QtGlobal>

enum FontDescriptorType : quint8
{
    OLD_FONT_DESCRIPTOR = 0,
    NEW_FONT_DESCRIPTOR = 1,
    MVB_FONT_DESCRIPTOR = 2
};

class FontDescriptor
{
private:
    quint8 descriptorType;
protected:
    FontDescriptor();
    FontDescriptor(quint8 descriptorType);
    FontDescriptor(const FontDescriptor &rhs);
    FontDescriptor & operator=(const FontDescriptor &rhs);
public:
    virtual ~FontDescriptor();
    virtual quint8 getType() const;
};

#endif // FONTDESCRIPTOR_H
