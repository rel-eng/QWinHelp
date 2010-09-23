/* Formatting commands classes.

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

#ifndef FORMATTINGCOMMANDS_H_
#define FORMATTINGCOMMANDS_H_

#include <QtGlobal>
#include <QByteArray>
#include <QString>

enum Commands : quint8
{
    END_OF_CHARACTER_FORMATTING = 0xFF,
    VFLD_NUMBER = 0x20,
    DTYPE_NUMBER = 0x21,
    FONT_NUMBER = 0x80,
    LINE_BREAK = 0x81,
    END_OF_PARAGRAPH = 0x82,
    TAB_COMMAND = 0x83,
    TYPE_86 = 0x86,
    TYPE_87 = 0x87,
    TYPE_88 = 0x88,
    END_OF_HOTSPOT = 0x89,
    NON_BREAK_SPACE = 0x8B,
    NON_BREAK_HYPHEN = 0x8C,
    MACRO_COMMAND = 0xC8,
    MACRO_WITHOUT_FONT_CHANGE = 0xCC,
    POPUP_JUMP_E0 = 0xE0,
    TOPIC_JUMP_E1 = 0xE1,
    POPUP_JUMP_E2 = 0xE2,
    TOPIC_JUMP_E3 = 0xE3,
    POPUP_JUMP_WITHOUT_FONT_CHANGE = 0xE6,
    TOPIC_JUMP_WITHOUT_FONT_CHANGE = 0xE7,
    POPUP_JUMP_INTO_EXTERNAL_FILE = 0xEA,
    POPUP_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE = 0xEB,
    TOPIC_JUMP_INTO_EXTERNAL_FILE = 0xEE,
    TOPIC_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE = 0xEF
};

enum TabType : quint16
{
    RIGHT_TAB = 1,
    CENTER_TAB = 2
};

enum PictureDataType : quint8
{
    EMBEDDED_OBJECT_DATA = 0x05,
    EMBEDDED_PICTURE_DATA_TYPE_3 = 0x03,
    EMBEDDED_PICTURE_DATA_TYPE_22 = 0x22
};

enum PictureEmbeddedType : qint16
{
    BMC_TYPE = 0,
    BMCWD_TYPE = 1
};

class FormattingCommand
{
private:
    quint8 type;
protected:
    FormattingCommand() : type(0)
    {
    };
    FormattingCommand(quint8 type) : type(type)
    {
    };
    FormattingCommand(const FormattingCommand& rhs) : type(rhs.type)
    {
    };
    FormattingCommand & operator=(const FormattingCommand &rhs)
    {
        if (this != &rhs)
        {
            this->type = rhs.type;
        }
        ; return *this;
    };
public:
    virtual ~FormattingCommand()
    {
    };
    quint8 getType() const
    {
        return this->type;
    };
};

class EndOfCharacterFormatting : public FormattingCommand
{
public:
    EndOfCharacterFormatting() : FormattingCommand(END_OF_CHARACTER_FORMATTING)
    {
    };
    EndOfCharacterFormatting(const EndOfCharacterFormatting& rhs) :
        FormattingCommand(rhs)
    {
    };
    EndOfCharacterFormatting & operator=(const EndOfCharacterFormatting &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~EndOfCharacterFormatting()
    {
    };
};

class VFLDNumberCommand : public FormattingCommand
{
private:
    qint32 vfldNumber;
public:
    VFLDNumberCommand() : FormattingCommand(VFLD_NUMBER), vfldNumber(0)
    {
    };
    VFLDNumberCommand(qint32 vfldNumber) : FormattingCommand(VFLD_NUMBER),
        vfldNumber(vfldNumber)
    {
    };
    VFLDNumberCommand(const VFLDNumberCommand& rhs) : FormattingCommand(rhs),
        vfldNumber(rhs.vfldNumber)
    {
    };
    VFLDNumberCommand & operator=(const VFLDNumberCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->vfldNumber =
                rhs.vfldNumber;
        }
        ; return *this;
    };
    virtual ~VFLDNumberCommand()
    {
    };
    qint32 getVfldNumber() const
    {
        return this->vfldNumber;
    };
};

class DTypeNumberCommand : public FormattingCommand
{
private:
    qint16 dtypeNumber;
public:
    DTypeNumberCommand() : FormattingCommand(DTYPE_NUMBER), dtypeNumber(0)
    {
    };
    DTypeNumberCommand(qint16 dtypeNumber) : FormattingCommand(DTYPE_NUMBER),
        dtypeNumber(dtypeNumber)
    {
    };
    DTypeNumberCommand(const DTypeNumberCommand& rhs) : FormattingCommand(rhs),
        dtypeNumber(rhs.dtypeNumber)
    {
    };
    DTypeNumberCommand & operator=(const DTypeNumberCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dtypeNumber =
                rhs.dtypeNumber;
        }
        ; return *this;
    };
    virtual ~DTypeNumberCommand()
    {
    };
    qint16 getDtypeNumber() const
    {
        return this->dtypeNumber;
    };
};

class FontNumberCommand : public FormattingCommand
{
private:
    qint16 fontNumber;
public:
    FontNumberCommand() : FormattingCommand(FONT_NUMBER), fontNumber(0)
    {
    };
    FontNumberCommand(qint16 fontNumber) : FormattingCommand(FONT_NUMBER),
        fontNumber(fontNumber)
    {
    };
    FontNumberCommand(const FontNumberCommand& rhs) : FormattingCommand(rhs),
        fontNumber(rhs.fontNumber)
    {
    };
    FontNumberCommand & operator=(const FontNumberCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->fontNumber =
                rhs.fontNumber;
        }
        ; return *this;
    };
    virtual ~FontNumberCommand()
    {
    };
    qint16 getFontNumber() const
    {
        return this->fontNumber;
    };
};

class LineBreakCommand : public FormattingCommand
{
public:
    LineBreakCommand() : FormattingCommand(LINE_BREAK)
    {
    };
    LineBreakCommand(const LineBreakCommand& rhs) : FormattingCommand(rhs)
    {
    };
    LineBreakCommand & operator=(const LineBreakCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~LineBreakCommand()
    {
    };
};

class EndOfParagraphCommand : public FormattingCommand
{
public:
    EndOfParagraphCommand() : FormattingCommand(END_OF_PARAGRAPH)
    {
    };
    EndOfParagraphCommand(const EndOfParagraphCommand& rhs) : FormattingCommand(
        rhs)
    {
    };
    EndOfParagraphCommand & operator=(const EndOfParagraphCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~EndOfParagraphCommand()
    {
    };
};

class TABCommand : public FormattingCommand
{
public:
    TABCommand() : FormattingCommand(TAB_COMMAND)
    {
    };
    TABCommand(const TABCommand& rhs) : FormattingCommand(rhs)
    {
    };
    TABCommand & operator=(const TABCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~TABCommand()
    {
    };
};

class Type86Command : public FormattingCommand
{
private:
    quint8 dataType;
    qint32 pictureSize;
protected:
    Type86Command() : FormattingCommand(TYPE_86), dataType(0), pictureSize(0)
    {
    };
    Type86Command(quint8 dataType,
        qint32 pictureSize) : FormattingCommand(TYPE_86), dataType(dataType),
        pictureSize(pictureSize)
    {
    };
    Type86Command(const Type86Command& rhs) : FormattingCommand(rhs),
        dataType(rhs.dataType), pictureSize(rhs.pictureSize)
    {
    };
    Type86Command & operator=(const Type86Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->pictureSize = rhs.pictureSize;
        }
        ; return *this;
    };
public:
    virtual ~Type86Command()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getPictureSize() const
    {
        return this->pictureSize;
    };
};

class Type86Subtype3Command : public Type86Command
{
private:
    qint16 pictureIsEmbedded;
    qint16 pictureNumber;
    QByteArray picture;
public:
    Type86Subtype3Command() : Type86Command(EMBEDDED_PICTURE_DATA_TYPE_3,
        0), pictureIsEmbedded(BMC_TYPE), pictureNumber(0), picture()
    {
    };
    Type86Subtype3Command(qint32 pictureSize,
        qint16 pictureIsEmbedded,
        qint16 pictureNumber,
        QByteArray picture) : Type86Command(EMBEDDED_PICTURE_DATA_TYPE_3,
        pictureSize), pictureIsEmbedded(pictureIsEmbedded),
        pictureNumber(pictureNumber), picture(picture)
    {
    };
    Type86Subtype3Command(const Type86Subtype3Command& rhs) : Type86Command(rhs),
        pictureIsEmbedded(rhs.pictureIsEmbedded),
        pictureNumber(rhs.pictureNumber),
        picture(rhs.picture)
    {
    };
    Type86Subtype3Command & operator=(const Type86Subtype3Command &rhs)
    {
        if (this != &rhs)
        {
            Type86Command::operator =(rhs); this->pictureIsEmbedded =
                rhs.pictureIsEmbedded; this->pictureNumber = rhs.pictureNumber;
            this->picture = rhs.picture;
        }
        ; return *this;
    };
    virtual ~Type86Subtype3Command()
    {
    };
    qint16 getPictureIsEmbedded() const
    {
        return this->pictureIsEmbedded;
    };
    qint16 getPictureNumber() const
    {
        return this->pictureNumber;
    };
    QByteArray getPicture() const
    {
        return this->picture;
    };
};

class Type86Subtype5Command : public Type86Command
{
private:
    QString embedded;
public:
    Type86Subtype5Command() : Type86Command(EMBEDDED_OBJECT_DATA, 0), embedded()
    {
    };
    Type86Subtype5Command(qint32 pictureSize,
        QString embedded) : Type86Command(EMBEDDED_OBJECT_DATA,
        pictureSize), embedded(embedded)
    {
    };
    Type86Subtype5Command(const Type86Subtype5Command& rhs) : Type86Command(rhs),
        embedded(rhs.embedded)
    {
    };
    Type86Subtype5Command & operator=(const Type86Subtype5Command &rhs)
    {
        if (this != &rhs)
        {
            Type86Command::operator =(rhs); this->embedded = rhs.embedded;
        }
        ; return *this;
    };
    virtual ~Type86Subtype5Command()
    {
    };
    QString getEmbedded() const
    {
        return this->embedded;
    };
};

class Type86Subtype22Command : public Type86Command
{
private:
    qint16 numberOfHotspots;
    qint16 pictureIsEmbedded;
    qint16 pictureNumber;
    QByteArray picture;
public:
    Type86Subtype22Command() : Type86Command(EMBEDDED_PICTURE_DATA_TYPE_22,
        0), numberOfHotspots(0), pictureIsEmbedded(BMC_TYPE), pictureNumber(0),
        picture()
    {
    };
    Type86Subtype22Command(qint32 pictureSize,
        qint16 numberOfHotspots,
        qint16 pictureIsEmbedded,
        qint16 pictureNumber,
        QByteArray picture) : Type86Command(EMBEDDED_PICTURE_DATA_TYPE_22,
        pictureSize), numberOfHotspots(numberOfHotspots),
        pictureIsEmbedded(pictureIsEmbedded), pictureNumber(pictureNumber),
        picture(picture)
    {
    };
    Type86Subtype22Command(const Type86Subtype22Command& rhs) : Type86Command(
        rhs), numberOfHotspots(rhs.numberOfHotspots),
        pictureIsEmbedded(rhs.pictureIsEmbedded),
        pictureNumber(rhs.pictureNumber),
        picture(rhs.picture)
    {
    };
    Type86Subtype22Command & operator=(const Type86Subtype22Command &rhs)
    {
        if (this != &rhs)
        {
            Type86Command::operator =(rhs); this->numberOfHotspots =
                rhs.numberOfHotspots; this->pictureIsEmbedded =
                rhs.pictureIsEmbedded; this->pictureNumber = rhs.pictureNumber;
            this->picture = rhs.picture;
        }
        ; return *this;
    };
    virtual ~Type86Subtype22Command()
    {
    };
    qint16 getNumberOfHotSpots() const
    {
        return this->numberOfHotspots;
    };
    qint16 getPictureIsEmbedded() const
    {
        return this->pictureIsEmbedded;
    };
    qint16 getPictureNumber() const
    {
        return this->pictureNumber;
    };
    QByteArray getPicture() const
    {
        return this->picture;
    };
};

class Type87Command : public FormattingCommand
{
private:
    quint8 dataType;
    qint32 pictureSize;
protected:
    Type87Command() : FormattingCommand(TYPE_87), dataType(0), pictureSize(0)
    {
    };
    Type87Command(quint8 dataType,
        qint32 pictureSize) : FormattingCommand(TYPE_87), dataType(dataType),
        pictureSize(pictureSize)
    {
    };
    Type87Command(const Type87Command& rhs) : FormattingCommand(rhs),
        dataType(rhs.dataType), pictureSize(rhs.pictureSize)
    {
    };
    Type87Command & operator=(const Type87Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->pictureSize = rhs.pictureSize;
        }
        ; return *this;
    };
public:
    virtual ~Type87Command()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getPictureSize() const
    {
        return this->pictureSize;
    };
};

class Type87Subtype3Command : public Type87Command
{
private:
    qint16 pictureIsEmbedded;
    qint16 pictureNumber;
    QByteArray picture;
public:
    Type87Subtype3Command() : Type87Command(EMBEDDED_PICTURE_DATA_TYPE_3,
        0), pictureIsEmbedded(BMC_TYPE), pictureNumber(0), picture()
    {
    };
    Type87Subtype3Command(qint32 pictureSize,
        qint16 pictureIsEmbedded,
        qint16 pictureNumber,
        QByteArray picture) : Type87Command(EMBEDDED_PICTURE_DATA_TYPE_3,
        pictureSize), pictureIsEmbedded(pictureIsEmbedded),
        pictureNumber(pictureNumber), picture(picture)
    {
    };
    Type87Subtype3Command(const Type87Subtype3Command& rhs) : Type87Command(rhs),
        pictureIsEmbedded(rhs.pictureIsEmbedded),
        pictureNumber(rhs.pictureNumber),
        picture(rhs.picture)
    {
    };
    Type87Subtype3Command & operator=(const Type87Subtype3Command &rhs)
    {
        if (this != &rhs)
        {
            Type87Command::operator =(rhs); this->pictureIsEmbedded =
                rhs.pictureIsEmbedded; this->pictureNumber = rhs.pictureNumber;
            this->picture = rhs.picture;
        }
        ; return *this;
    };
    virtual ~Type87Subtype3Command()
    {
    };
    qint16 getPictureIsEmbedded() const
    {
        return this->pictureIsEmbedded;
    };
    qint16 getPictureNumber() const
    {
        return this->pictureNumber;
    };
    QByteArray getPicture() const
    {
        return this->picture;
    };
};

class Type87Subtype5Command : public Type87Command
{
private:
    QString embedded;
public:
    Type87Subtype5Command() : Type87Command(EMBEDDED_OBJECT_DATA, 0), embedded()
    {
    };
    Type87Subtype5Command(qint32 pictureSize,
        QString embedded) : Type87Command(EMBEDDED_OBJECT_DATA,
        pictureSize), embedded(embedded)
    {
    };
    Type87Subtype5Command(const Type87Subtype5Command& rhs) : Type87Command(rhs),
        embedded(rhs.embedded)
    {
    };
    Type87Subtype5Command & operator=(const Type87Subtype5Command &rhs)
    {
        if (this != &rhs)
        {
            Type87Command::operator =(rhs); this->embedded = rhs.embedded;
        }
        ; return *this;
    };
    virtual ~Type87Subtype5Command()
    {
    };
    QString getEmbedded() const
    {
        return this->embedded;
    };
};

class Type87Subtype22Command : public Type87Command
{
private:
    qint16 numberOfHotspots;
    qint16 pictureIsEmbedded;
    qint16 pictureNumber;
    QByteArray picture;
public:
    Type87Subtype22Command() : Type87Command(EMBEDDED_PICTURE_DATA_TYPE_22,
        0), numberOfHotspots(0), pictureIsEmbedded(BMC_TYPE), pictureNumber(0),
        picture()
    {
    };
    Type87Subtype22Command(qint32 pictureSize,
        qint16 numberOfHotspots,
        qint16 pictureIsEmbedded,
        qint16 pictureNumber,
        QByteArray picture) : Type87Command(EMBEDDED_PICTURE_DATA_TYPE_22,
        pictureSize), numberOfHotspots(numberOfHotspots),
        pictureIsEmbedded(pictureIsEmbedded), pictureNumber(pictureNumber),
        picture(picture)
    {
    };
    Type87Subtype22Command(const Type87Subtype22Command& rhs) : Type87Command(
        rhs), numberOfHotspots(rhs.numberOfHotspots),
        pictureIsEmbedded(rhs.pictureIsEmbedded),
        pictureNumber(rhs.pictureNumber),
        picture(rhs.picture)
    {
    };
    Type87Subtype22Command & operator=(const Type87Subtype22Command &rhs)
    {
        if (this != &rhs)
        {
            Type87Command::operator =(rhs); this->numberOfHotspots =
                rhs.numberOfHotspots; this->pictureIsEmbedded =
                rhs.pictureIsEmbedded; this->pictureNumber = rhs.pictureNumber;
            this->picture = rhs.picture;
        }
        ; return *this;
    };
    virtual ~Type87Subtype22Command()
    {
    };
    qint16 getNumberOfHotSpots() const
    {
        return this->numberOfHotspots;
    };
    qint16 getPictureIsEmbedded() const
    {
        return this->pictureIsEmbedded;
    };
    qint16 getPictureNumber() const
    {
        return this->pictureNumber;
    };
    QByteArray getPicture() const
    {
        return this->picture;
    };
};

class Type88Command : public FormattingCommand
{
private:
    quint8 dataType;
    qint32 pictureSize;
protected:
    Type88Command() : FormattingCommand(TYPE_88), dataType(0), pictureSize(0)
    {
    };
    Type88Command(quint8 dataType,
        qint32 pictureSize) : FormattingCommand(TYPE_88), dataType(dataType),
        pictureSize(pictureSize)
    {
    };
    Type88Command(const Type88Command& rhs) : FormattingCommand(rhs),
        dataType(rhs.dataType), pictureSize(rhs.pictureSize)
    {
    };
    Type88Command & operator=(const Type88Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->pictureSize = rhs.pictureSize;
        }
        ; return *this;
    };
public:
    virtual ~Type88Command()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getPictureSize() const
    {
        return this->pictureSize;
    };
};

class Type88Subtype3Command : public Type88Command
{
private:
    qint16 pictureIsEmbedded;
    qint16 pictureNumber;
    QByteArray picture;
public:
    Type88Subtype3Command() : Type88Command(EMBEDDED_PICTURE_DATA_TYPE_3,
        0), pictureIsEmbedded(BMC_TYPE), pictureNumber(0), picture()
    {
    };
    Type88Subtype3Command(qint32 pictureSize,
        qint16 pictureIsEmbedded,
        qint16 pictureNumber,
        QByteArray picture) : Type88Command(EMBEDDED_PICTURE_DATA_TYPE_3,
        pictureSize), pictureIsEmbedded(pictureIsEmbedded),
        pictureNumber(pictureNumber), picture(picture)
    {
    };
    Type88Subtype3Command(const Type88Subtype3Command& rhs) : Type88Command(rhs),
        pictureIsEmbedded(rhs.pictureIsEmbedded),
        pictureNumber(rhs.pictureNumber),
        picture(rhs.picture)
    {
    };
    Type88Subtype3Command & operator=(const Type88Subtype3Command &rhs)
    {
        if (this != &rhs)
        {
            Type88Command::operator =(rhs); this->pictureIsEmbedded =
                rhs.pictureIsEmbedded; this->pictureNumber = rhs.pictureNumber;
            this->picture = rhs.picture;
        }
        ; return *this;
    };
    virtual ~Type88Subtype3Command()
    {
    };
    qint16 getPictureIsEmbedded() const
    {
        return this->pictureIsEmbedded;
    };
    qint16 getPictureNumber() const
    {
        return this->pictureNumber;
    };
    QByteArray getPicture() const
    {
        return this->picture;
    };
};

class Type88Subtype5Command : public Type88Command
{
private:
    QString embedded;
public:
    Type88Subtype5Command() : Type88Command(EMBEDDED_OBJECT_DATA, 0), embedded()
    {
    };
    Type88Subtype5Command(qint32 pictureSize,
        QString embedded) : Type88Command(EMBEDDED_OBJECT_DATA,
        pictureSize), embedded(embedded)
    {
    };
    Type88Subtype5Command(const Type88Subtype5Command& rhs) : Type88Command(rhs),
        embedded(rhs.embedded)
    {
    };
    Type88Subtype5Command & operator=(const Type88Subtype5Command &rhs)
    {
        if (this != &rhs)
        {
            Type88Command::operator =(rhs); this->embedded = rhs.embedded;
        }
        ; return *this;
    };
    virtual ~Type88Subtype5Command()
    {
    };
    QString getEmbedded() const
    {
        return this->embedded;
    };
};

class Type88Subtype22Command : public Type88Command
{
private:
    qint16 numberOfHotspots;
    qint16 pictureIsEmbedded;
    qint16 pictureNumber;
    QByteArray picture;
public:
    Type88Subtype22Command() : Type88Command(EMBEDDED_PICTURE_DATA_TYPE_22,
        0), numberOfHotspots(0), pictureIsEmbedded(BMC_TYPE), pictureNumber(0),
        picture()
    {
    };
    Type88Subtype22Command(qint32 pictureSize,
        qint16 numberOfHotspots,
        qint16 pictureIsEmbedded,
        qint16 pictureNumber,
        QByteArray picture) : Type88Command(EMBEDDED_PICTURE_DATA_TYPE_22,
        pictureSize), numberOfHotspots(numberOfHotspots),
        pictureIsEmbedded(pictureIsEmbedded), pictureNumber(pictureNumber),
        picture(picture)
    {
    };
    Type88Subtype22Command(const Type88Subtype22Command& rhs) : Type88Command(
        rhs), numberOfHotspots(rhs.numberOfHotspots),
        pictureIsEmbedded(rhs.pictureIsEmbedded),
        pictureNumber(rhs.pictureNumber),
        picture(rhs.picture)
    {
    };
    Type88Subtype22Command & operator=(const Type88Subtype22Command &rhs)
    {
        if (this != &rhs)
        {
            Type88Command::operator =(rhs); this->numberOfHotspots =
                rhs.numberOfHotspots; this->pictureIsEmbedded =
                rhs.pictureIsEmbedded; this->pictureNumber = rhs.pictureNumber;
            this->picture = rhs.picture;
        }
        ; return *this;
    };
    virtual ~Type88Subtype22Command()
    {
    };
    qint16 getNumberOfHotSpots() const
    {
        return this->numberOfHotspots;
    };
    qint16 getPictureIsEmbedded() const
    {
        return this->pictureIsEmbedded;
    };
    qint16 getPictureNumber() const
    {
        return this->pictureNumber;
    };
    QByteArray getPicture() const
    {
        return this->picture;
    };
};

class EndOfHotspotCommand : public FormattingCommand
{
public:
    EndOfHotspotCommand() : FormattingCommand(END_OF_HOTSPOT)
    {
    };
    EndOfHotspotCommand(const EndOfHotspotCommand& rhs) : FormattingCommand(rhs)
    {
    };
    EndOfHotspotCommand & operator=(const EndOfHotspotCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~EndOfHotspotCommand()
    {
    };
};

class NonBreakSpaceCommand : public FormattingCommand
{
public:
    NonBreakSpaceCommand() : FormattingCommand(NON_BREAK_SPACE)
    {
    };
    NonBreakSpaceCommand(const NonBreakSpaceCommand& rhs) : FormattingCommand(
        rhs)
    {
    };
    NonBreakSpaceCommand & operator=(const NonBreakSpaceCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~NonBreakSpaceCommand()
    {
    };
};

class NonBreakHyphenCommand : public FormattingCommand
{
public:
    NonBreakHyphenCommand() : FormattingCommand(NON_BREAK_HYPHEN)
    {
    };
    NonBreakHyphenCommand(const NonBreakHyphenCommand& rhs) : FormattingCommand(
        rhs)
    {
    };
    NonBreakHyphenCommand & operator=(const NonBreakHyphenCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs);
        }
        ; return *this;
    };
    virtual ~NonBreakHyphenCommand()
    {
    };
};

class MacroCommand : public FormattingCommand
{
private:
    quint16 length;
    QString macroString;
public:
    MacroCommand() : FormattingCommand(MACRO_COMMAND), length(0), macroString()
    {
    };
    MacroCommand(quint16 length,
        QString macroString) : FormattingCommand(MACRO_COMMAND), length(length),
        macroString(macroString)
    {
    };
    MacroCommand(const MacroCommand& rhs) : FormattingCommand(rhs),
        length(rhs.length), macroString(rhs.macroString)
    {
    };
    MacroCommand & operator=(const MacroCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->length = rhs.length;
            this->macroString = rhs.macroString;
        }
        ; return *this;
    };
    virtual ~MacroCommand()
    {
    };
    qint16 getLength() const
    {
        return this->length;
    };
    QString getMacroString() const
    {
        return this->macroString;
    };
};

class MacroWithoutFontChangeCommand : public FormattingCommand
{
private:
    quint16 length;
    QString macroString;
public:
    MacroWithoutFontChangeCommand() : FormattingCommand(
        MACRO_WITHOUT_FONT_CHANGE), length(0), macroString()
    {
    };
    MacroWithoutFontChangeCommand(quint16 length,
        QString macroString) : FormattingCommand(MACRO_COMMAND), length(length),
        macroString(macroString)
    {
    };
    MacroWithoutFontChangeCommand(const MacroWithoutFontChangeCommand& rhs) :
        FormattingCommand(rhs), length(rhs.length), macroString(rhs.macroString)
    {
    };
    MacroWithoutFontChangeCommand & operator=(
        const MacroWithoutFontChangeCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->length = rhs.length;
            this->macroString = rhs.macroString;
        }
        ; return *this;
    };
    virtual ~MacroWithoutFontChangeCommand()
    {
    };
    qint16 getLength() const
    {
        return this->length;
    };
    QString getMacroString() const
    {
        return this->macroString;
    };
};

class PopupJumpE0Command : public FormattingCommand
{
private:
    TopicOffset topicOffset;
public:
    PopupJumpE0Command() : FormattingCommand(POPUP_JUMP_E0), topicOffset()
    {
    };
    PopupJumpE0Command(TopicOffset topicOffset) : FormattingCommand(
        POPUP_JUMP_E0), topicOffset(topicOffset)
    {
    };
    PopupJumpE0Command(const PopupJumpE0Command& rhs) : FormattingCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    PopupJumpE0Command & operator=(const PopupJumpE0Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->topicOffset =
                rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~PopupJumpE0Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class TopicJumpE1Command : public FormattingCommand
{
private:
    TopicOffset topicOffset;
public:
    TopicJumpE1Command() : FormattingCommand(TOPIC_JUMP_E1), topicOffset()
    {
    };
    TopicJumpE1Command(TopicOffset topicOffset) : FormattingCommand(
        TOPIC_JUMP_E1), topicOffset(topicOffset)
    {
    };
    TopicJumpE1Command(const TopicJumpE1Command& rhs) : FormattingCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    TopicJumpE1Command & operator=(const TopicJumpE1Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->topicOffset =
                rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~TopicJumpE1Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class PopupJumpE2Command : public FormattingCommand
{
private:
    TopicOffset topicOffset;
public:
    PopupJumpE2Command() : FormattingCommand(POPUP_JUMP_E2), topicOffset()
    {
    };
    PopupJumpE2Command(TopicOffset topicOffset) : FormattingCommand(
        POPUP_JUMP_E2), topicOffset(topicOffset)
    {
    };
    PopupJumpE2Command(const PopupJumpE2Command& rhs) : FormattingCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    PopupJumpE2Command & operator=(const PopupJumpE2Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->topicOffset =
                rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~PopupJumpE2Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class TopicJumpE3Command : public FormattingCommand
{
private:
    TopicOffset topicOffset;
public:
    TopicJumpE3Command() : FormattingCommand(TOPIC_JUMP_E3), topicOffset()
    {
    };
    TopicJumpE3Command(TopicOffset topicOffset) : FormattingCommand(
        TOPIC_JUMP_E3), topicOffset(topicOffset)
    {
    };
    TopicJumpE3Command(const TopicJumpE3Command& rhs) : FormattingCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    TopicJumpE3Command & operator=(const TopicJumpE3Command &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->topicOffset =
                rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~TopicJumpE3Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class PopupJumpWithoutFontChangeCommand : public FormattingCommand
{
private:
    TopicOffset topicOffset;
public:
    PopupJumpWithoutFontChangeCommand() : FormattingCommand(
        POPUP_JUMP_WITHOUT_FONT_CHANGE), topicOffset()
    {
    };
    PopupJumpWithoutFontChangeCommand(TopicOffset topicOffset) :
        FormattingCommand(POPUP_JUMP_WITHOUT_FONT_CHANGE),
        topicOffset(topicOffset)
    {
    };
    PopupJumpWithoutFontChangeCommand(
        const PopupJumpWithoutFontChangeCommand& rhs) : FormattingCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    PopupJumpWithoutFontChangeCommand & operator=(
        const PopupJumpWithoutFontChangeCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->topicOffset =
                rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~PopupJumpWithoutFontChangeCommand()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class TopicJumpWithoutFontChangeCommand : public FormattingCommand
{
private:
    TopicOffset topicOffset;
public:
    TopicJumpWithoutFontChangeCommand() : FormattingCommand(
        TOPIC_JUMP_WITHOUT_FONT_CHANGE), topicOffset()
    {
    };
    TopicJumpWithoutFontChangeCommand(TopicOffset topicOffset) :
        FormattingCommand(TOPIC_JUMP_WITHOUT_FONT_CHANGE),
        topicOffset(topicOffset)
    {
    };
    TopicJumpWithoutFontChangeCommand(
        const TopicJumpWithoutFontChangeCommand& rhs) : FormattingCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    TopicJumpWithoutFontChangeCommand & operator=(
        const TopicJumpWithoutFontChangeCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->topicOffset =
                rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~TopicJumpWithoutFontChangeCommand()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class PopupJumpIntoExternalFileCommand : public FormattingCommand
{
private:
    quint8 dataType;
    qint16 sizeOfData;
protected:
    PopupJumpIntoExternalFileCommand() : FormattingCommand(
        POPUP_JUMP_INTO_EXTERNAL_FILE), dataType(0), sizeOfData(0)
    {
    };
    PopupJumpIntoExternalFileCommand(quint8 dataType,
        qint16 sizeOfData) : FormattingCommand(POPUP_JUMP_INTO_EXTERNAL_FILE),
        dataType(dataType), sizeOfData(sizeOfData)
    {
    };
    PopupJumpIntoExternalFileCommand(
        const PopupJumpIntoExternalFileCommand& rhs) : FormattingCommand(rhs),
        dataType(rhs.dataType), sizeOfData(rhs.sizeOfData)
    {
    };
    PopupJumpIntoExternalFileCommand & operator=(
        const PopupJumpIntoExternalFileCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->sizeOfData = rhs.sizeOfData;
        }
        ; return *this;
    };
public:
    virtual ~PopupJumpIntoExternalFileCommand()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getSizeOfData() const
    {
        return this->sizeOfData;
    };

};

class PopupJumpIntoExternalFileType0Command : public
    PopupJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
public:
    PopupJumpIntoExternalFileType0Command() : PopupJumpIntoExternalFileCommand(
        0, 0), topicOffset()
    {
    };
    PopupJumpIntoExternalFileType0Command(qint16 sizeOfData,
        TopicOffset topicOffset) : PopupJumpIntoExternalFileCommand(0,
        sizeOfData), topicOffset(topicOffset)
    {
    };
    PopupJumpIntoExternalFileType0Command(
        const PopupJumpIntoExternalFileType0Command& rhs) :
        PopupJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset)
    {
    };
    PopupJumpIntoExternalFileType0Command & operator=(
        const PopupJumpIntoExternalFileType0Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileType0Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class PopupJumpIntoExternalFileType1Command : public
    PopupJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
    quint8 windowNumber;
public:
    PopupJumpIntoExternalFileType1Command() : PopupJumpIntoExternalFileCommand(
        1, 0), topicOffset(), windowNumber(0)
    {
    };
    PopupJumpIntoExternalFileType1Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        quint8 windowNumber) : PopupJumpIntoExternalFileCommand(1,
        sizeOfData), topicOffset(topicOffset), windowNumber(windowNumber)
    {
    };
    PopupJumpIntoExternalFileType1Command(
        const PopupJumpIntoExternalFileType1Command& rhs) :
        PopupJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset),
        windowNumber(rhs.windowNumber)
    {
    };
    PopupJumpIntoExternalFileType1Command & operator=(
        const PopupJumpIntoExternalFileType1Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->windowNumber =
                rhs.windowNumber;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileType1Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    quint8 getWindowNumber() const
    {
        return this->windowNumber;
    };
};

class PopupJumpIntoExternalFileType4Command : public
    PopupJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
public:
    PopupJumpIntoExternalFileType4Command() : PopupJumpIntoExternalFileCommand(
        4, 0), topicOffset(), nameOfExternalFile()
    {
    };
    PopupJumpIntoExternalFileType4Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile) : PopupJumpIntoExternalFileCommand(4,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile)
    {
    };
    PopupJumpIntoExternalFileType4Command(
        const PopupJumpIntoExternalFileType4Command& rhs) :
        PopupJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset),
        nameOfExternalFile(rhs.nameOfExternalFile)
    {
    };
    PopupJumpIntoExternalFileType4Command & operator=(
        const PopupJumpIntoExternalFileType4Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileType4Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
};

class PopupJumpIntoExternalFileType6Command : public
    PopupJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
    QString windowName;
public:
    PopupJumpIntoExternalFileType6Command() : PopupJumpIntoExternalFileCommand(
        6, 0), topicOffset(), nameOfExternalFile(), windowName()
    {
    };
    PopupJumpIntoExternalFileType6Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile,
        QString windowName) : PopupJumpIntoExternalFileCommand(6,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile),
        windowName(windowName)
    {
    };
    PopupJumpIntoExternalFileType6Command(
        const PopupJumpIntoExternalFileType6Command& rhs) :
        PopupJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset),
        nameOfExternalFile(rhs.nameOfExternalFile), windowName(rhs.windowName)
    {
    };
    PopupJumpIntoExternalFileType6Command & operator=(
        const PopupJumpIntoExternalFileType6Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile; this->windowName = rhs.windowName;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileType6Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
    QString getWindowName() const
    {
        return this->windowName;
    };
};

class PopupJumpIntoExternalFileWithoutFontChangeCommand : public
    FormattingCommand
{
private:
    quint8 dataType;
    qint16 sizeOfData;
protected:
    PopupJumpIntoExternalFileWithoutFontChangeCommand() : FormattingCommand(
        POPUP_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE), dataType(0),
        sizeOfData(0)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeCommand(quint8 dataType,
        qint16 sizeOfData) : FormattingCommand(
        POPUP_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE), dataType(dataType),
        sizeOfData(sizeOfData)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeCommand(
        const PopupJumpIntoExternalFileWithoutFontChangeCommand& rhs) :
        FormattingCommand(rhs), dataType(rhs.dataType),
        sizeOfData(rhs.sizeOfData)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeCommand & operator=(
        const PopupJumpIntoExternalFileWithoutFontChangeCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->sizeOfData = rhs.sizeOfData;
        }
        ; return *this;
    };
public:
    virtual ~PopupJumpIntoExternalFileWithoutFontChangeCommand()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getSizeOfData() const
    {
        return this->sizeOfData;
    };

};

class PopupJumpIntoExternalFileWithoutFontChangeType0Command : public
    PopupJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
public:
    PopupJumpIntoExternalFileWithoutFontChangeType0Command() :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(0, 0), topicOffset()
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType0Command(qint16 sizeOfData,
        TopicOffset topicOffset) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(
        0, sizeOfData), topicOffset(topicOffset)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType0Command(
        const PopupJumpIntoExternalFileWithoutFontChangeType0Command& rhs) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType0Command & operator=(
        const PopupJumpIntoExternalFileWithoutFontChangeType0Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileWithoutFontChangeType0Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class PopupJumpIntoExternalFileWithoutFontChangeType1Command : public
    PopupJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
    quint8 windowNumber;
public:
    PopupJumpIntoExternalFileWithoutFontChangeType1Command() :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(1,
        0), topicOffset(), windowNumber(0)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType1Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        quint8 windowNumber) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(1,
        sizeOfData), topicOffset(topicOffset), windowNumber(windowNumber)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType1Command(
        const PopupJumpIntoExternalFileWithoutFontChangeType1Command& rhs) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset), windowNumber(rhs.windowNumber)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType1Command & operator=(
        const PopupJumpIntoExternalFileWithoutFontChangeType1Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->windowNumber =
                rhs.windowNumber;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileWithoutFontChangeType1Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    quint8 getWindowNumber() const
    {
        return this->windowNumber;
    };
};

class PopupJumpIntoExternalFileWithoutFontChangeType4Command : public
    PopupJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
public:
    PopupJumpIntoExternalFileWithoutFontChangeType4Command() :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(4,
        0), topicOffset(), nameOfExternalFile()
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType4Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(4,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType4Command(
        const PopupJumpIntoExternalFileWithoutFontChangeType4Command& rhs) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset), nameOfExternalFile(rhs.nameOfExternalFile)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType4Command & operator=(
        const PopupJumpIntoExternalFileWithoutFontChangeType4Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileWithoutFontChangeType4Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
};

class PopupJumpIntoExternalFileWithoutFontChangeType6Command : public
    PopupJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
    QString windowName;
public:
    PopupJumpIntoExternalFileWithoutFontChangeType6Command() :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(6,
        0), topicOffset(), nameOfExternalFile(), windowName()
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType6Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile,
        QString windowName) : PopupJumpIntoExternalFileWithoutFontChangeCommand(
        6,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile),
        windowName(windowName)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType6Command(
        const PopupJumpIntoExternalFileWithoutFontChangeType6Command& rhs) :
        PopupJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset), nameOfExternalFile(rhs.nameOfExternalFile),
        windowName(rhs.windowName)
    {
    };
    PopupJumpIntoExternalFileWithoutFontChangeType6Command & operator=(
        const PopupJumpIntoExternalFileWithoutFontChangeType6Command &rhs)
    {
        if (this != &rhs)
        {
            PopupJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile; this->windowName = rhs.windowName;
        }
        ; return *this;
    };
    virtual ~PopupJumpIntoExternalFileWithoutFontChangeType6Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
    QString getWindowName() const
    {
        return this->windowName;
    };
};

class TopicJumpIntoExternalFileCommand : public FormattingCommand
{
private:
    quint8 dataType;
    qint16 sizeOfData;
protected:
    TopicJumpIntoExternalFileCommand() : FormattingCommand(
        TOPIC_JUMP_INTO_EXTERNAL_FILE), dataType(0), sizeOfData(0)
    {
    };
    TopicJumpIntoExternalFileCommand(quint8 dataType,
        qint16 sizeOfData) : FormattingCommand(TOPIC_JUMP_INTO_EXTERNAL_FILE),
        dataType(dataType), sizeOfData(sizeOfData)
    {
    };
    TopicJumpIntoExternalFileCommand(
        const TopicJumpIntoExternalFileCommand& rhs) : FormattingCommand(rhs),
        dataType(rhs.dataType), sizeOfData(rhs.sizeOfData)
    {
    };
    TopicJumpIntoExternalFileCommand & operator=(
        const TopicJumpIntoExternalFileCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->sizeOfData = rhs.sizeOfData;
        }
        ; return *this;
    };
public:
    virtual ~TopicJumpIntoExternalFileCommand()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getSizeOfData() const
    {
        return this->sizeOfData;
    };

};

class TopicJumpIntoExternalFileType0Command : public
    TopicJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
public:
    TopicJumpIntoExternalFileType0Command() : TopicJumpIntoExternalFileCommand(
        0, 0), topicOffset()
    {
    };
    TopicJumpIntoExternalFileType0Command(qint16 sizeOfData,
        TopicOffset topicOffset) : TopicJumpIntoExternalFileCommand(0,
        sizeOfData), topicOffset(topicOffset)
    {
    };
    TopicJumpIntoExternalFileType0Command(
        const TopicJumpIntoExternalFileType0Command& rhs) :
        TopicJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset)
    {
    };
    TopicJumpIntoExternalFileType0Command & operator=(
        const TopicJumpIntoExternalFileType0Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileType0Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class TopicJumpIntoExternalFileType1Command : public
    TopicJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
    quint8 windowNumber;
public:
    TopicJumpIntoExternalFileType1Command() : TopicJumpIntoExternalFileCommand(
        1, 0), topicOffset(), windowNumber(0)
    {
    };
    TopicJumpIntoExternalFileType1Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        quint8 windowNumber) : TopicJumpIntoExternalFileCommand(1,
        sizeOfData), topicOffset(topicOffset), windowNumber(windowNumber)
    {
    };
    TopicJumpIntoExternalFileType1Command(
        const TopicJumpIntoExternalFileType1Command& rhs) :
        TopicJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset),
        windowNumber(rhs.windowNumber)
    {
    };
    TopicJumpIntoExternalFileType1Command & operator=(
        const TopicJumpIntoExternalFileType1Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->windowNumber =
                rhs.windowNumber;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileType1Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    quint8 getWindowNumber() const
    {
        return this->windowNumber;
    };
};

class TopicJumpIntoExternalFileType4Command : public
    TopicJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
public:
    TopicJumpIntoExternalFileType4Command() : TopicJumpIntoExternalFileCommand(
        4, 0), topicOffset(), nameOfExternalFile()
    {
    };
    TopicJumpIntoExternalFileType4Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile) : TopicJumpIntoExternalFileCommand(4,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile)
    {
    };
    TopicJumpIntoExternalFileType4Command(
        const TopicJumpIntoExternalFileType4Command& rhs) :
        TopicJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset),
        nameOfExternalFile(rhs.nameOfExternalFile)
    {
    };
    TopicJumpIntoExternalFileType4Command & operator=(
        const TopicJumpIntoExternalFileType4Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileType4Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
};

class TopicJumpIntoExternalFileType6Command : public
    TopicJumpIntoExternalFileCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
    QString windowName;
public:
    TopicJumpIntoExternalFileType6Command() : TopicJumpIntoExternalFileCommand(
        6, 0), topicOffset(), nameOfExternalFile(), windowName()
    {
    };
    TopicJumpIntoExternalFileType6Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile,
        QString windowName) : TopicJumpIntoExternalFileCommand(6,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile),
        windowName(windowName)
    {
    };
    TopicJumpIntoExternalFileType6Command(
        const TopicJumpIntoExternalFileType6Command& rhs) :
        TopicJumpIntoExternalFileCommand(rhs), topicOffset(rhs.topicOffset),
        nameOfExternalFile(rhs.nameOfExternalFile), windowName(rhs.windowName)
    {
    };
    TopicJumpIntoExternalFileType6Command & operator=(
        const TopicJumpIntoExternalFileType6Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile; this->windowName = rhs.windowName;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileType6Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
    QString getWindowName() const
    {
        return this->windowName;
    };
};

class TopicJumpIntoExternalFileWithoutFontChangeCommand : public
    FormattingCommand
{
private:
    quint8 dataType;
    qint16 sizeOfData;
protected:
    TopicJumpIntoExternalFileWithoutFontChangeCommand() : FormattingCommand(
        TOPIC_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE), dataType(0),
        sizeOfData(0)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeCommand(quint8 dataType,
        qint16 sizeOfData) : FormattingCommand(
        TOPIC_JUMP_INTO_EXTERNAL_FILE_WITHOUT_FONT_CHANGE), dataType(dataType),
        sizeOfData(sizeOfData)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeCommand(
        const TopicJumpIntoExternalFileWithoutFontChangeCommand& rhs) :
        FormattingCommand(rhs), dataType(rhs.dataType),
        sizeOfData(rhs.sizeOfData)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeCommand & operator=(
        const TopicJumpIntoExternalFileWithoutFontChangeCommand &rhs)
    {
        if (this != &rhs)
        {
            FormattingCommand::operator =(rhs); this->dataType = rhs.dataType;
            this->sizeOfData = rhs.sizeOfData;
        }
        ; return *this;
    };
public:
    virtual ~TopicJumpIntoExternalFileWithoutFontChangeCommand()
    {
    };
    quint8 getDataType() const
    {
        return this->dataType;
    };
    qint32 getSizeOfData() const
    {
        return this->sizeOfData;
    };

};

class TopicJumpIntoExternalFileWithoutFontChangeType0Command : public
    TopicJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
public:
    TopicJumpIntoExternalFileWithoutFontChangeType0Command() :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(0, 0), topicOffset()
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType0Command(qint16 sizeOfData,
        TopicOffset topicOffset) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(
        0, sizeOfData), topicOffset(topicOffset)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType0Command(
        const TopicJumpIntoExternalFileWithoutFontChangeType0Command& rhs) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType0Command & operator=(
        const TopicJumpIntoExternalFileWithoutFontChangeType0Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileWithoutFontChangeType0Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
};

class TopicJumpIntoExternalFileWithoutFontChangeType1Command : public
    TopicJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
    quint8 windowNumber;
public:
    TopicJumpIntoExternalFileWithoutFontChangeType1Command() :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(1,
        0), topicOffset(), windowNumber(0)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType1Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        quint8 windowNumber) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(1,
        sizeOfData), topicOffset(topicOffset), windowNumber(windowNumber)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType1Command(
        const TopicJumpIntoExternalFileWithoutFontChangeType1Command& rhs) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset), windowNumber(rhs.windowNumber)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType1Command & operator=(
        const TopicJumpIntoExternalFileWithoutFontChangeType1Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->windowNumber =
                rhs.windowNumber;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileWithoutFontChangeType1Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    quint8 getWindowNumber() const
    {
        return this->windowNumber;
    };
};

class TopicJumpIntoExternalFileWithoutFontChangeType4Command : public
    TopicJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
public:
    TopicJumpIntoExternalFileWithoutFontChangeType4Command() :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(4,
        0), topicOffset(), nameOfExternalFile()
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType4Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(4,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType4Command(
        const TopicJumpIntoExternalFileWithoutFontChangeType4Command& rhs) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset), nameOfExternalFile(rhs.nameOfExternalFile)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType4Command & operator=(
        const TopicJumpIntoExternalFileWithoutFontChangeType4Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileWithoutFontChangeType4Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
};

class TopicJumpIntoExternalFileWithoutFontChangeType6Command : public
    TopicJumpIntoExternalFileWithoutFontChangeCommand
{
private:
    TopicOffset topicOffset;
    QString nameOfExternalFile;
    QString windowName;
public:
    TopicJumpIntoExternalFileWithoutFontChangeType6Command() :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(6,
        0), topicOffset(), nameOfExternalFile(), windowName()
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType6Command(qint16 sizeOfData,
        TopicOffset topicOffset,
        QString nameOfExternalFile,
        QString windowName) : TopicJumpIntoExternalFileWithoutFontChangeCommand(
        6,
        sizeOfData), topicOffset(topicOffset),
        nameOfExternalFile(nameOfExternalFile),
        windowName(windowName)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType6Command(
        const TopicJumpIntoExternalFileWithoutFontChangeType6Command& rhs) :
        TopicJumpIntoExternalFileWithoutFontChangeCommand(rhs),
        topicOffset(rhs.topicOffset), nameOfExternalFile(rhs.nameOfExternalFile),
        windowName(rhs.windowName)
    {
    };
    TopicJumpIntoExternalFileWithoutFontChangeType6Command & operator=(
        const TopicJumpIntoExternalFileWithoutFontChangeType6Command &rhs)
    {
        if (this != &rhs)
        {
            TopicJumpIntoExternalFileWithoutFontChangeCommand::operator =(rhs);
            this->topicOffset = rhs.topicOffset; this->nameOfExternalFile =
                rhs.nameOfExternalFile; this->windowName = rhs.windowName;
        }
        ; return *this;
    };
    virtual ~TopicJumpIntoExternalFileWithoutFontChangeType6Command()
    {
    };
    TopicOffset getTopicOffset() const
    {
        return this->topicOffset;
    };
    QString getNameOfExternalFile() const
    {
        return this->nameOfExternalFile;
    };
    QString getWindowName() const
    {
        return this->windowName;
    };
};

#endif /* FORMATTINGCOMMANDS_H_ */
