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

#include <stdexcept>

size_t unpackLZ77(QFile &file,
    qint64 off,
    qint64 inputSize,
    void *outputBuffer,
    size_t outputBufferSize)
{
    seekFile(file, off);
    size_t unpackedBytes = static_cast<size_t>(0);
    memset(outputBuffer, 0, outputBufferSize);
    qint64 inputLeft = inputSize;
    while(unpackedBytes < outputBufferSize)
    {
        if(inputLeft == 0)
        {
            break;
        }
        quint8 bits = readUnsignedByte(file);
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
                quint8 wordL = readUnsignedByte(file);
                inputLeft--;
                if(inputLeft == 0)
                {
                    break;
                }
                quint8 wordH = readUnsignedByte(file);
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
                quint8 character = readUnsignedByte(file);
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
