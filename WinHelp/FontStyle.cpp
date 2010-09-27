/* Font Style implementation.

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

#include "FontStyle.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

FontStyle::FontStyle() : styleNum(0), basedOnStyleNum(0), font(), styleName()
{
    PRINT_DBG("Font style default constructor");
}

FontStyle::FontStyle(QFile &file,
    qint64 off,
    FontDescriptorType type,
    QTextCodec *codec) : styleNum(0), basedOnStyleNum(0), font(), styleName()
{
    PRINT_DBG("Loading font style at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    this->styleNum = readUnsignedWord(file);
    PRINT_DBG("        StyleNum: %d", this->styleNum);
    this->basedOnStyleNum = readUnsignedWord(file);
    PRINT_DBG("        Based on StyleNum: %d", this->basedOnStyleNum);
    PRINT_DBG("        Font descriptor: ");
    switch(type)
    {
    case OLD_FONT_DESCRIPTOR:
        this->font =
            QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor *>(new
                OldFontDescriptor(file, file.pos())));
        break;

    case NEW_FONT_DESCRIPTOR:
        this->font =
            QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor *>(new
                NewFontDescriptor(file, file.pos())));
        break;

    case MVB_FONT_DESCRIPTOR:
        this->font =
            QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor *>(new
                NewFontDescriptor(file, file.pos())));
        break;

    default:
        throw std::runtime_error("Invalid font descriptor type");
    }
    seekFile(file, file.pos() + Q_INT64_C(35));
    this->styleName = readFixedLengthString(file, 65, codec);
    PRINT_DBG("        Style name: %s", this->styleName.toLocal8Bit().data());
    PRINT_DBG("Font style loaded successfully");
}

FontStyle::FontStyle(const FontStyle& rhs) : styleNum(rhs.styleNum),
    basedOnStyleNum(rhs.basedOnStyleNum), styleName(rhs.styleName)
{
    PRINT_DBG("Font style copy constructor");
    if(!rhs.font.isNull())
    {
        switch(rhs.font->getType())
        {
        case OLD_FONT_DESCRIPTOR:
            this->font =
                QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor *>(
                    new
                    OldFontDescriptor(rhs.font.dynamicCast<OldFontDescriptor>()
                        .operator
                        *())));
            break;

        case NEW_FONT_DESCRIPTOR:
            this->font =
                QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor *>(
                    new
                    NewFontDescriptor(rhs.font.dynamicCast<NewFontDescriptor>()
                        .operator
                        *())));
            break;

        case MVB_FONT_DESCRIPTOR:
            this->font =
                QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor *>(
                    new
                    MVBFontDescriptor(rhs.font.dynamicCast<MVBFontDescriptor>()
                        .operator
                        *())));
            break;

        default:
            throw std::runtime_error("Unknown font descriptor type");
        }
    }
}

FontStyle::~FontStyle()
{
    PRINT_DBG("Font style destructor");
}

FontStyle & FontStyle::operator=(const FontStyle & rhs)
{
    PRINT_DBG("Font style assignment operator");
    if (this != &rhs)
    {
        this->styleNum = rhs.styleNum;
        this->basedOnStyleNum = rhs.basedOnStyleNum;
        if(!this->font.isNull())
        {
            this->font.clear();
        }
        if(!rhs.font.isNull())
        {
            switch(rhs.font->getType())
            {
            case OLD_FONT_DESCRIPTOR:
                this->font =
                    QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor
                        *>(
                        new OldFontDescriptor(rhs.font.dynamicCast<
                                OldFontDescriptor>().
                            operator*())));
                break;

            case NEW_FONT_DESCRIPTOR:
                this->font =
                    QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor
                        *>(
                        new NewFontDescriptor(rhs.font.dynamicCast<
                                NewFontDescriptor>().
                            operator*())));
                break;

            case MVB_FONT_DESCRIPTOR:
                this->font =
                    QSharedPointer<FontDescriptor>(dynamic_cast<FontDescriptor
                        *>(
                        new MVBFontDescriptor(rhs.font.dynamicCast<
                                MVBFontDescriptor>().
                            operator*())));
                break;

            default:
                throw std::runtime_error("Unknown font descriptor type");
            }
        }
        this->styleName = rhs.styleName;
    }
    return *this;
}

quint16 FontStyle::getStyleNum() const
{
    return this->styleNum;
}

quint16 FontStyle::getBasedOnStyleNum() const
{
    return this->basedOnStyleNum;
}

QSharedPointer<FontDescriptor> FontStyle::getFont() const
{
    return this->font;
}

QString FontStyle::getStyleName() const
{
    return this->styleName;
}
