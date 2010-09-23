/* Topic offset class implementation.

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

#include "TopicOffset.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

TopicOffset::TopicOffset() : topicBlockNumber(Q_INT64_C(0)),
    characterCount(Q_INT64_C(0))
{
    PRINT_DBG("Topic offset default constructor");
}

TopicOffset::TopicOffset(qint64 topicBlockNumber,
    qint64 characterCount) : topicBlockNumber(topicBlockNumber),
    characterCount(characterCount)
{
    PRINT_DBG("Topic offset constructor");
}

TopicOffset::TopicOffset(QFile &file, qint64 off)
{
    PRINT_DBG("Reading Topic offset at %lld", off);
    seekFile(file, off);
    quint8 topicOffsetIn0 = readUnsignedByte(file);
    quint8 topicOffsetIn1 = readUnsignedByte(file);
    quint8 topicOffsetIn2 = readUnsignedByte(file);
    quint8 topicOffsetIn3 = readUnsignedByte(file);
    qint64 byte_0 = static_cast<qint64>(topicOffsetIn0);
    qint64 byte_1l = static_cast<qint64>(topicOffsetIn1 & 0x7F);
    qint64 byte_1h = static_cast<qint64>((topicOffsetIn1 & 0x80) >> 7);
    qint64 byte_2 = static_cast<qint64>(topicOffsetIn2);
    qint64 byte_3 = static_cast<qint64>(topicOffsetIn3);
    this->characterCount = byte_0 + Q_INT64_C(256) * byte_1l;
    this->topicBlockNumber = byte_1h + Q_INT64_C(2) * byte_2 + Q_INT64_C(512) *
        byte_3;
    PRINT_DBG("        Character count: %lld", this->characterCount);
    PRINT_DBG("        Topic block number: %lld", this->topicBlockNumber);
    PRINT_DBG("        Topic offset raw: %d %d %d %d",
        topicOffsetIn0,
        topicOffsetIn1,
        topicOffsetIn2,
        topicOffsetIn3);
    PRINT_DBG("Topic offset loaded successfully");
}

TopicOffset::TopicOffset(const void *src, size_t srcSize)
{
    PRINT_DBG("Reading Topic offset");
    if(srcSize < static_cast<size_t>(size))
    {
        throw std::runtime_error("Unable to read Topic offset");
    }
    quint8 topicOffsetIn0 = getUnsignedByte(src);
    quint8 topicOffsetIn1 =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + static_cast<size_t>(1)));
    quint8 topicOffsetIn2 =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + static_cast<size_t>(2)));
    quint8 topicOffsetIn3 =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + static_cast<size_t>(3)));
    qint64 byte_0 = static_cast<qint64>(topicOffsetIn0);
    qint64 byte_1l = static_cast<qint64>(topicOffsetIn1 & 0x7F);
    qint64 byte_1h = static_cast<qint64>((topicOffsetIn1 & 0x80) >> 7);
    qint64 byte_2 = static_cast<qint64>(topicOffsetIn2);
    qint64 byte_3 = static_cast<qint64>(topicOffsetIn3);
    this->characterCount = byte_0 + Q_INT64_C(256) * byte_1l;
    this->topicBlockNumber = byte_1h + Q_INT64_C(2) * byte_2 + Q_INT64_C(512) *
        byte_3;
    PRINT_DBG("        Character count: %lld", this->characterCount);
    PRINT_DBG("        Topic block number: %lld", this->topicBlockNumber);
    PRINT_DBG("        Topic offset raw: %d %d %d %d",
        topicOffsetIn0,
        topicOffsetIn1,
        topicOffsetIn2,
        topicOffsetIn3);
    PRINT_DBG("Topic offset loaded successfully");
}

TopicOffset::TopicOffset(const TopicOffset& rhs) : topicBlockNumber(rhs.
    topicBlockNumber), characterCount(rhs.characterCount)
{
    PRINT_DBG("Topic offset copy constructor");
}

TopicOffset & TopicOffset::operator=(const TopicOffset & rhs)
{
    PRINT_DBG("Topic offset assignment operator");
    if (this != &rhs)
    {
        this->topicBlockNumber = rhs.topicBlockNumber;
        this->characterCount = rhs.characterCount;
    }
    return *this;
}

qint64 TopicOffset::getTopicBlockNumber() const
{
    return this->topicBlockNumber;
}

qint64 TopicOffset::getCharacterCount() const
{
    return this->characterCount;
}

TopicOffset::~TopicOffset()
{
    PRINT_DBG("Topic offset destructor");
}
