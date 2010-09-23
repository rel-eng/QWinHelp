/* Ftindex record class definition.

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

#ifndef FTINDEXRECORD_H_
#define FTINDEXRECORD_H_

#include <QtGlobal>
#include <QTextCodec>
#include <QFile>
#include <QString>

class FtindexRecord
{
private:
    QString dtype;
public:
    FtindexRecord();
    FtindexRecord(const QString &dtype);
    FtindexRecord(QFile &file, qint64 off, QTextCodec *codec);
    FtindexRecord(const FtindexRecord& rhs);
    virtual ~FtindexRecord();
    FtindexRecord & operator=(const FtindexRecord & rhs);
    QString getDType() const;
};

#endif /* FTINDEXRECORD_H_ */
