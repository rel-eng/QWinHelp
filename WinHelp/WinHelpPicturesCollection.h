/* WinHelp pictures collection definition.

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

#ifndef WINHELPPICTURESCOLLECTION_H
#define WINHELPPICTURESCOLLECTION_H

#include <QtGlobal>
#include <QFile>
#include <QString>
#include <QHash>
#include <QImage>

#include "WinHelpInternalDirectory.h"
#include "WinHelpPicture.h"

class WinHelpPicturesCollection
{
private:
    QHash<uint, WinHelpPicture> pictures;
public:
    WinHelpPicturesCollection();
    WinHelpPicturesCollection(QFile &file,
        const WinHelpInternalDirectory &directory);
    WinHelpPicturesCollection(const WinHelpPicturesCollection &rhs);
    virtual ~WinHelpPicturesCollection();
    WinHelpPicturesCollection &operator=(const WinHelpPicturesCollection &rhs);
    QImage getPicture(int index) const;
};

#endif // WINHELPPICTURESCOLLECTION_H
