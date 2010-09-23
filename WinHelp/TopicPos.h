/* Topic position class definition.

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

#ifndef TOPICPOS_H_
#define TOPICPOS_H_

#include <QtGlobal>
#include <QFile>

class TopicPos
{
private:
    qint64 topicBlockOffset;
    qint64 topicBlockNumber;
    bool invalid;
    quint32 rawPos;
    qint64 topicBlockHeaderSize;
    qint64 decompressSize;
public:
    static const qint64 size = 4;
    TopicPos();
    TopicPos(qint64 topicBlockOffset,
        qint64 topicBlockNumber,
        qint64 topicBlockHeaderSize,
        qint64 decompressSize);
    TopicPos(quint32 rawData,
        qint64 topicBlockHeaderSize,
        qint64 decompressSize);
    TopicPos(QFile &file,
        qint64 off,
        qint64 topicBlockHeaderSize,
        qint64 decompressSize);
    TopicPos(const TopicPos& rhs);
    virtual ~TopicPos();
    TopicPos & operator=(const TopicPos & rhs);
    TopicPos operator +(const TopicPos& rhs) const;
    TopicPos operator -(const TopicPos& rhs) const;
    qint64 getTopicBlockOffset() const;
    qint64 getTopicBlockNumber() const;
    bool isInvalid() const;
};

#endif /* TOPICPOS_H_ */
