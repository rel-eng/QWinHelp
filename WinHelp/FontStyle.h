/* Font Style definition.

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

#ifndef FONTSTYLE_H
#define FONTSTYLE_H

#include <QtGlobal>
#include <QFile>
#include <QString>
#include <QSharedPointer>
#include <QTextCodec>

#include "FontDescriptor.h"
#include "OldFontDescriptor.h"
#include "NewFontDescriptor.h"
#include "MVBFontDescriptor.h"

class FontStyle
{
private:
    quint16 styleNum;
    quint16 basedOnStyleNum;
    QSharedPointer<FontDescriptor> font;
    QString styleName;
public:
    FontStyle();
    FontStyle(QFile &file,
        qint64 off,
        FontDescriptorType type,
        QTextCodec *codec);
    FontStyle(const FontStyle& rhs);
    virtual ~FontStyle();
    FontStyle & operator=(const FontStyle & rhs);
    quint16 getStyleNum() const;
    quint16 getBasedOnStyleNum() const;
    QSharedPointer<FontDescriptor> getFont() const;
    QString getStyleName() const;
};

#endif // FONTSTYLE_H
