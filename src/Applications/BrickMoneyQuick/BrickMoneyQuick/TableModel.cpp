#include "TableModel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel (parent)
{

}

int TableModel::rowCount(const QModelIndex &) const
{
    return 200;
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 200;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QString("%1, %2").arg(index.column()).arg(index.row());
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}
