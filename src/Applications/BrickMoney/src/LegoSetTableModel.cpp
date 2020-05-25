#include "Logging.h"
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
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return m_dataSource->legoSetCount();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int /*role*/) const
{
	LOG_SCOPE_METHOD(L"");

    if (index.row() < 0 || index.row() >= m_dataSource->legoSetCount() )
        return QVariant();

    //The index is valid
    LegoSet *set = m_dataSource->legoSetAt(index.row());

    if ( index.column() == 0)
        return set->id();

    return QString("%1").arg(set->setNumber());
}

QVariant LegoSetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        // section is interpreted as column
        return LegoSet::HeaderInfo.value(section);
    } else {
        return QString();
    }

}

int LegoSetTableModel::columnWidth(int c, const QFont *font)
{
    if (!m_columnWidths[c]) {
        QString header = LegoSet::HeaderInfo.value(c);
        QFontMetrics defaultFontMetrics = QFontMetrics(QGuiApplication::font());
        QFontMetrics fm = (font ? QFontMetrics(*font) : defaultFontMetrics);
        int ret = fm.horizontalAdvance(headerData(c, Qt::Horizontal).toString() + QLatin1String(" ^")) + 8;
        for (int r = 0; r < m_dataSource->legoSetCount(); ++r) {
            LegoSet* set = m_dataSource->legoSetAt(r);
            QString cell;
            switch (c) {
            case 0: cell = QString("%0").arg(set->id()); break;
            case 1: cell = QString("%0").arg(set->setNumber()); break;
            default: cell ="";
            }
            ret = qMax(ret, fm.horizontalAdvance(cell));
        }
        m_columnWidths[c] = ret;
    }
    return m_columnWidths[c];
}


bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	LOG_SCOPE_METHOD(L"");

    LOG_TRACE("role: " << role << " r: " << index.row() << " c: " << index.column());
    if (!index.isValid() )
        return false;

    //LegoSet *set = m_dataSource->legoSetAt(index.row());

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
    endResetModel();
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

    m_signalConnected = true;

    endResetModel();
}

LegoSetDataSource *LegoSetTableModel::dataSource() const
{
    return m_dataSource;
}


