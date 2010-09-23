/* Keyindex record class definition.

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

#ifndef KEYINDEXRECORD_H_
#define KEYINDEXRECORD_H_

#include <QtGlobal>
#include <QTextCodec>
#include <QFile>
#include <QString>

class KeyindexRecord
{
private:
    /** btreename, 10 байт, btreename[1] is footnote character. */
    QString btreename;
    /** mapname, 10 байт. */
    QString mapname;
    /** dataname, 10 байт. */
    QString dataname;
    /** title, 80 байт. */
    QString title;
public:
    KeyindexRecord();
    KeyindexRecord(const QString &btreename,
        const QString &mapname,
        const QString &dataname,
        const QString &title);
    KeyindexRecord(QFile &file, qint64 off, QTextCodec *codec);
    KeyindexRecord(const KeyindexRecord& rhs);
    virtual ~KeyindexRecord();
    KeyindexRecord & operator=(const KeyindexRecord & rhs);
    QString getBtreeName() const;
    QString getMapName() const;
    QString getDataName() const;
    QString getTitle() const;
};

#endif /* KEYINDEXRECORD_H_ */
