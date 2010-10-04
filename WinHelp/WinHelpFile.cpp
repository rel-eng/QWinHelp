/* WinHelp file class implementation.

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

#include "WinHelpFile.h"

#include "Utils/DebugUtils.h"

#include "WinHelpFileHeader.h"
#include "WinHelpInternalDirectory.h"

#include <stdexcept>

WinHelpFile::WinHelpFile(QString filename) : file(filename)
{
    PRINT_DBG("WinHelp file constructor");
    if(!file.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Unable to open file");
    }
    WinHelpFileHeader header(file, Q_INT64_C(0));
    WinHelpInternalDirectory directory(file, header.getDirectoryStart());
    if(!directory.isFileExists(QString("|SYSTEM")))
    {
        throw std::runtime_error("No |SYSTEM file found");
    }
    this->system =
        WinHelpSystemFile(file, directory.getFileOffset(QString("|SYSTEM")));
    if(directory.isFileExists(QString("|PhrIndex")) &&
        directory.isFileExists(QString("|PhrImage")))
    {
        this->phrasesNewIndex = WinHelpPhrIndexFile(file,
            directory.getFileOffset(QString("|PhrIndex")));
        this->phrasesNewImage = WinHelpPhrImageFile(file,
            directory.getFileOffset(QString("|PhrImage")),
            this->system.getCodec(),
            this->phrasesNewIndex);
        this->phraseCompressed = true;
        this->hallCompression = true;
    }
    else
    {
        if(directory.isFileExists(QString("|Phrases")))
        {
            if(this->system.getHeader().getMinor() <= 16)
            {
                this->phrasesOld = WinHelpPhraseFile(file,
                    directory.getFileOffset(QString("|Phrases")),
                    this->system.getCodec(),
                    false,
                    false);
            }
            else
            {
                if(this->system.getHeader().getMinor() == 27)
                {
                    this->phrasesOld = WinHelpPhraseFile(file,
                        directory.getFileOffset(QString("|Phrases")),
                        this->system.getCodec(),
                        true,
                        true);
                }
                else
                {
                    this->phrasesOld = WinHelpPhraseFile(file,
                        directory.getFileOffset(QString("|Phrases")),
                        this->system.getCodec(),
                        true,
                        false);
                }
            }
            this->phraseCompressed = true;
            this->hallCompression = false;
        }
        else
        {
            this->phraseCompressed = false;
            this->hallCompression = false;
        }
    }
    if(!directory.isFileExists(QString("|FONT")))
    {
        throw std::runtime_error("No |FONT file found");
    }
    this->fontFile =
        WinHelpFontFile(file, directory.getFileOffset(QString(
                "|FONT")), this->system.getCodec());
    if(directory.isFileExists(QString("|CONTEXT")))
    {
        this->contextFile =
            WinHelpContextFile(file, directory.getFileOffset(QString("|CONTEXT")));
    }
    if(!directory.isFileExists(QString("|TOPIC")))
    {
        throw std::runtime_error("No |TOPIC file found");
    }
    if((!this->phraseCompressed) && (!this->hallCompression))
    {
        this->topicFile =
            WinHelpTopicFile(file, directory.getFileOffset(QString(
                    "|TOPIC")), this->system.getCodec(),
            this->system.getHeader().getMinor(),
            this->system.getHeader().getFlags());
    }
    else
    {
        if(this->phraseCompressed && (!this->hallCompression))
        {
            this->topicFile =
                WinHelpTopicFile(file, directory.getFileOffset(QString(
                        "|TOPIC")),
                this->system.getCodec(), this->phrasesOld,
                this->system.getHeader().getMinor(),
                this->system.getHeader().getFlags());
        }
        else
        {
            if(this->phraseCompressed && this->hallCompression)
            {
                this->topicFile = WinHelpTopicFile(file,
                    directory.getFileOffset(QString("|TOPIC")),
                    this->system.getCodec(),
                    this->phrasesNewImage,
                    this->system.getHeader().getMinor(),
                    this->system.getHeader().getFlags());
            }
        }
    }
    this->picturesCollection = WinHelpPicturesCollection(file, directory);
}

WinHelpFile::~WinHelpFile()
{
    PRINT_DBG("WinHelp file destructor");
    file.close();
}

const WinHelpSystemFile & WinHelpFile::getSystemFile() const
{
    return this->system;
}

const WinHelpTopicFile & WinHelpFile::getTopicFile() const
{
    return this->topicFile;
}

const WinHelpFontFile & WinHelpFile::getFontFile() const
{
    return this->fontFile;
}

const WinHelpContextFile & WinHelpFile::getContextFile() const
{
    return this->contextFile;
}

const WinHelpPicturesCollection & WinHelpFile::getPicturesCollection() const
{
    return this->picturesCollection;
}

WinHelpPicture WinHelpFile::getEmbeddedPicture(int index) const
{
    if ((index < 0) || (index >= this->topicFile.getEmbeddedPicturesList().size()))
    {
        return WinHelpPicture();
    }
    return this->topicFile.getEmbeddedPicturesList().at(index);
}
