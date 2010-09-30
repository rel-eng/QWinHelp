/* Threaded WinHelp file loader class definition.

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

#ifndef THREADEDWINHELPFILELOADER_H_
#define THREADEDWINHELPFILELOADER_H_

#include <QObject>
#include <QThread>
#include <QString>
#include <QScopedPointer>
#include <QMutex>
#include <QWaitCondition>

#include "WinHelp/TopicOffset.h"

#include "WinHelp/WinHelpFile.h"

class ThreadedWinHelpFileLoader : public QThread
{
    Q_OBJECT

public:
    ThreadedWinHelpFileLoader(QObject * parent = 0);
    virtual ~ThreadedWinHelpFileLoader();
    void loadFile(const QString &fileName);
    QString getHelpFileTitle();
    int getHelpFileTopicCount();
    QString getHelpFileTopicCaption(int index);
    QString getHelpFileTopicContents(int index);
    int getTopicIndex(int block, int character);
    TopicOffset getTopicOffset(quint32 hash);
    QString getFontStyles();

signals:
    void loadedFile(bool isSuccessfull,
        const QString &message,
        const QString &file);

protected:
    void run();

private:
    Q_DISABLE_COPY(ThreadedWinHelpFileLoader)
    QScopedPointer<WinHelpFile> helpFile;
    QMutex mutex;
    QWaitCondition condition;
    QString helpFileName;
    bool abort;
    bool restart;
};

#endif /* THREADEDWINHELPFILELOADER_H_ */
