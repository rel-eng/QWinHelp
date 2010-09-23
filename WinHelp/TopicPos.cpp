/* Topic position class implementation.

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

#include "TopicPos.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

TopicPos::TopicPos() :
    topicBlockOffset(Q_INT64_C(0)), topicBlockNumber(Q_INT64_C(0)),
    invalid(true), rawPos(0), topicBlockHeaderSize(Q_INT64_C(0)),
    decompressSize(Q_INT64_C(0))
{
    PRINT_DBG("TopicPos default constructor");
}

TopicPos::TopicPos(qint64 topicBlockOffset,
    qint64 topicBlockNumber,
    qint64 topicBlockHeaderSize,
    qint64 decompressSize) : topicBlockOffset(topicBlockOffset),
    topicBlockNumber(topicBlockNumber), invalid(false),
    rawPos(static_cast<quint32>(topicBlockNumber * decompressSize +
        topicBlockOffset + topicBlockHeaderSize)),
    topicBlockHeaderSize(topicBlockHeaderSize), decompressSize(decompressSize)
{
    PRINT_DBG("TopicPos constructor");
}

TopicPos::TopicPos(QFile &file, qint64 off, qint64 topicBlockHeaderSize,
    qint64 decompressSize) : topicBlockHeaderSize(topicBlockHeaderSize),
    decompressSize(decompressSize)
{
    PRINT_DBG("Reading TopicPos at %lld", off);
    seekFile(file, off);
    this->rawPos = readUnsignedDWord(file);
    qint64 topicPosRaw = static_cast<qint64> (rawPos);
    PRINT_DBG("        TopicPos raw value: %lld", topicPosRaw);
    if ((this->rawPos == 0) || (this->rawPos == 0xFFFFFFFF))
    {
        this->invalid = true;
        this->topicBlockOffset = Q_INT64_C(0);
        this->topicBlockNumber = Q_INT64_C(0);
    }
    else
    {
        this->invalid = false;
        if (topicPosRaw < topicBlockHeaderSize)
        {
            throw std::runtime_error("Invalid TopicPos value");
        }
        this->topicBlockOffset = (topicPosRaw - topicBlockHeaderSize)
            % decompressSize;
        this->topicBlockNumber = (topicPosRaw - topicBlockHeaderSize)
            / decompressSize;
        PRINT_DBG("        Topic block offset: %lld", this->topicBlockOffset);
        PRINT_DBG("        Topic block number: %lld", this->topicBlockNumber);
    }
    PRINT_DBG("TopicPos loaded successfully");
}

TopicPos::TopicPos(quint32 rawData,
    qint64 topicBlockHeaderSize,
    qint64 decompressSize) : topicBlockHeaderSize(topicBlockHeaderSize),
    decompressSize(decompressSize)
{
    PRINT_DBG("TopicPos constructor");
    this->rawPos = rawData;
    qint64 topicPosRaw = static_cast<qint64> (rawData);
    PRINT_DBG("        TopicPos raw value: %lld", topicPosRaw);
    if ((this->rawPos == 0) || (this->rawPos == 0xFFFFFFFF))
    {
        this->invalid = true;
        this->topicBlockOffset = Q_INT64_C(0);
        this->topicBlockNumber = Q_INT64_C(0);
    }
    else
    {
        this->invalid = false;
        if (topicPosRaw < topicBlockHeaderSize)
        {
            throw std::runtime_error("Invalid TopicPos value");
        }
        this->topicBlockOffset = (topicPosRaw - topicBlockHeaderSize)
            % decompressSize;
        this->topicBlockNumber = (topicPosRaw - topicBlockHeaderSize)
            / decompressSize;
        PRINT_DBG("        Topic block offset: %lld", this->topicBlockOffset);
        PRINT_DBG("        Topic block number: %lld", this->topicBlockNumber);
    }
    PRINT_DBG("TopicPos loaded successfully");
}

TopicPos::TopicPos(const TopicPos& rhs) :
    topicBlockOffset(rhs.topicBlockOffset), topicBlockNumber(
    rhs.topicBlockNumber), invalid(rhs.invalid), rawPos(rhs.rawPos),
    topicBlockHeaderSize(rhs.topicBlockHeaderSize),
    decompressSize(rhs.decompressSize)
{
    PRINT_DBG("TopicPos copy constructor");
}

TopicPos & TopicPos::operator=(const TopicPos & rhs)
{
    PRINT_DBG("TopicPos assignment operator");
    if (this != &rhs)
    {
        this->topicBlockOffset = rhs.topicBlockOffset;
        this->topicBlockNumber = rhs.topicBlockNumber;
        this->invalid = rhs.invalid;
        this->rawPos = rhs.rawPos;
        this->topicBlockHeaderSize = rhs.topicBlockHeaderSize;
        this->decompressSize = rhs.decompressSize;
    }
    return *this;
}

TopicPos TopicPos::operator +(const TopicPos& rhs) const
{
    if(this->invalid || rhs.invalid)
    {
        throw std::runtime_error("Invalid TopicPos value");
    }
    quint32 rawSum = this->rawPos + rhs.rawPos;
    TopicPos newPos(rawSum, this->topicBlockHeaderSize, this->decompressSize);
    if((newPos.topicBlockNumber > this->topicBlockNumber) &&
            (newPos.topicBlockOffset >= this->topicBlockHeaderSize))
    {
        return TopicPos(newPos.topicBlockOffset - this->topicBlockHeaderSize,
            newPos.topicBlockNumber,
            this->topicBlockHeaderSize,
            this->decompressSize);
    }
    else
    {
        return newPos;
    }
}

TopicPos TopicPos::operator -(const TopicPos& rhs) const
{
    if(this->invalid || rhs.invalid)
    {
        throw std::runtime_error("Invalid TopicPos value");
    }
    quint32 rawDiff = this->rawPos - rhs.rawPos;
    TopicPos newPos(rawDiff, this->topicBlockHeaderSize, this->decompressSize);
    if((newPos.topicBlockNumber < this->topicBlockNumber) &&
            ((newPos.topicBlockOffset + this->topicBlockHeaderSize) <=
            this->decompressSize))
    {
        return TopicPos(newPos.topicBlockOffset + this->topicBlockHeaderSize,
            newPos.topicBlockNumber,
            this->topicBlockHeaderSize,
            this->decompressSize);
    }
    else
    {
        return newPos;
    }
}

qint64 TopicPos::getTopicBlockOffset() const
{
    return this->topicBlockOffset;
}

qint64 TopicPos::getTopicBlockNumber() const
{
    return this->topicBlockNumber;
}

bool TopicPos::isInvalid() const
{
    return this->invalid;
}

TopicPos::~TopicPos()
{
    PRINT_DBG("TopicPos destructor");
}
