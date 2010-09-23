/* WinHelp Phrase file class definition.

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

#ifndef WINHELPPHRASEFILE_H_
#define WINHELPPHRASEFILE_H_

#include <QtGlobal>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QList>
#include <QByteArray>

class WinHelpPhraseFile
{
private:
    QList<QString> phrases;
    QList<QByteArray> phrasesRaw;
public:
    WinHelpPhraseFile();
    WinHelpPhraseFile(QFile &file,
        qint64 off,
        QTextCodec *codec,
        bool compressed,
        bool mvbHint);
    WinHelpPhraseFile(const WinHelpPhraseFile& rhs);
    virtual ~WinHelpPhraseFile();
    WinHelpPhraseFile & operator=(const WinHelpPhraseFile & rhs);
    int getPhrasesCount() const;
    QString getPhrase(int index) const;
    QByteArray getRawPhrase(int index) const;
};

#endif /* WINHELPPHRASEFILE_H_ */
