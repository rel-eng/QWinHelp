/* Help index model class definition.

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

#ifndef HELPINDEXMODEL_H_
#define HELPINDEXMODEL_H_

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QPair>
#include <QObject>

class HelpIndexModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    HelpIndexModel(QObject *parent = 0);
    HelpIndexModel(QList<QPair<int, QString> > listOfPairs, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index,
        const QVariant &value,
        int role = Qt::EditRole);
    bool insertRows(int position,
        int rows,
        const QModelIndex &index = QModelIndex());
    bool removeRows(int position,
        int rows,
        const QModelIndex &index = QModelIndex());
    QList<QPair<int, QString> > getList();
    void setList(QList<QPair<int, QString> > &list);
    virtual ~HelpIndexModel();

private:
    QList<QPair<int, QString> > listOfPairs;
};

#endif /* HELPINDEXMODEL_H_ */
