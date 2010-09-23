/* DLL maps record class definition.

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

#ifndef DLLMAPSRECORD_H_
#define DLLMAPSRECORD_H_

#include <QtGlobal>
#include <QTextCodec>
#include <QFile>
#include <QString>

class DLLMapsRecord
{
private:
    /** Win16RetailDLL. */
    QString win16RetailDLL;
    /** Win16DebugDLL. */
    QString win16DebugDLL;
    /** Win32RetailDLL. */
    QString win32RetailDLL;
    /** Win32DebugDLL. */
    QString win32DebugDLL;
public:
    DLLMapsRecord();
    DLLMapsRecord(const QString &win16RetailDLL,
        const QString &win16DebugDLL,
        const QString &win32RetailDLL,
        const QString &win32DebugDLL);
    DLLMapsRecord(QFile &file, qint64 off, QTextCodec *codec);
    DLLMapsRecord(const DLLMapsRecord& rhs);
    virtual ~DLLMapsRecord();
    DLLMapsRecord & operator=(const DLLMapsRecord & rhs);
    QString getWin16RetailDLL() const;
    QString getWin16DebugDLL() const;
    QString getWin32RetailDLL() const;
    QString getWin32DebugDLL() const;
};

#endif /* DLLMAPSRECORD_H_ */
