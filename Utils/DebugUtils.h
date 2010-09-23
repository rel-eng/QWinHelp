/* Debug functions.

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

#ifndef DEBUGUTILS_H_
#define DEBUGUTILS_H_

#include <QtDebug>

#ifdef DEBUG

#define PRINT_DBG(fmt, ...) { qDebug("DEBUG[%s:%d] %s(): " fmt, \
            __FILE__, \
            __LINE__, \
            __FUNCTION__, \
            ## __VA_ARGS__); }

#else

#define PRINT_DBG(fmt, ...) { do {} while(0); }

#endif

#endif /* DEBUGUTILS_H_ */
