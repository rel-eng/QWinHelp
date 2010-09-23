/* Locale identifier record class definition.

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

#ifndef LCIDRECORD_H_
#define LCIDRECORD_H_

#include "LCID.h"

#include <QtGlobal>
#include <QFile>

class LCIDRecord
{
private:
    LCID lcid;
    /** При сортировке Keywords игнорировать Non Spacing Marks в текстах без учета регистра символов, бит с маской 0x02 в соответствующем байте флагов. */
    bool ignoreNonSpacingMarksCaseInsensitive;
    /** При сортировке Keywords игнорировать Symbols (знаки препинания и т.п.) в текстах без учета регистра символов, бит с маской 0x04 в соответствующем байте флагов. */
    bool ignoreSymbolsCaseInsensitive;
    /** При сортировке Keywords игнорировать Non Spacing Marks в текстах с учетом регистра символов, бит с маской 0x02 в соответствующем байте флагов. */
    bool ignoreNonSpacingMarksCaseSensitive;
    /** При сортировке Keywords игнорировать Symbols (знаки препинания и т.п.) в текстах с учетом регистра символов, бит с маской 0x04 в соответствующем байте флагов. */
    bool ignoreSymbolsCaseSensitive;
public:
    LCIDRecord();
    LCIDRecord(QFile &file, qint64 off);
    LCIDRecord(const LCIDRecord& rhs);
    virtual ~LCIDRecord();
    LCIDRecord & operator=(const LCIDRecord & rhs);
    LCID getLCID() const;
    bool isIgnoreNonSpacingMarksCaseInsensitive() const;
    bool isIgnoreSymbolsCaseInsensitive() const;
    bool isIgnoreNonSpacingMarksCaseSensitive() const;
    bool isIgnoreSymbolsCaseSensitive() const;
};

#endif /* LCIDRECORD_H_ */
