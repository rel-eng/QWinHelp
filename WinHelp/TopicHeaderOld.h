/* Topic header old version class definition.

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

#ifndef TOPICHEADEROLD_H_
#define TOPICHEADEROLD_H_

#include <QtGlobal>
#include <QString>
#include <QList>
#include <QTextCodec>

#include <cstddef>

#include "TopicHeader.h"

class TopicHeaderOld : public TopicHeader
{
private:
    qint64 blockSize;
    quint32 prevTopicNumber;
    quint32 nextTopicNumber;
    QList<QString> texts;
public:
    static const qint64 size = 12;
    TopicHeaderOld();
    TopicHeaderOld(const void *src,
        size_t srcSize,
        const void *textSrc,
        size_t textSize,
        QTextCodec *codec);
    TopicHeaderOld(const TopicHeaderOld& rhs);
    virtual ~TopicHeaderOld();
    TopicHeaderOld & operator=(const TopicHeaderOld & rhs);
    qint64 getBlockSize() const;
    quint32 getPrevTopicNumber() const;
    quint32 getNextTopicNumber() const;
    virtual QString getTopicName() const;
    virtual QList<TopicOffset> getHyperlinks() const;
};

#endif /* TOPICHEADEROLD_H_ */
