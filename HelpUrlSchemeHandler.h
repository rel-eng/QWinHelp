/* Help URL scheme handler class definition.

   Copyright (C) 2018 rel-eng

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

#ifndef HELPURLSCHEMEHANDLER_H_
#define HELPURLSCHEMEHANDLER_H_

#include <QObject>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

#include "ThreadedWinHelpFileLoader.h"

class HelpUrlSchemeHandler : public QWebEngineUrlSchemeHandler {
    Q_OBJECT
public:
    HelpUrlSchemeHandler(ThreadedWinHelpFileLoader &winHelpFileLoader, QObject *parent = 0);
    virtual ~HelpUrlSchemeHandler();

    void requestStarted(QWebEngineUrlRequestJob *job) Q_DECL_OVERRIDE;
private:
    ThreadedWinHelpFileLoader &winHelpFileLoader;
};

#endif /* HELPURLSCHEMEHANDLER_H_ */
