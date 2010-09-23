/* Contents record class implementation.

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

#include "ContentsRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "SystemRecordHeader.h"

#include <QtDebug>

ContentsRecord::ContentsRecord() :
    topicOffset()
{
    PRINT_DBG("Contents record default constructor");
}

ContentsRecord::ContentsRecord(const TopicOffset &topicOffset) :
    topicOffset(topicOffset)
{
    PRINT_DBG("Contents record constructor");
}

ContentsRecord::ContentsRecord(QFile &file, qint64 off)
{
    PRINT_DBG("Reading Contents record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    if (hdr.getRecordSize() < Q_INT64_C(4))
    {
        qDebug() << "Contents record size is too small: "
                 << hdr.getRecordSize();
    }
    this->topicOffset = TopicOffset(file, off + SystemRecordHeader::size);
    PRINT_DBG("Contents record loaded successfully");
}

ContentsRecord::ContentsRecord(const ContentsRecord& rhs) :
    topicOffset(rhs.topicOffset)
{
    PRINT_DBG("Contents record copy constructor");
}

ContentsRecord & ContentsRecord::operator=(const ContentsRecord & rhs)
{
    PRINT_DBG("Contents record assignment operator");
    if (this != &rhs)
    {
        this->topicOffset = rhs.topicOffset;
    }
    return *this;
}

ContentsRecord::~ContentsRecord()
{
    PRINT_DBG("Contents record destructor");
}

TopicOffset ContentsRecord::getTopicOffset() const
{
    return this->topicOffset;
}
