#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableModel")

#include "LegoSetTableModel.h"
#include "LegoSetRecord.h"

#include <QFile>
#include <QDir>
#include <QUrl>

LegoSetTableModel::LegoSetTableModel(QObject *parent) : QAbstractTableModel (parent)
    , m_signalConnected(false)
{
    setObjectName("TableModel");
    setDataSource( new DataSource(this));
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return m_dataSource->dataItems().size();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return mLegoSetTableData.columnCount();
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int role) const
{
	LOG_SCOPE_METHOD(L"");

    if (index.row() < 0 || index.row() >= m_dataSource->dataItems().size() )
        return QVariant();

    //The index is valid
    LegoSet *set = m_dataSource->dataItems().at(index.row());

    if ( ImageNameRole == role)
        return set->imageName();
    if ( ImageFilePathRole == role)
        return set->imageFilePath();
    if ( SetNumberRole == role)
        return set->setNumber();
    if ( DescriptionRole == role)
        return set->description();
    if ( YearRole == role)
        return set->year();
    if ( RecommendedRetailPriceRole == role)
        return set->recommendedRetailPrice();
    if ( PurchasingPriceRole == role)
        return set->purchasingPrice();
    if ( CheaperPercentRole == role)
        return set->cheaperPercent();
    if ( SellerRole == role)
        return set->seller();
    if ( PurchaseDateRole == role)
        return set->purchaseDate();
    if ( RetailPrice == role)
        return set->retailPrice();

    return QVariant();
}


bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	LOG_SCOPE_METHOD(L"");

	LOG_DEBUG("role: " << role << " r: " << index.row() << " c: " << index.column());
    if (!index.isValid() )
        return false;

    LegoSet *set = m_dataSource->dataItems().at(index.row());
    bool somethingChanged = false;

    switch (static_cast<LegoSetRoles>(role)) {
    case LegoSetTableModel::ImageNameRole:
    {
        if (set->imageName() != value.toString() ) {
            set->setImageName(value.toString());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::ImageFilePathRole:
    {
        if (set->imageFilePath() != value.toString() ) {
            set->setImageFilePath(value.toString());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::SetNumberRole:
    {
        if (set->setNumber() != value.toInt() ) {
            set->setSetNumber(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::DescriptionRole:
    {
        if (set->description() != value.toString() ) {
            set->setDescription(value.toString());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::YearRole:
    {
        if (set->year() != value.toInt() ) {
            set->setYear(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::RecommendedRetailPriceRole:
    {
        qWarning("Floating point comparison needs context sanity check");
        if (! qFuzzyCompare(set->recommendedRetailPrice(), value.toDouble())) {
            set->setRecommendedRetailPrice(value.toDouble());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::PurchasingPriceRole:
    {
        qWarning("Floating point comparison needs context sanity check");
        if (! qFuzzyCompare(set->purchasingPrice(), value.toDouble())) {
            set->setPurchasingPrice(value.toDouble());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::CheaperPercentRole:
    {
        qWarning("Floating point comparison needs context sanity check");
        if (! qFuzzyCompare(set->cheaperPercent(), value.toDouble())) {
            set->setCheaperPercent(value.toDouble());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::SellerRole:
    {
        if (set->seller() != value.toString() ) {
            set->setSeller(value.toString());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::PurchaseDateRole:
    {
        if (set->purchaseDate() != value.toString() ) {
            set->setPurchaseDate(value.toString());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::RetailPrice:
    {
        qWarning("Floating point comparison needs context sanity check");
        if (! qFuzzyCompare(set->retailPrice(), value.toDouble())) {
            set->setRetailPrice(value.toDouble());
            somethingChanged = true;
        }
        break;
    }
    }

    if( somethingChanged){
        emit dataChanged(index,index,QVector<int>() << role);
        return true;
    }
    return false;
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
    QHash<int, QByteArray> roles;

    roles[ImageNameRole]="imageName";
    roles[ImageFilePathRole]="imageFilePath";
    roles[SetNumberRole]="setNumber";
    roles[DescriptionRole]="description";
    roles[YearRole]="year";
    roles[RecommendedRetailPriceRole]="recommendedRetailPrice";
    roles[PurchasingPriceRole]="purchasingPrice";
    roles[CheaperPercentRole]="cheaperPercent";
    roles[SellerRole]="seller";
    roles[PurchaseDateRole]="purchaseDate";
    roles[RetailPrice]="retailPrice";
    return roles;
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

