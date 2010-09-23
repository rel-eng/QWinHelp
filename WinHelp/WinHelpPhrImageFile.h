/* WinHelp PhrImage file class definition.

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

#ifndef WINHELPPHRIMAGEFILE_H_
#define WINHELPPHRIMAGEFILE_H_

#include <QtGlobal>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QList>
#include <QByteArray>

#include "WinHelpPhrIndexFile.h"

class WinHelpPhrImageFile
{
private:
    QList<QString> phrases;
    QList<QByteArray> phrasesRaw;
public:
    WinHelpPhrImageFile();
    WinHelpPhrImageFile(QFile &file,
        qint64 off,
        QTextCodec *codec,
        const WinHelpPhrIndexFile &index);
    WinHelpPhrImageFile(const WinHelpPhrImageFile& rhs);
    virtual ~WinHelpPhrImageFile();
    WinHelpPhrImageFile & operator=(const WinHelpPhrImageFile & rhs);
    int getPhrasesCount() const;
    QString getPhrase(int index) const;
    QByteArray getRawPhrase(int index) const;
};

#endif /* WINHELPPHRIMAGEFILE_H_ */
