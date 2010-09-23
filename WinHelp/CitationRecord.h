/* Citation record class definition.

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

#ifndef CITATIONRECORD_H_
#define CITATIONRECORD_H_

#include <QtGlobal>
#include <QTextCodec>
#include <QFile>
#include <QString>

class CitationRecord
{
private:
    QString citation;
public:
    CitationRecord();
    CitationRecord(const QString &citation);
    CitationRecord(QFile &file, qint64 off, QTextCodec *codec);
    CitationRecord(const CitationRecord& rhs);
    virtual ~CitationRecord();
    CitationRecord & operator=(const CitationRecord & rhs);
    QString getCitation() const;
};

#endif /* CITATIONRECORD_H_ */
