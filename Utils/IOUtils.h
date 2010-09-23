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

#include <cstddef>

void seekFile(QFile &file, qint64 offset);
quint8 readUnsignedByte(QFile &file);
qint8 readSignedByte(QFile &file);
quint16 readUnsignedWord(QFile &file);
qint16 readSignedWord(QFile &file);
quint32 readUnsignedDWord(QFile &file);
qint32 readSignedDWord(QFile &file);
quint64 readUnsignedQWord(QFile &file);
qint64 readSignedQWord(QFile &file);
void fillBuffer(QFile &file, qint64 length, void *buffer, size_t bufferSize);
QString readFixedLengthString(QFile &file, uint length, QTextCodec *codec);
QString readFixedLengthStringFromBuffer(const void *buffer,
    size_t bufferSize,
    size_t pos,
    uint length,
    QTextCodec *codec);
QString readNullTerminatedString(QFile &file, QTextCodec *codec, qint64 &length);
QRgb readRGBDword(QFile &file);
QRgb readRGBADword(QFile &file);
QRgb readBGRDword(QFile &file);
QRgb readBGRADword(QFile &file);
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

#endif /* IOUTILS_H_ */
