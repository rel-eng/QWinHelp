/* WinHelp Topic file class definition.

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

#ifndef WINHELPTOPICFILE_H_
#define WINHELPTOPICFILE_H_

#include <QtGlobal>
#include <QFile>
#include <QTextCodec>
#include <QCache>
#include <QList>
#include <QSharedPointer>
#include <QMultiHash>

#include "TopicPos.h"
#include "TopicBlock.h"
#include "TopicLinkHeader.h"
#include "WinHelpPhraseFile.h"
#include "WinHelpPhrImageFile.h"
#include "TopicRawDescriptor.h"
#include "WinHelpPicture.h"

#include <cstddef>

typedef QCache<qint64, TopicBlock> TopicBlockCache;

class WinHelpTopicFile
{
private:

    QList<QSharedPointer<TopicLink> > linkPointers;
    QMultiHash<qint64, int> linkBlocks;
    QList<QSharedPointer<TopicRawDescriptor> > rawDescriptors;
    QList<WinHelpPicture> embeddedImages;

    size_t CopyTopicBytes(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        TopicPos src,
        qint64 srcLength,
        void *dest,
        size_t destSize,
        TopicBlockCache &blockCache,
        bool w95Hint);
    TopicLinkHeader getTopicLinkHeader(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        qint64 topicBlockHeaderSize,
        qint64 blockDecompressSize,
        TopicPos src,
        TopicBlockCache &blockCache,
        bool w95Hint);
    size_t CopyTopicLinkData1(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        TopicPos src,
        void *dest,
        size_t destSize,
        TopicBlockCache &blockCache,
        const TopicLinkHeader &header,
        bool w95Hint);
    size_t CopyTopicLinkData2Uncompressed(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        TopicPos src,
        void *dest,
        size_t destSize,
        TopicBlockCache &blockCache,
        const TopicLinkHeader &header,
        bool w95Hint);
    size_t CopyTopicLinkData2CompressedOldPhrases(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        TopicPos src,
        void *dest,
        size_t destSize,
        TopicBlockCache &blockCache,
        TopicLinkHeader &header,
        const WinHelpPhraseFile &phrases,
        bool w95Hint);
    size_t CopyTopicLinkData2CompressedNewPhrases(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        TopicPos src,
        void *dest,
        size_t destSize,
        TopicBlockCache &blockCache,
        TopicLinkHeader &header,
        const WinHelpPhrImageFile &phrases,
        bool w95Hint);
    TopicLinkHeader parseTopicLinkUncompressed(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        qint64 topicBlockHeaderSize,
        qint64 blockDecompressSize,
        TopicPos src,
        TopicBlockCache &blockCache,
        bool isHC30,
        QTextCodec *codec,
        bool w95Hint);
    TopicLinkHeader parseTopicLinkCompressedOldPhrases(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        qint64 topicBlockHeaderSize,
        qint64 blockDecompressSize,
        TopicPos src,
        TopicBlockCache &blockCache,
        const WinHelpPhraseFile &phrases,
        bool isHC30,
        QTextCodec *codec,
        bool w95Hint);
    TopicLinkHeader parseTopicLinkCompressedNewPhrases(QFile &file,
        qint64 topicBlockSize,
        bool decompressTopicBlock,
        qint64 firstTopicBlockOffset,
        qint64 maxOffset,
        qint64 topicBlockHeaderSize,
        qint64 blockDecompressSize,
        TopicPos src,
        TopicBlockCache &blockCache,
        const WinHelpPhrImageFile &phrases,
        bool isHC30,
        QTextCodec *codec,
        bool w95Hint);
public:
    WinHelpTopicFile();
    WinHelpTopicFile(QFile &file,
        qint64 off,
        QTextCodec *codec,
        quint16 minor,
        quint16 flags);
    WinHelpTopicFile(QFile &file,
        qint64 off,
        QTextCodec *codec,
        const WinHelpPhraseFile &phrases,
        quint16 minor,
        quint16 flags);
    WinHelpTopicFile(QFile &file,
        qint64 off,
        QTextCodec *codec,
        const WinHelpPhrImageFile &phrases,
        quint16 minor,
        quint16 flags);
    WinHelpTopicFile(const WinHelpTopicFile& rhs);
    virtual ~WinHelpTopicFile();
    WinHelpTopicFile & operator=(const WinHelpTopicFile & rhs);
    int getTopicsCount() const;
    QString getTopicCaption(int index) const;
    QString getTopicContents(int index) const;
    int getTopicIndexByTopicOffset(int block, int character) const;
    const QList<WinHelpPicture> &getEmbeddedPicturesList() const;
};

#endif /* WINHELPTOPICFILE_H_ */
