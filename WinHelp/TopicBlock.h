/* Topic block class definition.

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

#ifndef TOPICBLOCK_H_
#define TOPICBLOCK_H_

#include <QtGlobal>
#include <QFile>
#include <QScopedArrayPointer>

#include "TopicBlockHeader.h"

#include <cstddef>

class TopicBlock
{
private:
    //Заголовок
    TopicBlockHeader header;
    QScopedArrayPointer<quint8> unpackedBlock;
    //Максимальный размер распаковываемых данных
    qint64 decompressSize;
    //Размер буфера с распакованными данными
    size_t bufferSize;
    //Размер действительных данных в буфере
    size_t actualDecompressedDataSize;
public:
    TopicBlock();
    TopicBlock(QFile &file,
        qint64 off,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 actualSize,
        bool w95Hint);
    TopicBlock(const TopicBlock& rhs);
    virtual ~TopicBlock();
    TopicBlock & operator=(const TopicBlock & rhs);
    qint64 getDecompressSize() const;
    size_t getActualDecompressedDataSize() const;
    size_t CopyDecompressedBytes(void *dest,
        size_t destSize,
        size_t srcOffset,
        size_t srcLength) const;
};

#endif /* TOPICBLOCK_H_ */
