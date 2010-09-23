/* WinHelp icon class implementation.

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

#include "WinHelpIcon.h"

#include "Utils/IOUtils.h"
#include "Utils/DebugUtils.h"

#include <QtDebug>
#include <QScopedArrayPointer>
#include <QColor>

#include <stdexcept>

WinHelpIcon::WinHelpIcon() :
    images()
{
    PRINT_DBG("Icon default constructor");
}

WinHelpIcon::WinHelpIcon(const QList<QImage> &images) :
    images(images)
{
    PRINT_DBG("Icon constructor");
}

WinHelpIcon::WinHelpIcon(QFile &file, qint64 off) :
    images()
{
    PRINT_DBG("Loading icon");
    seekFile(file, off);
    quint16 magicIn = readUnsignedWord(file);
    if (magicIn != 0)
    {
        throw std::runtime_error("Not an icon resource header");
    }
    PRINT_DBG("        Icon resource header magic: %d", magicIn);
    quint16 resourceTypeIn = readUnsignedWord(file);
    if (resourceTypeIn != 1)
    {
        throw std::runtime_error("Resource is not an icon");
    }
    PRINT_DBG("        Icon resource header resource type: %d", resourceTypeIn);
    quint16 imagesCountIn = readUnsignedWord(file);
    PRINT_DBG("        Icon resource header images count: %d", imagesCountIn);
    QScopedArrayPointer<quint8> widths(new quint8[imagesCountIn]);
    QScopedArrayPointer<quint8> heights(new quint8[imagesCountIn]);
    QScopedArrayPointer<quint32> colorCounts(new quint32[imagesCountIn]);
    QScopedArrayPointer<quint16> colorPlanes(new quint16[imagesCountIn]);
    QScopedArrayPointer<quint16> bitsPerPixelCount(new quint16[imagesCountIn]);
    QScopedArrayPointer<qint64> bitmapSizes(new qint64[imagesCountIn]);
    QScopedArrayPointer<qint64> bitmapOffsets(new qint64[imagesCountIn]);
    for (int index = 0; index < imagesCountIn; index++)
    {
        widths[index] = readUnsignedByte(file);
        PRINT_DBG("        Icon image directory image header width: %d",
            widths[index]);
        heights[index] = readUnsignedByte(file);
        PRINT_DBG("        Icon image directory image header height: %d",
            heights[index]);
        colorCounts[index] = static_cast<quint32> (readUnsignedByte(file));
        PRINT_DBG("        Icon image directory image header color count: %d",
            colorCounts[index]);
        quint8 reservedIn = readUnsignedByte(file);
        if (reservedIn != 0)
        {
            qDebug()
            << "Icon image directory image header reserved value is non-zero";
        }
        PRINT_DBG("        Icon image directory image header reserved: %d",
            reservedIn);
        colorPlanes[index] = readUnsignedWord(file);
        PRINT_DBG("        Icon image directory image header color planes: %d",
            colorPlanes[index]);
        bitsPerPixelCount[index] = readUnsignedWord(file);
        PRINT_DBG(
            "        Icon image directory image header bits per pixel count: %d",
            bitsPerPixelCount[index]);
        bitmapSizes[index] = static_cast<qint64> (readUnsignedDWord(file));
        PRINT_DBG("        Icon image directory image header bitmap size: %lld",
            bitmapSizes[index]);
        bitmapOffsets[index] = static_cast<qint64> (readUnsignedDWord(file));
        PRINT_DBG(
            "        Icon image directory image header bitmap offset: %lld",
            bitmapOffsets[index]);
    }
    for (int index = 0; index < imagesCountIn; index++)
    {
        seekFile(file, off + bitmapOffsets[index]);
        quint32 headerSizeIn = readUnsignedDWord(file);
        PRINT_DBG("        Icon image header size: %d", headerSizeIn);
        if (headerSizeIn == 40)
        {
            qint32 widthIn = readSignedDWord(file);
            PRINT_DBG("        Icon image width: %d", widthIn);
            qint64 realWidth = static_cast<qint64> (widths[index]);
            if (realWidth == 0)
            {
                realWidth = static_cast<qint64> (widthIn);
            }
            qint32 heightIn = readSignedDWord(file);
            PRINT_DBG("        Icon image height: %d", heightIn);
            qint64 realHeight = static_cast<qint64> (heights[index]);
            if (realHeight == 0)
            {
                realHeight = (static_cast<qint64> (heightIn)) / (Q_INT64_C(2));
            }
            quint16 colorPlanesIn = readUnsignedWord(file);
            PRINT_DBG("        Icon image color planes: %d", colorPlanesIn);
            quint16 bitsPerPixelCountIn = readUnsignedWord(file);
            PRINT_DBG("        Icon image bits per pixel count: %d",
                bitsPerPixelCountIn);
            if (colorCounts[index] == 0)
            {
                if (colorPlanesIn == 1)
                {
                    if (bitsPerPixelCountIn == 1)
                    {
                        colorCounts[index] = 2;
                    }
                    else
                    {
                        if (bitsPerPixelCountIn == 4)
                        {
                            colorCounts[index] = 16;
                        }
                        else
                        {
                            if (bitsPerPixelCountIn == 8)
                            {
                                colorCounts[index] = 256;
                            }
                            else
                            {
                                if (bitsPerPixelCountIn != 32)
                                {
                                    colorCounts[index] = 1
                                        << bitsPerPixelCountIn;
                                }
                            }
                        }
                    }
                }
                else
                {
                    colorCounts[index] = 1 << (bitsPerPixelCountIn
                        * colorPlanesIn);
                }
            }
            quint32 compressionMethodIn = readUnsignedDWord(file);
            PRINT_DBG("        Icon image compression method: %d",
                compressionMethodIn);
            quint32 imageSizeIn = readUnsignedDWord(file);
            PRINT_DBG("        Icon image size: %d", imageSizeIn);
            qint32 horizontalResolutionIn = readSignedDWord(file);
            PRINT_DBG("        Icon image horizontal resolution: %d",
                horizontalResolutionIn);
            qint32 verticalResolutionIn = readSignedDWord(file);
            PRINT_DBG("        Icon image vertical resolution: %d",
                verticalResolutionIn);
            quint32 colorsInPaletteIn = readUnsignedDWord(file);
            PRINT_DBG("        Icon image colors in palette: %d",
                colorsInPaletteIn);
            quint32 importantColorsUsedIn = readUnsignedDWord(file);
            PRINT_DBG("        Icon image imporatnt colors used: %d",
                importantColorsUsedIn);
            if ((realWidth != 0) && (realHeight != 0) && ((colorCounts[index]
                        == 0) || (colorCounts[index] == 2) ||
                        (colorCounts[index]
                        == 16) || (colorCounts[index] == 256)))
            {
                QImage image(static_cast<int> (realWidth),
                    static_cast<int> (realHeight), QImage::Format_ARGB32);
                if (colorCounts[index] == 2)
                {
                    QScopedArrayPointer<QRgb> palette(new QRgb[2]);
                    for (int i = 0; i < 2; i++)
                    {
                        palette[i] = readBGRDword(file);
                    }
                    qint64 scanlineBytes = ((realWidth + Q_INT64_C(31))
                        / (Q_INT64_C(32))) * Q_INT64_C(4);
                    QScopedArrayPointer<quint8> xorImage(
                        new quint8[scanlineBytes * realHeight]);
                    QScopedArrayPointer<quint8> andImage(
                        new quint8[scanlineBytes * realHeight]);
                    if (file.read(reinterpret_cast<char *> (xorImage.data()),
                                (scanlineBytes * realHeight)) != (scanlineBytes
                            * realHeight))
                    {
                        throw std::runtime_error("Unable to read icon image");
                    }
                    if (file.read(reinterpret_cast<char *> (andImage.data()),
                                (scanlineBytes * realHeight)) != (scanlineBytes
                            * realHeight))
                    {
                        throw std::runtime_error("Unable to read icon image");
                    }
                    quint8 masks[] =
                    {
                        128, 64, 32, 16, 8, 4, 2, 1
                    };
                    for (qint64 row = 0; row < realHeight; row++)
                    {
                        for (qint64 col = 0; col < realWidth; col++)
                        {
                            int colorIndex = 0;
                            if ((xorImage[row * scanlineBytes + col
                                        / (Q_INT64_C(8))] & masks[col
                                        % (Q_INT64_C(8))]) != 0)
                            {
                                colorIndex = 1;
                            }
                            else
                            {
                                colorIndex = 0;
                            }
                            if ((andImage[row * scanlineBytes + col
                                        / (Q_INT64_C(8))] & masks[col
                                        % (Q_INT64_C(8))]) != 0)
                            {
                                image.setPixel(col, realHeight - Q_INT64_C(1)
                                    - row, qRgba(qRed(palette[colorIndex]),
                                        qGreen(palette[colorIndex]), qBlue(
                                            palette[colorIndex]), 0));
                            }
                            else
                            {
                                image.setPixel(col, realHeight - Q_INT64_C(1)
                                    - row, qRgba(qRed(palette[colorIndex]),
                                        qGreen(palette[colorIndex]), qBlue(
                                            palette[colorIndex]), 0xFF));
                            }
                        }
                    }
                }
                else
                {
                    if (colorCounts[index] == 16)
                    {
                        QScopedArrayPointer<QRgb> palette(new QRgb[16]);
                        for (int i = 0; i < 16; i++)
                        {
                            palette[i] = readBGRDword(file);
                        }
                        qint64 scanlineBytesXor = ((realWidth * Q_INT64_C(4)
                                + Q_INT64_C(31)) / (Q_INT64_C(32)))
                            * Q_INT64_C(4);
                        qint64 scanlineBytesAnd = ((realWidth + Q_INT64_C(31))
                            / (Q_INT64_C(32))) * Q_INT64_C(4);
                        QScopedArrayPointer<quint8> xorImage(
                            new quint8[scanlineBytesXor * realHeight]);
                        QScopedArrayPointer<quint8> andImage(
                            new quint8[scanlineBytesAnd * realHeight]);
                        if (file.read(
                                reinterpret_cast<char *> (xorImage.data()),
                                    (scanlineBytesXor * realHeight))
                            != (scanlineBytesXor * realHeight))
                        {
                            throw std::runtime_error(
                                "Unable to read icon image");
                        }
                        if (file.read(
                                reinterpret_cast<char *> (andImage.data()),
                                    (scanlineBytesAnd * realHeight))
                            != (scanlineBytesAnd * realHeight))
                        {
                            throw std::runtime_error(
                                "Unable to read icon image");
                        }
                        quint8 masks[] =
                        {
                            128, 64, 32, 16, 8, 4, 2, 1
                        };
                        for (qint64 row = 0; row < realHeight; row++)
                        {
                            for (qint64 col = 0; col < realWidth; col++)
                            {
                                quint8 colorIndex = 0;
                                if ((col & Q_INT64_C(1)) == Q_INT64_C(0))
                                {
                                    colorIndex = xorImage[row
                                        * scanlineBytesXor + col
                                        / Q_INT64_C(2)];
                                    colorIndex = colorIndex >> 4;
                                }
                                else
                                {
                                    colorIndex = xorImage[row
                                        * scanlineBytesXor + col
                                        / Q_INT64_C(2)] & 15;
                                }
                                if ((andImage[row * scanlineBytesAnd + col
                                            / (Q_INT64_C(8))] & masks[col
                                            % (Q_INT64_C(8))]) != 0)
                                {
                                    image.setPixel(
                                        col,
                                        realHeight - Q_INT64_C(1) - row,
                                        qRgba(
                                            qRed(
                                                palette[static_cast<int> (
                                                        colorIndex)]),
                                            qGreen(
                                                palette[static_cast<int> (
                                                        colorIndex)]),
                                            qBlue(
                                                palette[static_cast<int> (
                                                        colorIndex)]),
                                            0));
                                }
                                else
                                {
                                    image.setPixel(
                                        col,
                                        realHeight - Q_INT64_C(1) - row,
                                        qRgba(
                                            qRed(
                                                palette[static_cast<int> (
                                                        colorIndex)]),
                                            qGreen(
                                                static_cast<int> (palette[
                                                        colorIndex])),
                                            qBlue(
                                                palette[static_cast<int> (
                                                        colorIndex)]),
                                            0xFF));
                                }
                            }
                        }
                    }
                    else
                    {
                        if (colorCounts[index] == 256)
                        {
                            QScopedArrayPointer<QRgb> palette(new QRgb[256]);
                            for (int i = 0; i < 256; i++)
                            {
                                palette[i] = readBGRDword(file);
                            }
                            qint64 scanlineBytesXor = ((realWidth
                                    * Q_INT64_C(8) + Q_INT64_C(31))
                                / (Q_INT64_C(32))) * Q_INT64_C(4);
                            qint64 scanlineBytesAnd = ((realWidth
                                    + Q_INT64_C(31)) / (Q_INT64_C(32)))
                                * Q_INT64_C(4);
                            QScopedArrayPointer<quint8> xorImage(
                                new quint8[scanlineBytesXor * realHeight]);
                            QScopedArrayPointer<quint8> andImage(
                                new quint8[scanlineBytesAnd * realHeight]);
                            if (file.read(
                                    reinterpret_cast<char *> (xorImage.data()),
                                        (scanlineBytesXor * realHeight))
                                != (scanlineBytesXor * realHeight))
                            {
                                throw std::runtime_error(
                                    "Unable to read icon image");
                            }
                            if (file.read(
                                    reinterpret_cast<char *> (andImage.data()),
                                        (scanlineBytesAnd * realHeight))
                                != (scanlineBytesAnd * realHeight))
                            {
                                throw std::runtime_error(
                                    "Unable to read icon image");
                            }
                            quint8 masks[] =
                            {
                                128, 64, 32, 16, 8, 4, 2, 1
                            };
                            for (qint64 row = 0; row < realHeight; row++)
                            {
                                for (qint64 col = 0; col < realWidth; col++)
                                {
                                    quint8 colorIndex = 0;
                                    colorIndex = xorImage[row
                                        * scanlineBytesXor + col];
                                    if ((andImage[row * scanlineBytesAnd + col
                                                / (Q_INT64_C(8))] & masks[col
                                                % (Q_INT64_C(8))]) != 0)
                                    {
                                        image.setPixel(
                                            col,
                                            realHeight - Q_INT64_C(1) - row,
                                            qRgba(
                                                qRed(
                                                    palette[static_cast<int> (
                                                            colorIndex)]),
                                                qGreen(
                                                    palette[static_cast<int> (
                                                            colorIndex)]),
                                                qBlue(
                                                    palette[static_cast<int> (
                                                            colorIndex)]),
                                                0));
                                    }
                                    else
                                    {
                                        image.setPixel(
                                            col,
                                            realHeight - Q_INT64_C(1) - row,
                                            qRgba(
                                                qRed(
                                                    palette[static_cast<int> (
                                                            colorIndex)]),
                                                qGreen(
                                                    static_cast<int> (palette[
                                                            colorIndex])),
                                                qBlue(
                                                    palette[static_cast<int> (
                                                            colorIndex)]),
                                                0xFF));
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (colorCounts[index] == 0)
                            {
                                qint64 scanlineBytes = ((realWidth
                                        * Q_INT64_C(32) + Q_INT64_C(31))
                                    / (Q_INT64_C(32))) * Q_INT64_C(4);
                                QScopedArrayPointer<quint8> imageBytes(
                                    new quint8[scanlineBytes * realHeight]);
                                if (file.read(
                                        reinterpret_cast<char *> (imageBytes.
                                            data()),
                                            (scanlineBytes * realHeight))
                                    != (scanlineBytes * realHeight))
                                {
                                    throw std::runtime_error(
                                        "Unable to read icon image");
                                }
                                for (qint64 row = 0; row < realHeight; row++)
                                {
                                    for (qint64 col = 0; col < realWidth; col++)
                                    {
                                        quint8 blueIn = imageBytes[row
                                            * scanlineBytes + col
                                            * Q_INT64_C(4)];
                                        quint8 greenIn = imageBytes[row
                                            * scanlineBytes + col
                                            * Q_INT64_C(4) + Q_INT64_C(1)];
                                        quint8 redIn = imageBytes[row
                                            * scanlineBytes + col
                                            * Q_INT64_C(4) + Q_INT64_C(2)];
                                        quint8 alphaIn = imageBytes[row
                                            * scanlineBytes + col
                                            * Q_INT64_C(4) + Q_INT64_C(3)];
                                        image.setPixel(col, realHeight
                                            - Q_INT64_C(1) - row,
                                            qRgba(redIn, greenIn, blueIn,
                                                alphaIn));
                                    }
                                }
                            }
                        }
                    }
                }
                this->images.append(image);
            }
            else
            {
                qDebug()
                <<
                "Unable to load icon image due to unsupported image data type";
            }
        }
        else
        {
            seekFile(file, off + bitmapOffsets[index]);
            QImage image;
            QScopedArrayPointer<quint8> imageBytes(
                new quint8[bitmapSizes[index]]);
            if (file.read(reinterpret_cast<char *> (imageBytes.data()),
                    bitmapSizes[index]) != bitmapSizes[index])
            {
                throw std::runtime_error("Unable to read icon image");
            }
            if (image.loadFromData(
                    reinterpret_cast<const uchar *> (imageBytes.data()),
                    static_cast<int> (bitmapSizes[index])))
            {
                images.append(image);
            }
            else
            {
                qDebug()
                << "Unable to load icon image due to unsupported image format";
            }
        }
    }
    PRINT_DBG("Icon loaded successfully");
}

WinHelpIcon::WinHelpIcon(const WinHelpIcon& rhs) :
    images(rhs.images)
{
    PRINT_DBG("Icon copy constructor");
}

WinHelpIcon::~WinHelpIcon()
{
    PRINT_DBG("Icon destructor");
}

WinHelpIcon & WinHelpIcon::operator=(const WinHelpIcon & rhs)
{
    PRINT_DBG("Icon assignment operator");
    if (this != &rhs)
    {
        this->images = rhs.images;
    }
    return *this;
}

const QImage & WinHelpIcon::getImage(int index) const
{
    if ((index < 0) || (index >= images.size()))
    {
        throw new std::out_of_range("Index of image is out of range");
    }
    return this->images.at(index);
}

int WinHelpIcon::getImageCount() const
{
    return this->images.count();
}
