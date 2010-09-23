/* Descriptor of Help topic class definition.

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

#ifndef TOPICRAWDESCRIPTOR_H_
#define TOPICRAWDESCRIPTOR_H_

#include <QtGlobal>
#include <QSharedPointer>
#include <QString>

#include "TopicHeader.h"
#include "DisplayableText.h"

class TopicRawDescriptor
{
private:
    QSharedPointer<TopicHeader> header;
    QList<QSharedPointer<DisplayableText> > texts;
    QString topicCaption;
    QString topicContents;
public:
    TopicRawDescriptor();
    TopicRawDescriptor(const QSharedPointer<TopicHeader> &header);
    TopicRawDescriptor(const TopicRawDescriptor &rhs);
    virtual ~TopicRawDescriptor();
    TopicRawDescriptor &operator=(const TopicRawDescriptor &rhs);
    void AddDisplayableText(const QSharedPointer<DisplayableText> &text);
    QString getCaption() const;
    QString getContents() const;
    void transformToHtml(int topicIndex, int topicCount);
};

#endif /* TOPICRAWDESCRIPTOR_H_ */
