/* Displayable text class definition.

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

#ifndef DISPLAYABLETEXT_H_
#define DISPLAYABLETEXT_H_

#include <QtGlobal>

#include "TopicLink.h"

class DisplayableText : public TopicLink
{
private:
    quint8 type;
protected:
    DisplayableText();
    DisplayableText(quint8 type);
    DisplayableText(const DisplayableText &rhs);
    DisplayableText &operator=(const DisplayableText &rhs);
public:
    virtual ~DisplayableText();
    quint8 getType() const;
    virtual QString getHTML(bool &empty) const = 0;
};

#endif /* DISPLAYABLETEXT_H_ */
