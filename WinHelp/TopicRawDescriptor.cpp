/* Descriptor of Help topic class implementation.

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

#include "TopicRawDescriptor.h"

#include "TopicHeaderOld.h"

#include <typeinfo>

TopicRawDescriptor::TopicRawDescriptor() : header(), texts(), topicCaption(),
    topicContents()
{
}

TopicRawDescriptor::TopicRawDescriptor(
    const QSharedPointer<TopicHeader> &header) : header(header), texts(),
    topicCaption(), topicContents()
{
}

TopicRawDescriptor::TopicRawDescriptor(const TopicRawDescriptor &rhs) : header(
    rhs.header), texts(rhs.texts), topicCaption(rhs.topicCaption),
    topicContents(rhs.topicContents)
{
}

TopicRawDescriptor::~TopicRawDescriptor()
{
}

TopicRawDescriptor & TopicRawDescriptor::operator=(
    const TopicRawDescriptor &rhs)
{
    if (this != &rhs)
    {
        this->header = rhs.header;
        this->texts = rhs.texts;
        this->topicCaption = rhs.topicCaption;
        this->topicContents = rhs.topicContents;
    }
    return *this;
}

void TopicRawDescriptor::AddDisplayableText(
    const QSharedPointer<DisplayableText> &text)
{
    this->texts.append(text);
}

QString TopicRawDescriptor::getCaption() const
{
    return this->topicCaption;
}

QString TopicRawDescriptor::getContents() const
{
    return this->topicContents;
}

void TopicRawDescriptor::transformToHtml(int topicIndex,
    int topicCount,
    QList<QSharedPointer<TopicLink> > linkPointers)
{
    this->topicCaption = this->header->getTopicName();
    if(this->topicCaption.isEmpty())
    {
        this->topicCaption = QString("UntitledTopic#%1").arg(topicIndex);
    }
    this->topicContents =
        "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"help://help.local/pages?style=0\" /><title>";
    QString titleOrig = this->topicCaption;
    this->topicContents +=
        titleOrig.replace(QChar('&'), QString("&amp;"),
        Qt::CaseInsensitive).replace(QChar('<'), QString(
            "&lt;"), Qt::CaseInsensitive).replace(QChar('>'), QString(
            "&gt;"), Qt::CaseInsensitive);
    this->topicContents += "</title></head><body>";
    bool emptyTopic = true;
    bool isInTable = false;
    for(int i = 0; i < this->texts.count(); i++)
    {
        bool emptyPara = false;
        QString para = this->texts.at(i)->getHTML(emptyPara);
        if(!emptyPara)
        {
            emptyTopic = false;
            if(this->texts.at(i)->getType() == 0x23)
            {
                if(!isInTable)
                {
                    topicContents += "<table>";
                    isInTable = true;
                }
            }
            else
            {
                if(isInTable)
                {
                    topicContents += "</table>";
                    isInTable = false;
                }
            }
            topicContents += para;
        }
    }
    ;
    if(isInTable)
    {
        topicContents += "</table>";
        isInTable = false;
    }
    if(emptyTopic)
    {
        this->topicContents += "Empty topic";
    }
    if(typeid(*(this->header.data())) == typeid(TopicHeaderOld))
    {
        topicContents +=
            "<table width=100%><tr><td><p style=\"text-align: left\">";
        qint32 prev = header.dynamicCast<TopicHeaderOld>()->getPrevTopicNumber();
        if(prev != -1)
        {
            if(prev >= 16)
            {
                topicContents += QString(
                    "<a href=help://help.local/pages?topic=%1>Previous topic</a>")
                    .
                    arg(prev - 16);
            }
        }
        topicContents += "</p></td><td><p style=\"text-align: right\">";
        qint32 next = header.dynamicCast<TopicHeaderOld>()->getNextTopicNumber();
        if(next != -1)
        {
            if(next >= 16)
            {
                topicContents += QString(
                    "<a href=help://help.local/pages?topic=%1>Next topic</a>").
                    arg(
                    next - 16);
            }
        }
        topicContents += "</p></td></tr></table>";
    }
    this->topicContents += "</body></html>";
    this->header.clear();
    this->texts.clear();
}
