/* Threaded WinHelp file loader class implementation.

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

#include "ThreadedWinHelpFileLoader.h"

#include <QMutexLocker>
#include <QDateTime>

#include <QtDebug>

ThreadedWinHelpFileLoader::ThreadedWinHelpFileLoader(QObject * parent) :
    QThread(parent), helpFile(), mutex(), condition(), helpFileName(),
    abort(false), restart(false)
{
}

ThreadedWinHelpFileLoader::~ThreadedWinHelpFileLoader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void ThreadedWinHelpFileLoader::loadFile(const QString &fileName)
{
    QMutexLocker locker(&mutex);

    this->helpFileName = fileName;

    if (!isRunning())
    {
        start();
    }
    else
    {
        restart = true;
        condition.wakeOne();
    }
}

void ThreadedWinHelpFileLoader::run()
{
    forever
    {
        mutex.lock();
        QString fileName = this->helpFileName;
        bool ifAbort = abort;
        mutex.unlock();

        if (ifAbort)
        {
            return;
        }
        QScopedPointer<WinHelpFile> newHelpFile;
        QString outputMessage;
        bool loadedSuccessfully = false;
        try
        {
            QScopedPointer<WinHelpFile> (new WinHelpFile(fileName)).swap(
                newHelpFile);
            outputMessage = tr("Successfully loaded %1.").arg(fileName);
            loadedSuccessfully = true;
        }
        catch (std::exception &ex)
        {
            outputMessage = tr("Cannot open file %1: %2.").arg(fileName).arg(
                ex.what());
        }
        catch (...)
        {
            outputMessage = tr("Cannot open file %1: Unknown exception.").arg(
                fileName);
        }
        mutex.lock();
        ifAbort = abort;
        mutex.unlock();
        if (ifAbort)
        {
            return;
        }
        mutex.lock();
        newHelpFile.swap(this->helpFile);
        bool ifRestart = restart;
        mutex.unlock();
        if (!ifRestart)
        {
            emit loadedFile(loadedSuccessfully, outputMessage, fileName);
        }
        mutex.lock();
        ifAbort = abort;
        mutex.unlock();
        if (ifAbort)
        {
            return;
        }
        mutex.lock();
        if (!restart)
        {
            condition.wait(&mutex);
        }
        restart = false;
        mutex.unlock();
    }
}

QString ThreadedWinHelpFileLoader::getHelpFileTitle()
{
    bool locked = mutex.tryLock();
    if (locked)
    {
        QString fileTitle;
        if (!helpFile.isNull())
        {
            fileTitle = helpFile->getSystemFile().getTitle();
        }
        else
        {
            fileTitle = tr("Loading...");
        }
        mutex.unlock();
        return fileTitle;
    }
    else
    {
        return tr("Loading...");
    }
}

int ThreadedWinHelpFileLoader::getHelpFileTopicCount()
{
    mutex.lock();
    int topicCount = helpFile->getTopicFile().getTopicsCount();
    mutex.unlock();
    return topicCount;
}

QString ThreadedWinHelpFileLoader::getHelpFileTopicCaption(int index)
{
    mutex.lock();
    QString topicCaption = helpFile->getTopicFile().getTopicCaption(index);
    mutex.unlock();
    return topicCaption;
}

QString ThreadedWinHelpFileLoader::getHelpFileTopicContents(int index)
{
    mutex.lock();
    QString topicContents = helpFile->getTopicFile().getTopicContents(index);
    mutex.unlock();
    return topicContents;
}

int ThreadedWinHelpFileLoader::getTopicIndex(int block, int character)
{
    mutex.lock();
    int topicIndex = helpFile->getTopicFile().getTopicIndexByTopicOffset(block,
        character);
    mutex.unlock();
    return topicIndex;
}

QString ThreadedWinHelpFileLoader::getFontStyles()
{
    mutex.lock();
    QString fontStyles = helpFile->getFontFile().getStyle();
    mutex.unlock();
    return fontStyles;
}

TopicOffset ThreadedWinHelpFileLoader::getTopicOffset(quint32 hash)
{
    mutex.lock();
    TopicOffset offset;
    if(helpFile->getContextFile().isHashExists(hash))
    {
        offset = helpFile->getContextFile().getOffset(hash);
    }
    mutex.unlock();
    return offset;
}
