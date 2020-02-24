#include "DataSource.h"
#include "LegoSet.h"

DataSource::DataSource(QObject *parent) : QObject(parent)
{

}

QList<LegoSet *> DataSource::dataItems()
{
    return m_legoSets;
}

void DataSource::addLegoSet(LegoSet *set)
{
    emit preLegoSetAdded();
    m_legoSets.append(set);
    emit postLegoSetAdded();
}

void DataSource::removeLegoSet(int index)
{
    emit preLegoSetRemoved(index);
    m_legoSets.removeAt(index);
    emit postLegoSetRemoved();
}

int DataSource::legoSetCount()
{
    return m_legoSets.count();
}

LegoSet *DataSource::legoSetAt(int index)
{
    return m_legoSets[index];
}

void DataSource::clearLegoSets()
{
    m_legoSets.clear();
}
