/* Deffont record class definition.

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

#ifndef DEFFONTRECORD_H_
#define DEFFONTRECORD_H_

#include <QtGlobal>
#include <QTextCodec>
#include <QFile>
#include <QString>

class DeffontRecord
{
private:
    /** Высота шрифта в пойнтах, 1 байт без знака. */
    quint8 heightInPoints;
    /** Набор символов, 1 байт без знака. */
    quint8 charset;
    /** Строка, содержащая имя шрифта. */
    QString fontName;
public:
    DeffontRecord();
    DeffontRecord(QFile &file, qint64 off, QTextCodec *codec);
    DeffontRecord(const DeffontRecord& rhs);
    virtual ~DeffontRecord();
    DeffontRecord & operator=(const DeffontRecord & rhs);
    QString getFontName() const;
    quint8 getFontCharset() const;
    quint8 getFontHeight() const;
};

#endif /* DEFFONTRECORD_H_ */
