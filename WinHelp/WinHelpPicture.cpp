/* WinHelp picture implementation.

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

#include "WinHelpPicture.h"

#include "Utils/DebugUtils.h"
#include "Utils/IOUtils.h"
#include "Utils/CompressionUtils.h"

#include "PictureHeader.h"
#include "PictureInfoHeader.h"
#include "MetafileHeader.h"

#include <stdexcept>

#include <QtDebug>
#include <QPainter>
#include <QScopedArrayPointer>
#include <QBuffer>

WinHelpPicture::WinHelpPicture() : pictures()
{
    PRINT_DBG("WinHelp picture default constructor");
}

WinHelpPicture::WinHelpPicture(QIODevice &device, qint64 off) : pictures()
{
    PRINT_DBG("Reading WinHelp picture at: %lld", off);
    seekFile(device, off);
    PictureHeader pictHdr(device, off);
    for(int i = 0; i < pictHdr.getNumberOfPictures(); i++)
    {
        qint64 pictOffset = off +
            static_cast<qint64>(pictHdr.getPictureOffset(i));
        PictureInfoHeader pictInfoHdr(device, pictOffset);
        switch(pictInfoHdr.getPictureType())
        {
        case DDB_PICTURE:
        {
            BitmapHeader bitmapHdr(device, pictOffset + PictureInfoHeader::size);
            seekFile(device, pictOffset + bitmapHdr.getCompressedOffset());
            switch(pictInfoHdr.getPackingMethod())
            {
            case UNCOMPRESSED_PICTURE:
            {
                this->pictures.append(LoadDDB(device, pictOffset +
                                static_cast<qint64>(bitmapHdr.
                                    getCompressedOffset()),
                                static_cast<size_t>(bitmapHdr.getCompressedSize()),
                                bitmapHdr));
            }
            break;

            case RUNLEN_COMPRESSED_PICTURE:
            {
                QBuffer uncompressedBuffer;
                uncompressedBuffer.open(QIODevice::ReadWrite);
                size_t sz = unpackRLEUnknownOutputSize(device,
                            pictOffset +
                            static_cast<qint64>(bitmapHdr.getCompressedOffset()),
                            static_cast<size_t>(bitmapHdr.getCompressedSize()),
                            uncompressedBuffer);
                this->pictures.append(LoadDDB(uncompressedBuffer, Q_INT64_C(0),
                                sz, bitmapHdr));
                uncompressedBuffer.close();
            }
            break;

            case LZ77_COMPRESSED_PICTURE:
            {
                QBuffer uncompressedBuffer;
                uncompressedBuffer.open(QIODevice::ReadWrite);
                size_t sz = unpackLZ77UnknownOutputSize(device,
                            pictOffset +
                            static_cast<qint64>(bitmapHdr.getCompressedOffset()),
                            static_cast<size_t>(bitmapHdr.getCompressedSize()),
                            uncompressedBuffer);
                this->pictures.append(LoadDDB(uncompressedBuffer, Q_INT64_C(0),
                                sz, bitmapHdr));
                uncompressedBuffer.close();
            }
            break;

            case LZ77_AND_RUNLEN_COMPRESSED_PICTURE:
            {
                QBuffer unpackedBuffer;
                unpackedBuffer.open(QIODevice::ReadWrite);
                size_t szUnpacked = unpackLZ77UnknownOutputSize(device,
                            pictOffset +
                            static_cast<qint64>(bitmapHdr.getCompressedOffset()),
                            static_cast<size_t>(bitmapHdr.getCompressedSize()),
                            unpackedBuffer);
                QBuffer uncompressedBuffer;
                uncompressedBuffer.open(QIODevice::ReadWrite);
                size_t sz = unpackRLEUnknownOutputSize(unpackedBuffer,
                            Q_INT64_C(0),
                            szUnpacked,
                            uncompressedBuffer);
                this->pictures.append(LoadDDB(uncompressedBuffer, Q_INT64_C(0),
                                sz, bitmapHdr));
                uncompressedBuffer.close();
            }
            break;

            default:
                throw std::runtime_error("Unknown image compression method");
            }
        }
        break;

        case DIB_PICTURE:
        {
            BitmapHeader bitmapHdr(device, pictOffset + PictureInfoHeader::size);
            int paletteSize = bitmapHdr.getColorsUsed();
            QScopedArrayPointer<QRgb> palette;
            if(paletteSize != 0)
            {
                if(paletteSize > 256)
                {
                    throw std::runtime_error("Palette is too big");
                }
                QScopedArrayPointer<QRgb>(new QRgb[paletteSize]).swap(palette);
                for(int i = 0; i < paletteSize; i++)
                {
                    palette[i] = readBGRDword(device);
                }
            }
            seekFile(device, pictOffset + bitmapHdr.getCompressedOffset());
            switch(pictInfoHdr.getPackingMethod())
            {
            case UNCOMPRESSED_PICTURE:
            {
                this->pictures.append(LoadDIB(device, pictOffset +
                                static_cast<qint64>(bitmapHdr.
                                    getCompressedOffset()),
                                static_cast<size_t>(bitmapHdr.getCompressedSize()),
                                bitmapHdr, palette));
            }
            break;

            case RUNLEN_COMPRESSED_PICTURE:
            {
                QBuffer uncompressedBuffer;
                uncompressedBuffer.open(QIODevice::ReadWrite);
                size_t sz = unpackRLEUnknownOutputSize(device,
                            pictOffset +
                            static_cast<qint64>(bitmapHdr.getCompressedOffset()),
                            static_cast<size_t>(bitmapHdr.getCompressedSize()),
                            uncompressedBuffer);
                this->pictures.append(LoadDIB(uncompressedBuffer, Q_INT64_C(0),
                                sz, bitmapHdr, palette));
                uncompressedBuffer.close();
            }
            break;

            case LZ77_COMPRESSED_PICTURE:
            {
                QBuffer uncompressedBuffer;
                uncompressedBuffer.open(QIODevice::ReadWrite);
                size_t sz = unpackLZ77UnknownOutputSize(device,
                            pictOffset +
                            static_cast<qint64>(bitmapHdr.getCompressedOffset()),
                            static_cast<size_t>(bitmapHdr.getCompressedSize()),
                            uncompressedBuffer);
                this->pictures.append(LoadDIB(uncompressedBuffer, Q_INT64_C(0),
                                sz, bitmapHdr, palette));
                uncompressedBuffer.close();
            }
            break;

            case LZ77_AND_RUNLEN_COMPRESSED_PICTURE:
            {
                QBuffer unpackedBuffer;
                unpackedBuffer.open(QIODevice::ReadWrite);
                size_t szUnpacked = unpackLZ77UnknownOutputSize(device,
                            pictOffset +
                            static_cast<qint64>(bitmapHdr.getCompressedOffset()),
                            static_cast<size_t>(bitmapHdr.getCompressedSize()),
                            unpackedBuffer);
                QBuffer uncompressedBuffer;
                uncompressedBuffer.open(QIODevice::ReadWrite);
                size_t sz = unpackRLEUnknownOutputSize(unpackedBuffer,
                            Q_INT64_C(0),
                            szUnpacked,
                            uncompressedBuffer);
                this->pictures.append(LoadDIB(uncompressedBuffer, Q_INT64_C(0),
                                sz, bitmapHdr, palette));
                uncompressedBuffer.close();
            }
            break;

            default:
                throw std::runtime_error("Unknown image compression method");
            }
        }
        break;

        case METAFILE_PICTURE:
        {
            MetafileHeader metafileHdr(device,
                    pictOffset + PictureInfoHeader::size);
            qDebug() << "Metafiles are not supported";
            if((metafileHdr.getWidth() * metafileHdr.getHeight()) <= 1920000)
            {
                QImage unsupportedImage(
                        metafileHdr.getWidth(),
                        metafileHdr.getHeight(), QImage::Format_ARGB32);
                QPainter painter;
                painter.begin(&unsupportedImage);
                painter.fillRect(0, 0,
                        metafileHdr.getWidth(),
                        metafileHdr.getHeight(), Qt::white);
                painter.drawText(20, 20, "Unsupported");
                painter.end();
                this->pictures.append(unsupportedImage);
            }
            else
            {
                qDebug() << "Metafile is too large";
                QImage unsupportedImage(100, 100, QImage::Format_ARGB32);
                QPainter painter;
                painter.begin(&unsupportedImage);
                painter.fillRect(0, 0,
                        metafileHdr.getWidth(),
                        metafileHdr.getHeight(), Qt::white);
                painter.drawText(20, 20, "Unsupported");
                painter.end();
                this->pictures.append(unsupportedImage);
            }
        }
        break;

        default:
            throw std::runtime_error( "Unknown picture type");
        }
    }
    PRINT_DBG("WinHelp picture loaded successfully");
}

WinHelpPicture::WinHelpPicture(const WinHelpPicture &rhs) : pictures(rhs.
    pictures)
{
    PRINT_DBG("WinHelp picture copy constructor");
}

WinHelpPicture::~WinHelpPicture()
{
    PRINT_DBG("WinHelp picture destructor");
}

WinHelpPicture & WinHelpPicture::operator=(const WinHelpPicture &rhs)
{
    PRINT_DBG("WinHelp picture assignment operator");
    if(this != &rhs)
    {
        this->pictures = rhs.pictures;
    }
    return *this;
}

QImage WinHelpPicture::LoadDDB(QIODevice &device,
    qint64 offset,
    size_t size,
    const BitmapHeader &hdr)
{
    seekFile(device, offset);
    if((hdr.getWidth() * hdr.getHeight()) > 100000000)
    {
        qDebug() << "Image is too large";
        QImage errorImg(100, 100, QImage::Format_ARGB32);
        QPainter painter;
        painter.begin(&errorImg);
        painter.fillRect(0, 0, 100, 100, Qt::white);
        painter.drawText(10, 10, "Image is too large");
        painter.end();
        return errorImg;
    }
    QImage image(hdr.getWidth(), hdr.getHeight(), QImage::Format_ARGB32);
    int colorsUsed = hdr.getColorsUsed();
    if(colorsUsed == 2)
    {
        size_t scanlineBytes =
            static_cast<size_t>(((hdr.getWidth() + 31) / 32) * 4);
        size_t imageSize = scanlineBytes * static_cast<size_t>(hdr.getHeight());
        if(imageSize > 0x40000000)
        {
            throw std::runtime_error("Requested too much memory for 1 bpp DDB");
        }
        QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
        memset(reinterpret_cast<void *>(imageData.data()), 0, imageSize);
        qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
        fillBuffer(device, toRead,
            reinterpret_cast<void *>(imageData.data()), imageSize);
        quint8 masks[] = {
            128, 64, 32, 16, 8, 4, 2, 1
        };
        for (size_t row = 0; row < static_cast<size_t>(hdr.getHeight()); row++)
        {
            for (size_t col = 0; col < static_cast<size_t>(hdr.getWidth());
                col++)
            {
                if ((imageData[row * scanlineBytes + col /
                            8] & masks[col % 8]) != 0)
                {
                    image.setPixel(col, hdr.getHeight() - 1 - row,
                        qRgb(255, 255, 255));
                }
                else
                {
                    image.setPixel(col, hdr.getHeight() - 1 - row, qRgb(0, 0, 0));
                }
            }
        }
    }
    else
    {
        if(colorsUsed == 16)
        {
            size_t scanlineBytes =
                static_cast<size_t>(((hdr.getWidth() * 4 + 31) / 32) * 4);
            size_t imageSize = scanlineBytes *
                static_cast<size_t>(hdr.getHeight());
            if(imageSize > 0x40000000)
            {
                throw std::runtime_error(
                    "Requested too much memory for 4 bpp DDB");
            }
            QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
            memset(reinterpret_cast<void *>(imageData.data()), 0, imageSize);
            qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
            fillBuffer(device, toRead,
                reinterpret_cast<void *>(imageData.data()), imageSize);
            for (size_t row = 0; row < static_cast<size_t>(hdr.getHeight());
                row++)
            {
                for (size_t col = 0; col < static_cast<size_t>(hdr.getWidth());
                    col++)
                {
                    quint8 colorIndex = 0;
                    if ((col & 1) == 0)
                    {
                        colorIndex = imageData[row * scanlineBytes + col / 2];
                        colorIndex = colorIndex >> 4;
                    }
                    else
                    {
                        colorIndex =
                            imageData[row * scanlineBytes + col / 2] & 15;
                    }
                    int color = qRound(
                            (static_cast<qreal>(colorIndex) / 15.0) * 255.0);
                    image.setPixel(col, hdr.getHeight() - 1 - row,
                        qRgb(color, color, color));
                }
            }
        }
        else
        {
            if(colorsUsed == 256)
            {
                size_t scanlineBytes =
                    static_cast<size_t>(((hdr.getWidth() * 8 + 31) / 32) * 4);
                size_t imageSize = scanlineBytes *
                    static_cast<size_t>(hdr.getHeight());
                if(imageSize > 0x40000000)
                {
                    throw std::runtime_error(
                        "Requested too much memory for 8 bpp DDB");
                }
                QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
                memset(reinterpret_cast<void *>(imageData.data()), 0, imageSize);
                qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
                fillBuffer(device, toRead,
                    reinterpret_cast<void *>(imageData.data()), imageSize);
                for (size_t row = 0; row < static_cast<size_t>(hdr.getHeight());
                    row++)
                {
                    for (size_t col = 0; col < static_cast<size_t>(hdr.getWidth());
                        col++)
                    {
                        quint8 colorIndex =
                            imageData[row * scanlineBytes + col];
                        image.setPixel(col, hdr.getHeight() - 1 - row,
                            qRgb(colorIndex, colorIndex, colorIndex));
                    }
                }
            }
            else
            {
                if((colorsUsed == 0) && (hdr.getBitCount() == 24))
                {
                    size_t scanlineBytes =
                        ((hdr.getWidth() * 24 + 31) / 32) * 4;
                    size_t imageSize = scanlineBytes *
                        static_cast<size_t>(hdr.getHeight());
                    if(imageSize > 0x40000000)
                    {
                        throw std::runtime_error(
                            "Requested too much memory for 24 bpp DDB");
                    }
                    QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
                    memset(
                        reinterpret_cast<void *>(imageData.data()), 0,
                        imageSize);
                    qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
                    fillBuffer(device, toRead,
                        reinterpret_cast<void *>(imageData.data()), imageSize);
                    for (size_t row = 0;
                        row < static_cast<size_t>(hdr.getHeight()); row++)
                    {
                        for (size_t col = 0;
                            col < static_cast<size_t>(hdr.getWidth()); col++)
                        {
                            quint8 blueIn =
                                imageData[row * scanlineBytes + col * 3];
                            quint8 greenIn =
                                imageData[row * scanlineBytes + col * 3 + 1];
                            quint8 redIn =
                                imageData[row * scanlineBytes + col * 3 + 2];
                            image.setPixel(col, hdr.getHeight() - 1 - row,
                                qRgb(redIn, greenIn, blueIn));
                        }
                    }
                }
                else
                {
                    qDebug() << "Unknown image data format";
                    QImage errorImg(hdr.getWidth(),
                        hdr.getHeight(), QImage::Format_ARGB32);
                    QPainter painter;
                    painter.begin(&errorImg);
                    painter.fillRect(0, 0, hdr.getWidth(),
                        hdr.getHeight(), Qt::white);
                    painter.drawText(10, 10, "Unknown data format");
                    painter.end();
                    return errorImg;
                }
            }
        }
    }
    return image;
}

QImage WinHelpPicture::LoadDIB(QIODevice &device,
    qint64 offset,
    size_t size,
    const BitmapHeader &hdr,
    const QScopedArrayPointer<QRgb> &palette)
{
    seekFile(device, offset);
    if((hdr.getWidth() * hdr.getHeight()) > 100000000)
    {
        qDebug() << "Image is too large";
        QImage errorImg(100, 100, QImage::Format_ARGB32);
        QPainter painter;
        painter.begin(&errorImg);
        painter.fillRect(0, 0, 100, 100, Qt::white);
        painter.drawText(10, 10, "Image is too large");
        painter.end();
        return errorImg;
    }
    QImage image(hdr.getWidth(), hdr.getHeight(), QImage::Format_ARGB32);
    int colorsUsed = hdr.getColorsUsed();
    if(colorsUsed == 2)
    {
        size_t scanlineBytes =
            static_cast<size_t>(((hdr.getWidth() + 31) / 32) * 4);
        size_t imageSize = scanlineBytes * static_cast<size_t>(hdr.getHeight());
        if(imageSize > 0x40000000)
        {
            throw std::runtime_error("Requested too much memory for 1 bpp DIB");
        }
        QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
        memset(reinterpret_cast<void *>(imageData.data()), 0, imageSize);
        qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
        fillBuffer(device, toRead,
            reinterpret_cast<void *>(imageData.data()), imageSize);
        quint8 masks[] = {
            128, 64, 32, 16, 8, 4, 2, 1
        };
        for (size_t row = 0; row < static_cast<size_t>(hdr.getHeight()); row++)
        {
            for (size_t col = 0; col < static_cast<size_t>(hdr.getWidth());
                col++)
            {
                if ((imageData[row * scanlineBytes + col /
                            8] & masks[col % 8]) != 0)
                {
                    image.setPixel(col, hdr.getHeight() - 1 - row, palette[1]);
                }
                else
                {
                    image.setPixel(col, hdr.getHeight() - 1 - row, palette[0]);
                }
            }
        }
    }
    else
    {
        if(colorsUsed == 16)
        {
            size_t scanlineBytes =
                static_cast<size_t>(((hdr.getWidth() * 4 + 31) / 32) * 4);
            size_t imageSize = scanlineBytes *
                static_cast<size_t>(hdr.getHeight());
            if(imageSize > 0x40000000)
            {
                throw std::runtime_error(
                    "Requested too much memory for 4 bpp DIB");
            }
            QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
            memset(reinterpret_cast<void *>(imageData.data()), 0, imageSize);
            qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
            fillBuffer(device, toRead,
                reinterpret_cast<void *>(imageData.data()), imageSize);
            for (size_t row = 0; row < static_cast<size_t>(hdr.getHeight());
                row++)
            {
                for (size_t col = 0; col < static_cast<size_t>(hdr.getWidth());
                    col++)
                {
                    quint8 colorIndex = 0;
                    if ((col & 1) == 0)
                    {
                        colorIndex = imageData[row * scanlineBytes + col / 2];
                        colorIndex = colorIndex >> 4;
                    }
                    else
                    {
                        colorIndex =
                            imageData[row * scanlineBytes + col / 2] & 15;
                    }
                    image.setPixel(col, hdr.getHeight() - 1 - row,
                        palette[colorIndex]);
                }
            }
        }
        else
        {
            if(colorsUsed == 256)
            {
                size_t scanlineBytes =
                    static_cast<size_t>(((hdr.getWidth() * 8 + 31) / 32) * 4);
                size_t imageSize = scanlineBytes *
                    static_cast<size_t>(hdr.getHeight());
                if(imageSize > 0x40000000)
                {
                    throw std::runtime_error(
                        "Requested too much memory for 8 bpp DIB");
                }
                QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
                memset(reinterpret_cast<void *>(imageData.data()), 0, imageSize);
                qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
                fillBuffer(device, toRead,
                    reinterpret_cast<void *>(imageData.data()), imageSize);
                for (size_t row = 0; row < static_cast<size_t>(hdr.getHeight());
                    row++)
                {
                    for (size_t col = 0; col < static_cast<size_t>(hdr.getWidth());
                        col++)
                    {
                        quint8 colorIndex =
                            imageData[row * scanlineBytes + col];
                        image.setPixel(col, hdr.getHeight() - 1 - row,
                            palette[colorIndex]);
                    }
                }
            }
            else
            {
                if((colorsUsed == 0) && (hdr.getBitCount() == 24))
                {
                    size_t scanlineBytes =
                        ((hdr.getWidth() * 24 + 31) / 32) * 4;
                    size_t imageSize = scanlineBytes *
                        static_cast<size_t>(hdr.getHeight());
                    if(imageSize > 0x40000000)
                    {
                        throw std::runtime_error(
                            "Requested too much memory for 24 bpp DIB");
                    }
                    QScopedArrayPointer<quint8> imageData(new quint8[imageSize]);
                    memset(
                        reinterpret_cast<void *>(imageData.data()), 0,
                        imageSize);
                    qint64 toRead = static_cast<qint64>(qMin(size, imageSize));
                    fillBuffer(device, toRead,
                        reinterpret_cast<void *>(imageData.data()), imageSize);
                    for (size_t row = 0;
                        row < static_cast<size_t>(hdr.getHeight()); row++)
                    {
                        for (size_t col = 0;
                            col < static_cast<size_t>(hdr.getWidth()); col++)
                        {
                            quint8 blueIn =
                                imageData[row * scanlineBytes + col * 3];
                            quint8 greenIn =
                                imageData[row * scanlineBytes + col * 3 + 1];
                            quint8 redIn =
                                imageData[row * scanlineBytes + col * 3 + 2];
                            image.setPixel(col, hdr.getHeight() - 1 - row,
                                qRgb(redIn, greenIn, blueIn));
                        }
                    }
                }
                else
                {
                    qDebug() << "Unknown image data format";
                    QImage errorImg(hdr.getWidth(),
                        hdr.getHeight(), QImage::Format_ARGB32);
                    QPainter painter;
                    painter.begin(&errorImg);
                    painter.fillRect(0, 0, hdr.getWidth(),
                        hdr.getHeight(), Qt::white);
                    painter.drawText(10, 10, "Unknown data format");
                    painter.end();
                    return errorImg;
                }
            }
        }
    }
    return image;
}

QImage WinHelpPicture::getBestPicture() const
{
    if(this->pictures.count() == 0)
    {
        qDebug() << "Empty image";
        QImage emptyImage(320, 240, QImage::Format_ARGB32);
        QPainter painter;
        painter.begin(&emptyImage);
        painter.fillRect(0, 0, 320, 240, Qt::white);
        painter.drawText(10, 10, "Empty image");
        painter.end();
        return emptyImage;
    }
    return this->pictures.last();
}
