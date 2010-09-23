/* Topic header class definition.

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

#ifndef TOPICHEADER_H_
#define TOPICHEADER_H_

#include <QtGlobal>
#include <QString>

#include "TopicLink.h"

class TopicHeader : public TopicLink
{
protected:
    TopicHeader();
    TopicHeader(const TopicHeader &rhs);
    TopicHeader &operator=(const TopicHeader &rhs);
public:
    virtual ~TopicHeader();
    virtual QString getTopicName() const = 0;
};

#endif /* TOPICHEADER_H_ */
