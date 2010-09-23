/* Topic header old version class implementation.

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

#include "TopicHeaderOld.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

TopicHeaderOld::TopicHeaderOld() : TopicHeader(), blockSize(Q_INT64_C(0)),
    prevTopicNumber(0), nextTopicNumber(0), texts()
{
    PRINT_DBG("Topic header old default constructor");
}

TopicHeaderOld::TopicHeaderOld(const void *src,
    size_t srcSize,
    const void *textSrc,
    size_t textSize,
    QTextCodec *codec) : TopicHeader(), texts()
{
    PRINT_DBG("Loading Topic header old");
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    if(srcSize < static_cast<size_t>(size))
    {
        throw std::runtime_error("Unable to load Topic header old");
    }
    this->blockSize = static_cast<qint64>(getUnsignedDWord(src));
    PRINT_DBG("        BlockSize: %lld", this->blockSize);
    this->prevTopicNumber =
        getUnsignedDWord(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8 *>(src) + static_cast<size_t>(4)));
    PRINT_DBG("        Prev topic number: %d", this->prevTopicNumber);
    this->nextTopicNumber =
        getUnsignedDWord(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8 *>(src) + static_cast<size_t>(8)));
    PRINT_DBG("        Next topic number: %d", this->nextTopicNumber);
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
    PRINT_DBG("Topic header old loaded successfully");
}

TopicHeaderOld::TopicHeaderOld(const TopicHeaderOld& rhs) : TopicHeader(rhs),
    blockSize(rhs.blockSize), prevTopicNumber(rhs.prevTopicNumber),
    nextTopicNumber(rhs.nextTopicNumber), texts(rhs.texts)
{
    PRINT_DBG("Topic header old copy constructor");
}

TopicHeaderOld::~TopicHeaderOld()
{
    PRINT_DBG("Topic header old destructor");
}

TopicHeaderOld & TopicHeaderOld::operator=(const TopicHeaderOld & rhs)
{
    PRINT_DBG("Topic header old assignment operator");
    if (this != &rhs)
    {
        TopicHeader::operator =(rhs);
        this->blockSize = rhs.blockSize;
        this->prevTopicNumber = rhs.prevTopicNumber;
        this->nextTopicNumber = rhs.nextTopicNumber;
        this->texts = rhs.texts;
    }
    return *this;
}

qint64 TopicHeaderOld::getBlockSize() const
{
    return this->blockSize;
}

quint32 TopicHeaderOld::getPrevTopicNumber() const
{
    return this->prevTopicNumber;
}

quint32 TopicHeaderOld::getNextTopicNumber() const
{
    return this->nextTopicNumber;
}

QString TopicHeaderOld::getTopicName() const
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

QList<TopicOffset> TopicHeaderOld::getHyperlinks() const
{
    return QList<TopicOffset>();
}
