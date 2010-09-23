/* Citation record class implementation.

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

#include "CitationRecord.h"

#include "SystemRecordHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

CitationRecord::CitationRecord() :
    citation()
{
    PRINT_DBG("Citation record default constructor");
}

CitationRecord::CitationRecord(const QString &citation) :
    citation(citation)
{
    PRINT_DBG("Citation record constructor");
}

CitationRecord::CitationRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading Citation record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    this->citation =
        readFixedLengthString(file,
        static_cast<uint>(hdr.getRecordSize()), codec);
    PRINT_DBG("        Citation: %s", this->citation.toLocal8Bit().data());
    PRINT_DBG("Citation record loaded successfully");
}

CitationRecord::CitationRecord(const CitationRecord& rhs) :
    citation(rhs.citation)
{
    PRINT_DBG("Citation record copy constructor");
}

CitationRecord & CitationRecord::operator=(const CitationRecord & rhs)
{
    PRINT_DBG("Citation record assignment operator");
    if (this != &rhs)
    {
        this->citation = rhs.citation;
    }
    return *this;
}

CitationRecord::~CitationRecord()
{
    PRINT_DBG("Citation record destructor");
}

QString CitationRecord::getCitation() const
{
    return this->citation;
}
