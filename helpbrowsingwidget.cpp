/* Help browsing widget class implementation.

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

#include "helpbrowsingwidget.h"

#include <QWebEngineHistory>

HelpBrowsingWidget::HelpBrowsingWidget(QWebEngineProfile* profile,
    QWidget *parent) :
    QWidget(parent)
{
    this->verticalLayout = new QVBoxLayout();
    this->verticalLayout->setSpacing(0);
    this->verticalLayout->setContentsMargins(0, 0, 0, 0);
    this->horizontalLayout = new QHBoxLayout();
    this->horizontalLayout->setSpacing(0);
    this->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    this->prevButton = new QPushButton(tr("Go back"));
    this->nextButton = new QPushButton(tr("Go forward"));
    this->horizontalLayout->addWidget(this->prevButton);
    this->horizontalLayout->addWidget(this->nextButton);
    this->verticalLayout->addLayout(this->horizontalLayout);
    this->browser = new HelpView(profile);
    this->verticalLayout->addWidget(this->browser);
    this->setLayout(this->verticalLayout);
    connect(this->browser, SIGNAL(urlChanged(const QUrl &)), this,
        SLOT(onURLChanged(const QUrl &)));
    connect(this->prevButton, SIGNAL(clicked()), this, SLOT(onBackButtonClicked()));
    connect(this->nextButton, SIGNAL(clicked()), this,
        SLOT(onForwardButtonClicked()));
}

void HelpBrowsingWidget::onURLChanged(const QUrl &url)
{
    emit urlChanged(url);
}

void HelpBrowsingWidget::onBackButtonClicked()
{
    if(this->browser->history()->canGoBack())
    {
        this->browser->history()->back();
    }
}

void HelpBrowsingWidget::onForwardButtonClicked()
{
    if(this->browser->history()->canGoForward())
    {
        this->browser->history()->forward();
    }
}

void HelpBrowsingWidget::goToURL(const QUrl &url)
{
    this->browser->setUrl(url);
}

QUrl HelpBrowsingWidget::getURL() const
{
    return this->browser->url();
}
