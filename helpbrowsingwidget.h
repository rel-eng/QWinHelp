/* Help browsing widget class definition.

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

#ifndef HELPBROWSINGWIDGET_H
#define HELPBROWSINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QUrl>

#include "HelpView.h"
#include "ThreadedWinHelpFileLoader.h"

class HelpBrowsingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpBrowsingWidget(ThreadedWinHelpFileLoader &winHelpFileLoader,
        QWidget *parent = 0);
    void goToURL(const QUrl &url);
    QUrl getURL() const;

signals:
    void urlChanged(const QUrl &url);
public slots:

private slots:
    void onURLChanged(const QUrl &url);
    void onBackButtonClicked();
    void onForwardButtonClicked();

private:
    HelpView *browser;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *prevButton;
    QPushButton *nextButton;
};

#endif // HELPBROWSINGWIDGET_H
