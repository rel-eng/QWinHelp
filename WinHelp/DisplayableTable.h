/* Displayable table class definition.

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

#ifndef DISPLAYABLETABLE_H_
#define DISPLAYABLETABLE_H_

#include <QtGlobal>
#include <QList>
#include <QByteArray>
#include <QSharedPointer>
#include <QTextCodec>

#include "TopicOffset.h"
#include "FormattingCommands.h"

#include "DisplayableText.h"

#include <cstddef>

class DisplayableTable : public DisplayableText
{
private:

    class BorderInfo
    {
public:
        bool isBorderBox;
        bool isBorderTop;
        bool isBorderLeft;
        bool isBorderBottom;
        bool isBorderRight;
        bool isBorderThick;
        bool isBorderDouble;
        qint16 borderWidth;
        BorderInfo() : isBorderBox(false), isBorderTop(false),
            isBorderLeft(false), isBorderBottom(false), isBorderRight(false),
            isBorderThick(false), isBorderDouble(false), borderWidth(0)
        {
        };
        BorderInfo(const BorderInfo &rhs) : isBorderBox(rhs.isBorderBox),
            isBorderTop(rhs.isBorderTop), isBorderLeft(rhs.isBorderLeft),
            isBorderBottom(rhs.isBorderBottom), isBorderRight(rhs.isBorderRight),
            isBorderThick(rhs.isBorderThick), isBorderDouble(rhs.isBorderDouble),
            borderWidth(rhs.borderWidth)
        {
        };
        BorderInfo &operator=(const BorderInfo &rhs)
        {
            if (this != &rhs)
            {
                this->isBorderBox = rhs.isBorderBox;
                this->isBorderTop = rhs.isBorderTop;
                this->isBorderLeft = rhs.isBorderLeft;
                this->isBorderBottom = rhs.isBorderBottom;
                this->isBorderRight = rhs.isBorderRight;
                this->isBorderThick = rhs.isBorderThick;
                this->isBorderDouble = rhs.isBorderDouble;
                this->borderWidth = rhs.borderWidth;
            }
            return *this;
        };
        ~BorderInfo()
        {
        };
    };

    class Tab
    {
public:
        quint16 tabStop;
        bool isTabTypePresent;
        quint16 tabType;
        Tab() : tabStop(0), isTabTypePresent(false), tabType(0)
        {
        };
        Tab(const Tab &rhs) : tabStop(rhs.tabStop),
            isTabTypePresent(rhs.isTabTypePresent), tabType(rhs.tabType)
        {
        };
        Tab &operator=(const Tab &rhs)
        {
            if (this != &rhs)
            {
                this->tabStop = rhs.tabStop;
                this->isTabTypePresent = rhs.isTabTypePresent;
                this->tabType = rhs.tabType;
            }
            return *this;
        };
        ~Tab()
        {
        };
    };

    class TabInfo
    {
public:
        qint16 numberOfTabStops;
        QList<Tab> tabs;
        TabInfo() : numberOfTabStops(0), tabs()
        {
        };
        TabInfo(const TabInfo &rhs) : numberOfTabStops(rhs.numberOfTabStops),
            tabs(rhs.tabs)
        {
        };
        TabInfo &operator=(const TabInfo &rhs)
        {
            if (this != &rhs)
            {
                this->numberOfTabStops = rhs.numberOfTabStops;
                this->tabs = rhs.tabs;
            }
            return *this;
        };
        ~TabInfo()
        {
        };
    };

    class ColumnInfo
    {
public:
        qint16 gapWidth;
        qint16 colWidth;
        ColumnInfo() : gapWidth(0), colWidth(0)
        {
        };
        ColumnInfo(const ColumnInfo &rhs) : gapWidth(rhs.gapWidth),
            colWidth(rhs.colWidth)
        {
        };
        ColumnInfo &operator=(const ColumnInfo &rhs)
        {
            if (this != &rhs)
            {
                this->gapWidth = rhs.gapWidth;
                this->colWidth = rhs.colWidth;
            }
            return *this;
        };
        ~ColumnInfo()
        {
        };
    };

    class ParagraphInfo
    {
public:
        qint16 column;
        quint16 id;
        bool isSpacingAbovePresent;
        bool isSpacingBelowPresent;
        bool isSpacingLinesPresent;
        bool isLeftIndentPresent;
        bool isRightIndentPresent;
        bool isFirstlineIndentPresent;
        bool isBorderinfoPresent;
        bool isTabinfoPresent;
        bool isRightAlignedParagraph;
        bool isCenterAlignedParagraph;
        qint16 spacingAbove;
        qint16 spacingBelow;
        qint16 spacingLines;
        qint16 leftIndent;
        qint16 rightIndent;
        qint16 firstlineIndent;
        BorderInfo borderInfo;
        TabInfo tabInfo;
        QList<QSharedPointer<FormattingCommand> > commands;
        ParagraphInfo() : column(0), id(0), isSpacingAbovePresent(false),
            isSpacingBelowPresent(false), isSpacingLinesPresent(false),
            isLeftIndentPresent(false), isRightIndentPresent(false),
            isFirstlineIndentPresent(false), isBorderinfoPresent(false),
            isTabinfoPresent(false), isRightAlignedParagraph(false),
            isCenterAlignedParagraph(false), spacingAbove(0), spacingBelow(0),
            spacingLines(0), leftIndent(0), rightIndent(0), firstlineIndent(0),
            borderInfo(), tabInfo(), commands()
        {
        };
        ParagraphInfo(const ParagraphInfo &rhs) : column(rhs.column), id(rhs.id),
            isSpacingAbovePresent(rhs.isSpacingAbovePresent),
            isSpacingBelowPresent(rhs.isSpacingBelowPresent),
            isSpacingLinesPresent(rhs.isSpacingLinesPresent),
            isLeftIndentPresent(rhs.isLeftIndentPresent),
            isRightIndentPresent(rhs.isRightIndentPresent),
            isFirstlineIndentPresent(rhs.isFirstlineIndentPresent),
            isBorderinfoPresent(rhs.isBorderinfoPresent),
            isTabinfoPresent(rhs.isTabinfoPresent),
            isRightAlignedParagraph(rhs.isRightAlignedParagraph),
            isCenterAlignedParagraph(rhs.isCenterAlignedParagraph),
            spacingAbove(rhs.spacingAbove), spacingBelow(rhs.spacingBelow),
            spacingLines(rhs.spacingLines), leftIndent(rhs.leftIndent),
            rightIndent(rhs.rightIndent), firstlineIndent(rhs.firstlineIndent),
            borderInfo(rhs.borderInfo), tabInfo(rhs.tabInfo),
            commands(rhs.commands)
        {
        };
        ParagraphInfo &operator=(const ParagraphInfo &rhs)
        {
            if (this != &rhs)
            {
                this->column = rhs.column;
                this->id = rhs.id;
                this->isSpacingAbovePresent = rhs.isSpacingAbovePresent;
                this->isSpacingBelowPresent = rhs.isSpacingBelowPresent;
                this->isSpacingLinesPresent = rhs.isSpacingLinesPresent;
                this->isLeftIndentPresent = rhs.isRightIndentPresent;
                this->isRightIndentPresent = rhs.isRightIndentPresent;
                this->isFirstlineIndentPresent = rhs.isFirstlineIndentPresent;
                this->isBorderinfoPresent = rhs.isBorderinfoPresent;
                this->isTabinfoPresent = rhs.isTabinfoPresent;
                this->isRightAlignedParagraph = rhs.isRightAlignedParagraph;
                this->isCenterAlignedParagraph = rhs.isCenterAlignedParagraph;
                this->spacingAbove = rhs.spacingAbove;
                this->spacingBelow = rhs.spacingBelow;
                this->spacingLines = rhs.spacingLines;
                this->leftIndent = rhs.leftIndent;
                this->rightIndent = rhs.rightIndent;
                this->firstlineIndent = rhs.firstlineIndent;
                this->borderInfo = rhs.borderInfo;
                this->tabInfo = rhs.tabInfo;
                this->commands = rhs.commands;
            }
            return *this;
        };
        ~ParagraphInfo()
        {
        };
    };

    qint64 topicSize;
    qint64 topicLength;
    quint8 numberOfColumns;
    quint8 tableType;
    qint16 minTableWidth;
    QList<ColumnInfo> columns;
    QList<ParagraphInfo> paragraphs;
    QList<QString> texts;
public:
    DisplayableTable();
    DisplayableTable(const void *src,
        size_t srcSize,
        const void *textSrc,
        size_t textSize,
        QTextCodec *codec);
    DisplayableTable(const DisplayableTable& rhs);
    virtual ~DisplayableTable();
    DisplayableTable & operator=(const DisplayableTable & rhs);
    virtual QList<TopicOffset> getHyperlinks() const;
    virtual QString getHTML(bool &empty) const;
};

#endif /* DISPLAYABLETABLE_H_ */
