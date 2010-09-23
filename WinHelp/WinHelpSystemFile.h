/* WinHelp System file class definition.

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

#ifndef WINHELPSYSTEMFILE_H_
#define WINHELPSYSTEMFILE_H_

#include "SystemHeader.h"
#include "TopicOffset.h"
#include "WinHelpIcon.h"
#include "WindowHpjRecord.h"
#include "WindowMvpRecord.h"
#include "LCIDRecord.h"
#include "DeffontRecord.h"
#include "KeyindexRecord.h"
#include "DLLMapsRecord.h"

#include <QtGlobal>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QList>

class WinHelpSystemFile
{
private:
    QString title;
    QString copyright;
    TopicOffset startingTopicOffset;
    QList<QString> startMacros;
    WinHelpIcon icon;
    QList<WindowHpjRecord> windowsHpj;
    QList<WindowMvpRecord> windowsMvp;
    QString citation;
    LCIDRecord lcid;
    QString contentFileName;
    DeffontRecord deffont;
    QList<QString> ftindexDTypes;
    QList<QString> groups;
    QString indexSeparators;
    KeyindexRecord keyindex;
    QString language;
    QList<DLLMapsRecord> dllMaps;
    SystemHeader sysHeader;
    QTextCodec *codec;
public:
    WinHelpSystemFile();
    WinHelpSystemFile(QFile &file, qint64 off);
    WinHelpSystemFile(QFile &file, qint64 off, QTextCodec *codec);
    WinHelpSystemFile(const WinHelpSystemFile& rhs);
    virtual ~WinHelpSystemFile();
    WinHelpSystemFile & operator=(const WinHelpSystemFile & rhs);
    QString getTitle() const;
    QString getCopyright() const;
    TopicOffset getStartingTopicOffset() const;
    int getStartMacrosCount() const;
    QString getStartMacro(int index) const;
    int getWindowsHpjCount() const;
    WindowHpjRecord getWindowHpj(int index) const;
    int getWindowsMvpCount() const;
    WindowMvpRecord getWindowMvp(int index) const;
    const SystemHeader & getHeader() const;
    const WinHelpIcon & getIcon() const;
    QString getCitation() const;
    const LCIDRecord &getLCID() const;
    QString getContentFileName() const;
    const DeffontRecord &getDeffontRecord() const;
    int getFtindexDTypesCount() const;
    QString getFtindexDType(int index) const;
    int getGroupsCount() const;
    QString getGroup(int index) const;
    QString getIndexSeparators() const;
    const KeyindexRecord &getKeyindex() const;
    QString getLanguage() const;
    int getDLLMapsCount() const;
    DLLMapsRecord getDLLMap(int index) const;
    QTextCodec *getCodec() const;
};

#endif /* WINHELPSYSTEMFILE_H_ */
