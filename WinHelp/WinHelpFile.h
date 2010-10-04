/* WinHelp file class definition.

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

#ifndef WINHELPFILE_H_
#define WINHELPFILE_H_

#include <QString>
#include <QFile>
#include <QtGlobal>

#include "WinHelpSystemFile.h"
#include "WinHelpPhraseFile.h"
#include "WinHelpPhrIndexFile.h"
#include "WinHelpPhrImageFile.h"
#include "WinHelpTopicFile.h"
#include "WinHelpFontFile.h"
#include "WinHelpContextFile.h"
#include "WinHelpPicturesCollection.h"
#include "WinHelpPicture.h"

class WinHelpFile
{
private:
    QFile file;
    WinHelpSystemFile system;
    WinHelpPhraseFile phrasesOld;
    WinHelpPhrIndexFile phrasesNewIndex;
    WinHelpPhrImageFile phrasesNewImage;
    WinHelpTopicFile topicFile;
    WinHelpFontFile fontFile;
    WinHelpContextFile contextFile;
    WinHelpPicturesCollection picturesCollection;
    WinHelpFile(const WinHelpFile& rhs);
    WinHelpFile & operator=(const WinHelpFile & rhs);
    bool phraseCompressed;
    bool hallCompression;
public:
    WinHelpFile(QString filename);
    virtual ~WinHelpFile();
    const WinHelpSystemFile & getSystemFile() const;
    const WinHelpTopicFile & getTopicFile() const;
    const WinHelpFontFile & getFontFile() const;
    const WinHelpContextFile & getContextFile() const;
    const WinHelpPicturesCollection & getPicturesCollection() const;
    WinHelpPicture getEmbeddedPicture(int index) const;
};

#endif /* WINHELPFILE_H_ */
