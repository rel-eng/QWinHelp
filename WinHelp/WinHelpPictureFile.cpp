/* WinHelp picture file implementation.

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

#include "WinHelpPictureFile.h"

#include "InternalDirectoryFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

WinHelpPictureFile::WinHelpPictureFile() : picture()
{
    PRINT_DBG("WinHelp picture file default constructor");
}

WinHelpPictureFile::WinHelpPictureFile(QFile &file, qint64 off) : picture()
{
    PRINT_DBG("Reading WinHelp picture file at: %lld", off);
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    this->picture = WinHelpPicture(file,
        off + InternalDirectoryFileHeader::size);
    PRINT_DBG("WinHelp picture file loaded successfully");
}

WinHelpPictureFile::WinHelpPictureFile(WinHelpPictureFile &rhs) : picture(rhs.
    picture)
{
    PRINT_DBG("WinHelp picture file copy constructor");
}

WinHelpPictureFile::~WinHelpPictureFile()
{
    PRINT_DBG("WinHelp picture file destructor");
}

WinHelpPictureFile & WinHelpPictureFile::operator=(
    const WinHelpPictureFile &rhs)
{
    PRINT_DBG("WinHelp picture file assignment operator");
    if(this != &rhs)
    {
        this->picture = rhs.picture;
    }
    return *this;
}

const WinHelpPicture &WinHelpPictureFile::getPicture() const
{
    return this->picture;
}
