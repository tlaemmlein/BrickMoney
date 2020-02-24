#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableModel")

#include "LegoSetTableModel.h"
#include "LegoSetRecord.h"

#include <QFile>
#include <QDir>
#include <QUrl>

LegoSetTableModel::LegoSetTableModel(QObject *parent) : QAbstractTableModel (parent)
{
    setObjectName("TableModel");
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return mLegoSetTableData.rowCount();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return mLegoSetTableData.columnCount();
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int role) const
{
	LOG_SCOPE_METHOD(L"");

    if (!index.isValid())
        return QVariant();

    return mLegoSetTableData.getData(index.row(), role);
}


bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	LOG_SCOPE_METHOD(L"");

	LOG_DEBUG("role: " << role << " r: " << index.row() << " c: " << index.column());
    if (!index.isValid() )
        return false;

    if (!mLegoSetTableData.setData(index.row(), value, role) )
        return false;

    emit dataChanged(index,index);

    return true;
}

void LegoSetTableModel::newEntry()
{
	LOG_SCOPE_METHOD(L"");

    if (insertRow(rowCount()))
    {
		LOG_DEBUG("Row inserted");
    }
}

void LegoSetTableModel::deleteEntry(int rowIndex)
{
	LOG_SCOPE_METHOD(L"");

    if (removeRow(rowIndex))
    {
		LOG_DEBUG("Row " << rowIndex << " deleted.");
    }
}

void LegoSetTableModel::clearAll()
{
	LOG_SCOPE_METHOD(L"");

    beginResetModel();
    mLegoSetTableData.clear();
    endResetModel();
}

void LegoSetTableModel::setDataSource(DataSource *datasource)
{
    if (m_dataSource == datasource)
        return;

    m_dataSource = datasource;
    emit dataSourceChanged(m_dataSource);
}


bool LegoSetTableModel::insertRows(int row, int count, const QModelIndex &)
{
    LOG_SCOPE_METHOD(L"");
    LOG_DEBUG("row: " << row << " count: " << count);
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        LegoSetRecord record("Empty.svg", "qrc:/images/Empty.svg", mLegoSetTableData.rowCount(),
                             QString("Beschreibung %1").arg(mLegoSetTableData.rowCount())
                                 ,2018, 10.0, 5.0, "www.lego.de", "07.07.2018", 15.0);

        mLegoSetTableData.append(record);
		LOG_DEBUG("row +i: " << row +i);
    }

    endInsertRows();
    return true;
}

bool LegoSetTableModel::removeRows(int row, int count, const QModelIndex &)
{
	LOG_SCOPE_METHOD(L"");
    LOG_DEBUG("row: " << row << " count: " << count);

    if ( row >= mLegoSetTableData.rowCount())
    {
        return false;
    }

    if ( count > 1)
    {
		LOG_ERROR("Sorry. Only one row can be removed.");
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    mLegoSetTableData.removeAt(row);

    endRemoveRows();
    return true;
}


QHash<int, QByteArray> LegoSetTableModel::roleNames() const
{
	LOG_SCOPE_METHOD(L"");
    return mLegoSetTableData.roleNames();
}

int LegoSetTableModel::roleID(const QString& roleName)
{
	LOG_SCOPE_METHOD(L"");
    return mLegoSetTableData.roleID(roleName);
}

void LegoSetTableModel::saveDataTo(const QChar &sep, QTextStream &out, const QString& projectFolder) const
{
	LOG_SCOPE_METHOD(L"");
    mLegoSetTableData.saveDataTo(sep, out, projectFolder);
}

void LegoSetTableModel::loadDataFrom(const QChar &sep, QTextStream &in, const QString &projectFolder)
{
    LOG_SCOPE_METHOD(L"");
    beginResetModel();

    mLegoSetTableData.loadDataFrom(sep, in, projectFolder);

    if ( 0 == mLegoSetTableData.rowCount())
        LOG_ERROR("LegoSetTableData is empty. Nothing was load.");

    endResetModel();
}

DataSource *LegoSetTableModel::dataSource() const
{
    return m_dataSource;
}

QQmlListProperty<LegoSet> LegoSetTableModel::legoSets() const
{
    return m_legoSets;
}

