/* Window record as defined in HPJ project file class implementation.

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

#include "WindowHpjRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "SystemRecordHeader.h"

#include <QtDebug>

WindowHpjRecord::WindowHpjRecord() :
    typeIsValid(false), nameIsValid(false), captionIsValid(false), xIsValid(
    false), yIsValid(false), widthIsValid(false), heightIsValid(false),
    maximizeWindow(false), rgbIsValid(false), rgbNsrIsValid(false),
    windowsAlwaysOnTop(false), autoSizeHeight(false), windowType(),
    windowName(), windowCaption(), x(0), y(0), width(0), height(0),
    maximize(0), rgbColor(qRgb(0xFF, 0xFF, 0xFF)), rgbNsrColor(qRgb(
        0xFF, 0xFF, 0xFF))
{
    PRINT_DBG("WindowHpj record default constructor");
}

WindowHpjRecord::WindowHpjRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading WindowHpj record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    if (hdr.getRecordSize() < Q_INT64_C(90))
    {
        qDebug() << "WindowHpj record size is too small: "
                 << hdr.getRecordSize();
    }
    quint8 flagsLIn = readUnsignedByte(file);
    PRINT_DBG("        Flags low byte: %d", flagsLIn);
    this->typeIsValid = false;
    if ((flagsLIn & 0x1) != 0)
    {
        this->typeIsValid = true;
        PRINT_DBG("        Window type field is valid");
    }
    ;
    this->nameIsValid = false;
    if ((flagsLIn & 0x2) != 0)
    {
        this->nameIsValid = true;
        PRINT_DBG("        Window name field is valid");
    }
    ;
    this->captionIsValid = false;
    if ((flagsLIn & 0x4) != 0)
    {
        this->captionIsValid = true;
        PRINT_DBG("        Window caption field is valid");
    }
    ;
    this->xIsValid = false;
    if ((flagsLIn & 0x8) != 0)
    {
        this->xIsValid = true;
        PRINT_DBG("        X field is valid");
    }
    ;
    this->yIsValid = false;
    if ((flagsLIn & 0x10) != 0)
    {
        this->yIsValid = true;
        PRINT_DBG("        Y field is valid");
    }
    ;
    this->widthIsValid = false;
    if ((flagsLIn & 0x20) != 0)
    {
        this->widthIsValid = true;
        PRINT_DBG("        Width field is valid");
    }
    ;
    this->heightIsValid = false;
    if ((flagsLIn & 0x40) != 0)
    {
        this->heightIsValid = true;
        PRINT_DBG("        Height type field is valid");
    }
    ;
    this->maximizeWindow = false;
    if ((flagsLIn & 0x80) != 0)
    {
        this->maximizeWindow = true;
        PRINT_DBG("        Maximize field is valid");
    }
    ;
    quint8 flagsHIn = readUnsignedByte(file);
    PRINT_DBG("        Flags high byte: %d", flagsHIn);
    this->rgbIsValid = false;
    if ((flagsHIn & 0x1) != 0)
    {
        this->rgbIsValid = true;
        PRINT_DBG("        RGB field is valid");
    }
    ;
    this->rgbNsrIsValid = false;
    if ((flagsHIn & 0x2) != 0)
    {
        this->rgbNsrIsValid = true;
        PRINT_DBG("        RGB nsr field is valid");
    }
    ;
    this->windowsAlwaysOnTop = false;
    if ((flagsHIn & 0x4) != 0)
    {
        this->windowsAlwaysOnTop = true;
        PRINT_DBG("        Window always on top flag is set");
    }
    ;
    this->autoSizeHeight = false;
    if ((flagsHIn & 0x8) != 0)
    {
        this->autoSizeHeight = true;
        PRINT_DBG("        Auto size height flag is set");
    }
    ;
    this->windowType = readFixedLengthString(file, 10, codec);
    PRINT_DBG("        Window type: %s", this->windowType.toLocal8Bit().data());
    this->windowName = readFixedLengthString(file, 9, codec);
    PRINT_DBG("        Window name: %s", this->windowName.toLocal8Bit().data());
    this->windowCaption = readFixedLengthString(file, 51, codec);
    PRINT_DBG("        Window caption: %s",
        this->windowCaption.toLocal8Bit().data());
    this->x = readUnsignedWord(file);
    PRINT_DBG("        X: %d", this->x);
    this->y = readUnsignedWord(file);
    PRINT_DBG("        Y: %d", this->y);
    this->width = readUnsignedWord(file);
    PRINT_DBG("        Width: %d", this->width);
    this->height = readUnsignedWord(file);
    PRINT_DBG("        Height: %d", this->height);
    this->maximize = readUnsignedWord(file);
    PRINT_DBG("        Maximize: %d", this->maximize);
    this->rgbColor = readRGBDword(file);
    PRINT_DBG("        Red: %d", qRed(this->rgbColor));
    PRINT_DBG("        Green: %d", qGreen(this->rgbColor));
    PRINT_DBG("        Blue: %d", qBlue(this->rgbColor));
    this->rgbNsrColor = readRGBDword(file);
    PRINT_DBG("        Red nsr: %d", qRed(this->rgbNsrColor));
    PRINT_DBG("        Green nsr: %d", qGreen(this->rgbNsrColor));
    PRINT_DBG("        Blue nsr: %d", qBlue(this->rgbNsrColor));
    PRINT_DBG("WindowHpj record loaded successfully");
}

WindowHpjRecord::WindowHpjRecord(const WindowHpjRecord& rhs) :
    typeIsValid(rhs.typeIsValid), nameIsValid(rhs.nameIsValid), captionIsValid(
    rhs.captionIsValid), xIsValid(rhs.xIsValid),
    yIsValid(rhs.yIsValid), widthIsValid(rhs.widthIsValid),
    heightIsValid(rhs.heightIsValid),
    maximizeWindow(rhs.maximizeWindow), rgbIsValid(rhs.rgbIsValid),
    rgbNsrIsValid(rhs.rgbNsrIsValid), windowsAlwaysOnTop(
    rhs.windowsAlwaysOnTop),
    autoSizeHeight(rhs.autoSizeHeight), windowType(rhs.windowType),
    windowName(rhs.windowName), windowCaption(rhs.windowCaption), x(
    rhs.x), y(rhs.y), width(rhs.width), height(rhs.height),
    maximize(rhs.maximize), rgbColor(rhs.rgbColor), rgbNsrColor(
    rhs.rgbNsrColor)
{
    PRINT_DBG("WindowHpj record copy constructor");
}

WindowHpjRecord & WindowHpjRecord::operator=(const WindowHpjRecord & rhs)
{
    PRINT_DBG("WindowHpj record assignment operator");
    if (this != &rhs)
    {
        this->typeIsValid = rhs.typeIsValid;
        this->nameIsValid = rhs.nameIsValid;
        this->captionIsValid = rhs.captionIsValid;
        this->xIsValid = rhs.xIsValid;
        this->yIsValid = rhs.yIsValid;
        this->widthIsValid = rhs.widthIsValid;
        this->heightIsValid = rhs.heightIsValid;
        this->maximizeWindow = rhs.maximizeWindow;
        this->rgbIsValid = rhs.rgbIsValid;
        this->rgbNsrIsValid = rhs.rgbNsrIsValid;
        this->windowsAlwaysOnTop = rhs.windowsAlwaysOnTop;
        this->autoSizeHeight = rhs.autoSizeHeight;
        this->windowType = rhs.windowType;
        this->windowName = rhs.windowName;
        this->windowCaption = rhs.windowCaption;
        this->x = rhs.x;
        this->y = rhs.y;
        this->width = rhs.width;
        this->height = rhs.height;
        this->maximize = rhs.maximize;
        this->rgbColor = rhs.rgbColor;
        this->rgbNsrColor = rhs.rgbNsrColor;
    }
    return *this;
}

WindowHpjRecord::~WindowHpjRecord()
{
    PRINT_DBG("WindowHpj record destructor");
}

bool WindowHpjRecord::isTypeValid() const
{
    return this->typeIsValid;
}

bool WindowHpjRecord::isNameValid() const
{
    return this->nameIsValid;
}

bool WindowHpjRecord::isCaptionValid() const
{
    return this->captionIsValid;
}

bool WindowHpjRecord::isXValid() const
{
    return this->xIsValid;
}

bool WindowHpjRecord::isYValid() const
{
    return this->yIsValid;
}

bool WindowHpjRecord::isWidthValid() const
{
    return this->widthIsValid;
}

bool WindowHpjRecord::isHeightValid() const
{
    return this->heightIsValid;
}

bool WindowHpjRecord::isWindowMaximized() const
{
    return this->maximizeWindow;
}

bool WindowHpjRecord::isRgbValid() const
{
    return this->rgbIsValid;
}

bool WindowHpjRecord::isRgbNsrValid() const
{
    return this->rgbNsrIsValid;
}

bool WindowHpjRecord::isWindowsAlwaysOnTop() const
{
    return this->windowsAlwaysOnTop;
}

bool WindowHpjRecord::isAutoSizeHeight() const
{
    return this->autoSizeHeight;
}

QString WindowHpjRecord::getWindowType() const
{
    return this->windowType;
}

QString WindowHpjRecord::getWindowName() const
{
    return this->windowName;
}

QString WindowHpjRecord::getWindowCaption() const
{
    return this->windowCaption;
}

quint16 WindowHpjRecord::getX() const
{
    return this->x;
}

quint16 WindowHpjRecord::getY() const
{
    return this->y;
}

quint16 WindowHpjRecord::getWidth() const
{
    return this->width;
}

quint16 WindowHpjRecord::getHeight() const
{
    return this->height;
}

quint16 WindowHpjRecord::getMaximize() const
{
    return this->maximize;
}

QRgb WindowHpjRecord::getRgbColor()
{
    return this->rgbColor;
}

QRgb WindowHpjRecord::getRgbNsrColor()
{
    return this->rgbNsrColor;
}
