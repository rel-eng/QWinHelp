/* I/O functions implementation.

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

#include "IOUtils.h"

#include <QtEndian>
#include <QScopedArrayPointer>
#include <QScopedPointer>
#include <QTextDecoder>

#include <stdexcept>

void seekFile(QFile &file, qint64 offset)
{
    if (!file.seek(offset))
    {
        throw std::runtime_error("Unable to seek file");
    }
}

quint8 readUnsignedByte(QFile &file)
{
    quint8 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    return result;
}

qint8 readSignedByte(QFile &file)
{
    qint8 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    return result;
}

quint16 readUnsignedWord(QFile &file)
{
    quint16 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(2)) != Q_INT64_C(2))
    {
        throw std::runtime_error("Unable to read file");
    }
    result = qFromLittleEndian(result);
    return result;
}

qint16 readSignedWord(QFile &file)
{
    qint16 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(2)) != Q_INT64_C(2))
    {
        throw std::runtime_error("Unable to read file");
    }
    result = qFromLittleEndian(result);
    return result;
}

quint32 readUnsignedDWord(QFile &file)
{
    quint32 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(4)) != Q_INT64_C(4))
    {
        throw std::runtime_error("Unable to read file");
    }
    result = qFromLittleEndian(result);
    return result;
}

qint32 readSignedDWord(QFile &file)
{
    qint32 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(4)) != Q_INT64_C(4))
    {
        throw std::runtime_error("Unable to read file");
    }
    result = qFromLittleEndian(result);
    return result;
}

quint64 readUnsignedQWord(QFile &file)
{
    quint64 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(8)) != Q_INT64_C(8))
    {
        throw std::runtime_error("Unable to read file");
    }
    result = qFromLittleEndian(result);
    return result;
}

qint64 readSignedQWord(QFile &file)
{
    qint64 result = 0;
    if (file.read(reinterpret_cast<char *> (&result),
            Q_INT64_C(8)) != Q_INT64_C(8))
    {
        throw std::runtime_error("Unable to read file");
    }
    result = qFromLittleEndian(result);
    return result;
}

void fillBuffer(QFile &file, qint64 length, void *buffer, size_t bufferSize)
{
    if(length < Q_INT64_C(0))
    {
        throw std::runtime_error("Invalid length parameter value");
    }
    if(bufferSize < static_cast<size_t>(length))
    {
        throw std::runtime_error("Buffer is too small");
    }
    if (file.read(reinterpret_cast<char *> (buffer), length) != length)
    {
        throw std::runtime_error("Unable to read file");
    }
}

QString readFixedLengthString(QFile &file, uint length, QTextCodec *codec)
{
    if(codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    QScopedArrayPointer<quint8> stringIn(
        new quint8[static_cast<size_t>(length)]);
    if (file.read(reinterpret_cast<char *> (stringIn.data()),
            static_cast<qint64>(length)) != static_cast<qint64>(length))
    {
        throw std::runtime_error("Unable to read file");
    }
    uint stringLength = qstrnlen(
        reinterpret_cast<char *> (stringIn.data()), length);
    QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
    if (decoder.isNull())
    {
        throw std::runtime_error("Unable to create text decoder");
    }
    return decoder->toUnicode(
        reinterpret_cast<const char *> (stringIn.data()),
        static_cast<int> (stringLength));
}

QString readFixedLengthStringFromBuffer(const void *buffer,
    size_t bufferSize,
    size_t pos,
    uint length,
    QTextCodec *codec)
{
    if(codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    QScopedArrayPointer<quint8> stringIn(
        new quint8[static_cast<size_t>(length)]);
    if((pos + static_cast<size_t>(length)) <= bufferSize)
    {
        size_t last = pos + static_cast<size_t>(length);
        size_t strPtr = static_cast<size_t>(0);
        for(size_t ptr = pos; ptr < last; ptr++)
        {
            stringIn[strPtr] = (reinterpret_cast<const quint8 *>(buffer))[ptr];
            strPtr++;
        }
    }
    else
    {
        size_t last = bufferSize;
        size_t strPtr = static_cast<size_t>(0);
        for(size_t ptr = pos; ptr < last; ptr++)
        {
            stringIn[strPtr] = (reinterpret_cast<const quint8 *>(buffer))[ptr];
            strPtr++;
        }
        for(size_t ptr = strPtr; ptr < static_cast<size_t>(length); ptr++)
        {
            stringIn[ptr] = 0;
        }
    }
    uint stringLength = qstrnlen(
        reinterpret_cast<char *> (stringIn.data()), length);
    QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
    if (decoder.isNull())
    {
        throw std::runtime_error("Unable to create text decoder");
    }
    return decoder->toUnicode(
        reinterpret_cast<const char *> (stringIn.data()),
        static_cast<int> (stringLength));
}

QString readNullTerminatedString(QFile &file, QTextCodec *codec, qint64 &length)
{
    if(codec == NULL)
    {
        throw std::runtime_error("Codec is NULL");
    }
    qint64 pos = file.pos();
    while (true)
    {
        quint8 nextSymbol = 0;
        if (file.read(reinterpret_cast<char *> (&nextSymbol),
                Q_INT64_C(1)) != Q_INT64_C(1))
        {
            throw std::runtime_error("Unable to read file");
        }
        if (nextSymbol == 0)
        {
            break;
        }
    }
    qint64 stringLength = file.pos() - pos;
    length = stringLength;
    seekFile(file, pos);
    QScopedArrayPointer<quint8> stringIn(new quint8[stringLength]);
    if (file.read(reinterpret_cast<char *> (stringIn.data()),
            stringLength) != stringLength)
    {
        throw std::runtime_error("Unable to read file");
    }
    QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
    if (decoder.isNull())
    {
        throw std::runtime_error("Unable to create text decoder");
    }
    return decoder->toUnicode(
        reinterpret_cast<const char *> (stringIn.data()),
        static_cast<int> (stringLength - Q_INT64_C(1)));
}

QRgb readRGBDword(QFile &file)
{
    quint8 redIn = 0;
    if (file.read(reinterpret_cast<char *> (&redIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 greenIn = 0;
    if (file.read(reinterpret_cast<char *> (&greenIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 blueIn = 0;
    if (file.read(reinterpret_cast<char *> (&blueIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 alphaIn = 0;
    if (file.read(reinterpret_cast<char *> (&alphaIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    return qRgb(static_cast<int>(redIn),
        static_cast<int>(greenIn),
        static_cast<int>(blueIn));
}

QRgb readRGBADword(QFile &file)
{
    quint8 redIn = 0;
    if (file.read(reinterpret_cast<char *> (&redIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 greenIn = 0;
    if (file.read(reinterpret_cast<char *> (&greenIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 blueIn = 0;
    if (file.read(reinterpret_cast<char *> (&blueIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 alphaIn = 0;
    if (file.read(reinterpret_cast<char *> (&alphaIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    return qRgba(static_cast<int>(redIn),
        static_cast<int>(greenIn),
        static_cast<int>(blueIn),
        static_cast<int>(alphaIn));
}

QRgb readBGRDword(QFile &file)
{
    quint8 blueIn = 0;
    if (file.read(reinterpret_cast<char *> (&blueIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 greenIn = 0;
    if (file.read(reinterpret_cast<char *> (&greenIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 redIn = 0;
    if (file.read(reinterpret_cast<char *> (&redIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 alphaIn = 0;
    if (file.read(reinterpret_cast<char *> (&alphaIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    return qRgb(static_cast<int>(redIn),
        static_cast<int>(greenIn),
        static_cast<int>(blueIn));
}

QRgb readBGRADword(QFile &file)
{
    quint8 blueIn = 0;
    if (file.read(reinterpret_cast<char *> (&blueIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 greenIn = 0;
    if (file.read(reinterpret_cast<char *> (&greenIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 redIn = 0;
    if (file.read(reinterpret_cast<char *> (&redIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    quint8 alphaIn = 0;
    if (file.read(reinterpret_cast<char *> (&alphaIn),
            Q_INT64_C(1)) != Q_INT64_C(1))
    {
        throw std::runtime_error("Unable to read file");
    }
    return qRgba(static_cast<int>(redIn),
        static_cast<int>(greenIn),
        static_cast<int>(blueIn),
        static_cast<int>(alphaIn));
}

quint8 getUnsignedByte(const void *src)
{
    quint8 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 1);
    return result;
}

qint8 getSignedByte(const void *src)
{
    qint8 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 1);
    return result;
}

quint16 getUnsignedWord(const void *src)
{
    quint16 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 2);
    result = qFromLittleEndian(result);
    return result;
}

qint16 getSignedWord(const void *src)
{
    qint16 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 2);
    result = qFromLittleEndian(result);
    return result;
}

quint32 getUnsignedDWord(const void *src)
{
    quint32 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 4);
    result = qFromLittleEndian(result);
    return result;
}

qint32 getSignedDWord(const void *src)
{
    qint32 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 4);
    result = qFromLittleEndian(result);
    return result;
}

quint64 getUnsignedQWord(const void *src)
{
    quint64 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 8);
    result = qFromLittleEndian(result);
    return result;
}

qint64 getSignedQWord(const void *src)
{
    qint64 result = 0;
    memcpy(reinterpret_cast<void *>(&result), src, 8);
    result = qFromLittleEndian(result);
    return result;
}

size_t copyBytesFromBuffer(const void *src,
    size_t srcSize,
    size_t srcOffset,
    size_t length,
    void *dest,
    size_t destSize)
{
    size_t toCopy = length;
    if(destSize < toCopy)
    {
        toCopy = destSize;
    }
    if(srcOffset + toCopy > srcSize)
    {
        toCopy = srcSize - srcOffset;
    }
    if(srcOffset >= srcSize)
    {
        toCopy = 0;
    }
    if(toCopy > 0)
    {
        memcpy(dest,
            reinterpret_cast<const void *>(reinterpret_cast<const quint8 *>(src)
                +
                srcOffset), toCopy);
    }
    else
    {
        toCopy = 0;
    }
    return toCopy;
}

QString getNullTerminatedString(const void *src,
    size_t offset,
    size_t srcSize,
    QTextCodec *codec,
    size_t &stringLength)
{
    size_t ptr = offset;
    size_t strEnd = offset;
    bool nullFound = false;
    while(ptr < srcSize)
    {
        if((reinterpret_cast<const quint8 *>(src))[ptr] == 0)
        {
            strEnd = ptr;
            nullFound = true;
            break;
        }
        else
        {
            ptr++;
        }
    }
    if(!nullFound)
    {
        throw std::runtime_error("No end of string found");
    }
    if((strEnd + static_cast<size_t>(1)) < offset)
    {
        throw std::runtime_error("No end of string found");
    }
    size_t strLen = strEnd - offset + static_cast<size_t>(1);
    stringLength = strLen;
    if(strLen > static_cast<size_t>(1))
    {
        QScopedArrayPointer<quint8> stringIn(new quint8[strLen]);
        memcpy(reinterpret_cast<void *>(stringIn.data()),
            reinterpret_cast<const void *>(reinterpret_cast<const quint8 *>(src)
                +
                offset), strLen);
        QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
        if (decoder.isNull())
        {
            throw std::runtime_error("Unable to create text decoder");
        }
        return decoder->toUnicode(reinterpret_cast<const char *> (stringIn.data(
                    )), static_cast<int> (strLen - static_cast<size_t>(1)));
    }
    else
    {
        return QString();
    }
}

QList<QString> extractNullTerminatedStrings(const void *src,
    size_t srcSize,
    QTextCodec *codec)
{
    QList<QString> result;
    size_t ptr = static_cast<size_t>(0);
    size_t strStart = static_cast<size_t>(0);
    const quint8 *source = reinterpret_cast<const quint8 *>(src);
    while(ptr < srcSize)
    {
        if(source[ptr] == 0)
        {
            size_t strLen = ptr - strStart + static_cast<size_t>(1);
            if(strLen > static_cast<size_t>(1))
            {
                QScopedArrayPointer<quint8> stringIn(new quint8[strLen]);
                memcpy(
                    reinterpret_cast<void *>(stringIn.data()),
                    reinterpret_cast<const void *>(source + strStart), strLen);
                strStart = ptr + static_cast<size_t>(1);
                QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
                if (decoder.isNull())
                {
                    throw std::runtime_error("Unable to create text decoder");
                }
                result.append(decoder->toUnicode(reinterpret_cast<const char *> (
                            stringIn.data()),
                        static_cast<int> (strLen - static_cast<size_t>(1))));
            }
            else
            {
                strStart = ptr + static_cast<size_t>(1);
                result.append(QString());
            }
        }
        ptr++;
    }
    if(ptr - strStart > static_cast<size_t>(0))
    {
        size_t strLen = ptr - strStart;
        QScopedArrayPointer<quint8> stringIn(new quint8[strLen]);
        memcpy(reinterpret_cast<void *>(stringIn.data()),
            reinterpret_cast<const void *>(source + strStart), strLen);
        QScopedPointer<QTextDecoder> decoder(codec->makeDecoder());
        if (decoder.isNull())
        {
            throw std::runtime_error("Unable to create text decoder");
        }
        result.append(decoder->toUnicode(reinterpret_cast<const char *> (
                    stringIn.data()), strLen));
    }
    return result;
}

void checkOffsetLengthSize(size_t offset, size_t length, size_t size)
{
    if(offset + length > size)
    {
        throw std::runtime_error("Memory overrun");
    }
}

quint16 getCompressedUnsignedWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead)
{
    checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
    quint8 lowByte =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + offset));
    if((lowByte & 1) != 0)
    {
        checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
        bytesRead = static_cast<size_t>(2);
        return getUnsignedWord(reinterpret_cast<const void *>(reinterpret_cast<
                    const quint8 *>(src) + offset)) / 2;
    }
    else
    {
        bytesRead = static_cast<size_t>(1);
        return static_cast<quint16>(lowByte) / 2;
    }
}

qint16 getCompressedSignedWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead)
{
    checkOffsetLengthSize(offset, static_cast<size_t>(1), srcSize);
    quint8 lowByte =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + offset));
    if((lowByte & 1) != 0)
    {
        checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
        bytesRead = static_cast<size_t>(2);
        return static_cast<qint16>((static_cast<qint32>(getUnsignedWord(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8 *>(src) +
                            offset))) - 0x8000) / 2);
    }
    else
    {
        bytesRead = static_cast<size_t>(1);
        return static_cast<qint16>((static_cast<qint32>(lowByte) - 0x80) / 2);
    }
}

quint32 getCompressedUnsignedDWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead)
{
    checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
    quint8 lowByte =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + offset));
    if((lowByte & 1) != 0)
    {
        checkOffsetLengthSize(offset, static_cast<size_t>(4), srcSize);
        bytesRead = static_cast<size_t>(4);
        return getUnsignedDWord(reinterpret_cast<const void *>(reinterpret_cast
                    <const quint8 *>(src) + offset)) / 2;
    }
    else
    {
        bytesRead = static_cast<size_t>(2);
        return static_cast<quint32>(getUnsignedWord(reinterpret_cast<const void
                    *>(reinterpret_cast<const quint8 *>(src) + offset))) / 2;
    }
}

qint32 getCompressedSignedDWord(const void *src,
    size_t offset,
    size_t srcSize,
    size_t &bytesRead)
{
    checkOffsetLengthSize(offset, static_cast<size_t>(2), srcSize);
    quint8 lowByte =
        getUnsignedByte(reinterpret_cast<const void *>(reinterpret_cast<const
                quint8
                *>(src) + offset));
    if((lowByte & 1) != 0)
    {
        checkOffsetLengthSize(offset, static_cast<size_t>(4), srcSize);
        bytesRead = static_cast<size_t>(4);
        return static_cast<qint32>((static_cast<qint64>(getUnsignedDWord(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8 *>(src) +
                            offset))) - Q_INT64_C(0x80000000)) / Q_INT64_C(2));
    }
    else
    {
        bytesRead = static_cast<size_t>(2);
        return static_cast<qint32>((static_cast<qint64>(getUnsignedWord(
                        reinterpret_cast<const void *>(reinterpret_cast<const
                                quint8 *>(src) +
                            offset))) - Q_INT64_C(0x8000)) / Q_INT64_C(2));
    }
}
