/* Help reply IO device class definition.

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

#ifndef HELPREPLYIODEVICE_H_
#define HELPREPLYIODEVICE_H_

#include <QObject>
#include <QIODevice>
#include <QUrl>
#include <QByteArray>

class HelpReplyIoDevice : public QIODevice {
    Q_OBJECT
public:
    HelpReplyIoDevice(const QByteArray &data);
    virtual ~HelpReplyIoDevice();
    virtual qint64 bytesAvailable() const Q_DECL_OVERRIDE;
    virtual void close() Q_DECL_OVERRIDE;
protected:
    virtual qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    virtual qint64 writeData(const char *data, qint64 maxlen) Q_DECL_OVERRIDE;
private:
    QByteArray data;
    const qint64 dataLength;
};

#endif /* HELPREPLYIODEVICE_H_ */
