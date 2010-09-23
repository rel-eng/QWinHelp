/* Topic header new version class definition.

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

#ifndef TOPICHEADERNEW_H_
#define TOPICHEADERNEW_H_

#include <QtGlobal>
#include <QString>
#include <QList>
#include <QTextCodec>

#include "TopicOffset.h"
#include "TopicPos.h"

#include "TopicHeader.h"

#include <cstddef>

class TopicHeaderNew : public TopicHeader
{
private:
    qint64 blockSize;
    TopicOffset browseBack;
    TopicOffset browseForward;
    quint32 topicNumber;
    TopicPos nonScroll;
    TopicPos scroll;
    TopicPos nextTopic;
    QList<QString> texts;
public:
    static const qint64 size = 28;
    TopicHeaderNew();
    TopicHeaderNew(const void *src,
        size_t srcSize,
        qint64 topicBlockHeaderSize,
        qint64 decompressSize,
        const void *textSrc,
        size_t textSize,
        QTextCodec *codec);
    TopicHeaderNew(const TopicHeaderNew& rhs);
    virtual ~TopicHeaderNew();
    TopicHeaderNew & operator=(const TopicHeaderNew & rhs);
    qint64 getBlockSize() const;
    TopicOffset getBrowseBack() const;
    TopicOffset getBrowseForward() const;
    quint32 getTopicNumber() const;
    TopicPos getNonScroll() const;
    TopicPos getScroll() const;
    TopicPos getNextTopic() const;
    virtual QString getTopicName() const;
    virtual QList<TopicOffset> getHyperlinks() const;
};

#endif /* TOPICHEADERNEW_H_ */
