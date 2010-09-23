/* Topic link header class definition.

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

#ifndef TOPICLINKHEADER_H_
#define TOPICLINKHEADER_H_

#include <QtGlobal>

#include "TopicPos.h"

#include <cstddef>

class TopicLinkHeader
{
private:
    qint64 blockSize;
    qint64 dataLen2;
    TopicPos prevBlock;
    TopicPos nextBlock;
    qint64 dataLen1;
    quint8 recordType;
public:
    static const qint64 size = 21;
    TopicLinkHeader();
    TopicLinkHeader(const void *src,
        size_t srcSize,
        qint64 topicBlockHeaderSize,
        qint64 blockDecompressSize);
    TopicLinkHeader(const TopicLinkHeader& rhs);
    virtual ~TopicLinkHeader();
    TopicLinkHeader & operator=(const TopicLinkHeader & rhs);
    qint64 getBlockSize() const;
    qint64 getDataLen2() const;
    TopicPos getPrevBlock() const;
    TopicPos getNextBlock() const;
    qint64 getDataLen1() const;
    quint8 getRecordType() const;
};

#endif /* TOPICLINKHEADER_H_ */
