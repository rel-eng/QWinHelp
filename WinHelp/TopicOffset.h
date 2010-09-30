/* Topic offset class definition.

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

#ifndef TOPICOFFSET_H_
#define TOPICOFFSET_H_

#include <QtGlobal>
#include <QFile>

#include <cstddef>

class TopicOffset
{
private:
    /** 17 старших бит TOPICOFFSET. */
    qint64 topicBlockNumber;
    /** 15 младших бит TOPICOFFSET. */
    qint64 characterCount;
    quint32 rawValue;
public:
    /** Размер смещения в байтах. */
    static const qint64 size = 4;
    TopicOffset();
    TopicOffset(qint64 topicBlockNumber, qint64 characterCount);
    TopicOffset(QFile &file, qint64 off);
    TopicOffset(const void *src, size_t srcSize);
    TopicOffset(const TopicOffset& rhs);
    virtual ~TopicOffset();
    TopicOffset & operator=(const TopicOffset & rhs);
    qint64 getTopicBlockNumber() const;
    qint64 getCharacterCount() const;
    quint32 getRawValue() const;
};

#endif /* TOPICOFFSET_H_ */
