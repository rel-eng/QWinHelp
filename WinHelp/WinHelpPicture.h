/* WinHelp picture definition.

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

#ifndef WINHELPPICTURE_H
#define WINHELPPICTURE_H

#include <QtGlobal>
#include <QImage>
#include <QList>
#include <QColor>
#include <QIODevice>
#include <QSharedPointer>

#include <cstddef>

#include "BitmapHeader.h"

class WinHelpPicture
{
private:
    QList<QImage> pictures;
    QImage LoadDDB(QIODevice &device,
        qint64 offset,
        size_t size,
        const BitmapHeader &hdr);
    QImage LoadDIB(QIODevice &device,
        qint64 offset,
        size_t size,
        const BitmapHeader &hdr,
        const QScopedArrayPointer<QRgb> &palette);
public:
    WinHelpPicture();
    WinHelpPicture(QIODevice &device, qint64 off);
    WinHelpPicture(const WinHelpPicture &rhs);
    virtual ~WinHelpPicture();
    WinHelpPicture &operator=(const WinHelpPicture &rhs);
    QImage getBestPicture() const;
};

#endif // WINHELPPICTURE_H
