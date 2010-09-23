/* Window record as defined in MVP project file class definition.

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

#ifndef WINDOWMVPRECORD_H_
#define WINDOWMVPRECORD_H_

#include <QtGlobal>
#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QColor>

class WindowMvpRecord
{
private:
    /** Содержит ли поле type корректные данные. */
    bool typeIsValid;
    /** Содержит ли поле name корректные данные. */
    bool nameIsValid;
    /** Содержит ли поле caption корректные данные. */
    bool captionIsValid;
    /** Содержит ли поле x корректные данные. */
    bool xIsValid;
    /** Содержит ли поле y корректные данные. */
    bool yIsValid;
    /** Содержит ли поле width корректные данные. */
    bool widthIsValid;
    /** Содержит ли поле height корректные данные. */
    bool heightIsValid;
    /** Содержит ли поле maximize корректные данные (возможно также означает, нужно ли максимизировать окно) . */
    bool maximizeWindow;
    /** Содержит ли поле rgb корректные данные. */
    bool rgbIsValid;
    /** Содержит ли поле rgbNsr корректные данные. */
    bool rgbNsrIsValid;
    /** Нужно ли поддерживать окно всегда поверх других. */
    bool windowsAlwaysOnTop;
    /** Нужно ли автоматически устанавливать высоту окна. */
    bool autoSizeHeight;
    /** Строка с описанием типа окна, 10 байт. */
    QString windowType;
    /** Строка с именем окна, 9 байт. */
    QString windowName;
    /** Строка с названием окна, 51 байт. */
    QString windowCaption;
    /** Координата x окна (от 0 до 1000), 2 байта без знака. */
    quint16 x;
    /** Координата y окна (от 0 до 1000), 2 байта без знака. */
    quint16 y;
    /** Ширина окна (от 0 до 1000), 2 байта без знака. */
    quint16 width;
    /** Высота окна (от 0 до 1000), 2 байта без знака. */
    quint16 height;
    /** Флаг максимизации и стили окна, 2 байта без знака. */
    quint16 maximize;
    /** Цвет прокручиваемой области, 4 байта. */
    QRgb rgbColor;
    /** Цвет непрокручиваемой области, 4 байта. */
    QRgb rgbNsrColor;
    /** Цвет 3, 4 байта. */
    QRgb rgbColor3;
    /** Координата X2, 2 байта без знака. */
    quint16 x2;
    /** Координата Y2, 2 байта без знака. */
    quint16 y2;
    /** Координата X3, 2 байта без знака. */
    quint16 x3;
    /** Координата Y3, 2 байта без знака. */
    quint16 y3;
    /** Ширина 2, 2 байта без знака. */
    quint16 width2;
    /** Высота 2, 2 байта без знака. */
    quint16 height2;
public:
    WindowMvpRecord();
    WindowMvpRecord(QFile &file, qint64 off, QTextCodec *codec);
    WindowMvpRecord(const WindowMvpRecord& rhs);
    virtual ~WindowMvpRecord();
    WindowMvpRecord & operator=(const WindowMvpRecord & rhs);
    bool isTypeValid() const;
    bool isNameValid() const;
    bool isCaptionValid() const;
    bool isXValid() const;
    bool isYValid() const;
    bool isWidthValid() const;
    bool isHeightValid() const;
    bool isWindowMaximized() const;
    bool isRgbValid() const;
    bool isRgbNsrValid() const;
    bool isWindowsAlwaysOnTop() const;
    bool isAutoSizeHeight() const;
    QString getWindowType() const;
    QString getWindowName() const;
    QString getWindowCaption() const;
    quint16 getX() const;
    quint16 getY() const;
    quint16 getWidth() const;
    quint16 getHeight() const;
    quint16 getMaximize() const;
    QRgb getRgbColor();
    QRgb getRgbNsrColor();
    QRgb getRgbColor3();
    quint16 getX2() const;
    quint16 getY2() const;
    quint16 getX3() const;
    quint16 getY3() const;
    quint16 getWidth2() const;
    quint16 getHeight2() const;
};

#endif /* WINDOWMVPRECORD_H_ */
