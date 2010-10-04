/* WinHelp picture file definition.

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

#ifndef WINHELPPICTUREFILE_H
#define WINHELPPICTUREFILE_H

#include <QtGlobal>
#include <QFile>

#include "WinHelpPicture.h"

class WinHelpPictureFile
{
private:
    WinHelpPicture picture;
public:
    WinHelpPictureFile();
    WinHelpPictureFile(QFile &file, qint64 off);
    WinHelpPictureFile(WinHelpPictureFile &rhs);
    virtual ~WinHelpPictureFile();
    WinHelpPictureFile &operator=(const WinHelpPictureFile &rhs);
    const WinHelpPicture &getPicture() const;
};

#endif // WINHELPPICTUREFILE_H
