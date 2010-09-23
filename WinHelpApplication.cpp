/* Application class implementation.

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

#include "WinHelpApplication.h"

#include <stdexcept>

WinHelpApplication::WinHelpApplication(int &c, char **v) : QApplication(c, v)
{
}

bool WinHelpApplication::notify(QObject *rec, QEvent *ev)
{
    try
    {
        return QApplication::notify(rec, ev);
    }
    catch (std::exception &ex)
    {
        qDebug() << "EXCEPTION: " << ex.what();
        return false;
    }
    catch (...)
    {
        qDebug() << "Unknown exception!";
        return false;
    }
}
