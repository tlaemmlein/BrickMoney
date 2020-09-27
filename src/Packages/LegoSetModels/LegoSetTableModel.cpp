#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableModel")

#include "LegoSetTableModel.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QFontMetrics>
#include <QGuiApplication>

LegoSetTableModel::LegoSetTableModel(QObject *parent) : QAbstractTableModel(parent)
    , m_signalConnected(false)
{
    setObjectName("LegoSetTableModel");
    setDataSource( new LegoSetDataSource(this));
    m_roleNames = QAbstractTableModel::roleNames();
    // TODO loop over the QMetaEnum
    m_roleNames.insert(int(Role::Sort), QByteArray("sort"));
    m_roleNames.insert(int(Role::Number), QByteArray("number"));
    m_roleNames.insert(int(Role::Type), QByteArray("type"));
}

LegoSetTableModel::~LegoSetTableModel()
{

}

QHash<int, QByteArray> LegoSetTableModel::roleNames() const
{
    return m_roleNames;
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return m_dataSource->legoSetCount();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return LegoSetProperty::COUNT;
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int role) const
{
	LOG_SCOPE_METHOD(L"");

    if (index.row() < 0 || index.row() >= m_dataSource->legoSetCount() )
        return QVariant();

    //The index is valid
    LegoSet *set = m_dataSource->legoSetAt(index.row());

    int colIndex = index.column();

    switch (role) {
    case Qt::DisplayRole:
        return set->data(LegoSetProperty(colIndex));
    case Qt::InitialSortOrderRole: {
        bool numeric = false;
        set->data(LegoSetProperty(colIndex)).toFloat(&numeric);
        if (numeric)
            return Qt::DescendingOrder;
        return Qt::AscendingOrder;
    }
    case int(LegoSetTableModel::Role::Sort):
        return set->data(LegoSetProperty(colIndex));
    case int(LegoSetTableModel::Role::Number):
        return set->data(LegoSetProperty(colIndex)).toDouble();
    case int(LegoSetTableModel::Role::Type):
    {
        // TODO this is silly: make a virtual in the Category perhaps?
        LegoSetProperty prop = LegoSetProperty(colIndex);
        switch (prop) {
        case isSelected:
            return QLatin1String("selection");
        case id:
        case setNumber:
        case description:
        case year:
            return QLatin1String("readonly");
        case cheaperPercent:
        case recommendedRetailPrice:
        case profitEuros:
        case profitPercent:
            return QLatin1String("readonlydouble");
        case imageUrl:
            return QLatin1String("image");
        case seller:
        case soldOver:
        case buyer:
            return QLatin1String("text");
        case purchasingPrice:
        case retailPrice:
            return QLatin1String("double");
        case purchaseDate:
        case saleDate:
            return QLatin1String("date");
        default:
            return QLatin1String("string");
        }
    }
    default:
        return QVariant();
    }
    return QVariant();
}

QVariant LegoSetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        // section is interpreted as column
        return LegoSet::displayName(LegoSetProperty(section));
    } else {
        return QString();
    }

    return QString();
}

int LegoSetTableModel::columnWidth(int colIndex, const QFont *font)
{
    QFontMetrics defaultFontMetrics = QFontMetrics(QGuiApplication::font());
    QFontMetrics fm = (font ? QFontMetrics(*font) : defaultFontMetrics);

    QString header = LegoSet::displayName(LegoSetProperty(colIndex));

    int colWidth = fm.horizontalAdvance(header + QLatin1String(" ^")) + 8;
    for (int r = 0; r < m_dataSource->legoSetCount(); ++r) {
        LegoSet* set = m_dataSource->legoSetAt(r);
        QString val = "image";
        if ( LegoSetProperty(colIndex) != LegoSetProperty::imageUrl)
        {
            val = set->data(LegoSetProperty(colIndex)).toString();
        }

        if (LegoSetProperty(colIndex) == LegoSetProperty::recommendedRetailPrice
            || LegoSetProperty(colIndex) == LegoSetProperty::purchasingPrice
            || LegoSetProperty(colIndex) == LegoSetProperty::cheaperPercent
            || LegoSetProperty(colIndex) == LegoSetProperty::retailPrice
            || LegoSetProperty(colIndex) == LegoSetProperty::profitEuros
            || LegoSetProperty(colIndex) == LegoSetProperty::profitPercent)
        {
            double d = set->data(LegoSetProperty(colIndex)).toDouble();
            val = QString::number(d, 'f', 2);
        }

        colWidth = qMax(colWidth, fm.horizontalAdvance(val));
    }
    int additionalWidth = 0;
    if (LegoSetProperty(colIndex) == LegoSetProperty::purchaseDate
        || LegoSetProperty(colIndex) == LegoSetProperty::saleDate)
        additionalWidth = 10;
    if (LegoSetProperty(colIndex) == LegoSetProperty::description
        || LegoSetProperty(colIndex) == LegoSetProperty::buyer
        || LegoSetProperty(colIndex) == LegoSetProperty::seller
        || LegoSetProperty(colIndex) == LegoSetProperty::soldOver)
        additionalWidth = 20;
    colWidth += additionalWidth;
    return colWidth;
}


bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
    LOG_SCOPE_METHOD(L"");

	const int row = index.row();
    int colIndex = index.column();

    LOG_TRACE("role: " << role << " r: " << row << " c: " << colIndex);
    if (!index.isValid() )
        return false;

    LegoSet *set = m_dataSource->legoSetAt(index.row());

    if (!set->setData(LegoSetProperty(colIndex), value))
	{
		return false;
	}

    QModelIndex startOfRow = this->index(row, 0);
    QModelIndex endOfRow = this->index(row, columnCount() - 1);
	emit dataChanged(startOfRow, endOfRow);
	return true;
}

Qt::ItemFlags LegoSetTableModel::flags(const QModelIndex &index) const
{
    if (index.row() < 0 || index.row() >= m_dataSource->legoSetCount() )
        return QAbstractTableModel::flags(index);

    int colIndex = index.column();

    if (LegoSetProperty(colIndex) == LegoSetProperty::isSelected
        //id,
        || LegoSetProperty(colIndex) == setNumber
        //imageUrl,
        //description,
        //year,
        //recommendedRetailPrice,
        || LegoSetProperty(colIndex) == purchasingPrice
        //cheaperPercent,
        || LegoSetProperty(colIndex) == seller
        || LegoSetProperty(colIndex) == purchaseDate
        || LegoSetProperty(colIndex) == retailPrice
        //profitEuros,
        //profitPercent,
        || LegoSetProperty(colIndex) == saleDate
        || LegoSetProperty(colIndex) == soldOver
        || LegoSetProperty(colIndex) == buyer)
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;

    return QAbstractTableModel::flags(index);
}


LegoSet* LegoSetTableModel::addLegoSet(int setNumber)
{
	LOG_SCOPE_METHOD(L"");

    LOG_INFO("SetNumber: " << setNumber);

	auto set = new LegoSet(setNumber, this);

    m_dataSource->addLegoSet( set );

    return set;
}

QString LegoSetTableModel::getSelectedLegoSetIDs()
{
    QString IDs;
    for (const auto& set : m_dataSource->getSelectedLegoSets())
    {
        IDs += QString::number(set->id()) + " ";
    }

    return IDs;
}

void LegoSetTableModel::removeSelectedLegoSets()
{
    LOG_SCOPE_METHOD(L"");
    m_dataSource->removeSelectedLegoSets();
}

void LegoSetTableModel::clearAll()
{
	LOG_SCOPE_METHOD(L"");

    beginResetModel();
    m_dataSource->clearLegoSets();
    for ( int i = 0; i < m_columnWidths.size(); ++i)
        m_columnWidths[i] = 0;
    endResetModel();
	emit modelReset();
}

bool LegoSetTableModel::selectionIsDirty() const
{
    return m_SelectionIsDirty;
}

int LegoSetTableModel::numberOfLegoSets() const
{
    return m_dataSource->legoSetCount();
}

int LegoSetTableModel::numberOfSelectedLegoSets() const
{
    return m_dataSource->getNumberSelectedLegoSets();
}


void LegoSetTableModel::setDataSource(LegoSetDataSource *dataSource)
{
    beginResetModel();

    if( m_dataSource && m_signalConnected)
        m_dataSource->disconnect(this);

    m_dataSource = dataSource;

    connect(m_dataSource,&LegoSetDataSource::preLegoSetAdded,this,[=](){
        const int index = m_dataSource->legoSetCount();
        beginInsertRows(QModelIndex(),index,index);
    });

    connect(m_dataSource,&LegoSetDataSource::postLegoSetAdded,this,[=](){
        emit numberOfLegoSetsChanged(m_dataSource->legoSetCount());
        endInsertRows();
    });

    connect(m_dataSource,&LegoSetDataSource::preLegoSetRemoved,this,[=](int index){
        beginRemoveRows(QModelIndex(), index, index);
    });

    connect(m_dataSource,&LegoSetDataSource::postLegoSetRemoved,this,[=](){
        emit numberOfLegoSetsChanged(m_dataSource->legoSetCount());
        endRemoveRows();
    });

	connect(m_dataSource, &LegoSetDataSource::resetLegoSets, this, [=]() {
		emit modelReset();
	});

    connect(m_dataSource, &LegoSetDataSource::selectionIsDirtyChanged, [&] (bool isDirty) {
        m_SelectionIsDirty = isDirty;
        emit selectionIsDirtyChanged(isDirty);
        });

    m_SelectionIsDirty = m_dataSource->selectionIsDirty();

    connect(m_dataSource, &LegoSetDataSource::numberSelectedLegoSetsChanged, this, &LegoSetTableModel::numberOfSelectedLegoSetsChanged);

    emit numberOfSelectedLegoSetsChanged(m_dataSource->getNumberSelectedLegoSets());

    m_signalConnected = true;

    emit numberOfLegoSetsChanged(m_dataSource->legoSetCount());

    endResetModel();
	emit modelReset();
}

LegoSetDataSource *LegoSetTableModel::dataSource() const
{
    return m_dataSource;
}




