/* WinHelp PhrIndex file class definition.

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

#ifndef WINHELPPHRINDEXFILE_H_
#define WINHELPPHRINDEXFILE_H_

#include <QtGlobal>
#include <QFile>
#include <QVector>

struct IntAllignedCursor
{
    quint8 currentBitIndex;
    quint8 currentDwordByte0;
    quint8 currentDwordByte1;
    quint8 currentDwordByte2;
    quint8 currentDwordByte3;
};

struct ByteAllignedCursor
{
    quint8 currentBitIndex;
    quint8 currentByte;
};

class WinHelpPhrIndexFile
{
private:
    /** Размер неупакованного phrImage, 32 бита без знака. */
    qint64 phrImageSize;
    /** Размер упакованного phrImage, 32 бита без знака. */
    qint64 phrImageCompressedSize;
    /** Массив смещений фраз, nEntries+1 штука. */
    QVector<uint> phrasesOffset;
    /** Массив битов, по одному биту на фразу, предположительно предназначены для того чтобы исключать некоторые фразы из полнотекстового поиска. */
    QVector<bool> phrasesBits;

    bool getNextBitIntAlligned(QFile &file, IntAllignedCursor &cur);
    bool getNextBitByteAlligned(QFile &file, ByteAllignedCursor &cur);
public:
    WinHelpPhrIndexFile();
    WinHelpPhrIndexFile(QFile &file, qint64 off);
    WinHelpPhrIndexFile(const WinHelpPhrIndexFile& rhs);
    virtual ~WinHelpPhrIndexFile();
    WinHelpPhrIndexFile & operator=(const WinHelpPhrIndexFile & rhs);
    qint64 getPhrImageSize() const;
    qint64 getPhrImageCompressedSize() const;
    int getPhraseOffsetsCount() const;
    uint getPhraseOffset(int index) const;
    int getPhraseBitsCount() const;
    bool getPhraseBit(int index) const;
};

#endif /* WINHELPPHRINDEXFILE_H_ */
