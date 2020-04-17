#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableModel")

#include "LegoSetTableModel.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QQmlListProperty>

LegoSetTableModel::LegoSetTableModel(QObject *parent) : QAbstractTableModel(parent)
    , m_signalConnected(false)
{
    setObjectName("LegoSetTableModel");
    setDataSource( new DataSource(this));

    m_roles[ImageNameRole]="imageName";
    m_roles[ImageUrl]="imageUrl";
    m_roles[SetNumberRole]="setNumber";
    m_roles[DescriptionRole]="description";
    m_roles[YearRole]="year";
    m_roles[RecommendedRetailPriceRole]="recommendedRetailPrice";
    m_roles[PurchasingPriceRole]="purchasingPrice";
    m_roles[CheaperPercentRole]="cheaperPercent";
    m_roles[SellerRole]="seller";
    m_roles[PurchaseDateRole]="purchaseDate";
    m_roles[RetailPrice]="retailPrice";
    m_roles[ProfitEuros]="profitEuros";
    m_roles[ProfitPercent]="profitPercent";
    m_roles[SaleDate]="saleDate";
    m_roles[SoldOver]="soldOver";
    m_roles[Buyer]="buyer";
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return m_dataSource->legoSetCount();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return m_roles.size();
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int role) const
{
	LOG_SCOPE_METHOD(L"");

    if (index.row() < 0 || index.row() >= m_dataSource->legoSetCount() )
        return QVariant();

    //The index is valid
    LegoSet *set = m_dataSource->legoSetAt(index.row());

    if ( ImageNameRole == role)
        return set->imageName();
    if ( ImageUrl == role)
        return set->imageUrl();
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
    if ( ProfitEuros == role)
        return set->profitEuros();
    if (ProfitPercent == role)
        return set->profitPercent();
    if (SaleDate == role)
        return set->saleDate();
    if (SoldOver == role)
        return set->soldOver();
    if (Buyer == role)
        return set->buyer();

    return QVariant();
}


bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	LOG_SCOPE_METHOD(L"");

    LOG_TRACE("role: " << role << " r: " << index.row() << " c: " << index.column());
    if (!index.isValid() )
        return false;

    LegoSet *set = m_dataSource->legoSetAt(index.row());
    bool somethingChanged = false;
    QVector<int> changedRoles;

    switch (static_cast<LegoSetRoles>(role)) {
    case LegoSetTableModel::SetNumberRole:
    {
        if (set->setNumber() != value.toInt() ) {
            set->setSetNumber(value.toInt());
            changedRoles << ImageNameRole << ImageUrl<< DescriptionRole
                         << YearRole << RecommendedRetailPriceRole << CheaperPercentRole;
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::PurchasingPriceRole:
    {
        if (! qFuzzyCompare(set->purchasingPrice(), value.toDouble())) {
            set->setPurchasingPrice(value.toDouble());
            changedRoles << CheaperPercentRole << ProfitEuros << ProfitPercent;
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::CheaperPercentRole:
    {
//        if (! qFuzzyCompare(set->cheaperPercent(), value.toDouble())) {
//            set->setPurchasingPrice(value.toDouble());
            somethingChanged = true;
//        }
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
        if (set->purchaseDate() != value.toDate() ) {
            set->setPurchaseDate(value.toDate());
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::RetailPrice:
    {
        if (! qFuzzyCompare(set->retailPrice(), value.toDouble())) {
            set->setRetailPrice(value.toDouble());
            changedRoles << ProfitEuros << ProfitPercent;
            somethingChanged = true;
        }
        break;
    }
    case LegoSetTableModel::ProfitEuros:
    {
        somethingChanged = true;
        break;
    }
    case ProfitPercent:
    {
        somethingChanged = true;
        break;
    }
    case SaleDate:
    {
        if (set->saleDate() != value.toDate() ) {
            set->setSaleDate(value.toDate());
            somethingChanged = true;
        }
        break;
    }
    case SoldOver:
    {
        if (set->soldOver() != value.toString()) {
            set->setSoldOver((value.toString()));
            somethingChanged = true;
        }
        break;
    }
    case Buyer:
    {
        if (set->buyer() != value.toString()) {
            set->setBuyer((value.toString()));
            somethingChanged = true;
        }
        break;
    }
    }

    if( somethingChanged){
        changedRoles << role;
        emit dataChanged(index,index,changedRoles);
        return true;
    }
    return false;
}

LegoSet* LegoSetTableModel::addLegoSet(int setNumber)
{
	LOG_SCOPE_METHOD(L"");

    LOG_INFO("SetNumber: " << setNumber);

	auto set = new LegoSet(setNumber, this);

    m_dataSource->addLegoSet( set );

	return set;
}

void LegoSetTableModel::removeLegoSet(int rowIndex)
{
	LOG_SCOPE_METHOD(L"");

    m_dataSource->removeLegoSet(rowIndex);
}

void LegoSetTableModel::clearAll()
{
	LOG_SCOPE_METHOD(L"");

    beginResetModel();
    m_dataSource->clearLegoSets();
    endResetModel();
}


QHash<int, QByteArray> LegoSetTableModel::roleNames() const
{
    return m_roles;
}

void LegoSetTableModel::setDataSource(DataSource *dataSource)
{
    beginResetModel();

    if( m_dataSource && m_signalConnected)
        m_dataSource->disconnect(this);

    m_dataSource = dataSource;

    connect(m_dataSource,&DataSource::preLegoSetAdded,this,[=](){
        const int index = m_dataSource->legoSetCount();
        beginInsertRows(QModelIndex(),index,index);
    });

    connect(m_dataSource,&DataSource::postLegoSetAdded,this,[=](){
        endInsertRows();
    });

    connect(m_dataSource,&DataSource::preLegoSetRemoved,this,[=](int index){
        beginRemoveRows(QModelIndex(), index, index);
    });

    connect(m_dataSource,&DataSource::postLegoSetRemoved,this,[=](){
        endRemoveRows();
    });

    m_signalConnected = true;

    endResetModel();
}

DataSource *LegoSetTableModel::dataSource() const
{
    return m_dataSource;
}

void LegoSetTableModel::appendLegoSet(QQmlListProperty<LegoSet> *list, LegoSet *set)
{
    reinterpret_cast<LegoSetTableModel *> (list->data)->dataSource()->addLegoSet(set);
}

int LegoSetTableModel::legoSetCount(QQmlListProperty<LegoSet> *list)
{
    return reinterpret_cast<LegoSetTableModel *> (list->data)->dataSource()->legoSetCount();
}

LegoSet *LegoSetTableModel::legoSet(QQmlListProperty<LegoSet> *list, int index)
{
    return reinterpret_cast<LegoSetTableModel *> (list->data)->dataSource()->legoSetAt(index);
}

void LegoSetTableModel::clearLegoSets(QQmlListProperty<LegoSet> *list)
{
    reinterpret_cast<LegoSetTableModel *> (list->data)->dataSource()->clearLegoSets();
}


QQmlListProperty<LegoSet> LegoSetTableModel::legoSets()
{
    return QQmlListProperty<LegoSet>(this, this,
                                    &LegoSetTableModel::appendLegoSet,
                                    &LegoSetTableModel::legoSetCount,
                                    &LegoSetTableModel::legoSet,
                                    &LegoSetTableModel::clearLegoSets);
}

