/* Old font descriptor definition.

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

#ifndef OLDFONTDESCRIPTOR_H
#define OLDFONTDESCRIPTOR_H

#include <QtGlobal>
#include <QFile>
#include <QColor>

#include "FontDescriptor.h"

enum OldFontFamily : quint8
{
    OLD_FAM_MODERN = 0x01,
    OLD_FAM_ROMAN = 0x02,
    OLD_FAM_SWISS_OR_TECH_OR_NIL = 0x03,
    OLD_FAM_SCRIPT = 0x04,
    OLD_FAM_DECOR = 0x05
};

class OldFontDescriptor : public FontDescriptor
{
private:
    bool bold;
    bool italic;
    bool underline;
    bool strikeOut;
    bool doubleUnderline;
    bool smallCaps;
    quint8 halfPoints;
    quint8 fontFamily;
    quint16 facenameIndex;
    QRgb fgColor;
    QRgb bgColor;
public:
    OldFontDescriptor();
    OldFontDescriptor(QFile &file, qint64 off);
    OldFontDescriptor(const OldFontDescriptor& rhs);
    virtual ~OldFontDescriptor();
    OldFontDescriptor & operator=(const OldFontDescriptor & rhs);
    bool isBold() const;
    bool isItalic() const;
    bool isUnderline() const;
    bool isStrikeOut() const;
    bool isDoubleUnderline() const;
    bool isSmallCaps() const;
    quint8 getHalfPoints() const;
    quint8 getFontFamily() const;
    quint16 getFacenameIndex() const;
    QRgb getFgColor() const;
    QRgb getBgColor() const;
};

#endif // OLDFONTDESCRIPTOR_H
