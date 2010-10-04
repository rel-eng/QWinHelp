/* WinHelp pictures collection implementation.

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

#include "WinHelpPicturesCollection.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "WinHelpPictureFile.h"

#include <QtDebug>
#include <QImage>
#include <QPainter>

#include <stdexcept>

WinHelpPicturesCollection::WinHelpPicturesCollection() : pictures()
{
}

WinHelpPicturesCollection::WinHelpPicturesCollection(QFile &file,
    const WinHelpInternalDirectory &directory) : pictures()
{
    QList<QPair<QString,
            qint64> > pairs =
        directory.getFilesByRegExp(QRegExp("^\\|?bm\\d+$", Qt::CaseInsensitive));
    for(int i = 0; i < pairs.count(); i++)
    {
        QRegExp rx("(\\d+$)");
        int pos = rx.indexIn(pairs.at(i).first);
        if(pos > -1)
        {
            uint picIndex = rx.cap().toUInt();
            if(!pictures.contains(picIndex))
            {
                WinHelpPictureFile pictFile(file, pairs.at(i).second);
                this->pictures.insert(picIndex, pictFile.getPicture());
            }
            else
            {
                qDebug() << "Duplicated picture names";
            }
        }
    }
}

WinHelpPicturesCollection::WinHelpPicturesCollection(
    const WinHelpPicturesCollection &rhs) : pictures(rhs.pictures)
{
}

WinHelpPicturesCollection::~WinHelpPicturesCollection()
{
}

WinHelpPicturesCollection & WinHelpPicturesCollection::operator=(
    const WinHelpPicturesCollection &rhs)
{
    if(this != &rhs)
    {
        this->pictures = rhs.pictures;
    }
    return *this;
}

QImage WinHelpPicturesCollection::getPicture(int index) const
{
    if(!this->pictures.contains(index))
    {
        qDebug() << "No image";
        QImage noImage(320, 240, QImage::Format_ARGB32);
        QPainter painter;
        painter.begin(&noImage);
        painter.fillRect(0, 0, 320, 240, Qt::white);
        painter.drawText(10, 10, "No image");
        painter.end();
        return noImage;
    }
    return this->pictures.value(index).getBestPicture();
}
