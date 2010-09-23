/* Displayable text new version class implementation.

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

#include "DisplayableTextNew.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

DisplayableTextNew::DisplayableTextNew() : DisplayableText(0x20),
    topicSize(Q_INT64_C(0)), topicLength(Q_INT64_C(0)), paragraphs(), texts()
{
    PRINT_DBG("Displayable text new default constructor");
}

DisplayableTextNew::DisplayableTextNew(const void *src,
    size_t srcSize,
    const void *textSrc,
    size_t textSize,
    QTextCodec *codec) : DisplayableText(0x20), paragraphs(), texts()
{
    if(codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    PRINT_DBG("Loading displayable text new");
    if((srcSize > static_cast<size_t>(0)) && (src != NULL))
    {
        size_t bytesRead = static_cast<size_t>(0);
        size_t offset = static_cast<size_t>(0);
        this->topicSize =
            static_cast<qint64>(getCompressedSignedDWord(src, offset, srcSize,
                bytesRead));
        offset += bytesRead;
        PRINT_DBG("        Topic size: %lld", this->topicSize);
        topicLength =
            static_cast<qint64>(getCompressedUnsignedWord(src, offset, srcSize,
                bytesRead));
        offset += bytesRead;
        PRINT_DBG("        Topic length: %lld", topicLength);
        ParagraphInfo paragraphInfo;
        checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
        quint8 unknownUnsignedByte =
            getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<
                    const
                    quint8 *>(src) + offset));
        offset += static_cast<size_t>(1);
        PRINT_DBG("        Unknown unsigned byte: %d", unknownUnsignedByte);
        checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
        qint8 unknownBiasedByte =
            getSignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                    quint8 *>(src) + offset));
        offset += static_cast<size_t>(1);
        PRINT_DBG("        Unknown biased byte: %d", unknownBiasedByte);
        checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
        paragraphInfo.id =
            getUnsignedWord(reinterpret_cast<const void *>(reinterpret_cast<
                    const
                    quint8 *>(src) + offset));
        offset += static_cast<size_t>(2);
        PRINT_DBG("        Id: %d", paragraphInfo.id);
        checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
        quint8 flagsL =
            getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<
                    const
                    quint8 *>(src) + offset));
        offset += static_cast<size_t>(1);
        quint8 flagsH =
            getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<
                    const
                    quint8 *>(src) + offset));
        offset += static_cast<size_t>(1);
        PRINT_DBG("        Flags low byte: %d", flagsL);
        PRINT_DBG("        Flags high byte: %d", flagsH);
        bool isUnknownPresent = false;
        if((flagsL & 0x01) != 0)
        {
            isUnknownPresent = true;
            PRINT_DBG("        IsUnknownPresent flag set");
        }
        paragraphInfo.isSpacingAbovePresent = false;
        if((flagsL & 0x02) != 0)
        {
            paragraphInfo.isSpacingAbovePresent = true;
            PRINT_DBG("        IsSpacingAbovePresent flag set");
        }
        paragraphInfo.isSpacingBelowPresent = false;
        if((flagsL & 0x04) != 0)
        {
            paragraphInfo.isSpacingBelowPresent = true;
            PRINT_DBG("        IsSpacingBelowPresent flag set");
        }
        paragraphInfo.isSpacingLinesPresent = false;
        if((flagsL & 0x08) != 0)
        {
            paragraphInfo.isSpacingLinesPresent = true;
            PRINT_DBG("        IsSpacingLinesPresent flag set");
        }
        paragraphInfo.isLeftIndentPresent = false;
        if((flagsL & 0x10) != 0)
        {
            paragraphInfo.isLeftIndentPresent = true;
            PRINT_DBG("        IsLeftIndentPresent flag set");
        }
        paragraphInfo.isRightIndentPresent = false;
        if((flagsL & 0x20) != 0)
        {
            paragraphInfo.isRightIndentPresent = true;
            PRINT_DBG("        IsRightIndentPresent flag set");
        }
        paragraphInfo.isFirstlineIndentPresent = false;
        if((flagsL & 0x40) != 0)
        {
            paragraphInfo.isFirstlineIndentPresent = true;
            PRINT_DBG("        IsFirstLineIndentPresent flag set");
        }
        bool unusedFlag = false;
        if((flagsL & 0x80) != 0)
        {
            unusedFlag = true;
            PRINT_DBG("        Unused flag set");
        }
        paragraphInfo.isBorderinfoPresent = false;
        if((flagsH & 0x01) != 0)
        {
            paragraphInfo.isBorderinfoPresent = true;
            PRINT_DBG("        IsBorderinfoPresent flag set");
        }
        paragraphInfo.isTabinfoPresent = false;
        if((flagsH & 0x02) != 0)
        {
            paragraphInfo.isTabinfoPresent = true;
            PRINT_DBG("        IsTabinfoPresent flag set");
        }
        paragraphInfo.isRightAlignedParagraph = false;
        if((flagsH & 0x04) != 0)
        {
            paragraphInfo.isRightAlignedParagraph = true;
            PRINT_DBG("        IsRightAlignedParagraph flag set");
        }
        paragraphInfo.isCenterAlignedParagraph = false;
        if((flagsH & 0x08) != 0)
        {
            paragraphInfo.isCenterAlignedParagraph = true;
            PRINT_DBG("        IsCenterAlignedParagraph flag set");
        }
        if(isUnknownPresent)
        {
            qint32 unknownSignedDWord = getCompressedSignedDWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Unknown signed dword: %d", unknownSignedDWord);
        }
        if(paragraphInfo.isSpacingAbovePresent)
        {
            paragraphInfo.spacingAbove = getCompressedSignedWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Spacing above: %d", paragraphInfo.spacingAbove);
        }
        else
        {
            paragraphInfo.spacingAbove = 0;
        }
        if(paragraphInfo.isSpacingBelowPresent)
        {
            paragraphInfo.spacingBelow = getCompressedSignedWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Spacing below: %d", paragraphInfo.spacingBelow);
        }
        else
        {
            paragraphInfo.spacingBelow = 0;
        }
        if(paragraphInfo.isSpacingLinesPresent)
        {
            paragraphInfo.spacingLines = getCompressedSignedWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Spacing lines: %d", paragraphInfo.spacingLines);
        }
        else
        {
            paragraphInfo.spacingLines = 0;
        }
        if(paragraphInfo.isLeftIndentPresent)
        {
            paragraphInfo.leftIndent = getCompressedSignedWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Left indent: %d", paragraphInfo.leftIndent);
        }
        else
        {
            paragraphInfo.leftIndent = 0;
        }
        if(paragraphInfo.isRightIndentPresent)
        {
            paragraphInfo.rightIndent = getCompressedSignedWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Right indent: %d", paragraphInfo.rightIndent);
        }
        else
        {
            paragraphInfo.rightIndent = 0;
        }
        if(paragraphInfo.isFirstlineIndentPresent)
        {
            paragraphInfo.firstlineIndent = getCompressedSignedWord(src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Firstline indent: %d",
                paragraphInfo.firstlineIndent);
        }
        else
        {
            paragraphInfo.firstlineIndent = 0;
        }
        if(paragraphInfo.isBorderinfoPresent)
        {
            checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
            quint16 borderFlags =
                getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast
                        <
                        const quint8 *>(src) + offset));
            offset += static_cast<size_t>(1);
            PRINT_DBG("        Border flags byte: %d", borderFlags);
            paragraphInfo.borderInfo.isBorderBox = false;
            if((borderFlags & 0x01) != 0)
            {
                paragraphInfo.borderInfo.isBorderBox = true;
                PRINT_DBG("        IsBorderBox flag set");
            }
            paragraphInfo.borderInfo.isBorderTop = false;
            if((borderFlags & 0x02) != 0)
            {
                paragraphInfo.borderInfo.isBorderTop = true;
                PRINT_DBG("        IsBorderTop flag set");
            }
            paragraphInfo.borderInfo.isBorderLeft = false;
            if((borderFlags & 0x04) != 0)
            {
                paragraphInfo.borderInfo.isBorderLeft = true;
                PRINT_DBG("        IsBorderLeft flag set");
            }
            paragraphInfo.borderInfo.isBorderBottom = false;
            if((borderFlags & 0x08) != 0)
            {
                paragraphInfo.borderInfo.isBorderBottom = true;
                PRINT_DBG("        IsBorderBottom flag set");
            }
            paragraphInfo.borderInfo.isBorderRight = false;
            if((borderFlags & 0x10) != 0)
            {
                paragraphInfo.borderInfo.isBorderRight = true;
                PRINT_DBG("        IsBorderRight flag set");
            }
            paragraphInfo.borderInfo.isBorderThick = false;
            if((borderFlags & 0x20) != 0)
            {
                paragraphInfo.borderInfo.isBorderThick = true;
                PRINT_DBG("        IsBorderThick flag set");
            }
            paragraphInfo.borderInfo.isBorderDouble = false;
            if((borderFlags & 0x40) != 0)
            {
                paragraphInfo.borderInfo.isBorderDouble = true;
                PRINT_DBG("        IsBorderDouble flag set");
            }
            bool unusedBorderFlag = false;
            if((borderFlags & 0x80) != 0)
            {
                unusedBorderFlag = true;
                PRINT_DBG("        Unused border flag set");
            }
            checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
            paragraphInfo.borderInfo.borderWidth = getSignedWord(
                reinterpret_cast<const void *>(reinterpret_cast<const quint8 *>(
                        src)
                    + offset));
            offset += static_cast<size_t>(2);
            PRINT_DBG("        Border width: %d",
                paragraphInfo.borderInfo.borderWidth);
        }
        else
        {
            paragraphInfo.borderInfo.isBorderBox = false;
            paragraphInfo.borderInfo.isBorderTop = false;
            paragraphInfo.borderInfo.isBorderLeft = false;
            paragraphInfo.borderInfo.isBorderBottom = false;
            paragraphInfo.borderInfo.isBorderRight = false;
            paragraphInfo.borderInfo.isBorderThick = false;
            paragraphInfo.borderInfo.isBorderDouble = false;
            paragraphInfo.borderInfo.borderWidth = 0;
        }
        if(paragraphInfo.isTabinfoPresent)
        {
            paragraphInfo.tabInfo.numberOfTabStops = getCompressedSignedWord(
                src,
                offset,
                srcSize,
                bytesRead);
            offset += bytesRead;
            PRINT_DBG("        Number of tab stops: %d",
                paragraphInfo.tabInfo.numberOfTabStops);
            for(qint16 i = 0; i < paragraphInfo.tabInfo.numberOfTabStops; i++)
            {
                Tab tab;
                quint16 tabstop = getCompressedUnsignedWord(src,
                    offset,
                    srcSize,
                    bytesRead);
                offset += bytesRead;
                tab.tabStop = tabstop % 0x4000;
                PRINT_DBG("        Tab stop: %d", tab.tabStop);
                if((tabstop % 0x8000) - (tabstop % 0x4000) != 0)
                {
                    tab.isTabTypePresent = true;
                    tab.tabType = getCompressedUnsignedWord(src,
                        offset,
                        srcSize,
                        bytesRead);
                    offset += bytesRead;
                    PRINT_DBG("        IsTabTypePresent flag set");
                    PRINT_DBG("        Tab stop: %d", tab.tabType);
                }
                else
                {
                    tab.isTabTypePresent = false;
                    tab.tabType = 0;
                }
                paragraphInfo.tabInfo.tabs.append(tab);
            }
        }
        else
        {
            paragraphInfo.tabInfo.numberOfTabStops = 0;
        }
        PRINT_DBG("        Reading formatting commands:");
        bool hasMoreCommands = true;
        while(hasMoreCommands)
        {
            checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
            quint8 commandType =
                getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast
                        <
                        const quint8 *>(src) + offset));
            offset += static_cast<size_t>(1);
            switch(commandType)
            {
            case END_OF_CHARACTER_FORMATTING:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                EndOfCharacterFormatting())));
                hasMoreCommands = false;
                PRINT_DBG("        End of character formatting command");
            }
            break;

            case VFLD_NUMBER:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(4), srcSize);
                qint32 vfldNumber =
                        getSignedDWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(4);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                VFLDNumberCommand(
                                    vfldNumber))));
                PRINT_DBG("        VFLD number command");
                PRINT_DBG("        VFLD number: %d", vfldNumber);
            }
            break;

            case DTYPE_NUMBER:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                qint16 dtypeNumber =
                        getSignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                DTypeNumberCommand(
                                    dtypeNumber))));
                PRINT_DBG("        DType number command");
                PRINT_DBG("        DType number: %d", dtypeNumber);
            }
            break;

            case FONT_NUMBER:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                qint16 fontNumber =
                        getSignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                FontNumberCommand(
                                    fontNumber))));
                PRINT_DBG("        Font number command");
                PRINT_DBG("        Font number: %d", fontNumber);
            }
            break;

            case LINE_BREAK:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                LineBreakCommand())));
                PRINT_DBG("        Line break command");
            }
            break;

            case END_OF_PARAGRAPH:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                EndOfParagraphCommand())));
                PRINT_DBG("        End of paragraph command");
            }
            break;

            case TAB_COMMAND:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new TABCommand())));
                PRINT_DBG("        Tab command");
            }
            break;

            case TYPE_86:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                qint32 pictureSize = getCompressedSignedDWord(src,
                        offset,
                        srcSize,
                        bytesRead);
                offset += bytesRead;
                switch(dataType)
                {
                case EMBEDDED_PICTURE_DATA_TYPE_3:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 pictureIsEmbedded =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureIsEmbedded == 0)
                    {
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(2),
                                    srcSize);
                        qint16 pictureNumber =
                                    getSignedWord(reinterpret_cast<const void *>(
                                        reinterpret_cast<const quint8 *>(src) +
                                        offset));
                        offset += static_cast<size_t>(2);
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type86Subtype3Command(
                                                pictureSize, pictureIsEmbedded,
                                                pictureNumber,
                                                QByteArray()))));
                        PRINT_DBG("        Type 0x86 command subtype 3");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                        PRINT_DBG("        Picture number: %d", pictureNumber);
                    }
                    else
                    {
                        if(pictureSize <= 2)
                        {
                            throw std::runtime_error(
                                        "Embedded picture is too small");
                        }
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(pictureSize - 2),
                                    srcSize);
                        QScopedArrayPointer<quint8> image(
                                    new quint8[static_cast<size_t> (pictureSize
                                            -
                                            2)]);
                        offset += copyBytesFromBuffer(
                                    src,
                                    srcSize,
                                    offset,
                                    static_cast<size_t> (pictureSize - 2),
                                    reinterpret_cast<void *> (image.data()),
                                    static_cast<size_t> (pictureSize - 2));
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type86Subtype3Command(
                                                pictureSize, pictureIsEmbedded,
                                                0,
                                                QByteArray(reinterpret_cast<
                                                        const char *>(image.
                                                        data()),
                                                    static_cast<size_t>(
                                                        pictureSize - 2))))));
                        PRINT_DBG("        Type 0x86 command subtype 3");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                    }
                }
                break;

                case EMBEDDED_OBJECT_DATA:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown1 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown2 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown3 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureSize <= 6)
                    {
                        throw std::runtime_error(
                                    "Embedded picture is too small");
                    }
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(pictureSize - 6), srcSize);
                    QString embedded = readFixedLengthStringFromBuffer(src,
                                srcSize,
                                offset,
                                static_cast<uint>(pictureSize - 6),
                                codec);
                    offset += static_cast<size_t>(pictureSize - 6);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new Type86Subtype5Command(pictureSize,
                                            embedded))));
                    PRINT_DBG("        Type 0x86 command subtype 5");
                    PRINT_DBG("        Picture size: %d", pictureSize);
                    PRINT_DBG("        Unknown1: %d", unknown1);
                    PRINT_DBG("        Unknown2: %d", unknown2);
                    PRINT_DBG("        Unknown3: %d", unknown3);
                    PRINT_DBG("        Embedded: %s",
                                embedded.toLocal8Bit().data());

                }
                break;

                case EMBEDDED_PICTURE_DATA_TYPE_22:
                {
                    qint16 numberOfHotspots = getCompressedUnsignedWord(src,
                                offset,
                                srcSize,
                                bytesRead);
                    offset += bytesRead;
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 pictureIsEmbedded =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureIsEmbedded == 0)
                    {
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(2),
                                    srcSize);
                        qint16 pictureNumber =
                                    getSignedWord(reinterpret_cast<const void *>(
                                        reinterpret_cast<const quint8 *>(src) +
                                        offset));
                        offset += static_cast<size_t>(2);
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type86Subtype22Command(
                                                pictureSize, numberOfHotspots,
                                                pictureIsEmbedded,
                                                pictureNumber, QByteArray()))));
                        PRINT_DBG("        Type 0x86 command subtype 22");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Number of hotspots: %d",
                                    numberOfHotspots);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                        PRINT_DBG("        Picture number: %d", pictureNumber);
                    }
                    else
                    {
                        if(pictureSize <= 2)
                        {
                            throw std::runtime_error(
                                        "Embedded picture is too small");
                        }
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(pictureSize - 2),
                                    srcSize);
                        QScopedArrayPointer<quint8> image(
                                    new quint8[static_cast<size_t> (pictureSize
                                            -
                                            2)]);
                        offset += copyBytesFromBuffer(
                                    src,
                                    srcSize,
                                    offset,
                                    static_cast<size_t> (pictureSize - 2),
                                    reinterpret_cast<void *> (image.data()),
                                    static_cast<size_t> (pictureSize - 2));
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type86Subtype22Command(
                                                pictureSize, numberOfHotspots,
                                                pictureIsEmbedded,
                                                0,
                                                QByteArray(reinterpret_cast<
                                                        const char *>(image.
                                                        data()),
                                                    static_cast<size_t>(
                                                        pictureSize - 2))))));
                        PRINT_DBG("        Type 0x86 command subtype 22");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Number of hotspots: %d",
                                    numberOfHotspots);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                    }
                }
                break;

                default:
                    throw std::runtime_error("Unknown picture data type");
                }
            }
            break;

            case TYPE_87:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                qint32 pictureSize = getCompressedSignedDWord(src,
                        offset,
                        srcSize,
                        bytesRead);
                offset += bytesRead;
                switch(dataType)
                {
                case EMBEDDED_PICTURE_DATA_TYPE_3:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 pictureIsEmbedded =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureIsEmbedded == 0)
                    {
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(2),
                                    srcSize);
                        qint16 pictureNumber =
                                    getSignedWord(reinterpret_cast<const void *>(
                                        reinterpret_cast<const quint8 *>(src) +
                                        offset));
                        offset += static_cast<size_t>(2);
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type87Subtype3Command(
                                                pictureSize, pictureIsEmbedded,
                                                pictureNumber,
                                                QByteArray()))));
                        PRINT_DBG("        Type 0x87 command subtype 3");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                        PRINT_DBG("        Picture number: %d", pictureNumber);
                    }
                    else
                    {
                        if(pictureSize <= 2)
                        {
                            throw std::runtime_error(
                                        "Embedded picture is too small");
                        }
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(pictureSize - 2),
                                    srcSize);
                        QScopedArrayPointer<quint8> image(
                                    new quint8[static_cast<size_t> (pictureSize
                                            -
                                            2)]);
                        offset += copyBytesFromBuffer(
                                    src,
                                    srcSize,
                                    offset,
                                    static_cast<size_t> (pictureSize - 2),
                                    reinterpret_cast<void *> (image.data()),
                                    static_cast<size_t> (pictureSize - 2));
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type87Subtype3Command(
                                                pictureSize, pictureIsEmbedded,
                                                0,
                                                QByteArray(reinterpret_cast<
                                                        const char *>(image.
                                                        data()),
                                                    static_cast<size_t>(
                                                        pictureSize - 2))))));
                        PRINT_DBG("        Type 0x87 command subtype 3");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                    }
                }
                break;

                case EMBEDDED_OBJECT_DATA:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown1 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown2 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown3 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureSize <= 6)
                    {
                        throw std::runtime_error(
                                    "Embedded picture is too small");
                    }
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(pictureSize - 6), srcSize);
                    QString embedded = readFixedLengthStringFromBuffer(src,
                                srcSize,
                                offset,
                                static_cast<uint>(pictureSize - 6),
                                codec);
                    offset += static_cast<size_t>(pictureSize - 6);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new Type87Subtype5Command(pictureSize,
                                            embedded))));
                    PRINT_DBG("        Type 0x87 command subtype 5");
                    PRINT_DBG("        Picture size: %d", pictureSize);
                    PRINT_DBG("        Unknown1: %d", unknown1);
                    PRINT_DBG("        Unknown2: %d", unknown2);
                    PRINT_DBG("        Unknown3: %d", unknown3);
                    PRINT_DBG("        Embedded: %s",
                                embedded.toLocal8Bit().data());

                }
                break;

                case EMBEDDED_PICTURE_DATA_TYPE_22:
                {
                    qint16 numberOfHotspots = getCompressedUnsignedWord(src,
                                offset,
                                srcSize,
                                bytesRead);
                    offset += bytesRead;
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 pictureIsEmbedded =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureIsEmbedded == 0)
                    {
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(2),
                                    srcSize);
                        qint16 pictureNumber =
                                    getSignedWord(reinterpret_cast<const void *>(
                                        reinterpret_cast<const quint8 *>(src) +
                                        offset));
                        offset += static_cast<size_t>(2);
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type87Subtype22Command(
                                                pictureSize, numberOfHotspots,
                                                pictureIsEmbedded,
                                                pictureNumber, QByteArray()))));
                        PRINT_DBG("        Type 0x87 command subtype 22");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Number of hotspots: %d",
                                    numberOfHotspots);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                        PRINT_DBG("        Picture number: %d", pictureNumber);
                    }
                    else
                    {
                        if(pictureSize <= 2)
                        {
                            throw std::runtime_error(
                                        "Embedded picture is too small");
                        }
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(pictureSize - 2),
                                    srcSize);
                        QScopedArrayPointer<quint8> image(
                                    new quint8[static_cast<size_t> (pictureSize
                                            -
                                            2)]);
                        offset += copyBytesFromBuffer(
                                    src,
                                    srcSize,
                                    offset,
                                    static_cast<size_t> (pictureSize - 2),
                                    reinterpret_cast<void *> (image.data()),
                                    static_cast<size_t> (pictureSize - 2));
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type87Subtype22Command(
                                                pictureSize, numberOfHotspots,
                                                pictureIsEmbedded,
                                                0,
                                                QByteArray(reinterpret_cast<
                                                        const char *>(image.
                                                        data()),
                                                    static_cast<size_t>(
                                                        pictureSize - 2))))));
                        PRINT_DBG("        Type 0x87 command subtype 22");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Number of hotspots: %d",
                                    numberOfHotspots);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                    }
                }
                break;

                default:
                    throw std::runtime_error("Unknown picture data type");
                }
            }
            break;

            case TYPE_88:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                qint32 pictureSize = getCompressedSignedDWord(src,
                        offset,
                        srcSize,
                        bytesRead);
                offset += bytesRead;
                switch(dataType)
                {
                case EMBEDDED_PICTURE_DATA_TYPE_3:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 pictureIsEmbedded =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureIsEmbedded == 0)
                    {
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(2),
                                    srcSize);
                        qint16 pictureNumber =
                                    getSignedWord(reinterpret_cast<const void *>(
                                        reinterpret_cast<const quint8 *>(src) +
                                        offset));
                        offset += static_cast<size_t>(2);
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type88Subtype3Command(
                                                pictureSize, pictureIsEmbedded,
                                                pictureNumber,
                                                QByteArray()))));
                        PRINT_DBG("        Type 0x88 command subtype 3");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                        PRINT_DBG("        Picture number: %d", pictureNumber);
                    }
                    else
                    {
                        if(pictureSize <= 2)
                        {
                            throw std::runtime_error(
                                        "Embedded picture is too small");
                        }
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(pictureSize /* - 4*/),
                                    srcSize);
                        QScopedArrayPointer<quint8> image(
                                    new quint8[static_cast<size_t> (pictureSize /* - 4*/)
                                    ]);
                        offset += copyBytesFromBuffer(
                                    src,
                                    srcSize,
                                    offset,
                                    static_cast<size_t> (pictureSize - 2),
                                    reinterpret_cast<void *> (image.data()),
                                    static_cast<size_t> (pictureSize - 2));
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type88Subtype3Command(
                                                pictureSize, pictureIsEmbedded,
                                                0,
                                                QByteArray(reinterpret_cast<
                                                        const char *>(image.
                                                        data()),
                                                    static_cast<size_t>(
                                                        pictureSize - 2))))));
                        PRINT_DBG("        Type 0x88 command subtype 3");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                    }
                }
                break;

                case EMBEDDED_OBJECT_DATA:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown1 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown2 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 unknown3 =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureSize <= 6)
                    {
                        throw std::runtime_error(
                                    "Embedded picture is too small");
                    }
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(pictureSize - 6), srcSize);
                    QString embedded = readFixedLengthStringFromBuffer(src,
                                srcSize,
                                offset,
                                static_cast<uint>(pictureSize - 6),
                                codec);
                    offset += static_cast<size_t>(pictureSize - 6);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new Type88Subtype5Command(pictureSize,
                                            embedded))));
                    PRINT_DBG("        Type 0x88 command subtype 5");
                    PRINT_DBG("        Picture size: %d", pictureSize);
                    PRINT_DBG("        Unknown1: %d", unknown1);
                    PRINT_DBG("        Unknown2: %d", unknown2);
                    PRINT_DBG("        Unknown3: %d", unknown3);
                    PRINT_DBG("        Embedded: %s",
                                embedded.toLocal8Bit().data());

                }
                break;

                case EMBEDDED_PICTURE_DATA_TYPE_22:
                {
                    qint16 numberOfHotspots = getCompressedUnsignedWord(src,
                                offset,
                                srcSize,
                                bytesRead);
                    offset += bytesRead;
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(2),
                                srcSize);
                    qint16 pictureIsEmbedded =
                                getSignedWord(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(2);
                    if(pictureIsEmbedded == 0)
                    {
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(2),
                                    srcSize);
                        qint16 pictureNumber =
                                    getSignedWord(reinterpret_cast<const void *>(
                                        reinterpret_cast<const quint8 *>(src) +
                                        offset));
                        offset += static_cast<size_t>(2);
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type88Subtype22Command(
                                                pictureSize, numberOfHotspots,
                                                pictureIsEmbedded,
                                                pictureNumber, QByteArray()))));
                        PRINT_DBG("        Type 0x88 command subtype 22");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Number of hotspots: %d",
                                    numberOfHotspots);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                        PRINT_DBG("        Picture number: %d", pictureNumber);
                    }
                    else
                    {
                        if(pictureSize <= 2)
                        {
                            throw std::runtime_error(
                                        "Embedded picture is too small");
                        }
                        checkOffsetLengthSize(offset,
                                    static_cast<size_t>(pictureSize - 2),
                                    srcSize);
                        QScopedArrayPointer<quint8> image(
                                    new quint8[static_cast<size_t> (pictureSize
                                            -
                                            2)]);
                        offset += copyBytesFromBuffer(
                                    src,
                                    srcSize,
                                    offset,
                                    static_cast<size_t> (pictureSize - 2),
                                    reinterpret_cast<void *> (image.data()),
                                    static_cast<size_t> (pictureSize - 2));
                        paragraphInfo.commands.append(QSharedPointer<
                                        FormattingCommand>(dynamic_cast<
                                            FormattingCommand *>(new
                                            Type88Subtype22Command(
                                                pictureSize, numberOfHotspots,
                                                pictureIsEmbedded,
                                                0,
                                                QByteArray(reinterpret_cast<
                                                        const char *>(image.
                                                        data()),
                                                    static_cast<size_t>(
                                                        pictureSize - 2))))));
                        PRINT_DBG("        Type 0x88 command subtype 22");
                        PRINT_DBG("        Picture size: %d", pictureSize);
                        PRINT_DBG("        Number of hotspots: %d",
                                    numberOfHotspots);
                        PRINT_DBG("        Picture is embedded: %d",
                                    pictureIsEmbedded);
                    }
                }
                break;

                default:
                    throw std::runtime_error("Unknown picture data type");
                }
            }
            break;

            case END_OF_HOTSPOT:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                EndOfHotspotCommand())));
                PRINT_DBG("        End of hotspot command");
            }
            break;

            case NON_BREAK_SPACE:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                NonBreakSpaceCommand())));
                PRINT_DBG("        Non break space command");
            }
            break;

            case NON_BREAK_HYPHEN:
            {
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                NonBreakHyphenCommand())));
                PRINT_DBG("        End of paragraph command");
            }
            break;

            case MACRO_COMMAND:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                quint16 macroLength =
                        getUnsignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(macroLength),
                        srcSize);
                QString macroString = readFixedLengthStringFromBuffer(src,
                        srcSize,
                        offset,
                        static_cast<uint>(macroLength),
                        codec);
                offset += static_cast<size_t>(macroLength);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new MacroCommand(
                                    macroLength, macroString))));
                PRINT_DBG("        Macro command");
                PRINT_DBG("        Macro length: %d", macroLength);
                PRINT_DBG("        Macro: %s", macroString.toLocal8Bit().data());
            }
            break;

            case MACRO_WITHOUT_FONT_CHANGE:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                quint16 macroLength =
                        getUnsignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(macroLength),
                        srcSize);
                QString macroString = readFixedLengthStringFromBuffer(src,
                        srcSize,
                        offset,
                        static_cast<uint>(macroLength),
                        codec);
                offset += static_cast<size_t>(macroLength);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                MacroWithoutFontChangeCommand(macroLength,
                                    macroString))));
                PRINT_DBG("        Macro without font change command");
                PRINT_DBG("        Macro length: %d", macroLength);
                PRINT_DBG("        Macro: %s", macroString.toLocal8Bit().data());
            }
            break;

            case POPUP_JUMP_E0:
            {
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(TopicOffset::size), srcSize);
                PRINT_DBG("        Popup jump 0xE0 type command");
                PRINT_DBG("        TopicOffset:");
                TopicOffset topicOffset(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8
                                *>(src) + offset),
                        srcSize - offset);
                offset += static_cast<size_t>(TopicOffset::size);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                PopupJumpE0Command(
                                    topicOffset))));
            }
            break;

            case TOPIC_JUMP_E1:
            {
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(TopicOffset::size), srcSize);
                PRINT_DBG("        Topic jump 0xE1 type command");
                PRINT_DBG("        TopicOffset:");
                TopicOffset topicOffset(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8
                                *>(src) + offset),
                        srcSize - offset);
                offset += static_cast<size_t>(TopicOffset::size);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                TopicJumpE1Command(
                                    topicOffset))));
            }
            break;

            case POPUP_JUMP_E2:
            {
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(TopicOffset::size), srcSize);
                PRINT_DBG("        Popup jump 0xE2 type command");
                PRINT_DBG("        TopicOffset:");
                TopicOffset topicOffset(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8
                                *>(src) + offset),
                        srcSize - offset);
                offset += static_cast<size_t>(TopicOffset::size);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                PopupJumpE2Command(
                                    topicOffset))));
            }
            break;

            case TOPIC_JUMP_E3:
            {
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(TopicOffset::size), srcSize);
                PRINT_DBG("        Topic jump 0xE3 type command");
                PRINT_DBG("        TopicOffset:");
                TopicOffset topicOffset(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8
                                *>(src) + offset),
                        srcSize - offset);
                offset += static_cast<size_t>(TopicOffset::size);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                TopicJumpE3Command(
                                    topicOffset))));
            }
            break;

            case POPUP_JUMP_WITHOUT_FONT_CHANGE:
            {
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(TopicOffset::size), srcSize);
                PRINT_DBG("        Popup jump without font change command");
                PRINT_DBG("        TopicOffset:");
                TopicOffset topicOffset(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8
                                *>(src) + offset),
                        srcSize - offset);
                offset += static_cast<size_t>(TopicOffset::size);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                PopupJumpWithoutFontChangeCommand(topicOffset))));
            }
            break;

            case TOPIC_JUMP_WITHOUT_FONT_CHANGE:
            {
                checkOffsetLengthSize(offset,
                        static_cast<size_t>(TopicOffset::size), srcSize);
                PRINT_DBG("        Topic jump without font change command");
                PRINT_DBG("        TopicOffset:");
                TopicOffset topicOffset(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8
                                *>(src) + offset),
                        srcSize - offset);
                offset += static_cast<size_t>(TopicOffset::size);
                paragraphInfo.commands.append(QSharedPointer<FormattingCommand>(
                            dynamic_cast<FormattingCommand *>(new
                                TopicJumpWithoutFontChangeCommand(topicOffset))));
            }
            break;

            case POPUP_JUMP_INTO_EXTERNAL_FILE:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                qint16 structLength =
                        getSignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                switch(dataType)
                {
                case 0:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file subtype 0 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileType0Command(
                                            structLength, topicOffset))));
                }
                break;

                case 1:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file subtype 1 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(1),
                                srcSize);
                    quint8 windowNumber =
                                getUnsignedByte(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(1);
                    PRINT_DBG("        WindowNumber: %d", structLength);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileType1Command(
                                            structLength, topicOffset,
                                            windowNumber))));
                }
                break;

                case 4:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file subtype 4 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileType4Command(
                                            structLength, topicOffset, filename))));
                }
                break;

                case 6:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file subtype 6 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    size_t windownameLength = static_cast<size_t>(0);
                    QString windowname = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                windownameLength);
                    offset += windownameLength;
                    PRINT_DBG("        Window name: %s", windowname.toLocal8Bit(
                                    ).data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileType6Command(
                                            structLength, topicOffset, filename,
                                            windowname))));
                }
                break;

                default:
                    throw std::runtime_error(
                            "Unknown data type in formatting jump command");
                }
            }
            break;

            case POPUP_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                qint16 structLength =
                        getSignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                switch(dataType)
                {
                case 0:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file without font change subtype 0 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileWithoutFontChangeType0Command(
                                            structLength, topicOffset))));
                }
                break;

                case 1:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file without font change subtype 1 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(1),
                                srcSize);
                    quint8 windowNumber =
                                getUnsignedByte(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(1);
                    PRINT_DBG("        WindowNumber: %d", structLength);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileWithoutFontChangeType1Command(
                                            structLength, topicOffset,
                                            windowNumber))));
                }
                break;

                case 4:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file without font change subtype 4 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileWithoutFontChangeType4Command(
                                            structLength, topicOffset, filename))));
                }
                break;

                case 6:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Popup jump into external file without font change subtype 6 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    size_t windownameLength = static_cast<size_t>(0);
                    QString windowname = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                windownameLength);
                    offset += windownameLength;
                    PRINT_DBG("        Window name: %s", windowname.toLocal8Bit(
                                    ).data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        PopupJumpIntoExternalFileWithoutFontChangeType6Command(
                                            structLength, topicOffset, filename,
                                            windowname))));
                }
                break;

                default:
                    throw std::runtime_error(
                            "Unknown data type in formatting jump command");
                }
            }
            break;

            case TOPIC_JUMP_INTO_EXTERNAL_FILE:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                qint16 structLength =
                        getSignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                switch(dataType)
                {
                case 0:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file subtype 0 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileType0Command(
                                            structLength, topicOffset))));
                }
                break;

                case 1:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file subtype 1 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(1),
                                srcSize);
                    quint8 windowNumber =
                                getUnsignedByte(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(1);
                    PRINT_DBG("        WindowNumber: %d", structLength);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileType1Command(
                                            structLength, topicOffset,
                                            windowNumber))));
                }
                break;

                case 4:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file subtype 4 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileType4Command(
                                            structLength, topicOffset, filename))));
                }
                break;

                case 6:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file subtype 6 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    size_t windownameLength = static_cast<size_t>(0);
                    QString windowname = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                windownameLength);
                    offset += windownameLength;
                    PRINT_DBG("        Window name: %s", windowname.toLocal8Bit(
                                    ).data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileType6Command(
                                            structLength, topicOffset, filename,
                                            windowname))));
                }
                break;

                default:
                    throw std::runtime_error(
                            "Unknown data type in formatting jump command");
                }
            }
            break;

            case TOPIC_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE:
            {
                checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
                qint16 structLength =
                        getSignedWord(reinterpret_cast<const void *>(
                            reinterpret_cast<const quint8 *>(src) + offset));
                offset += static_cast<size_t>(2);
                checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
                quint8 dataType =
                        getUnsignedByte(reinterpret_cast<const void *>(
                            reinterpret_cast
                                <const quint8 *>(src) + offset));
                offset += static_cast<size_t>(1);
                switch(dataType)
                {
                case 0:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file without font change subtype 0 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileWithoutFontChangeType0Command(
                                            structLength, topicOffset))));
                }
                break;

                case 1:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file without font change subtype 1 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(1),
                                srcSize);
                    quint8 windowNumber =
                                getUnsignedByte(reinterpret_cast<const void *>(
                                    reinterpret_cast<const quint8 *>(src) +
                                    offset));
                    offset += static_cast<size_t>(1);
                    PRINT_DBG("        WindowNumber: %d", structLength);
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileWithoutFontChangeType1Command(
                                            structLength, topicOffset,
                                            windowNumber))));
                }
                break;

                case 4:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file without font change subtype 4 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileWithoutFontChangeType4Command(
                                            structLength, topicOffset, filename))));
                }
                break;

                case 6:
                {
                    checkOffsetLengthSize(offset,
                                static_cast<size_t>(TopicOffset::size), srcSize);
                    PRINT_DBG(
                                "        Topic jump into external file without font change subtype 6 command");
                    PRINT_DBG("        Data length: %d", structLength);
                    PRINT_DBG("        TopicOffset:");
                    TopicOffset topicOffset(
                                reinterpret_cast<const void *>(reinterpret_cast
                                        <
                                        const quint8 *>(src) + offset),
                                srcSize - offset);
                    offset += static_cast<size_t>(TopicOffset::size);
                    size_t filenameLength = static_cast<size_t>(0);
                    QString filename = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                filenameLength);
                    offset += filenameLength;
                    PRINT_DBG("        External file name: %s",
                                filename.toLocal8Bit().data());
                    size_t windownameLength = static_cast<size_t>(0);
                    QString windowname = getNullTerminatedString(src,
                                offset,
                                srcSize,
                                codec,
                                windownameLength);
                    offset += windownameLength;
                    PRINT_DBG("        Window name: %s", windowname.toLocal8Bit(
                                    ).data());
                    paragraphInfo.commands.append(QSharedPointer<
                                    FormattingCommand>(dynamic_cast<
                                        FormattingCommand *>(
                                        new
                                        TopicJumpIntoExternalFileWithoutFontChangeType6Command(
                                            structLength, topicOffset, filename,
                                            windowname))));
                }
                break;

                default:
                    throw std::runtime_error(
                            "Unknown data type in formatting jump command");
                }
            }
            break;

            default:
                throw std::runtime_error("Unknown formatting command");
            }
        }
        this->paragraphs.append(paragraphInfo);
    }
    else
    {
        qDebug() << "Empty formatting info";
    }
    if((textSize > static_cast<size_t>(0)) && (textSrc != NULL))
    {
        texts = extractNullTerminatedStrings(textSrc, textSize, codec);
        for(int i = 0; i < texts.size(); i++)
        {
            PRINT_DBG("Text: %s", texts.at(i).toLocal8Bit().data());
        }
    }
    else
    {
        qDebug() << "Empty topic text";
    }
    PRINT_DBG("Displayable text new loaded successfully");
}

DisplayableTextNew::DisplayableTextNew(const DisplayableTextNew& rhs) :
    DisplayableText(rhs), topicSize(rhs.topicSize), topicLength(rhs.topicLength),
    paragraphs(rhs.paragraphs), texts(rhs.texts)
{
    PRINT_DBG("Displayable text new copy constructor");
}

DisplayableTextNew::~DisplayableTextNew()
{
    PRINT_DBG("Displayable text new destructor");
}

DisplayableTextNew & DisplayableTextNew::operator=(
    const DisplayableTextNew & rhs)
{
    PRINT_DBG("Displayable text new assignment operator");
    if (this != &rhs)
    {
        DisplayableText::operator =(rhs);
        this->topicSize = rhs.topicSize;
        this->topicLength = rhs.topicLength;
        this->paragraphs = rhs.paragraphs;
        this->texts = rhs.texts;
    }
    return *this;
}

QList<TopicOffset> DisplayableTextNew::getHyperlinks() const
{
    QList<TopicOffset> result;
    QListIterator<QSharedPointer<FormattingCommand> > i(this->paragraphs.at(
            0).commands);
    while(i.hasNext())
    {
        QSharedPointer<FormattingCommand> cmd = i.next();
        switch(cmd->getType())
        {
        case POPUP_JUMP_E0:
            result.append(cmd.dynamicCast<PopupJumpE0Command>()->getTopicOffset());
            break;

        case TOPIC_JUMP_E1:
            result.append(cmd.dynamicCast<TopicJumpE1Command>()->getTopicOffset());
            break;

        case POPUP_JUMP_E2:
            result.append(cmd.dynamicCast<PopupJumpE2Command>()->getTopicOffset());
            break;

        case TOPIC_JUMP_E3:
            result.append(cmd.dynamicCast<TopicJumpE3Command>()->getTopicOffset());
            break;

        case POPUP_JUMP_WITHOUT_FONT_CHANGE:
            result.append(
                cmd.dynamicCast<PopupJumpWithoutFontChangeCommand>()->
                getTopicOffset());
            break;

        case TOPIC_JUMP_WITHOUT_FONT_CHANGE:
            result.append(
                cmd.dynamicCast<TopicJumpWithoutFontChangeCommand>()->
                getTopicOffset());
            break;
        }
    }
    return result;
}

QString DisplayableTextNew::getHTML(bool &empty) const
{
    QString result;
    int textLength = 0;
    bool isInParagraph = false;
    QString openingPara = "<p style=\"";
    if(this->paragraphs.at(0).isRightAlignedParagraph)
    {
        openingPara += "text-align: right;";
    }
    else
    {
        if(this->paragraphs.at(0).isCenterAlignedParagraph)
        {
            openingPara += "text-align: center;";
        }
        else
        {
            openingPara += "text-align: left;";
        }
    }
    if(this->paragraphs.at(0).isFirstlineIndentPresent)
    {
        openingPara += QString("text-indent: %1pt;").arg(this->paragraphs.at(
                0).firstlineIndent);
    }
    if(this->paragraphs.at(0).isLeftIndentPresent)
    {
        openingPara += QString("margin-left: %1pt;").arg(this->paragraphs.at(
                0).leftIndent);
    }
    if(this->paragraphs.at(0).isRightIndentPresent)
    {
        openingPara += QString("margin-right: %1pt;").arg(this->paragraphs.at(
                0).rightIndent);
    }
    if(this->paragraphs.at(0).isSpacingAbovePresent)
    {
        openingPara += QString("margin-top: %1pt;").arg(this->paragraphs.at(
                0).spacingAbove);
    }
    if(this->paragraphs.at(0).isSpacingBelowPresent)
    {
        openingPara += QString("margin-bottom: %1pt;").arg(
            2 * this->paragraphs.at(0).spacingBelow);
    }
    openingPara += "\">";
    for(int i = 0; i < this->texts.count(); i++)
    {
        QString str = this->texts.at(i);
        textLength += this->texts.at(i).length();
        QString escapedString;
        if(!str.isEmpty())
        {
            escapedString = str.replace(QChar('&'), QString(
                    "&amp;"), Qt::CaseInsensitive).replace(QChar('<'), QString(
                    "&lt;"), Qt::CaseInsensitive).replace(QChar('>'), QString(
                    "&gt;"), Qt::CaseInsensitive);
        }
        if(i < this->paragraphs.at(0).commands.count())
        {
            switch(this->paragraphs.at(0).commands.at(i)->getType())
            {
            case NON_BREAK_HYPHEN:
                if(!isInParagraph)
                {
                    result += openingPara;
                    isInParagraph = true;
                }
                result += "&#x2011;";
                break;

            case LINE_BREAK:
                if(!isInParagraph)
                {
                    result += openingPara;
                    isInParagraph = true;
                }
                result += escapedString;
                result += "<br>";
                break;

            case END_OF_PARAGRAPH:
                if(!isInParagraph)
                {
                    result += openingPara;
                    isInParagraph = true;
                }
                result += escapedString;
                result += "</p>";
                isInParagraph = false;
                break;

            case NON_BREAK_SPACE:
                if(!isInParagraph)
                {
                    result += openingPara;
                    isInParagraph = true;
                }
                result += escapedString;
                result += "&nbsp;";
                break;

            case END_OF_CHARACTER_FORMATTING:
                break;

            default:
                if(!isInParagraph)
                {
                    result += openingPara;
                    isInParagraph = true;
                }
                result += escapedString;
            }
        }
    }
    if(textLength == 0)
    {
        empty = true;
    }
    else
    {
        empty = false;
    }
    return result;
}
