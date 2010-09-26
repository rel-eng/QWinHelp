/* Topic link class implementation.

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

#include "TopicLink.h"

TopicLink::TopicLink() : topicDescriptorNumber(0), topicNumber(0),
    characterCount(0)
{
}

TopicLink::TopicLink(int topicDescriptorNumber,
    int topicNumber) : topicDescriptorNumber(topicDescriptorNumber),
    topicNumber(topicNumber), characterCount(0)
{
}

TopicLink::TopicLink(const TopicLink &rhs) : topicDescriptorNumber(rhs.
    topicDescriptorNumber), topicNumber(rhs.topicNumber),
    characterCount(rhs.characterCount)
{
}

TopicLink::~TopicLink()
{
}

TopicLink & TopicLink::operator=(const TopicLink &rhs)
{
    if (this != &rhs)
    {
        this->topicDescriptorNumber = rhs.topicDescriptorNumber;
        this->topicNumber = rhs.topicNumber;
        this->characterCount = rhs.characterCount;
    }
    return *this;
}

int TopicLink::getTopicDescriptorNumber() const
{
    return this->topicDescriptorNumber;
}

int TopicLink::getTopicLinkNumber() const
{
    return this->topicNumber;
}

int TopicLink::getCharacterCount() const
{
    return this->characterCount;
}
