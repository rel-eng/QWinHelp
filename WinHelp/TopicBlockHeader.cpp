/* Topic block header class implementation.

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

#include "TopicBlockHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

TopicBlockHeader::TopicBlockHeader() : lastTopicLink(), firstTopicLink(),
    lastTopicHeader()
{
    PRINT_DBG("Topic block header default constructor");
}

TopicBlockHeader::TopicBlockHeader(QFile &file,
    qint64 off,
    qint64 decompressSize)
{
    PRINT_DBG("Reading Topic block header at %lld", off);
    seekFile(file, off);
    this->lastTopicLink = TopicPos(file, off, size, decompressSize);
    this->firstTopicLink = TopicPos(file,
        off + TopicPos::size,
        size,
        decompressSize);
    this->lastTopicHeader = TopicPos(file, off + Q_INT64_C(
            2) * TopicPos::size, size, decompressSize);
    PRINT_DBG("Topic block header loaded successfully");
}

TopicBlockHeader::TopicBlockHeader(const TopicBlockHeader& rhs) : lastTopicLink(
    rhs.lastTopicLink), firstTopicLink(rhs.firstTopicLink),
    lastTopicHeader(rhs.lastTopicHeader)
{
    PRINT_DBG("Topic block header copy constructor");
}

TopicBlockHeader & TopicBlockHeader::operator=(const TopicBlockHeader & rhs)
{
    PRINT_DBG("Topic block header assignment operator");
    if (this != &rhs)
    {
        this->lastTopicLink = rhs.lastTopicLink;
        this->firstTopicLink = rhs.firstTopicLink;
        this->lastTopicHeader = rhs.lastTopicHeader;
    }
    return *this;
}

TopicPos TopicBlockHeader::getLastTopicLink() const
{
    return this->lastTopicLink;
}

TopicPos TopicBlockHeader::getFirstTopicLink() const
{
    return this->firstTopicLink;
}

TopicPos TopicBlockHeader::getLastTopicHeader() const
{
    return this->lastTopicHeader;
}

TopicBlockHeader::~TopicBlockHeader()
{
    PRINT_DBG("Topic block header destructor");
}
