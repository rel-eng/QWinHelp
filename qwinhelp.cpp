/* Main window class implementation.

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

#include "qwinhelp.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

QWinHelp::QWinHelp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    mdiArea = new QMdiArea();
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this,
        SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)), this,
        SLOT(setActiveSubWindow(QWidget*)));
    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAct, SIGNAL(triggered()), mdiArea,
        SLOT(activatePreviousSubWindow()));
    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    updateWindowMenu();
    connect(ui.menuWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    updateMenus();
    ui.actionOpen->setShortcuts(QKeySequence::Open);
    ui.actionExit->setShortcuts(QKeySequence::Quit);
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

QWinHelp::~QWinHelp()
{

}

void QWinHelp::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if(mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

QMdiSubWindow *QWinHelp::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList())
    {
        WinHelpWindow *mdiChild = qobject_cast<WinHelpWindow *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
        {
            return window;
        }
    }
    return NULL;
}

QMdiSubWindow *QWinHelp::activeMdiChild()
{
    return mdiArea->activeSubWindow();
}

void QWinHelp::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr(
            "Open File"), "", tr("Help files (*.hlp *.mvb)"));
    if (!fileName.isEmpty())
    {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing)
        {
            mdiArea->setActiveSubWindow(existing);
            return;
        }
        QMdiSubWindow *subWindow = createMdiChild();
        WinHelpWindow *child = qobject_cast<WinHelpWindow *>(subWindow->widget());
        if (child->loadFile(fileName))
        {
            statusBar()->showMessage(tr("File loaded"), 2000);
            subWindow->show();
        }
        else
        {
            subWindow->close();
        }
    }
}

void QWinHelp::about()
{
    QMessageBox::about(this, tr("About QWinHelp"),
        tr(
            "QWinHelp 0.1.0\nCopyright (C) 2010-2013 rel-eng\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\nYou should have received a copy of the GNU General Public License along with this program.  If not, see http://www.gnu.org/licenses/."));
}

QMdiSubWindow *QWinHelp::createMdiChild()
{
    WinHelpWindow *child = new WinHelpWindow();
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    subWindow->setWidget(child);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    return subWindow;
}

void QWinHelp::setActiveSubWindow(QWidget *window)
{
    if (!window)
    {
        return;
    }
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void QWinHelp::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != NULL);
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);
}

void QWinHelp::updateWindowMenu()
{
    ui.menuWindow->clear();
    ui.menuWindow->addAction(closeAct);
    ui.menuWindow->addAction(closeAllAct);
    ui.menuWindow->addSeparator();
    ui.menuWindow->addAction(tileAct);
    ui.menuWindow->addAction(cascadeAct);
    ui.menuWindow->addSeparator();
    ui.menuWindow->addAction(nextAct);
    ui.menuWindow->addAction(previousAct);
    ui.menuWindow->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
        WinHelpWindow *child = qobject_cast<WinHelpWindow *>(windows.at(
                i)->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        else
        {
            text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = ui.menuWindow->addAction(text);
        action->setCheckable(true);
        action->setChecked(windows.at(i) == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}
