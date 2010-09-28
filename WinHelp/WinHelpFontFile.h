/* WinHelp Font file definition.

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

#ifndef WINHELPFONTFILE_H
#define WINHELPFONTFILE_H

#include <QtGlobal>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QList>
#include <QSharedPointer>

#include "FontDescriptor.h"
#include "OldFontDescriptor.h"
#include "NewFontDescriptor.h"
#include "MVBFontDescriptor.h"
#include "FontStyle.h"

class WinHelpFontFile
{
    quint16 numFaceNames;
    quint16 numDescriptors;
    quint16 faceNamesOffset;
    quint16 descriptorsOffset;
    quint16 numStyles;
    quint16 stylesOffset;
    quint16 numCharMapTables;
    quint16 charMapTablesOffset;
    bool stylesValid;
    bool charMapTablesValid;
    QList<QString> faceNames;
    QList<QSharedPointer<FontDescriptor> > fontDescriptors;
    QList<FontStyle> fontStyles;
    QList<QString> charMapTablesNames;
    QString styleDescription;
public:
    WinHelpFontFile();
    WinHelpFontFile(QFile &file, qint64 off, QTextCodec *codec);
    WinHelpFontFile(const WinHelpFontFile &rhs);
    virtual ~WinHelpFontFile();
    const WinHelpFontFile & operator=(const WinHelpFontFile &rhs);
    QString getStyle() const;
};

#endif // WINHELPFONTFILE_H
