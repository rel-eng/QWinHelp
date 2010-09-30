/* WinHelp Context file definition.

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

#ifndef WINHELPCONTEXTFILE_H
#define WINHELPCONTEXTFILE_H

#include <QtGlobal>
#include <QFile>
#include <QHash>

#include "TopicOffset.h"

class WinHelpContextFile
{
private:
    QHash<quint32, TopicOffset> hashTable;
    qint64 getPageOffset(qint64 pagesOffset, qint64 pageSize, qint64 pageNumber);
public:
    WinHelpContextFile();
    WinHelpContextFile(QFile &file, qint64 off);
    WinHelpContextFile(const WinHelpContextFile &rhs);
    virtual ~WinHelpContextFile();
    WinHelpContextFile &operator=(const WinHelpContextFile &rhs);
    bool isHashExists(quint32 hash) const;
    TopicOffset getOffset(quint32 hash) const;
    void addHash(quint32 hash, const TopicOffset &offset);
};

#endif // WINHELPCONTEXTFILE_H
