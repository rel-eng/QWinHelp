/* Compression functions implementation.

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

#include "CompressionUtils.h"

#include "IOUtils.h"

#include <cstring>

#include <QScopedArrayPointer>

#include <stdexcept>

size_t unpackLZ77(QIODevice &device,
    qint64 off,
    qint64 inputSize,
    void *outputBuffer,
    size_t outputBufferSize)
{
    seekFile(device, off);
    size_t unpackedBytes = static_cast<size_t>(0);
    memset(outputBuffer, 0, outputBufferSize);
    qint64 inputLeft = inputSize;
    while(unpackedBytes < outputBufferSize)
    {
        if(inputLeft == 0)
        {
            break;
        }
        quint8 bits = readUnsignedByte(device);
        inputLeft--;
        quint8 mask = 1;
        for(int index = 0; index < 8; index++)
        {
            if((bits & mask) != 0)
            {
                if(inputLeft == 0)
                {
                    break;
                }
                quint8 wordL = readUnsignedByte(device);
                inputLeft--;
                if(inputLeft == 0)
                {
                    break;
                }
                quint8 wordH = readUnsignedByte(device);
                inputLeft--;
                quint16 pos = static_cast<quint16>(wordL) +
                    static_cast<quint16>(wordH & 0x0F) * 256;
                quint8 len = (((wordH & 0xF0) >> 4) & 0x0F) + 3;
                for(quint8 i = 0; i < len; i++)
                {
                    if(unpackedBytes <
                            (static_cast<size_t>(pos) + static_cast<size_t>(1)))
                    {
                        throw std::runtime_error(
                            "Bufer underrun during LZ77 unpack");
                    }
                        (reinterpret_cast<quint8 *>(outputBuffer))[
                        unpackedBytes] =
                        (reinterpret_cast<quint8 *>(outputBuffer))[
                        unpackedBytes -
                        static_cast<size_t>(pos) - static_cast<size_t>(1)];
                    unpackedBytes++;
                    if(unpackedBytes >= outputBufferSize)
                    {
                        break;
                    }
                }
            }
            else
            {
                if(inputLeft == Q_INT64_C(0))
                {
                    break;
                }
                quint8 character = readUnsignedByte(device);
                inputLeft--;
                    (reinterpret_cast<quint8 *>(outputBuffer))[unpackedBytes] =
                    character;
                unpackedBytes++;
            }
            if(unpackedBytes >= outputBufferSize)
            {
                break;
            }
            if(index < 7)
            {
                mask = mask * 2;
            }
        }
    }
    return unpackedBytes;
}

size_t unpackLZ77UnknownOutputSize(QIODevice &device,
    qint64 off,
    size_t inputSize,
    QIODevice &outputDevice)
{
    seekFile(device, off);
    if(inputSize > 0x40000000)
    {
        throw std::runtime_error("Requested too much memory for LZ77 unpacking");
    }
    QScopedArrayPointer<quint8> inputBuf(new quint8[inputSize]);
    fillBuffer(device, inputSize,
        reinterpret_cast<void *>(inputBuf.data()), inputSize);
    size_t allocSize = 4 * inputSize;
    if(allocSize > 0x40000000)
    {
        throw std::runtime_error("Requested too much memory for LZ77 unpacking");
    }
    QScopedArrayPointer<quint8> outBuf(new quint8[allocSize]);
    size_t unpackedBytes = static_cast<size_t>(0);
    size_t inputPtr = static_cast<size_t>(0);
    while(inputPtr < inputSize)
    {
        quint8 bits = inputBuf[inputPtr];
        inputPtr++;
        quint8 mask = 1;
        for(int index = 0; index < 8; index++)
        {
            if((bits & mask) != 0)
            {
                if(inputPtr >= inputSize)
                {
                    break;
                }
                quint8 wordL = inputBuf[inputPtr];
                inputPtr++;
                if(inputPtr >= inputSize)
                {
                    break;
                }
                quint8 wordH = inputBuf[inputPtr];
                inputPtr++;
                quint16 pos = static_cast<quint16>(wordL) +
                    static_cast<quint16>(wordH & 0x0F) * 256;
                quint8 len = (((wordH & 0xF0) >> 4) & 0x0F) + 3;
                for(quint8 i = 0; i < len; i++)
                {
                    if(unpackedBytes <
                            (static_cast<size_t>(pos) + static_cast<size_t>(1)))
                    {
                        throw std::runtime_error(
                            "Bufer underrun during LZ77 unpack");
                    }
                    if(unpackedBytes >= allocSize)
                    {
                        if((2 * allocSize) > 0x40000000)
                        {
                            throw std::runtime_error(
                                "Requested too much memory for LZ77 unpacking");
                        }
                        QScopedArrayPointer<quint8> reallocBuf(
                            new quint8[2 * allocSize]);
                        memcpy(
                            reinterpret_cast<void *>(reallocBuf.data()),
                            reinterpret_cast<const void *>(outBuf.data()),
                            allocSize);
                        allocSize *= 2;
                        outBuf.swap(reallocBuf);
                    }
                    outBuf[unpackedBytes] =
                        outBuf[static_cast<size_t>(unpackedBytes - pos - 1)];
                    unpackedBytes++;
                }
            }
            else
            {
                if(inputPtr >= inputSize)
                {
                    break;
                }
                quint8 character = inputBuf[inputPtr];
                inputPtr++;
                if(unpackedBytes >= allocSize)
                {
                    if((2 * allocSize) > 0x40000000)
                    {
                        throw std::runtime_error(
                            "Requested too much memory for LZ77 unpacking");
                    }
                    QScopedArrayPointer<quint8> reallocBuf(
                        new quint8[2 * allocSize]);
                    memcpy(
                        reinterpret_cast<void *>(reallocBuf.data()),
                        reinterpret_cast<const void *>(outBuf.data()),
                        allocSize);
                    allocSize *= 2;
                    outBuf.swap(reallocBuf);
                }
                outBuf[unpackedBytes] = character;
                unpackedBytes++;
            }
            if(index < 7)
            {
                mask = mask * 2;
            }
        }
    }
    if(outputDevice.write(reinterpret_cast<const char *>(outBuf.data()),
            static_cast<qint64>(unpackedBytes)) !=
        static_cast<qint64>(unpackedBytes))
    {
        throw std::runtime_error("Unable to write to buffer");
    }
    return unpackedBytes;
}

size_t unpackRLEUnknownOutputSize(QIODevice &device,
    qint64 off,
    size_t inputSize,
    QIODevice &outputDevice)
{
    seekFile(device, off);
    if(inputSize > 0x40000000)
    {
        throw std::runtime_error("Requested too much memory for RLE unpacking");
    }
    QScopedArrayPointer<quint8> inputBuf(new quint8[inputSize]);
    fillBuffer(device, inputSize,
        reinterpret_cast<void *>(inputBuf.data()), inputSize);
    size_t allocSize = 4 * inputSize;
    if(allocSize > 0x40000000)
    {
        throw std::runtime_error("Requested too much memory for RLE unpacking");
    }
    QScopedArrayPointer<quint8> outBuf(new quint8[allocSize]);
    size_t unpackedBytes = static_cast<size_t>(0);
    size_t inputPtr = static_cast<size_t>(0);
    while(inputPtr < inputSize)
    {
        quint8 chr = inputBuf[inputPtr];
        inputPtr++;
        if((chr & 0x80) != 0)
        {
            size_t len = static_cast<size_t>(chr & 0x7F);
            for(size_t i = 0; i < len; i++)
            {
                if(inputPtr >= inputSize)
                {
                    break;
                }
                if(unpackedBytes >= allocSize)
                {
                    if((2 * allocSize) > 0x40000000)
                    {
                        throw std::runtime_error(
                            "Requested too much memory for RLE unpacking");
                    }
                    QScopedArrayPointer<quint8> reallocBuf(
                        new quint8[2 * allocSize]);
                    memcpy(
                        reinterpret_cast<void *>(reallocBuf.data()),
                        reinterpret_cast<const void *>(outBuf.data()),
                        allocSize);
                    allocSize *= 2;
                    outBuf.swap(reallocBuf);
                }
                outBuf[unpackedBytes] = inputBuf[inputPtr];
                inputPtr++;
                unpackedBytes++;
            }
        }
        else
        {
            if(inputPtr >= inputSize)
            {
                break;
            }
            quint8 toCopy = inputBuf[inputPtr];
            inputPtr++;
            for(quint8 i = 0; i < chr; i++)
            {
                if(unpackedBytes >= allocSize)
                {
                    if((2 * allocSize) > 0x40000000)
                    {
                        throw std::runtime_error(
                            "Requested too much memory for RLE unpacking");
                    }
                    QScopedArrayPointer<quint8> reallocBuf(
                        new quint8[2 * allocSize]);
                    memcpy(
                        reinterpret_cast<void *>(reallocBuf.data()),
                        reinterpret_cast<const void *>(outBuf.data()),
                        allocSize);
                    allocSize *= 2;
                    outBuf.swap(reallocBuf);
                }
                outBuf[unpackedBytes] = toCopy;
                unpackedBytes++;
            }
        }
    }
    if(outputDevice.write(reinterpret_cast<const char *>(outBuf.data()),
            static_cast<qint64>(unpackedBytes)) !=
        static_cast<qint64>(unpackedBytes))
    {
        throw std::runtime_error("Unable to write to buffer");
    }
    return unpackedBytes;
}
