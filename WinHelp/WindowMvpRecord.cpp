/* Window record as defined in MVP project file class implementation.

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

#include "WindowMvpRecord.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include "SystemRecordHeader.h"

#include <QtDebug>

WindowMvpRecord::WindowMvpRecord() :
    typeIsValid(false), nameIsValid(false), captionIsValid(false), xIsValid(
    false), yIsValid(false), widthIsValid(false), heightIsValid(false),
    maximizeWindow(false), rgbIsValid(false), rgbNsrIsValid(false),
    windowsAlwaysOnTop(false), autoSizeHeight(false), windowType(),
    windowName(), windowCaption(), x(0), y(0), width(0), height(0),
    maximize(0), rgbColor(qRgb(0xFF, 0xFF, 0xFF)), rgbNsrColor(qRgb(
        0xFF, 0xFF, 0xFF)), rgbColor3(qRgb(0xFF, 0xFF, 0xFF)),
    x2(0), y2(0), x3(0), y3(0), width2(0), height2(0)
{
    PRINT_DBG("WindowMvp record default constructor");
}

WindowMvpRecord::WindowMvpRecord(QFile &file, qint64 off, QTextCodec *codec)
{
    PRINT_DBG("Reading WindowMvp record at %lld", off);
    seekFile(file, off);
    SystemRecordHeader hdr(file, off);
    if (hdr.getRecordSize() < Q_INT64_C(108))
    {
        qDebug() << "WindowMvp record size is too small: "
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
    quint8 moreFlagsIn = readUnsignedByte(file);
    PRINT_DBG("        More flags byte: %d", moreFlagsIn);
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
    quint8 unknownIn = readUnsignedByte(file);
    PRINT_DBG("        Unknown byte: %d", unknownIn);
    this->rgbNsrColor = readRGBDword(file);
    PRINT_DBG("        Red nsr: %d", qRed(this->rgbNsrColor));
    PRINT_DBG("        Green nsr: %d", qGreen(this->rgbNsrColor));
    PRINT_DBG("        Blue nsr: %d", qBlue(this->rgbNsrColor));
    this->rgbColor3 = readRGBDword(file);
    PRINT_DBG("        Red 3: %d", qRed(this->rgbColor3));
    PRINT_DBG("        Green 3: %d", qGreen(this->rgbColor3));
    PRINT_DBG("        Blue 3: %d", qBlue(this->rgbColor3));
    this->x2 = readUnsignedWord(file);
    PRINT_DBG("        X2: %d", this->x2);
    this->y2 = readUnsignedWord(file);
    PRINT_DBG("        Y2: %d", this->y2);
    this->width2 = readUnsignedWord(file);
    PRINT_DBG("        Width2: %d", this->width2);
    this->height2 = readUnsignedWord(file);
    PRINT_DBG("        Height2: %d", this->height2);
    this->x3 = readUnsignedWord(file);
    PRINT_DBG("        X3: %d", this->x3);
    this->y3 = readUnsignedWord(file);
    PRINT_DBG("        Y3: %d", this->y3);
    PRINT_DBG("WindowMvp record loaded successfully");
}

WindowMvpRecord::WindowMvpRecord(const WindowMvpRecord& rhs) :
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
    rhs.rgbNsrColor), rgbColor3(rhs.rgbColor3), x2(rhs.x2), y2(
    rhs.y2), x3(rhs.x3), y3(rhs.y3), width2(rhs.width2),
    height2(rhs.height2)
{
    PRINT_DBG("WindowMvp record copy constructor");
}

WindowMvpRecord & WindowMvpRecord::operator=(const WindowMvpRecord & rhs)
{
    PRINT_DBG("WindowMvp record assignment operator");
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
        this->rgbColor3 = rhs.rgbColor3;
        this->x2 = rhs.x2;
        this->y2 = rhs.y2;
        this->x3 = rhs.y3;
        this->y3 = rhs.y3;
    }
    return *this;
}

WindowMvpRecord::~WindowMvpRecord()
{
    PRINT_DBG("WindowMvp record destructor");
}

bool WindowMvpRecord::isTypeValid() const
{
    return this->typeIsValid;
}

bool WindowMvpRecord::isNameValid() const
{
    return this->nameIsValid;
}

bool WindowMvpRecord::isCaptionValid() const
{
    return this->captionIsValid;
}

bool WindowMvpRecord::isXValid() const
{
    return this->xIsValid;
}

bool WindowMvpRecord::isYValid() const
{
    return this->yIsValid;
}

bool WindowMvpRecord::isWidthValid() const
{
    return this->widthIsValid;
}

bool WindowMvpRecord::isHeightValid() const
{
    return this->heightIsValid;
}

bool WindowMvpRecord::isWindowMaximized() const
{
    return this->maximizeWindow;
}

bool WindowMvpRecord::isRgbValid() const
{
    return this->rgbIsValid;
}

bool WindowMvpRecord::isRgbNsrValid() const
{
    return this->rgbNsrIsValid;
}

bool WindowMvpRecord::isWindowsAlwaysOnTop() const
{
    return this->windowsAlwaysOnTop;
}

bool WindowMvpRecord::isAutoSizeHeight() const
{
    return this->autoSizeHeight;
}

QString WindowMvpRecord::getWindowType() const
{
    return this->windowType;
}

QString WindowMvpRecord::getWindowName() const
{
    return this->windowName;
}

QString WindowMvpRecord::getWindowCaption() const
{
    return this->windowCaption;
}

quint16 WindowMvpRecord::getX() const
{
    return this->x;
}

quint16 WindowMvpRecord::getY() const
{
    return this->y;
}

quint16 WindowMvpRecord::getWidth() const
{
    return this->width;
}

quint16 WindowMvpRecord::getHeight() const
{
    return this->height;
}

quint16 WindowMvpRecord::getMaximize() const
{
    return this->maximize;
}

QRgb WindowMvpRecord::getRgbColor()
{
    return this->rgbColor;
}

QRgb WindowMvpRecord::getRgbNsrColor()
{
    return this->rgbNsrColor;
}

QRgb WindowMvpRecord::getRgbColor3()
{
    return this->rgbColor3;
}

quint16 WindowMvpRecord::getX2() const
{
    return this->x2;
}

quint16 WindowMvpRecord::getY2() const
{
    return this->y2;
}

quint16 WindowMvpRecord::getX3() const
{
    return this->x3;
}

quint16 WindowMvpRecord::getY3() const
{
    return this->y3;
}

quint16 WindowMvpRecord::getWidth2() const
{
    return this->width2;
}

quint16 WindowMvpRecord::getHeight2() const
{
    return this->height2;
}
