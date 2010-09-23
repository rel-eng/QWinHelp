/* WinHelp PhrImage file class implementation.

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

#include "WinHelpPhrImageFile.h"

#include "InternalDirectoryFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/CompressionUtils.h"

#include <QScopedArrayPointer>

#include <stdexcept>

WinHelpPhrImageFile::WinHelpPhrImageFile() :
    phrases(), phrasesRaw()
{
    PRINT_DBG("WinHelp PhrImage file default constructor");
}

WinHelpPhrImageFile::WinHelpPhrImageFile(QFile &file, qint64 off,
    QTextCodec *codec, const WinHelpPhrIndexFile &index) :
    phrases(), phrasesRaw()
{
    PRINT_DBG("Loading WinHelp PhrImage file at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    seekFile(file, off + InternalDirectoryFileHeader::size);
    if (index.getPhrImageCompressedSize() < index.getPhrImageSize())
    {
        PRINT_DBG("Reading compressed phrases at %lld", file.pos());
        QScopedArrayPointer<quint8> uncompressedPhrases(
            new quint8[static_cast<size_t> (index.getPhrImageSize())]);
        unpackLZ77(file, file.pos(), index.getPhrImageCompressedSize(),
            uncompressedPhrases.data(),
            static_cast<size_t> (index.getPhrImageSize()));
        quint64 pointer = 0;
        for (quint16 i = 0; i
            < (static_cast<quint16> (index.getPhraseOffsetsCount()) - 1); i++)
        {
            uint size = index.getPhraseOffset(i + 1) - index.getPhraseOffset(i);
            QString phrase = readFixedLengthStringFromBuffer(
                uncompressedPhrases.data(),
                static_cast<size_t> (index.getPhrImageSize()),
                static_cast<size_t> (pointer), size, codec);
            this->phrases.append(phrase);
            QScopedArrayPointer<quint8> phraseRaw(
                new quint8[static_cast<size_t> (size)]);
            copyBytesFromBuffer(
                reinterpret_cast<const void *> (uncompressedPhrases.data()),
                static_cast<size_t> (index.getPhrImageSize()),
                static_cast<size_t> (pointer),
                static_cast<size_t> (size),
                reinterpret_cast<void *> (phraseRaw.data()),
                static_cast<size_t> (size));
            this->phrasesRaw.append(QByteArray(
                    reinterpret_cast<const char *> (phraseRaw.data()),
                    static_cast<int>(size)));
            pointer += static_cast<quint64> (size);
            PRINT_DBG("        Phrase: %s", phrase.toLocal8Bit().data());
        }
    }
    else
    {
        PRINT_DBG("Reading phrases at %lld", file.pos());
        for (quint16 i = 0; i
            < (static_cast<quint16> (index.getPhraseOffsetsCount()) - 1); i++)
        {
            uint size = index.getPhraseOffset(i + 1) - index.getPhraseOffset(i);
            qint64 posMem = file.pos();
            QString phrase = readFixedLengthString(file, size, codec);
            this->phrases.append(phrase);
            QScopedArrayPointer<quint8> phraseRaw(
                new quint8[static_cast<size_t> (size)]);
            seekFile(file, posMem);
            fillBuffer(file, static_cast<qint64> (size),
                reinterpret_cast<void *> (phraseRaw.data()),
                static_cast<size_t> (size));
            this->phrasesRaw.append(QByteArray(
                    reinterpret_cast<const char *> (phraseRaw.data()),
                    static_cast<int>(size)));
            PRINT_DBG("        Phrase: %s", phrase.toLocal8Bit().data());
        }
    }
    PRINT_DBG("WinHelp PhrImage file loaded successfully");
}

WinHelpPhrImageFile::WinHelpPhrImageFile(const WinHelpPhrImageFile& rhs) :
    phrases(rhs.phrases), phrasesRaw(rhs.phrasesRaw)
{
    PRINT_DBG("WinHelp PhrImage file copy constructor");
}

WinHelpPhrImageFile::~WinHelpPhrImageFile()
{
    PRINT_DBG("WinHelp PhrImage file destructor");
}

WinHelpPhrImageFile & WinHelpPhrImageFile::operator=(
    const WinHelpPhrImageFile & rhs)
{
    PRINT_DBG("WinHelp PhrImage file assignment operator");
    if (this != &rhs)
    {
        this->phrases = rhs.phrases;
        this->phrasesRaw = rhs.phrasesRaw;
    }
    return *this;
}

int WinHelpPhrImageFile::getPhrasesCount() const
{
    return phrases.count();
}

QString WinHelpPhrImageFile::getPhrase(int index) const
{
    if ((index < 0) || (index >= phrases.size()))
    {
        throw new std::out_of_range("Index of phrase is out of range");
    }
    return phrases.at(index);
}

QByteArray WinHelpPhrImageFile::getRawPhrase(int index) const
{
    if ((index < 0) || (index >= phrasesRaw.size()))
    {
        throw new std::out_of_range("Index of phrase is out of range");
    }
    return phrasesRaw.at(index);
}
