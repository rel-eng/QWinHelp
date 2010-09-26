/* MDI child window class definition.

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

#ifndef WINHELPWINDOW_H
#define WINHELPWINDOW_H

#include <QtGui/QWidget>
#include <QScopedPointer>
#include <QCloseEvent>
#include <QStringList>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QSignalMapper>
#include "ui_winhelpwindow.h"

#include "WinHelp/WinHelpFile.h"
#include "ThreadedWinHelpFileLoader.h"
#include "HelpIndexModel.h"

class WinHelpWindow : public QWidget
{
    Q_OBJECT

public:
    WinHelpWindow(QWidget *parent = 0);
    ~WinHelpWindow();

    bool loadFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void fileLoaded(bool isSuccessfull,
        const QString &message,
        const QString &file);
    void indexSearchLineTextChanged(QString text);
    void indexSearchListDoubleClicked(const QModelIndex & index);
    void pagesTabsTabCloseRequested(int index);
    void tabURLChanged(QWidget *widget);

private:
    Q_DISABLE_COPY(WinHelpWindow)
    Ui::WinHelpWindowClass ui;

    QString curFile;
    ThreadedWinHelpFileLoader winHelpFileLoader;
    HelpIndexModel topics;
    QSortFilterProxyModel topicsProxy;
    QSignalMapper *tabMapper;

    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void fillTopicList();
};

#endif // WINHELPWINDOW_H
