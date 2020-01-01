#include "LegoSetTableModel.h"

#include <QDebug>
#include <QScopeGuard>

LegoSetTableModel::LegoSetTableModel(QObject *parent) : QAbstractTableModel (parent)
{
    setObjectName("TableModel");
//    table.append(LegoSetTableData("https://www.brickmerge.de/img/sets/l/LEGO_41599_alt1.jpg", table.size(), QString("Beschreibung %1").arg(table.size())));
//    table.append(LegoSetTableData("https://www.brickmerge.de/img/sets/l/LEGO_41599_alt3.jpg", table.size(), QString("Beschreibung %1").arg(table.size())));
//    table.append(LegoSetTableData("qrc:/images/WonderWoman.png", table.size(), QString("Beschreibung %1").arg(table.size())));
//    table.append(LegoSetTableData("qrc:/images/WonderWoman.png", table.size(), QString("Beschreibung %1").arg(table.size())));
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return mLegoSetTableData.size();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return LegoSetTableData::count;
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int role) const
{
//    auto cleanup = qScopeGuard([] { qDebug() << "--- " << __FUNCTION__; });
//    qDebug() << "+++ " << __FUNCTION__;
    if (!index.isValid())
        return QVariant();

//    qDebug() << "index.row(): " << index.row();
//    qDebug() << "table.size(): " << table.size();
    if (index.row() >= mLegoSetTableData.size() || mLegoSetTableData.size() < 0)
        return QVariant();

//    qDebug() << "role: " << role;

    LegoSetTableData row = mLegoSetTableData.at(index.row());
//    qDebug() << "row: " << row;
//    qDebug() << "row.size(): " << row.size();

    switch (role) {
    case ImageRole:
    {
        return QVariant(row.image);
    }
    case SetNumberRole:
    {
        return QVariant(row.setnumber);
    }
    case DescriptionRole:
    {
        return QVariant(row.description);
    }
    default:
        break;
    }

    return QVariant();
}

QVariant LegoSetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Bild");

        case 1:
            return tr("Set Nummer");

        case 2:
            return tr("Beschreibung");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qDebug() << __FUNCTION__;
//    qDebug() << "role: " << role << " r: " << index.row() << " c: " << index.column();
    if (!index.isValid() )
        return false;

    if ( index.row() < 0 || index.row() >= mLegoSetTableData.size() ||
        index.column() < 0 || index.column() >= LegoSetTableData::count)
        return false;

    switch (role) {
    case ImageRole:
    {
        return false;
    }
    case SetNumberRole:
    {
//        qDebug() << value.toInt();
//        qDebug() << table.at(index.row()).setnumber;
        mLegoSetTableData[index.row()].setnumber = value.toInt();
        break;
    }
    case DescriptionRole:
    {
        mLegoSetTableData[index.row()].description = value.toString();
        break;
    }
    default:
        break;
    }

    emit dataChanged(index,index);

    return true;
}

void LegoSetTableModel::newEntry()
{
//    qDebug() << "+++ " << __FUNCTION__;
    //insertRows(rowCount(), 1, QModelIndex());
    if (insertRow(rowCount()))
    {
//        qDebug() << "Row inserted";
    }

//    qDebug() << "--- " <<__FUNCTION__;
}

void LegoSetTableModel::deleteEntry(int rowIndex)
{
//    qDebug() << "+++ " << __FUNCTION__;

    if (removeRow(rowIndex))
    {
//        qDebug() << "Row " << rowIndex << " deleted.";
    }

    //    qDebug() << "--- " <<__FUNCTION__;
}

void LegoSetTableModel::clearAll()
{
    //qDebug() << "+++ " << __FUNCTION__;
    beginResetModel();
    mLegoSetTableData.clear();
    endResetModel();
    //qDebug() << "--- " <<__FUNCTION__;
}


bool LegoSetTableModel::insertRows(int row, int count, const QModelIndex &)
{
//    qDebug() << "+++ " << __FUNCTION__;
//    qDebug() << "count: " << count;
//    qDebug() << "row: " << row;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        mLegoSetTableData.append(LegoSetTableData("qrc:/images/WonderWoman.png", mLegoSetTableData.size(), QString("Beschreibung %1").arg(mLegoSetTableData.size())));
//        qDebug() << "row +i: " << row +i;
    }

    endInsertRows();
//    qDebug() << "--- " <<__FUNCTION__;
    return true;
}

bool LegoSetTableModel::removeRows(int row, int count, const QModelIndex &)
{
//    qDebug() << "+++ " << __FUNCTION__;
//    qDebug() << "count: " << count;
//    qDebug() << "row: " << row;

    if ( row >= mLegoSetTableData.size())
    {
        return false;
    }

    if ( count > 1)
    {
//        qDebug() << "Sorry. Only one row can be removed.";
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    mLegoSetTableData.removeAt(row);

    endRemoveRows();

//    qDebug() << "--- " <<__FUNCTION__;
    return true;
}


QHash<int, QByteArray> LegoSetTableModel::roleNames() const
{
//    qDebug() << __FUNCTION__;
    QHash<int, QByteArray> roles;
    roles[ImageRole] = "image";
    roles[SetNumberRole] = "setnumber";
    roles[DescriptionRole] = "description";
    return roles;
}

int LegoSetTableModel::roleID(QString roleName)
{
    if (roleName == "image" )
    {
        return ImageRole;
    }

    if (roleName == "setnumber") {
        return SetNumberRole;
    }

    if ( roleName =="description" )
    {
        return DescriptionRole;
    }

    return -1;
}

void LegoSetTableModel::saveDataTo(const QChar &sep, QTextStream &out) const
{
    for(const auto & entry : mLegoSetTableData)
    {
        out << entry.image << sep << entry.setnumber << sep << entry.description << "\n";
    }
}

