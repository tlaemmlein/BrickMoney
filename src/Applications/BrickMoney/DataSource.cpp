#include "Logging.h"
SET_LOGGER("BrickMoney.DataSource")

#include "DataSource.h"
#include "LegoSet.h"
#include "BrickMoneySettings.h"

#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QDate>
#include <QFileInfo>

DataSource::DataSource(QObject *parent) : QObject(parent)
{

}

void DataSource::addLegoSet(LegoSet *set)
{
    emit preLegoSetAdded();
    m_legoSets.append(set);
    emit postLegoSetAdded();
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(true);
}

void DataSource::removeLegoSet(int index)
{
    emit preLegoSetRemoved(index);
    m_legoSets.removeAt(index);
    emit postLegoSetRemoved();
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(true);
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
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(true);
}

void DataSource::saveLegoSets()
{
    LOG_SCOPE_METHOD(L"");
    saveLegoSetsImpl(BrickMoneySettings::Inst()->brickMoneyFilePath());
}

void DataSource::saveLegoSets(const QString &fileUrlPath)
{
    LOG_SCOPE_METHOD(L"");
    const QString localFilePath = toLocalFile(fileUrlPath);
    if (saveLegoSetsImpl(localFilePath))
        BrickMoneySettings::Inst()->setBrickMoneyFilePath(localFilePath);
}

void DataSource::loadLegoSets(const QString &fileUrlPath)
{
    LOG_SCOPE_METHOD(L"");

    const QString filePath = toLocalFile(fileUrlPath);

    if ( filePath.isEmpty())
    {
        LOG_ERROR(__FUNCTION__ << ": The brick money file path is empty. " << filePath.toStdWString());
        return;
    }

    LOG_INFO("Start to load from " << filePath.toStdWString());

    QFile cvsData(filePath);

    if ( !cvsData.open(QFile::ReadOnly | QIODevice::Text))
    {
        LOG_ERROR("Could not read from " << filePath.toStdWString());
        return;
    }

    QTextStream input(&cvsData);
    input.setCodec("UTF-8");

    const QChar del = ';';

    const int expectedCols = 8;

    while (!input.atEnd()) {
        QString line = input.readLine();
        QStringList row = line.split(del);
        if (row.size() != expectedCols)
            continue;

        LegoSet* set = new LegoSet(this);

        set->setSetNumber(row.at(0).toInt());
        set->setPurchasingPrice(row.at(1).toDouble());
        set->setSeller(row.at(2));
        set->setPurchaseDate(QDate::fromString(row.at(3)));
        set->setRetailPrice(row.at(4).toDouble());
        set->setSaleDate(QDate::fromString(row.at(5)));
        set->setSoldOver(row.at(6));
        set->setBuyer(row.at(7));

        addLegoSet(set);
    }

    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(false);
    BrickMoneySettings::Inst()->setBrickMoneyFilePath(filePath);
}

QString DataSource::toLocalFile(const QString &fileUrl)
{
    LOG_INFO("fileUrl " << fileUrl.toStdWString());
    QFileInfo info(fileUrl);

    if ( info.exists() )
    {
        LOG_INFO("exists");
        return fileUrl;
    }

    QUrl url(fileUrl);

    return url.toLocalFile();
}

bool DataSource::saveLegoSetsImpl(const QString& filePath)
{
    LOG_SCOPE_METHOD(L"");

    if ( filePath.isEmpty())
    {
        LOG_ERROR("The brick money file path is empty");
        return false;
    }

    LOG_INFO("Start to save to " << filePath.toStdWString());

    QFile cvsData(filePath);

    if ( !cvsData.open(QFile::WriteOnly))
    {
        LOG_ERROR("Could not save to " << filePath.toStdWString());
        return false;
    }

    QTextStream output(&cvsData);
    output.setCodec("UTF-8");

    const QChar del = ';';

    for(const auto& set: m_legoSets)
    {
        output << set->setNumber() << del
               << set->purchasingPrice() << del
               << set->seller() << del
               << set->purchaseDate().toString() << del
               << set->retailPrice() << del
               << set->saleDate().toString() << del
               << set->soldOver() << del
               << set->buyer() << "\n";
    }
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(false);
    return true;
}
