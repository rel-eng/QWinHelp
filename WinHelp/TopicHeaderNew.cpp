/* Topic header new version class implementation.

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

#include "TopicHeaderNew.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

TopicHeaderNew::TopicHeaderNew() : TopicHeader(), blockSize(Q_INT64_C(0)),
    browseBack(), browseForward(), topicNumber(0), nonScroll(), scroll(),
    nextTopic(),
    texts()
{
    PRINT_DBG("Topic header new default constructor");
}

TopicHeaderNew::TopicHeaderNew(const void *src,
    size_t srcSize,
    qint64 topicBlockHeaderSize,
    qint64 decompressSize,
    const void *textSrc,
    size_t textSize,
    QTextCodec *codec) : TopicHeader(), texts()
{
    PRINT_DBG("Loading Topic header new");
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    if(srcSize < static_cast<size_t>(size))
    {
        throw std::runtime_error("Unable to load Topic header new");
    }
    this->blockSize = static_cast<qint64>(getUnsignedDWord(src));
    PRINT_DBG("        BlockSize: %lld", this->blockSize);
    PRINT_DBG("        Browse back: ");
    this->browseBack =
        TopicOffset(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8 *>(
                src) + static_cast<size_t>(4)), srcSize - static_cast<size_t>(4));
    PRINT_DBG("        Browse forward: ");
    this->browseForward =
        TopicOffset(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8 *>(
                src) + static_cast<size_t>(8)), srcSize - static_cast<size_t>(8));
    this->topicNumber =
        static_cast<qint64>(getUnsignedDWord(reinterpret_cast<const void *>(
                reinterpret_cast<const quint8 *>(src) + static_cast<size_t>(12))));
    PRINT_DBG("        TopicNumber: %d", this->topicNumber);
    PRINT_DBG("        Non scroll: ");
    this->nonScroll =
        TopicPos(getUnsignedDWord(
            reinterpret_cast<const void *>(reinterpret_cast<
                    const quint8 *>(src) + static_cast<size_t>(16))),
        topicBlockHeaderSize,
        decompressSize);
    PRINT_DBG("        Scroll: ");
    this->scroll =
        TopicPos(getUnsignedDWord(
            reinterpret_cast<const void *>(reinterpret_cast<
                    const quint8 *>(src) + static_cast<size_t>(20))),
        topicBlockHeaderSize,
        decompressSize);
    PRINT_DBG("        Next topic: ");
    this->nextTopic =
        TopicPos(getUnsignedDWord(
            reinterpret_cast<const void *>(reinterpret_cast<
                    const quint8 *>(src) + static_cast<size_t>(24))),
        topicBlockHeaderSize,
        decompressSize);
    if((textSize > static_cast<size_t>(0)) && (textSrc != NULL))
    {
        texts = extractNullTerminatedStrings(textSrc, textSize, codec);
        for(int i = 0; i < texts.size(); i++)
        {
            PRINT_DBG("Text: %s", texts.at(i).toLocal8Bit().data());
        }
    }
    else
    {
        qDebug() << "Empty topic header text";
    }
    PRINT_DBG("Topic header new loaded successfully");
}

TopicHeaderNew::TopicHeaderNew(const TopicHeaderNew& rhs) : TopicHeader(rhs),
    blockSize(rhs.blockSize), browseBack(rhs.browseBack),
    browseForward(rhs.browseForward), topicNumber(rhs.topicNumber),
    nonScroll(rhs.nonScroll), scroll(rhs.scroll), nextTopic(rhs.nextTopic),
    texts(rhs.texts)
{
    PRINT_DBG("Topic header new copy constructor");
}

TopicHeaderNew::~TopicHeaderNew()
{
    PRINT_DBG("Topic header new destructor");
}

TopicHeaderNew & TopicHeaderNew::operator=(const TopicHeaderNew & rhs)
{
    PRINT_DBG("Topic header new assignment operator");
    if (this != &rhs)
    {
        TopicHeader::operator =(rhs);
        this->blockSize = rhs.blockSize;
        this->browseBack = rhs.browseBack;
        this->browseForward = rhs.browseForward;
        this->topicNumber = rhs.topicNumber;
        this->nonScroll = rhs.nonScroll;
        this->scroll = rhs.scroll;
        this->nextTopic = rhs.nextTopic;
        this->texts = rhs.texts;
    }
    return *this;
}

qint64 TopicHeaderNew::getBlockSize() const
{
    return this->blockSize;
}

TopicOffset TopicHeaderNew::getBrowseBack() const
{
    return this->browseBack;
}

TopicOffset TopicHeaderNew::getBrowseForward() const
{
    return this->browseForward;
}

quint32 TopicHeaderNew::getTopicNumber() const
{
    return this->topicNumber;
}

TopicPos TopicHeaderNew::getNonScroll() const
{
    return this->nonScroll;
}

TopicPos TopicHeaderNew::getScroll() const
{
    return this->scroll;
}

TopicPos TopicHeaderNew::getNextTopic() const
{
    return this->nextTopic;
}

QString TopicHeaderNew::getTopicName() const
{
    if(!texts.isEmpty())
    {
        return this->texts.at(0);
    }
    else
    {
        return QString();
    }
}

QList<TopicOffset> TopicHeaderNew::getHyperlinks() const
{
    return QList<TopicOffset>();
}
