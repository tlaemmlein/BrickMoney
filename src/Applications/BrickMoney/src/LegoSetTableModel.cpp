#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableModel")

#include "LegoSetTableModel.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QDebug>

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

    auto c = index.column();

    switch (role) {
    case Qt::DisplayRole:
        return set->data(LegoSetProperty(c));
    case Qt::InitialSortOrderRole: {
        bool numeric = false;
        set->data(LegoSetProperty(c)).toFloat(&numeric);
        if (numeric)
            return Qt::DescendingOrder;
        return Qt::AscendingOrder;
    }
    case int(LegoSetTableModel::Role::Sort):
        return set->data(LegoSetProperty(c));
    case int(LegoSetTableModel::Role::Number):
        return set->data(LegoSetProperty(c)).toDouble();
    case int(LegoSetTableModel::Role::Type):
    {
        // TODO this is silly: make a virtual in the Category perhaps?
        LegoSetProperty prop = LegoSetProperty(c);
        switch (prop) {
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
        return displayName(LegoSetProperty(section));
    } else {
        return QString();
    }

    return QString();
}

int LegoSetTableModel::columnWidth(int c, const QFont *font)
{
    //if (!m_columnWidths[c]) {
        QString header = displayName(LegoSetProperty(c));

        QFontMetrics defaultFontMetrics = QFontMetrics(QGuiApplication::font());
        QFontMetrics fm = (font ? QFontMetrics(*font) : defaultFontMetrics);

        int ret = fm.horizontalAdvance(headerData(c, Qt::Horizontal).toString() + QLatin1String(" ^")) + 8;
        for (int r = 0; r < m_dataSource->legoSetCount(); ++r) {
            LegoSet* set = m_dataSource->legoSetAt(r);
            QString val = "image";
            if ( LegoSetProperty(c) != LegoSetProperty::imageUrl)
            {
                val = set->data(LegoSetProperty(c)).toString();
            }

            if (LegoSetProperty(c) == LegoSetProperty::recommendedRetailPrice
                || LegoSetProperty(c) == LegoSetProperty::purchasingPrice
                || LegoSetProperty(c) == LegoSetProperty::cheaperPercent
                || LegoSetProperty(c) == LegoSetProperty::retailPrice
                || LegoSetProperty(c) == LegoSetProperty::profitEuros
                || LegoSetProperty(c) == LegoSetProperty::profitPercent)
            {
                double d = set->data(LegoSetProperty(c)).toDouble();
                val = QString::number(d, 'f', 2);
            }
            //qDebug() << val;

            ret = qMax(ret, fm.horizontalAdvance(val));

            //qDebug() << ret;
        }
        int additionalWidth = 0;
        if (LegoSetProperty(c) == LegoSetProperty::purchaseDate
            || LegoSetProperty(c) == LegoSetProperty::saleDate)
            additionalWidth = 10;
        if (LegoSetProperty(c) == LegoSetProperty::description
            || LegoSetProperty(c) == LegoSetProperty::buyer
            || LegoSetProperty(c) == LegoSetProperty::seller
            || LegoSetProperty(c) == LegoSetProperty::soldOver)
            additionalWidth = 20;
        ret += additionalWidth;
        //qDebug() << "result: " << ret;

        m_columnWidths[c] = ret;
    //}
    //qDebug() << "c: " << c << " ret: " << ret;
    return m_columnWidths[c];
}


bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
	LOG_SCOPE_METHOD(L"");

	const int row = index.row();
	const int col = index.column();

    LOG_TRACE("role: " << role << " r: " << row << " c: " << col);
    if (!index.isValid() )
        return false;

    LegoSet *set = m_dataSource->legoSetAt(index.row());

	if (!set->setData(LegoSetProperty(col), value))
	{
		return false;
	}

	QModelIndex startOfRow = this->index(row, 0);
	QModelIndex endOfRow = this->index(row, LegoSetProperty::COUNT - 1);
	emit dataChanged(startOfRow, endOfRow);
	return true;
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
    for ( int i = 0; i < m_columnWidths.size(); ++i)
        m_columnWidths[i] = 0;
    endResetModel();
	emit modelReset();
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
        endInsertRows();
    });

    connect(m_dataSource,&LegoSetDataSource::preLegoSetRemoved,this,[=](int index){
        beginRemoveRows(QModelIndex(), index, index);
    });

    connect(m_dataSource,&LegoSetDataSource::postLegoSetRemoved,this,[=](){
        endRemoveRows();
    });

	connect(m_dataSource, &LegoSetDataSource::resetLegoSets, this, [=]() {
		emit modelReset();
	});

    m_signalConnected = true;

    endResetModel();
	emit modelReset();
}

LegoSetDataSource *LegoSetTableModel::dataSource() const
{
    return m_dataSource;
}


