/* Help URL scheme handler class implementation.

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

#include <QUrlQuery>
#include <QBuffer>
#include <QImage>

#include "HelpUrlSchemeHandler.h"
#include "HelpReplyIoDevice.h"

HelpUrlSchemeHandler::HelpUrlSchemeHandler(ThreadedWinHelpFileLoader &winHelpFileLoader,
    QObject *parent) : QWebEngineUrlSchemeHandler(parent), winHelpFileLoader(winHelpFileLoader)
{
}

HelpUrlSchemeHandler::~HelpUrlSchemeHandler()
{
}

void HelpUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request) {
    QUrl requestUrl = request->requestUrl();
    QByteArray requestMethod = request->requestMethod();
    if (requestUrl.scheme() != "help")
    {
        request->fail(QWebEngineUrlRequestJob::UrlNotFound);
        return;
    }
    if (requestMethod != QByteArray("GET"))
    {
        request->fail(QWebEngineUrlRequestJob::UrlNotFound);
        return;
    }
    if(!requestUrl.hasQuery())
    {
        request->fail(QWebEngineUrlRequestJob::UrlNotFound);
        return;
    }
    QUrlQuery urlQuery(requestUrl);
    QByteArray content;
    if(urlQuery.hasQueryItem(QString("topic")))
    {
        bool topicIndexValid = true;
        int topicIndex = urlQuery.queryItemValue(QString("topic")).toInt(
            &topicIndexValid,
            10);
        if(!topicIndexValid)
        {
            request->fail(QWebEngineUrlRequestJob::UrlNotFound);
            return;
        }
        QString topicContents =
            winHelpFileLoader.getHelpFileTopicContents(topicIndex);
        if(topicContents.isEmpty())
        {
            request->fail(QWebEngineUrlRequestJob::UrlNotFound);
            return;
        }
        content = topicContents.toUtf8();
        QIODevice *reply = new HelpReplyIoDevice(content);
        request->reply(QByteArray("text/html"), reply);
        return;
    }
    else
    {
        if(urlQuery.hasQueryItem(QString("block")) &&
            urlQuery.hasQueryItem(QString("character")))
        {
            bool topicBlockValid = true;
            int block = urlQuery.queryItemValue(QString("block")).toInt(
                &topicBlockValid,
                10);
            bool topicCharacterValid = true;
            int character =
                urlQuery.queryItemValue(QString("character")).toInt(
                &topicCharacterValid,
                10);
            if(!(topicBlockValid && topicCharacterValid))
            {
                request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                return;
            }
            int topicIndex = winHelpFileLoader.getTopicIndex(block,
                character);
            QString topicContents =
                winHelpFileLoader.getHelpFileTopicContents(
                topicIndex);
            if(topicContents.isEmpty())
            {
                request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                return;
            }
            content = topicContents.toUtf8();
            QIODevice *reply = new HelpReplyIoDevice(content);
            request->reply(QByteArray("text/html"), reply);
            return;
        }
        else
        {
            if(urlQuery.hasQueryItem(QString("style")))
            {
                bool styleNumValid = true;
                int styleNum =
                    urlQuery.queryItemValue(QString("style")).toInt(
                    &styleNumValid,
                    10);
                if(!styleNumValid)
                {
                    request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                    return;
                }
                if(styleNum != 0)
                {
                    request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                    return;
                }
                QString fontStyles =
                    winHelpFileLoader.getFontStyles();
                if(fontStyles.isEmpty())
                {
                    request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                    return;
                }
                content = fontStyles.toUtf8();
                QIODevice *reply = new HelpReplyIoDevice(content);
                request->reply(QByteArray("text/css"), reply);
                return;
            }
            else
            {
                if(urlQuery.hasQueryItem(QString("hash")))
                {
                    bool hashNumValid = true;
                    uint hashNum =
                        urlQuery.queryItemValue(QString("hash")).toUInt(
                        &hashNumValid,
                        10);
                    if(!hashNumValid)
                    {
                        request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                        return;
                    }
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
                    if(topicContents.isEmpty())
                    {
                        request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                        return;
                    }
                    content = topicContents.toUtf8();
                    QIODevice *reply = new HelpReplyIoDevice(content);
                    request->reply(QByteArray("text/html"), reply);
                    return;
                }
                else
                {
                    if(urlQuery.hasQueryItem(QString("image")))
                    {
                        bool imageNumValid = true;
                        uint imageNum =
                            urlQuery.queryItemValue(QString("image")).toUInt(
                            &imageNumValid,
                            10);
                        if(!imageNumValid)
                        {
                            request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                            return;
                        }
                        QImage image = winHelpFileLoader.getImage(
                            imageNum);
                        QByteArray ba;
                        QBuffer buffer(&ba);
                        buffer.open(QIODevice::WriteOnly);
                        image.save(&buffer, "PNG");
                        content = ba;
                        QIODevice *reply = new HelpReplyIoDevice(content);
                        request->reply(QByteArray("image/png"), reply);
                        return;
                    }
                    else
                    {
                        if(urlQuery.hasQueryItem(QString("embeddedimage")))
                        {
                            bool imageNumValid = true;
                            uint imageNum =
                                urlQuery.queryItemValue(QString(
                                    "embeddedimage")).toUInt(
                                &imageNumValid,
                                10);
                            if(!imageNumValid)
                            {
                                request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                                return;
                            }
                            QImage image =
                                winHelpFileLoader.getEmbeddedImage(
                                imageNum);
                            QByteArray ba;
                            QBuffer buffer(&ba);
                            buffer.open(QIODevice::WriteOnly);
                            image.save(&buffer, "PNG");
                            content = ba;
                            QIODevice *reply = new HelpReplyIoDevice(content);
                            request->reply(QByteArray("image/png"), reply);
                            return;
                        }
                        else
                        {
                            request->fail(QWebEngineUrlRequestJob::UrlNotFound);
                            return;
                        }
                    }
                }
            }
        }
    }
}
