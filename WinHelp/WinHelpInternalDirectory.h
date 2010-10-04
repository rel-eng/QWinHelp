/* WinHelp internal directory class definition.

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

#ifndef WINHELPINTERNALDIRECTORY_H_
#define WINHELPINTERNALDIRECTORY_H_

#include <QtGlobal>
#include <QFile>
#include <QHash>
#include <QString>
#include <QPair>
#include <QRegExp>
#include <QList>

class WinHelpInternalDirectory
{
private:
    QHash<QString, qint64> directory;
    qint64 getPageOffset(qint64 pagesOffset, qint64 pageSize, qint64 pageNumber);
public:
    WinHelpInternalDirectory();
    WinHelpInternalDirectory(QFile &file, qint64 off);
    WinHelpInternalDirectory(const WinHelpInternalDirectory& rhs);
    virtual ~WinHelpInternalDirectory();
    WinHelpInternalDirectory & operator=(const WinHelpInternalDirectory & rhs);
    bool isFileExists(QString filename) const;
    qint64 getFileOffset(QString filename) const;
    void addFile(QString filename, qint64 offset);
    QList<QPair<QString, qint64> > getFilesByRegExp(QRegExp regexp) const;
};

#endif /* WINHELPINTERNALDIRECTORY_H_ */
