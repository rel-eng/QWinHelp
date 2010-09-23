/* Help access model class implementation.

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

#include "HelpIndexModel.h"

HelpIndexModel::HelpIndexModel(QObject *parent) : QAbstractTableModel(parent)
{
}

HelpIndexModel::HelpIndexModel(QList<QPair<int, QString> > listOfPairs,
    QObject *parent) : QAbstractTableModel(parent), listOfPairs(listOfPairs)
{
}

int HelpIndexModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listOfPairs.size();
}

int HelpIndexModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant HelpIndexModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if ((index.row() >= listOfPairs.size()) || (index.row() < 0))
    {
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {
        QPair<int, QString> pair = listOfPairs.at(index.row());
        if (index.column() == 0)
        {
            return pair.first;
        }
        else
        {
            if (index.column() == 1)
            {
                return pair.second;
            }
        }
    }
    return QVariant();
}

QVariant HelpIndexModel::headerData(int section,
    Qt::Orientation orientation,
    int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return tr("Topic index");

        case 1:
            return tr("Topic caption");

        default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags HelpIndexModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool HelpIndexModel::setData(const QModelIndex &index,
    const QVariant &value,
    int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row();
        QPair<int, QString> p = listOfPairs.value(row);
        if (index.column() == 0)
        {
            p.first = value.toInt();
        }
        else
        {
            if (index.column() == 1)
            {
                p.second = value.toString();
            }
            else
            {
                return false;
            }
        }
        listOfPairs.replace(row, p);
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

bool HelpIndexModel::insertRows(int position,
    int rows,
    const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for(int row = 0; row < rows; row++)
    {
        QPair<int, QString> pair(0, "");
        listOfPairs.insert(position, pair);
    }
    endInsertRows();
    return true;
}

bool HelpIndexModel::removeRows(int position,
    int rows,
    const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
    {
        listOfPairs.removeAt(position);
    }
    endRemoveRows();
    return true;
}

QList<QPair<int, QString> > HelpIndexModel::getList()
{
    return listOfPairs;
}

void HelpIndexModel::setList(QList<QPair<int, QString> > &list)
{
    this->beginResetModel();
    this->listOfPairs = list;
    this->endResetModel();
}

HelpIndexModel::~HelpIndexModel()
{
}
