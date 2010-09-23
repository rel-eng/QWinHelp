/* Topic link class definition.

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

#ifndef TOPICLINK_H_
#define TOPICLINK_H_

#include <QtGlobal>
#include <QList>

#include "TopicOffset.h"

class TopicLink
{
protected:
    TopicLink();
    TopicLink(const TopicLink &rhs);
    TopicLink &operator=(const TopicLink &rhs);
public:
    virtual ~TopicLink();
    virtual QList<TopicOffset> getHyperlinks() const = 0;
};

#endif /* TOPICLINK_H_ */
