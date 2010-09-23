/* Displayable text class implementation.

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

#include "DisplayableText.h"

DisplayableText::DisplayableText() : TopicLink(), type(0)
{
}

DisplayableText::DisplayableText(quint8 type) : TopicLink(), type(type)
{
}

DisplayableText::DisplayableText(const DisplayableText &rhs) : TopicLink(rhs),
    type(rhs.type)
{
}

DisplayableText::~DisplayableText()
{
}

DisplayableText & DisplayableText::operator=(const DisplayableText &rhs)
{
    if (this != &rhs)
    {
        TopicLink::operator =(rhs);
        this->type = rhs.type;
    }
    return *this;
}

quint8 DisplayableText::getType() const
{
    return this->type;
}
