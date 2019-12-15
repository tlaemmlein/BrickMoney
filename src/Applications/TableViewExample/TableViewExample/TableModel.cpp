#include "TableModel.h"

#include <QDebug>

TableModel::TableModel(QObject *parent) : QAbstractTableModel (parent), mIndex(0)
{
    table.append({"Set Nummer"});

    for (int i = 0; i < 3; ++i) {
        table.append({QString("%1").arg(mIndex)});
        ++mIndex;
    }
}

int TableModel::rowCount(const QModelIndex &) const
{
    return table.size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return table.at(0).size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case TableDataRole:
    {
        return table.at(index.row()).at(index.column());
    }
    case HeadingRole:
    {
        if (index.row()==0){
            return true;
        }else{
            return false;
        }
    }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    qDebug() << __FUNCTION__;
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << __FUNCTION__;
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() < 0 || index.row() >= table.size() ||
        index.column() < 0 || index.column() >= table.at(0).size())
        return false;

    QString test = table.at(index.row()).at(index.column());
    //= value.toString();
    emit dataChanged(index,index);
    return true;
}

void TableModel::newEntry()
{
    qDebug() << "+++ " << __FUNCTION__;
    //insertRows(rowCount(), 1, QModelIndex());
    if (insertRow(rowCount()))
    {
        qDebug() << "Row inserted";
    }

    qDebug() << "--- " <<__FUNCTION__;
}


bool TableModel::insertRows(int row, int count, const QModelIndex &)
{
    qDebug() << "+++ " << __FUNCTION__;
    qDebug() << "count: " << count;
    qDebug() << "row: " << row;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        ++mIndex;
        table.append({QString("%1").arg(mIndex)});
        qDebug() << "mIndex: " << mIndex;
    }

    endInsertRows();
    qDebug() << "--- " <<__FUNCTION__;
    return true;
}

//bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//}


