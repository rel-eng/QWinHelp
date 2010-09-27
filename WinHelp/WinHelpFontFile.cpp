/* WinHelp Font file implementation.

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

#include "WinHelpFontFile.h"

#include "InternalDirectoryFileHeader.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <stdexcept>

WinHelpFontFile::WinHelpFontFile() : numFaceNames(0), numDescriptors(0),
    faceNamesOffset(0), descriptorsOffset(0), numStyles(0), stylesOffset(0),
    numCharMapTables(0), charMapTablesOffset(0), stylesValid(false),
    charMapTablesValid(false), faceNames(), fontDescriptors(), fontStyles(),
    charMapTablesNames()
{
    PRINT_DBG("WinHelp font file default constructor");
}

WinHelpFontFile::WinHelpFontFile(QFile &file, qint64 off,
    QTextCodec *codec) : faceNames(), fontDescriptors(), fontStyles(),
    charMapTablesNames()
{
    PRINT_DBG("Loading WinHelp font file at %lld", off);
    if (codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    seekFile(file, off);
    InternalDirectoryFileHeader hdr(file, off);
    seekFile(file, off + InternalDirectoryFileHeader::size);
    this->numFaceNames = readUnsignedWord(file);
    PRINT_DBG("        NumFaceNames: %d", this->numFaceNames);
    this->numDescriptors = readUnsignedWord(file);
    PRINT_DBG("        NumDescriptors: %d", this->numDescriptors);
    this->faceNamesOffset = readUnsignedWord(file);
    PRINT_DBG("        FaceNamesOffset: %d", this->faceNamesOffset);
    this->descriptorsOffset = readUnsignedWord(file);
    PRINT_DBG("        DescriptorsOffset: %d", this->descriptorsOffset);
    this->numStyles = 0;
    this->stylesOffset = 0;
    this->stylesValid = false;
    this->numCharMapTables = 0;
    this->charMapTablesOffset = 0;
    this->charMapTablesValid = false;
    if(this->faceNamesOffset >= 12)
    {
        this->numStyles = readUnsignedWord(file);
        PRINT_DBG("        NumStyles: %d", this->numStyles);
        this->stylesOffset = readUnsignedWord(file);
        PRINT_DBG("        StylesOffset: %d", this->stylesOffset);
        this->stylesValid = true;
        if(this->faceNamesOffset >= 16)
        {
            this->numCharMapTables = readUnsignedWord(file);
            PRINT_DBG("        NumCharMapTables: %d", this->numCharMapTables);
            this->charMapTablesOffset = readUnsignedWord(file);
            PRINT_DBG("        CharMapTablesOffset: %d",
                this->charMapTablesOffset);
            this->charMapTablesValid = true;
        }
    }
    seekFile(file, off + InternalDirectoryFileHeader::size +
        static_cast<qint64>(this->faceNamesOffset));
    uint faceNameLength =
        (static_cast<uint>(this->descriptorsOffset) -
        static_cast<uint>(this->faceNamesOffset)) /
        static_cast<uint>(this->numFaceNames);
    for(quint16 i = 0; i < this->numFaceNames; i++)
    {
        this->faceNames.append(readFixedLengthString(file, faceNameLength,
                codec));
        PRINT_DBG("        FaceName: %s",
            this->faceNames.last().toLocal8Bit().data());
    }
    PRINT_DBG("        Font descriptors:");
    seekFile(file, off + InternalDirectoryFileHeader::size +
        static_cast<qint64>(this->descriptorsOffset));
    if(this->charMapTablesValid)
    {
        for(quint16 i = 0; i < this->numDescriptors; i++)
        {
            this->fontDescriptors.append(QSharedPointer<FontDescriptor>(
                    dynamic_cast<FontDescriptor *>(new MVBFontDescriptor(file,
                            file.pos()))));
        }
    }
    else
    {
        if(this->stylesValid)
        {
            for(quint16 i = 0; i < this->numDescriptors; i++)
            {
                this->fontDescriptors.append(QSharedPointer<FontDescriptor>(
                        dynamic_cast<FontDescriptor *>(new NewFontDescriptor(
                                file,
                                file.pos()))));
            }
        }
        else
        {
            for(quint16 i = 0; i < this->numDescriptors; i++)
            {
                this->fontDescriptors.append(QSharedPointer<FontDescriptor>(
                        dynamic_cast<FontDescriptor *>(new OldFontDescriptor(
                                file,
                                file.pos()))));
            }
        }
    }
    if(this->stylesValid)
    {
        PRINT_DBG("        Font styles:");
        seekFile(file, off + InternalDirectoryFileHeader::size +
            static_cast<qint64>(this->stylesOffset));
        if(this->charMapTablesValid)
        {
            for(quint16 i = 0; i < this->numStyles; i++)
            {
                this->fontStyles.append(FontStyle(file, file.pos(),
                        MVB_FONT_DESCRIPTOR, codec));
            }
        }
        else
        {
            for(quint16 i = 0; i < this->numStyles; i++)
            {
                this->fontStyles.append(FontStyle(file, file.pos(),
                        NEW_FONT_DESCRIPTOR, codec));
            }
        }
    }
    if(this->charMapTablesValid)
    {
        PRINT_DBG("        Font char map tables:");
        seekFile(file, off + InternalDirectoryFileHeader::size +
            static_cast<qint64>(this->stylesOffset));
        for(quint16 i = 0; i < this->numCharMapTables; i++)
        {
            this->charMapTablesNames.append(readFixedLengthString(file, 32,
                    codec));
            PRINT_DBG("        CharMap Table Name: %s",
                this->charMapTablesNames.last().toLocal8Bit().data());
        }
    }
    PRINT_DBG("WinHelp font file loaded successfully");
}

WinHelpFontFile::WinHelpFontFile(const WinHelpFontFile &rhs) : numFaceNames(rhs
    .numFaceNames), numDescriptors(rhs.numDescriptors),
    faceNamesOffset(rhs.faceNamesOffset),
    descriptorsOffset(rhs.descriptorsOffset),
    numStyles(rhs.numStyles), stylesOffset(rhs.stylesOffset),
    numCharMapTables(rhs.numCharMapTables),
    charMapTablesOffset(rhs.charMapTablesOffset), stylesValid(rhs.stylesValid),
    charMapTablesValid(rhs.charMapTablesValid), faceNames(rhs.faceNames),
    fontDescriptors(rhs.fontDescriptors), fontStyles(rhs.fontStyles),
    charMapTablesNames(rhs.charMapTablesNames)
{
    PRINT_DBG("WinHelp font file copy constructor");
}

WinHelpFontFile::~WinHelpFontFile()
{
    PRINT_DBG("WinHelp font file destructor");
}

const WinHelpFontFile & WinHelpFontFile::operator=(const WinHelpFontFile &rhs)
{
    PRINT_DBG("WinHelp font file assignment operator");
    if (this != &rhs)
    {
        this->numFaceNames = rhs.numFaceNames;
        this->numDescriptors = rhs.numDescriptors;
        this->faceNamesOffset = rhs.faceNamesOffset;
        this->descriptorsOffset = rhs.descriptorsOffset;
        this->numStyles = rhs.numStyles;
        this->stylesOffset = rhs.stylesOffset;
        this->numCharMapTables = rhs.numCharMapTables;
        this->charMapTablesOffset = rhs.charMapTablesOffset;
        this->stylesValid = rhs.stylesValid;
        this->charMapTablesValid = rhs.charMapTablesValid;
        this->faceNames = rhs.faceNames;
        this->fontDescriptors = rhs.fontDescriptors;
        this->fontStyles = rhs.fontStyles;
        this->charMapTablesNames = rhs.charMapTablesNames;
    }
    return *this;
}
