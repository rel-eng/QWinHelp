/* I/O functions prototypes.

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

#ifndef IOUTILS_H_
#define IOUTILS_H_

#include <QtGlobal>
#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QColor>
#include <QList>
#include <QIODevice>

#include <cstddef>

void seekFile(QIODevice &device, qint64 offset);
quint8 readUnsignedByte(QIODevice &device);
qint8 readSignedByte(QIODevice &device);
quint16 readUnsignedWord(QIODevice &device);
qint16 readSignedWord(QIODevice &device);
quint32 readUnsignedDWord(QIODevice &device);
qint32 readSignedDWord(QIODevice &device);
quint64 readUnsignedQWord(QIODevice &device);
qint64 readSignedQWord(QIODevice &device);
void fillBuffer(QIODevice &device,
    qint64 length,
    void *buffer,
    size_t bufferSize);
QString readFixedLengthString(QFile &file, uint length, QTextCodec *codec);
QString readFixedLengthStringFromBuffer(const void *buffer,
    size_t bufferSize,
    size_t pos,
    uint length,
    QTextCodec *codec);
QString readNullTerminatedString(QFile &file, QTextCodec *codec, qint64 &length);
QRgb readRGBDword(QIODevice &device);
QRgb readRGBADword(QIODevice &device);
QRgb readBGRDword(QIODevice &device);
QRgb readBGRADword(QIODevice &device);
QRgb readRGBBytes(QIODevice &device);
QRgb getRGBDword(const void *src);
QRgb getRGBADword(const void *src);
QRgb getBGRDword(const void *src);
QRgb getBGRADword(const void *src);
QRgb getRGBBytes(const void *src);
quint8 getUnsignedByte(const void *src);
qint8 getSignedByte(const void *src);
quint16 getUnsignedWord(const void *src);
qint16 getSignedWord(const void *src);
quint32 getUnsignedDWord(const void *src);
qint32 getSignedDWord(const void *src);
quint64 getUnsignedQWord(const void *src);
qint64 getSignedQWord(const void *src);
size_t copyBytesFromBuffer(const void *src,
    size_t srcSize,
    size_t srcOffset,
    size_t length,
    void *dest,
    size_t destSize);
QList<QString> extractNullTerminatedStrings(const void *src,
    size_t srcSize,
    QTextCodec *codec);
QString getNullTerminatedString(const void *src,
    size_t offset,
    size_t srcSize,
    QTextCodec *codec,
    size_t &stringLength);
void checkOffsetLengthSize(size_t offset, size_t length, size_t size);
quint16 getCompressedUnsignedWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead);
qint16 getCompressedSignedWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead);
quint32 getCompressedUnsignedDWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead);
qint32 getCompressedSignedDWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead);
quint16 readCompressedUnsignedWord(QIODevice &device);
qint16 readCompressedSignedWord(QIODevice &device);
quint32 readCompressedUnsignedDWord(QIODevice &device);
qint32 readCompressedSignedDWord(QIODevice &device);

#endif /* IOUTILS_H_ */
