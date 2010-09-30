/* Help reply class implementation.

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

#include "HelpReply.h"

#include <QTimer>

HelpReply::HelpReply(ThreadedWinHelpFileLoader &winHelpFileLoader,
    const QUrl &url,
    bool giveError) : QNetworkReply()
{
    offset = 0;
    setUrl(url);
    open(ReadOnly | Unbuffered);
    if(!giveError)
    {
        if(url.hasQuery())
        {
            if(url.hasQueryItem(QString("topic")))
            {
                bool topicIndexValid = true;
                int topicIndex = url.queryItemValue(QString("topic")).toInt(
                    &topicIndexValid,
                    10);
                if(topicIndexValid)
                {
                    QString topicContents =
                        winHelpFileLoader.getHelpFileTopicContents(topicIndex);
                    if(!topicContents.isEmpty())
                    {
                        this->content = topicContents.toUtf8();
                        setHeader(QNetworkRequest::ContentTypeHeader,
                            QVariant("text/html; charset=UTF-8"));
                        setHeader(QNetworkRequest::ContentLengthHeader,
                            QVariant(this->content.size()));
                        setError(QNetworkReply::NoError, "No Error");
                    }
                    else
                    {
                        setError(ContentNotFoundError, "Not found");
                    }
                }
                else
                {
                    setError(ContentNotFoundError, "Not found");
                }
            }
            else
            {
                if(url.hasQueryItem(QString("block")) &&
                    url.hasQueryItem(QString("character")))
                {
                    bool topicBlockValid = true;
                    int block = url.queryItemValue(QString("block")).toInt(
                        &topicBlockValid,
                        10);
                    bool topicCharacterValid = true;
                    int character =
                        url.queryItemValue(QString("character")).toInt(
                        &topicCharacterValid,
                        10);
                    if(topicBlockValid && topicCharacterValid)
                    {
                        int topicIndex = winHelpFileLoader.getTopicIndex(block,
                            character);
                        QString topicContents =
                            winHelpFileLoader.getHelpFileTopicContents(
                            topicIndex);
                        if(!topicContents.isEmpty())
                        {
                            this->content = topicContents.toUtf8();
                            setHeader(QNetworkRequest::ContentTypeHeader,
                                QVariant("text/html; charset=UTF-8"));
                            setHeader(QNetworkRequest::ContentLengthHeader,
                                QVariant(this->content.size()));
                            setError(QNetworkReply::NoError, "No Error");
                        }
                        else
                        {
                            setError(ContentNotFoundError, "Not found");
                        }
                    }
                    else
                    {
                        setError(ContentNotFoundError, "Not found");
                    }
                }
                else
                {
                    if(url.hasQueryItem(QString("style")))
                    {
                        bool styleNumValid = true;
                        int styleNum =
                            url.queryItemValue(QString("style")).toInt(
                            &styleNumValid,
                            10);
                        if(styleNumValid)
                        {
                            if(styleNum == 0)
                            {
                                QString fontStyles =
                                    winHelpFileLoader.getFontStyles();
                                if(!fontStyles.isEmpty())
                                {
                                    this->content = fontStyles.toUtf8();
                                    setHeader(
                                        QNetworkRequest::ContentTypeHeader,
                                        QVariant("text/css; charset=UTF-8"));
                                    setHeader(
                                        QNetworkRequest::ContentLengthHeader,
                                        QVariant(this->content.size()));
                                    setError(QNetworkReply::NoError, "No Error");
                                }
                                else
                                {
                                    setError(ContentNotFoundError, "Not found");
                                }
                            }
                            else
                            {
                                setError(ContentNotFoundError, "Not found");
                            }
                        }
                        else
                        {
                            setError(ContentNotFoundError, "Not found");
                        }
                    }
                    else
                    {
                        if(url.hasQueryItem(QString("hash")))
                        {
                            bool hashNumValid = true;
                            uint hashNum =
                                url.queryItemValue(QString("hash")).toUInt(
                                &hashNumValid,
                                10);
                            if(hashNumValid)
                            {
                                TopicOffset offset =
                                    winHelpFileLoader.getTopicOffset(
                                    static_cast<
                                        quint32>(hashNum));
                                int topicIndex =
                                    winHelpFileLoader.getTopicIndex(
                                    static_cast<int>(
                                        offset.getTopicBlockNumber()),
                                    static_cast<int>(offset.getCharacterCount()));
                                QString topicContents =
                                    winHelpFileLoader.getHelpFileTopicContents(
                                    topicIndex);
                                if(!topicContents.isEmpty())
                                {
                                    this->content = topicContents.toUtf8();
                                    setHeader(
                                        QNetworkRequest::ContentTypeHeader,
                                        QVariant("text/html; charset=UTF-8"));
                                    setHeader(
                                        QNetworkRequest::ContentLengthHeader,
                                        QVariant(this->content.size()));
                                    setError(QNetworkReply::NoError, "No Error");
                                }
                                else
                                {
                                    setError(ContentNotFoundError, "Not found");
                                }
                            }
                            else
                            {
                                setError(ContentNotFoundError, "Not found");
                            }
                        }
                        else
                        {
                            setError(ContentNotFoundError, "Not found");
                        }
                    }
                }
            }
        }
        else
        {
            setError(ContentNotFoundError, "Not found");
        }
    }
    else
    {
        setError(ContentNotFoundError, "Not found");
    }
    QTimer::singleShot(0, this, SLOT(signalCompleted()));
}

HelpReply::~HelpReply()
{
}

void HelpReply::signalCompleted()
{
    QNetworkReply::NetworkError errorCode = error();
    if (errorCode != QNetworkReply::NoError)
    {
        emit error( errorCode );
    }
    else
    {
        emit readyRead();
        emit finished();
    }
}

void HelpReply::abort()
{
}

qint64 HelpReply::bytesAvailable() const
{
    return content.size() - offset;
}

bool HelpReply::isSequential() const
{
    return true;
}

qint64 HelpReply::readData(char *data, qint64 maxSize)
{
    if (offset < content.size())
    {
        qint64 number = qMin(maxSize, content.size() - offset);
        memcpy(data, content.constData() + offset, number);
        offset += number;
        return number;
    }
    else
    {
        return -1;
    }
}
