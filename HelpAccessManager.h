/* Help access manager class definition.

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

#ifndef HELPACCESSMANAGER_H_
#define HELPACCESSMANAGER_H_

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QIODevice>

#include "ThreadedWinHelpFileLoader.h"

class HelpAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    HelpAccessManager(ThreadedWinHelpFileLoader &winHelpFileLoader,
        QObject *parent = 0);
    virtual ~HelpAccessManager();
protected:
    QNetworkReply *createRequest(Operation operation,
        const QNetworkRequest &request,
        QIODevice *device);
private:
    ThreadedWinHelpFileLoader &winHelpFileLoader;
};

#endif /* HELPACCESSMANAGER_H_ */
