/* Language record class implementation.

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

#include "LanguageRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

LanguageRecord::LanguageRecord() :
    language()
{
    PRINT_DBG("Language record default constructor");
}

LanguageRecord::LanguageRecord(const QString &language) :
    language(language)
{
    PRINT_DBG("Language record constructor");
}

LanguageRecord::LanguageRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Language record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->language =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        Language: %s", this->language.toLocal8Bit().data());
    PRINT_DBG("Language record loaded successfully");
}

LanguageRecord::LanguageRecord(const LanguageRecord& rhs) :
    language(rhs.language)
{
    PRINT_DBG("Language record copy constructor");
}

LanguageRecord & LanguageRecord::operator=(const LanguageRecord & rhs)
{
    PRINT_DBG("Language record assignment operator");
    if (this != &rhs)
    {
        this->language = rhs.language;
    }
    return *this;
}

LanguageRecord::~LanguageRecord()
{
    PRINT_DBG("Language record destructor");
}

QString LanguageRecord::getLanguage() const
{
    return this->language;
}
