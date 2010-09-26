/* MDI child window class implementation.

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

#include "winhelpwindow.h"

#include <QMessageBox>
#include <QFileInfo>

#include <QtDebug>

#include <stdexcept>

#include "helpbrowsingwidget.h"

WinHelpWindow::WinHelpWindow(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    QList<int> sizes;
    sizes.push_back(100);
    sizes.push_back(400);
    ui.splitter->setSizes(sizes);
    connect(&winHelpFileLoader,
        SIGNAL(loadedFile(bool, const QString &,
                const QString &)), this,
        SLOT(fileLoaded(bool, const QString &, const QString &)));
    connect(this->ui.indexSearchLine, SIGNAL(textChanged(QString)), this,
        SLOT(indexSearchLineTextChanged(QString)));
    connect(this->ui.indexSearchList, SIGNAL(doubleClicked(
                const QModelIndex &)), this,
        SLOT(indexSearchListDoubleClicked(const QModelIndex &)));
    connect(this->ui.pagesTabs, SIGNAL(tabCloseRequested(int)), this,
        SLOT(pagesTabsTabCloseRequested(int)));
    this->topicsProxy.setSourceModel(qobject_cast<QAbstractItemModel *>(&(this
                ->topics)));
    this->ui.indexSearchList->setModel(qobject_cast<QAbstractItemModel *>(&(
                this->topicsProxy)));
    this->ui.indexSearchList->setModelColumn(1);
    this->topicsProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->topicsProxy.sort(1, Qt::AscendingOrder);
    this->topicsProxy.setFilterRegExp(QRegExp(this->ui.indexSearchLine->text(),
            Qt::CaseInsensitive, QRegExp::FixedString));
    this->topicsProxy.setFilterKeyColumn(1);
    tabMapper = new QSignalMapper(this);
    connect(tabMapper, SIGNAL(mapped(QWidget*)), this,
        SLOT(tabURLChanged(QWidget*)));
}

WinHelpWindow::~WinHelpWindow()
{
}

bool WinHelpWindow::loadFile(const QString &fileName)
{
    winHelpFileLoader.loadFile(fileName);

    setCurrentFile(fileName);

    return true;
}

void WinHelpWindow::fileLoaded(bool isSuccessfull,
    const QString &message,
    const QString &file)
{
    setCurrentFile(file);
    fillTopicList();
    if(!isSuccessfull)
    {
        QMessageBox::warning(this, tr("HelpViewer"), message);
        if(this->parentWidget() != NULL)
        {
            this->parentWidget()->close();
        }
    }
}

void WinHelpWindow::indexSearchLineTextChanged(QString text)
{
    Q_UNUSED(text);
    this->topicsProxy.setFilterRegExp(QRegExp(this->ui.indexSearchLine->text(),
            Qt::CaseInsensitive, QRegExp::FixedString));
    this->topicsProxy.setFilterKeyColumn(1);
}

void WinHelpWindow::indexSearchListDoubleClicked(const QModelIndex & index)
{
    int topicIndex =
        this->topicsProxy.data(this->topicsProxy.index(
            index.row(), 0), Qt::DisplayRole).toInt();
    HelpBrowsingWidget *newView = new HelpBrowsingWidget(
        this->winHelpFileLoader);
    QUrl topicUrl(QString("help://help.local/pages?topic=%1").arg(topicIndex));
    newView->goToURL(topicUrl);
    this->ui.pagesTabs->addTab(newView,
        this->winHelpFileLoader.getHelpFileTopicCaption(topicIndex));
    this->ui.pagesTabs->setCurrentIndex(this->ui.pagesTabs->count() - 1);
    connect(newView, SIGNAL(urlChanged(const QUrl &)), tabMapper, SLOT(map()));
    tabMapper->setMapping(newView, newView);
}

void WinHelpWindow::pagesTabsTabCloseRequested(int index)
{
    this->ui.pagesTabs->removeTab(index);
}

void WinHelpWindow::tabURLChanged(QWidget *widget)
{
    int tabIndex = this->ui.pagesTabs->indexOf(widget);
    if(tabIndex != -1)
    {
        HelpBrowsingWidget *helpBrowser = qobject_cast<HelpBrowsingWidget *>(
            widget);
        if(helpBrowser != NULL)
        {
            QUrl newUrl = helpBrowser->getURL();
            if(newUrl.hasQuery())
            {
                if(newUrl.hasQueryItem(QString("topic")))
                {
                    bool isInt = false;
                    int topicIndex =
                        newUrl.queryItemValue(QString("topic")).toInt(&isInt,
                        10);
                    if(isInt)
                    {
                        this->ui.pagesTabs->setTabText(tabIndex,
                            this->winHelpFileLoader.getHelpFileTopicCaption(
                                topicIndex));
                    }
                }
                else
                {
                    if(newUrl.hasQueryItem(QString("block")) &&
                        newUrl.hasQueryItem(QString("character")))
                    {
                        bool topicBlockValid = true;
                        int block =
                            newUrl.queryItemValue(QString("block")).toInt(
                            &topicBlockValid,
                            10);
                        bool topicCharacterValid = true;
                        int character =
                            newUrl.queryItemValue(QString("character")).toInt(
                            &topicCharacterValid,
                            10);
                        if(topicBlockValid && topicCharacterValid)
                        {
                            int topicIndex = winHelpFileLoader.getTopicIndex(
                                block,
                                character);
                            this->ui.pagesTabs->setTabText(tabIndex,
                                this->winHelpFileLoader.getHelpFileTopicCaption(
                                    topicIndex));
                        }
                    }
                }
            }
        }
    }
}

void WinHelpWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QString WinHelpWindow::currentFile()
{
    return this->curFile;
}

QString WinHelpWindow::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void WinHelpWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    if (!winHelpFileLoader.getHelpFileTitle().isEmpty())
    {
        setWindowTitle(userFriendlyCurrentFile() + " - "
            + winHelpFileLoader.getHelpFileTitle());
    }
    else
    {
        setWindowTitle(userFriendlyCurrentFile());
    }
}

QString WinHelpWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void WinHelpWindow::fillTopicList()
{
    int topicCount = this->winHelpFileLoader.getHelpFileTopicCount();
    QList<QPair<int, QString> > topicList;
    for(int i = 0; i < topicCount; i++)
    {
        topicList.append(QPair<int, QString>(i,
                this->winHelpFileLoader.getHelpFileTopicCaption(i)));
    }
    this->topics.setList(topicList);
    this->topicsProxy.sort(1, Qt::AscendingOrder);
    this->topicsProxy.setFilterRegExp(QRegExp(this->ui.indexSearchLine->text(),
            Qt::CaseInsensitive, QRegExp::FixedString));
    this->topicsProxy.setFilterKeyColumn(1);
}
