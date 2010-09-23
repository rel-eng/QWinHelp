/* Main window class definition.

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

#ifndef QWINHELP_H
#define QWINHELP_H

#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QAction>
#include <QSignalMapper>
#include <QString>
#include "ui_qwinhelp.h"

#include "WinHelp/WinHelpFile.h"

#include "winhelpwindow.h"

class QWinHelp : public QMainWindow
{
    Q_OBJECT

public:
    QWinHelp(QWidget *parent = 0);
    ~QWinHelp();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::QWinHelpClass ui;
    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;

    QMdiSubWindow *findMdiChild(const QString &fileName);
    QMdiSubWindow *activeMdiChild();

private slots:
    void openFile();
    void about();
    QMdiSubWindow *createMdiChild();
    void setActiveSubWindow(QWidget *window);
    void updateMenus();
    void updateWindowMenu();
};

#endif // QWINHELP_H
