/* WinHelp icon class definition.

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

#ifndef WINHELPICON_H_
#define WINHELPICON_H_

#include <QtGlobal>
#include <QImage>
#include <QFile>
#include <QList>

class WinHelpIcon
{
private:
    QList<QImage> images;
public:
    WinHelpIcon();
    WinHelpIcon(const QList<QImage> &images);
    WinHelpIcon(QFile &file, qint64 off);
    WinHelpIcon(const WinHelpIcon& rhs);
    virtual ~WinHelpIcon();
    WinHelpIcon & operator=(const WinHelpIcon & rhs);
    const QImage &getImage(int index) const;
    int getImageCount() const;
};

#endif /* WINHELPICON_H_ */
