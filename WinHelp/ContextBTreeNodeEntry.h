/* Context BTree node entry definition.

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

#ifndef CONTEXTBTREENODEENTRY_H
#define CONTEXTBTREENODEENTRY_H

#include <QtGlobal>
#include <QFile>

#include "TopicOffset.h"

class ContextBTreeNodeEntry
{
private:
    quint32 hash;
    TopicOffset offset;
public:
    static const qint64 size = 8;
    ContextBTreeNodeEntry();
    ContextBTreeNodeEntry(quint32 hash, TopicOffset offset);
    ContextBTreeNodeEntry(QFile &file, qint64 off);
    ContextBTreeNodeEntry(const ContextBTreeNodeEntry& rhs);
    virtual ~ContextBTreeNodeEntry();
    ContextBTreeNodeEntry & operator=(const ContextBTreeNodeEntry & rhs);
    quint32 getHash() const;
    TopicOffset getOffset() const;
};

#endif // CONTEXTBTREENODEENTRY_H
