/* Help reply class definition.

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

#ifndef HELPREPLY_H_
#define HELPREPLY_H_

#include <QObject>
#include <QNetworkReply>
#include <QStringList>
#include <QUrl>
#include <QByteArray>

#include "ThreadedWinHelpFileLoader.h"

class HelpReply : public QNetworkReply
{
    Q_OBJECT
public:
    HelpReply(ThreadedWinHelpFileLoader &winHelpFileLoader,
        const QUrl &url,
        bool giveError);
    void abort();
    qint64 bytesAvailable() const;
    bool isSequential() const;
    virtual ~HelpReply();
protected:
    qint64 readData(char *data, qint64 maxSize);
private slots:
    void signalCompleted();
private:
    QByteArray content;
    qint64 offset;
};

#endif /* HELPREPLY_H_ */
