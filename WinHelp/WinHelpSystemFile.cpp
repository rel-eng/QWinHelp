/* WinHelp System file class implementation.

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

#include "WinHelpSystemFile.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"
#include "Utils/TextUtils.h"

#include "InternalDirectoryFileHeader.h"
#include "SystemRecordHeader.h"
#include "CharsetRecord.h"
#include "TitleRecord.h"
#include "CopyrightRecord.h"
#include "ContentsRecord.h"
#include "ConfigRecord.h"
#include "IconRecord.h"
#include "CitationRecord.h"
#include "CNTRecord.h"
#include "FtindexRecord.h"
#include "GroupsRecord.h"
#include "IndexSRecord.h"
#include "LanguageRecord.h"

#include <QtDebug>
#include <QMultiHash>
#include <QtAlgorithms>

#include <stdexcept>

WinHelpSystemFile::WinHelpSystemFile() :
    title(), copyright(), startingTopicOffset(), startMacros(), icon(),
    windowsHpj(), windowsMvp(), citation(), lcid(), contentFileName(),
    deffont(), ftindexDTypes(), groups(), indexSeparators(),
    keyindex(), language(), dllMaps(), sysHeader(), codec(NULL)
{
    PRINT_DBG("WinHelp system file default constructor");
}

WinHelpSystemFile::WinHelpSystemFile(QFile &file, qint64 off) :
    title(), copyright(), startingTopicOffset(), startMacros(), icon(),
    windowsHpj(), windowsMvp(), citation(), lcid(), contentFileName(),
    deffont(), ftindexDTypes(), groups(), indexSeparators(),
    keyindex(), language(), dllMaps(), sysHeader(), codec(NULL)
{
    PRINT_DBG("Reading WinHelp system file at: %lld", off);
    seekFile(file, off);
    InternalDirectoryFileHeader fileHeader(file, off);
    qint64 dataEnd = off + InternalDirectoryFileHeader::size
        + fileHeader.getUsedSpace();
    sysHeader = SystemHeader(file, off + InternalDirectoryFileHeader::size);
    if (sysHeader.getMinor() <= 16)
    {
        PRINT_DBG("Reading old title at %lld",
                (off + InternalDirectoryFileHeader::size + SystemHeader::size));
        seekFile(file, off + InternalDirectoryFileHeader::size
            + SystemHeader::size);
        this->codec = getDefaultTextCodec();
        qDebug() << "Defaulting to CP1252";
        qint64 length = Q_INT64_C(0);
        this->title = readNullTerminatedString(file, this->codec, length);
        PRINT_DBG("        Title: %s", this->title.toLocal8Bit().data());
        PRINT_DBG("        Length of title: %lld", length);
        PRINT_DBG("Old title loaded successfully");
    }
    else
    {
        PRINT_DBG("Reading headers of WinHelp system file records");
        QMultiHash<quint16, qint64> recordOffsets;
        qint64 file_pos = file.pos();
        while (file_pos < dataEnd)
        {
            SystemRecordHeader hdr(file, file_pos);
            recordOffsets.insert(hdr.getRecordType(), file_pos);
            file_pos += hdr.getRecordSize() + SystemRecordHeader::size;
        }
        if (file_pos > dataEnd)
        {
            qDebug() << "Records in System file are longer than System file";
        }
        PRINT_DBG("Headers of WinHelp system file records loaded successfully");
        PRINT_DBG("Loading charset");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CHARSET_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CHARSET_RECORD) > 1)
            {
                qDebug() << "More than one charset record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CHARSET_RECORD);
            CharsetRecord charsetRecord(file, offsets.at(0));
            this->codec = charsetRecord.getCodec();
        }
        else
        {
            this->codec = getDefaultTextCodec();
            qDebug() << "Defaulting to CP1252";
        }
        PRINT_DBG("Charset loaded successfully");
        PRINT_DBG("Loading title");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::TITLE_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::TITLE_RECORD) > 1)
            {
                qDebug() << "More than one title record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::TITLE_RECORD);
            TitleRecord titleRecord(file, offsets.at(0), this->codec);
            this->title = titleRecord.getTitle();
        }
        else
        {
            qDebug() << "No title record";
        }
        PRINT_DBG("Title loaded successfully");
        PRINT_DBG("Loading copyright");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::COPYRIGHT_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::COPYRIGHT_RECORD) > 1)
            {
                qDebug() << "More than one copyright record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::COPYRIGHT_RECORD);
            CopyrightRecord copyrightRecord(file, offsets.at(0), this->codec);
            this->copyright = copyrightRecord.getCopyright();
        }
        else
        {
            qDebug() << "No copyright record";
        }
        PRINT_DBG("Copyright loaded successfully");
        PRINT_DBG("Loading contents");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CONTENTS_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CONTENTS_RECORD) > 1)
            {
                qDebug() << "More than one contents record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CONTENTS_RECORD);
            ContentsRecord contentsRecord(file, offsets.at(0));
            this->startingTopicOffset = contentsRecord.getTopicOffset();
        }
        else
        {
            qDebug() << "No contents record";
        }
        PRINT_DBG("Contents loaded successfully");
        PRINT_DBG("Loading config");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CONFIG_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CONFIG_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                ConfigRecord configRecord(file, offsets.at(i), this->codec);
                this->startMacros.append(configRecord.getMacro());
            }
        }
        else
        {
            qDebug() << "No config record";
        }
        PRINT_DBG("Config loaded successfully");
        PRINT_DBG("Loading icon");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::ICON_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::ICON_RECORD) > 1)
            {
                qDebug() << "More than one icon record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::ICON_RECORD);
            IconRecord iconRecord(file, offsets.at(0));
            this->icon = iconRecord.getIcon();
        }
        else
        {
            qDebug() << "No icon record";
        }
        PRINT_DBG("Icon loaded successfully");
        PRINT_DBG("Loading window record");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::WINDOW_HPJ_FILE_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::WINDOW_HPJ_FILE_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                SystemRecordHeader hdr(file, offsets.at(i));
                if ((hdr.getRecordSize() >= Q_INT64_C(90))
                    && (hdr.getRecordSize() < Q_INT64_C(108)))
                {
                    WindowHpjRecord windowRecord(file, offsets.at(i),
                        this->codec);
                    this->windowsHpj.append(windowRecord);
                }
                else
                {
                    if (hdr.getRecordSize() >= Q_INT64_C(108))
                    {
                        WindowMvpRecord windowRecord(file, offsets.at(i),
                            this->codec);
                        this->windowsMvp.append(windowRecord);
                    }
                }
            }
        }
        else
        {
            qDebug() << "No Window record";
        }
        PRINT_DBG("Window record loaded successfully");
        PRINT_DBG("Loading citation");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CITATION_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CITATION_RECORD) > 1)
            {
                qDebug() << "More than one citation record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CITATION_RECORD);
            CitationRecord citationRecord(file, offsets.at(0), this->codec);
            this->citation = citationRecord.getCitation();
        }
        else
        {
            qDebug() << "No citation record";
        }
        PRINT_DBG("Citation loaded successfully");
        PRINT_DBG("Loading LCID");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::LCID_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::LCID_RECORD) > 1)
            {
                qDebug() << "More than one LCID record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::LCID_RECORD);
            this->lcid = LCIDRecord(file, offsets.at(0));
        }
        else
        {
            qDebug() << "No LCID record";
        }
        PRINT_DBG("LCID loaded successfully");
        PRINT_DBG("Loading CNT record");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CNT_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CNT_RECORD) > 1)
            {
                qDebug() << "More than one CNT record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CNT_RECORD);
            CNTRecord cntRecord(file, offsets.at(0), this->codec);
            this->contentFileName = cntRecord.getContentFileName();
        }
        else
        {
            qDebug() << "No CNT record";
        }
        PRINT_DBG("CNT record loaded successfully");
        if (this->sysHeader.getMinor() == SystemHeader::hcw40_format)
        {
            PRINT_DBG("Loading DEFFONT");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::DEFFONT_RECORD)))
            {
                if (recordOffsets.count(SystemRecordType::DEFFONT_RECORD) > 1)
                {
                    qDebug() << "More than one DEFFONT record";
                }
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::DEFFONT_RECORD);
                this->deffont = DeffontRecord(file, offsets.at(0), this->codec);
            }
            else
            {
                qDebug() << "No DEFFONT record";
            }
            PRINT_DBG("DEFFONT loaded successfully");
        }
        else
        {
            PRINT_DBG("Loading ftindex");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::FTINDEX_RECORD)))
            {
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::FTINDEX_RECORD);
                qSort(offsets);
                for (int i = 0; i < offsets.size(); i++)
                {
                    FtindexRecord ftindexRecord(file, offsets.at(i),
                        this->codec);
                    this->ftindexDTypes.append(ftindexRecord.getDType());
                }
            }
            else
            {
                qDebug() << "No ftindex record";
            }
            PRINT_DBG("Ftindex loaded successfully");
        }
        PRINT_DBG("Loading groups");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::GROUPS_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::GROUPS_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                GroupsRecord groupsRecord(file, offsets.at(i), this->codec);
                this->groups.append(groupsRecord.getGroup());
            }
        }
        else
        {
            qDebug() << "No groups record";
        }
        PRINT_DBG("Groups loaded successfully");
        if (this->sysHeader.getMinor() == SystemHeader::hcw40_format)
        {
            PRINT_DBG("Loading IndexS");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::INDEX_S_RECORD)))
            {
                if (recordOffsets.count(SystemRecordType::INDEX_S_RECORD) > 1)
                {
                    qDebug() << "More than one IndexS record";
                }
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::INDEX_S_RECORD);
                IndexSRecord indexSRecord(file, offsets.at(0), this->codec);
                this->indexSeparators = indexSRecord.getIndexSeparators();
            }
            else
            {
                qDebug() << "No IndexS record";
            }
            PRINT_DBG("IndexS loaded successfully");
        }
        else
        {
            PRINT_DBG("Loading Keyindex");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::KEYINDEX_RECORD)))
            {
                if (recordOffsets.count(SystemRecordType::KEYINDEX_RECORD) > 1)
                {
                    qDebug() << "More than one KEYINDEX record";
                }
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::KEYINDEX_RECORD);
                this->keyindex = KeyindexRecord(file, offsets.at(0),
                    this->codec);
            }
            else
            {
                qDebug() << "No Keyindex record";
            }
            PRINT_DBG("Keyindex loaded successfully");
        }
        PRINT_DBG("Loading language");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::LANGUAGE_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::LANGUAGE_RECORD) > 1)
            {
                qDebug() << "More than one language record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::LANGUAGE_RECORD);
            LanguageRecord languageRecord(file, offsets.at(0), this->codec);
            this->language = languageRecord.getLanguage();
        }
        else
        {
            qDebug() << "No language record";
        }
        PRINT_DBG("Language loaded successfully");
        PRINT_DBG("Loading DLLMaps");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::DLLMAPS_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::DLLMAPS_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                DLLMapsRecord dllmapsRecord(file, offsets.at(i), this->codec);
                this->dllMaps.append(dllmapsRecord);
            }
        }
        else
        {
            qDebug() << "No DLLMaps record";
        }
        PRINT_DBG("DLLMaps loaded successfully");
    }
    PRINT_DBG("WinHelp system file loaded successfully");
}

WinHelpSystemFile::WinHelpSystemFile(QFile &file, qint64 off,
    QTextCodec *codec) :
    title(), copyright(), startingTopicOffset(), startMacros(), icon(),
    windowsHpj(), windowsMvp(), citation(), lcid(), contentFileName(),
    deffont(), ftindexDTypes(), groups(), indexSeparators(),
    keyindex(), language(), dllMaps(), sysHeader(), codec(codec)
{
    PRINT_DBG("Reading WinHelp system file at %lld", off);
    if (this->codec == NULL)
    {
        throw std::invalid_argument("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader fileHeader(file, off);
    qint64 dataEnd = off + InternalDirectoryFileHeader::size
        + fileHeader.getUsedSpace();
    SystemHeader sysHeader(file, off + InternalDirectoryFileHeader::size);
    if (sysHeader.getMinor() <= 16)
    {
        PRINT_DBG("Reading old title at %lld",
                (off + InternalDirectoryFileHeader::size + SystemHeader::size));
        seekFile(file, off + InternalDirectoryFileHeader::size
            + SystemHeader::size);
        qint64 length = Q_INT64_C(0);
        this->title = readNullTerminatedString(file, codec, length);
        PRINT_DBG("        Title: %s", this->title.toLocal8Bit().data());
        PRINT_DBG("        Length of title: %lld", length);
        PRINT_DBG("Old title loaded successfully");
    }
    else
    {
        PRINT_DBG("Reading headers of WinHelp system file records");
        QMultiHash<quint16, qint64> recordOffsets;
        qint64 file_pos = file.pos();
        while (file_pos < dataEnd)
        {
            SystemRecordHeader hdr(file, file_pos);
            recordOffsets.insert(hdr.getRecordType(), file_pos);
            file_pos += hdr.getRecordSize() + SystemRecordHeader::size;
        }
        if (file_pos > dataEnd)
        {
            qDebug() << "Records in System file are longer than System file";
        }
        PRINT_DBG("Headers of WinHelp system file records loaded successfully");
        PRINT_DBG("Loading charset");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CHARSET_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CHARSET_RECORD) > 1)
            {
                qDebug() << "More than one charset record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CHARSET_RECORD);
            CharsetRecord charsetRecord(file, offsets.at(0));
        }
        PRINT_DBG("Charset loaded successfully");
        PRINT_DBG("Loading title");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::TITLE_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::TITLE_RECORD) > 1)
            {
                qDebug() << "More than one title record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::TITLE_RECORD);
            TitleRecord titleRecord(file, offsets.at(0), this->codec);
            this->title = titleRecord.getTitle();
        }
        else
        {
            qDebug() << "No title record";
        }
        PRINT_DBG("Title loaded successfully");
        PRINT_DBG("Loading copyright");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::COPYRIGHT_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::COPYRIGHT_RECORD) > 1)
            {
                qDebug() << "More than one copyright record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::COPYRIGHT_RECORD);
            CopyrightRecord copyrightRecord(file, offsets.at(0), this->codec);
            this->copyright = copyrightRecord.getCopyright();
        }
        else
        {
            qDebug() << "No copyright record";
        }
        PRINT_DBG("Copyright loaded successfully");
        PRINT_DBG("Loading contents");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CONTENTS_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CONTENTS_RECORD) > 1)
            {
                qDebug() << "More than one contents record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CONTENTS_RECORD);
            ContentsRecord contentsRecord(file, offsets.at(0));
            this->startingTopicOffset = contentsRecord.getTopicOffset();
        }
        else
        {
            qDebug() << "No contents record";
        }
        PRINT_DBG("Contents loaded successfully");
        PRINT_DBG("Loading config");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CONFIG_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CONFIG_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                ConfigRecord configRecord(file, offsets.at(i), this->codec);
                this->startMacros.append(configRecord.getMacro());
            }
        }
        else
        {
            qDebug() << "No config record";
        }
        PRINT_DBG("Config loaded successfully");
        PRINT_DBG("Loading icon");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::ICON_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::ICON_RECORD) > 1)
            {
                qDebug() << "More than one icon record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::ICON_RECORD);
            IconRecord iconRecord(file, offsets.at(0));
            this->icon = iconRecord.getIcon();
        }
        else
        {
            qDebug() << "No icon record";
        }
        PRINT_DBG("Icon loaded successfully");
        PRINT_DBG("Loading window record");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::WINDOW_HPJ_FILE_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::WINDOW_HPJ_FILE_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                SystemRecordHeader hdr(file, offsets.at(i));
                if ((hdr.getRecordSize() >= Q_INT64_C(90))
                    && (hdr.getRecordSize() < Q_INT64_C(108)))
                {
                    WindowHpjRecord windowRecord(file, offsets.at(i),
                        this->codec);
                    this->windowsHpj.append(windowRecord);
                }
                else
                {
                    if (hdr.getRecordSize() >= Q_INT64_C(108))
                    {
                        WindowMvpRecord windowRecord(file, offsets.at(i),
                            this->codec);
                        this->windowsMvp.append(windowRecord);
                    }
                }
            }
        }
        else
        {
            qDebug() << "No Window record";
        }
        PRINT_DBG("Window record loaded successfully");
        PRINT_DBG("Loading citation");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CITATION_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CITATION_RECORD) > 1)
            {
                qDebug() << "More than one citation record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CITATION_RECORD);
            CitationRecord citationRecord(file, offsets.at(0), this->codec);
            this->citation = citationRecord.getCitation();
        }
        else
        {
            qDebug() << "No citation record";
        }
        PRINT_DBG("Citation loaded successfully");
        PRINT_DBG("Loading LCID");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::LCID_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::LCID_RECORD) > 1)
            {
                qDebug() << "More than one LCID record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::LCID_RECORD);
            this->lcid = LCIDRecord(file, offsets.at(0));
        }
        else
        {
            qDebug() << "No LCID record";
        }
        PRINT_DBG("LCID loaded successfully");
        PRINT_DBG("Loading CNT record");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::CNT_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::CNT_RECORD) > 1)
            {
                qDebug() << "More than one CNT record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::CNT_RECORD);
            CNTRecord cntRecord(file, offsets.at(0), this->codec);
            this->contentFileName = cntRecord.getContentFileName();
        }
        else
        {
            qDebug() << "No CNT record";
        }
        PRINT_DBG("CNT record loaded successfully");
        if (this->sysHeader.getMinor() == SystemHeader::hcw40_format)
        {
            PRINT_DBG("Loading DEFFONT");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::DEFFONT_RECORD)))
            {
                if (recordOffsets.count(SystemRecordType::DEFFONT_RECORD) > 1)
                {
                    qDebug() << "More than one DEFFONT record";
                }
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::DEFFONT_RECORD);
                this->deffont = DeffontRecord(file, offsets.at(0), this->codec);
            }
            else
            {
                qDebug() << "No DEFFONT record";
            }
            PRINT_DBG("DEFFONT loaded successfully");
        }
        else
        {
            PRINT_DBG("Loading ftindex");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::FTINDEX_RECORD)))
            {
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::FTINDEX_RECORD);
                qSort(offsets);
                for (int i = 0; i < offsets.size(); i++)
                {
                    FtindexRecord ftindexRecord(file, offsets.at(i),
                        this->codec);
                    this->ftindexDTypes.append(ftindexRecord.getDType());
                }
            }
            else
            {
                qDebug() << "No ftindex record";
            }
            PRINT_DBG("Ftindex loaded successfully");
        }
        PRINT_DBG("Loading groups");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::GROUPS_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::GROUPS_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                GroupsRecord groupsRecord(file, offsets.at(i), this->codec);
                this->groups.append(groupsRecord.getGroup());
            }
        }
        else
        {
            qDebug() << "No groups record";
        }
        PRINT_DBG("Groups loaded successfully");
        if (this->sysHeader.getMinor() == SystemHeader::hcw40_format)
        {
            PRINT_DBG("Loading IndexS");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::INDEX_S_RECORD)))
            {
                if (recordOffsets.count(SystemRecordType::INDEX_S_RECORD) > 1)
                {
                    qDebug() << "More than one IndexS record";
                }
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::INDEX_S_RECORD);
                IndexSRecord indexSRecord(file, offsets.at(0), this->codec);
                this->indexSeparators = indexSRecord.getIndexSeparators();
            }
            else
            {
                qDebug() << "No IndexS record";
            }
            PRINT_DBG("IndexS loaded successfully");
        }
        else
        {
            PRINT_DBG("Loading Keyindex");
            if (recordOffsets.contains(
                    static_cast<quint16> (SystemRecordType::KEYINDEX_RECORD)))
            {
                if (recordOffsets.count(SystemRecordType::KEYINDEX_RECORD) > 1)
                {
                    qDebug() << "More than one KEYINDEX record";
                }
                QList<qint64> offsets = recordOffsets.values(
                    SystemRecordType::KEYINDEX_RECORD);
                this->keyindex = KeyindexRecord(file, offsets.at(0),
                    this->codec);
            }
            else
            {
                qDebug() << "No Keyindex record";
            }
            PRINT_DBG("Keyindex loaded successfully");
        }
        PRINT_DBG("Loading language");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::LANGUAGE_RECORD)))
        {
            if (recordOffsets.count(SystemRecordType::LANGUAGE_RECORD) > 1)
            {
                qDebug() << "More than one language record";
            }
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::LANGUAGE_RECORD);
            LanguageRecord languageRecord(file, offsets.at(0), this->codec);
            this->language = languageRecord.getLanguage();
        }
        else
        {
            qDebug() << "No language record";
        }
        PRINT_DBG("Language loaded successfully");
        PRINT_DBG("Loading DLLMaps");
        if (recordOffsets.contains(
                static_cast<quint16> (SystemRecordType::DLLMAPS_RECORD)))
        {
            QList<qint64> offsets = recordOffsets.values(
                SystemRecordType::DLLMAPS_RECORD);
            qSort(offsets);
            for (int i = 0; i < offsets.size(); i++)
            {
                DLLMapsRecord dllmapsRecord(file, offsets.at(i), this->codec);
                this->dllMaps.append(dllmapsRecord);
            }
        }
        else
        {
            qDebug() << "No DLLMaps record";
        }
        PRINT_DBG("DLLMaps loaded successfully");
    }
    PRINT_DBG("WinHelp system file loaded successfully");
}

WinHelpSystemFile::WinHelpSystemFile(const WinHelpSystemFile& rhs) :
    title(rhs.title), copyright(rhs.copyright), startingTopicOffset(
    rhs.startingTopicOffset), startMacros(rhs.startMacros), icon(
    rhs.icon), windowsHpj(rhs.windowsHpj), windowsMvp(rhs.windowsMvp),
    citation(rhs.citation), lcid(rhs.lcid), contentFileName(
    rhs.contentFileName), deffont(rhs.deffont), ftindexDTypes(
    rhs.ftindexDTypes), groups(rhs.groups), indexSeparators(
    rhs.indexSeparators), keyindex(rhs.keyindex), language(
    rhs.language), dllMaps(rhs.dllMaps), sysHeader(
    rhs.sysHeader), codec(rhs.codec)
{
    PRINT_DBG("WinHelp system file copy constructor");
}

QString WinHelpSystemFile::getTitle() const
{
    return title;
}

QString WinHelpSystemFile::getCopyright() const
{
    return copyright;
}

TopicOffset WinHelpSystemFile::getStartingTopicOffset() const
{
    return startingTopicOffset;
}

int WinHelpSystemFile::getStartMacrosCount() const
{
    return startMacros.count();
}

QString WinHelpSystemFile::getStartMacro(int index) const
{
    if ((index < 0) || (index >= startMacros.size()))
    {
        throw new std::out_of_range("Index of start macro is out of range");
    }
    return startMacros.at(index);
}

const WinHelpIcon & WinHelpSystemFile::getIcon() const
{
    return this->icon;
}

int WinHelpSystemFile::getWindowsHpjCount() const
{
    return windowsHpj.count();
}

WindowHpjRecord WinHelpSystemFile::getWindowHpj(int index) const
{
    if ((index < 0) || (index >= windowsHpj.size()))
    {
        throw new std::out_of_range("Index of windowHpj is out of range");
    }
    return windowsHpj.at(index);
}

int WinHelpSystemFile::getWindowsMvpCount() const
{
    return windowsMvp.count();
}

WindowMvpRecord WinHelpSystemFile::getWindowMvp(int index) const
{
    if ((index < 0) || (index >= windowsMvp.size()))
    {
        throw new std::out_of_range("Index of windowMvp is out of range");
    }
    return windowsMvp.at(index);
}

QString WinHelpSystemFile::getCitation() const
{
    return citation;
}

const LCIDRecord &WinHelpSystemFile::getLCID() const
{
    return lcid;
}

QString WinHelpSystemFile::getContentFileName() const
{
    return contentFileName;
}

const DeffontRecord &WinHelpSystemFile::getDeffontRecord() const
{
    return deffont;
}

int WinHelpSystemFile::getFtindexDTypesCount() const
{
    return ftindexDTypes.count();
}

QString WinHelpSystemFile::getFtindexDType(int index) const
{
    if ((index < 0) || (index >= ftindexDTypes.size()))
    {
        throw new std::out_of_range("Index of ftindexDType is out of range");
    }
    return ftindexDTypes.at(index);
}

int WinHelpSystemFile::getGroupsCount() const
{
    return groups.count();
}

QString WinHelpSystemFile::getGroup(int index) const
{
    if ((index < 0) || (index >= groups.size()))
    {
        throw new std::out_of_range("Index of group is out of range");
    }
    return groups.at(index);
}

QString WinHelpSystemFile::getIndexSeparators() const
{
    return indexSeparators;
}

const KeyindexRecord &WinHelpSystemFile::getKeyindex() const
{
    return keyindex;
}

QString WinHelpSystemFile::getLanguage() const
{
    return language;
}

int WinHelpSystemFile::getDLLMapsCount() const
{
    return dllMaps.count();
}

DLLMapsRecord WinHelpSystemFile::getDLLMap(int index) const
{
    if ((index < 0) || (index >= dllMaps.size()))
    {
        throw new std::out_of_range("Index of DLL map is out of range");
    }
    return dllMaps.at(index);
}

QTextCodec *WinHelpSystemFile::getCodec() const
{
    return codec;
}

WinHelpSystemFile & WinHelpSystemFile::operator=(const WinHelpSystemFile & rhs)
{
    PRINT_DBG("WinHelp system file assignment operator");
    if (this != &rhs)
    {
        this->title = rhs.title;
        this->copyright = rhs.copyright;
        this->startingTopicOffset = rhs.startingTopicOffset;
        this->startMacros = rhs.startMacros;
        this->icon = rhs.icon;
        this->windowsHpj = rhs.windowsHpj;
        this->windowsMvp = rhs.windowsMvp;
        this->citation = rhs.citation;
        this->lcid = rhs.lcid;
        this->contentFileName = rhs.contentFileName;
        this->deffont = rhs.deffont;
        this->ftindexDTypes = rhs.ftindexDTypes;
        this->groups = rhs.groups;
        this->indexSeparators = rhs.indexSeparators;
        this->keyindex = rhs.keyindex;
        this->language = rhs.language;
        this->dllMaps = rhs.dllMaps;
        this->sysHeader = rhs.sysHeader;
        this->codec = rhs.codec;
    }
    return *this;
}

WinHelpSystemFile::~WinHelpSystemFile()
{
    PRINT_DBG("WinHelp system file destructor");
}

const SystemHeader & WinHelpSystemFile::getHeader() const
{
    return this->sysHeader;
}
