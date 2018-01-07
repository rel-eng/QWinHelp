/* Help reply IO device class implementation.

   Copyright (C) 2018 rel-eng

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

#include <QTimer>

#include "HelpReplyIoDevice.h"

HelpReplyIoDevice::HelpReplyIoDevice(const QByteArray &data): data(data), dataLength(data.length())
{
    setOpenMode(QIODevice::ReadOnly);

    QTimer::singleShot(0, this, SLOT(readyRead()));
    QTimer::singleShot(0, this, SLOT(readChannelFinished()));
}

HelpReplyIoDevice::~HelpReplyIoDevice()
{
}

qint64 HelpReplyIoDevice::bytesAvailable() const
{
    return data.length() + QIODevice::bytesAvailable();
}

void HelpReplyIoDevice::close()
{
    QIODevice::close();
    deleteLater();
}

qint64 HelpReplyIoDevice::readData(char *buffer, qint64 maxlen)
{
    qint64 len = qMin(qint64(data.length()), maxlen);
    if (len > 0) {
        memcpy(buffer, data.constData(), len);
        data.remove(0, len);
    }
    return len;
}

qint64 HelpReplyIoDevice::writeData(const char *buffer, qint64 maxlen)
{
    Q_UNUSED(buffer)
    Q_UNUSED(maxlen)
    return 0;
}
