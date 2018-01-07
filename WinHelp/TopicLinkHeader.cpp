/* Topic link header class implementation.

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

#include "TopicLinkHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

TopicLinkHeader::TopicLinkHeader() : blockSize(Q_INT64_C(0)),
    dataLen2(Q_INT64_C(0)), prevBlock(), nextBlock(), dataLen1(Q_INT64_C(0)),
    recordType(0)
{
    PRINT_DBG("Topic link header default constructor");
}

TopicLinkHeader::TopicLinkHeader(const void *src,
    size_t srcSize,
    qint64 topicBlockHeaderSize,
    qint64 blockDecompressSize)
{
    PRINT_DBG("Loading Topic link header");
    if(srcSize < static_cast<size_t>(size))
    {
        throw std::runtime_error("Unable to load Topic link header");
    }
    this->blockSize = static_cast<qint64>(getUnsignedDWord(src));
    PRINT_DBG("        BlockSize: %lld", this->blockSize);
    this->dataLen2 =
        static_cast<qint64>(getUnsignedDWord(reinterpret_cast<const void *>(
                reinterpret_cast<const quint8 *>(src) + static_cast<size_t>(4))));
    PRINT_DBG("        DataLen2: %lld", this->dataLen2);
    PRINT_DBG("        PrevBlock:");
    this->prevBlock =
        TopicPos(getUnsignedDWord(
            reinterpret_cast<const void *>(reinterpret_cast<
                    const quint8 *>(src) + static_cast<size_t>(8))),
        topicBlockHeaderSize,
        blockDecompressSize);
    PRINT_DBG("        Next:");
    this->nextBlock =
        TopicPos(getUnsignedDWord(
            reinterpret_cast<const void *>(reinterpret_cast<
                    const quint8 *>(src) + static_cast<size_t>(12))),
        topicBlockHeaderSize,
        blockDecompressSize);
    this->dataLen1 =
        static_cast<qint64>(getUnsignedDWord(reinterpret_cast<const void *>(
                reinterpret_cast<const quint8 *>(src) + static_cast<size_t>(16))));
    PRINT_DBG("        DataLen1: %lld", this->dataLen1);
    this->recordType =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + static_cast<size_t>(20)));
    PRINT_DBG("        Record type: %d", this->recordType);
    PRINT_DBG("Topic link header loaded successfully");
}

TopicLinkHeader::TopicLinkHeader(const TopicLinkHeader& rhs) : blockSize(rhs.
    blockSize), dataLen2(rhs.dataLen2), prevBlock(rhs.prevBlock),
    nextBlock(rhs.nextBlock), dataLen1(rhs.dataLen1), recordType(rhs.recordType)
{
    PRINT_DBG("Topic link header copy constructor");
}

TopicLinkHeader::~TopicLinkHeader()
{
    PRINT_DBG("Topic link header destructor");
}

TopicLinkHeader & TopicLinkHeader::operator=(const TopicLinkHeader & rhs)
{
    PRINT_DBG("Topic link header assignment operator");
    if (this != &rhs)
    {
        this->blockSize = rhs.blockSize;
        this->dataLen2 = rhs.dataLen2;
        this->prevBlock = rhs.prevBlock;
        this->nextBlock = rhs.nextBlock;
        this->dataLen1 = rhs.dataLen1;
        this->recordType = rhs.recordType;
    }
    return *this;
}

qint64 TopicLinkHeader::getBlockSize() const
{
    return this->blockSize;
}

qint64 TopicLinkHeader::getDataLen2() const
{
    return this->dataLen2;
}

TopicPos TopicLinkHeader::getPrevBlock() const
{
    return this->prevBlock;
}

TopicPos TopicLinkHeader::getNextBlock() const
{
    return this->nextBlock;
}

qint64 TopicLinkHeader::getDataLen1() const
{
    return this->dataLen1;
}

quint8 TopicLinkHeader::getRecordType() const
{
    return this->recordType;
}
