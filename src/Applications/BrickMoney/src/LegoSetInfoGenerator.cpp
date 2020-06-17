#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetInfoGenerator")

#include "LegoSetInfoGenerator.h"

#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

bool LegoSetInfoGenerator::mIsDataBaseReady = false;
std::vector<LegoSetInfo> LegoSetInfoGenerator::mLegoSetDatabase {};



LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent)
{
    LOG_SCOPE_METHOD(L"");
    qRegisterMetaType<LegoSetInfo>();
    const QString legoSetDatabasePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/LegoDatabase";
    const QString legoSetDatabaseFilePath = legoSetDatabasePath + "/BrickMoneyDatabase.csv";
    mLegoSetImages = legoSetDatabasePath + "/images";
    fillDatabase(legoSetDatabaseFilePath);
}

bool LegoSetInfoGenerator::querySetNumber(int num)
{
    LOG_SCOPE_METHOD(L"");
    LOG_TRACE(QString("querySetNumber %1").arg(num).toStdWString());

    for(const LegoSetInfo& info : mLegoSetDatabase)
    {
        if (info.setNumber == num)
        {
            sendSignals(info);
            return true;
        }
    }

    emit setNumberNotFound();
	return false;
}

int LegoSetInfoGenerator::nextSetNumber(int currentSetNumber)
{
    LOG_SCOPE_METHOD(L"");
    LOG_INFO(QString("nextSetNumber %1").arg(currentSetNumber).toStdWString());

    for( size_t index = 0; index < mLegoSetDatabase.size(); ++index )
    {
        if ( mLegoSetDatabase.at(index).setNumber >= currentSetNumber)
        {
            size_t nextIndex = mLegoSetDatabase.at(index).setNumber == currentSetNumber
                               && index < mLegoSetDatabase.size() -1 ? index+1 : index;
            if ( nextIndex <= mLegoSetDatabase.size() )
            {
                LOG_INFO(QString("Found %1").arg(mLegoSetDatabase.at(nextIndex).setNumber).toStdWString());
                sendSignals( mLegoSetDatabase.at(nextIndex) );
                return mLegoSetDatabase.at(nextIndex).setNumber;
            }
        }
    }
    emit setNumberNotFound();
    return 0;
}

int LegoSetInfoGenerator::previousSetNumber(int currentSetNumber)
{
    LOG_SCOPE_METHOD(L"");
    LOG_INFO(QString("previousSetNumber %1").arg(currentSetNumber).toStdWString());

    for( size_t index = mLegoSetDatabase.size()-1; index >= 0 ; --index )
    {
        if ( mLegoSetDatabase.at(index).setNumber <= currentSetNumber)
        {
            size_t prevIndex = mLegoSetDatabase.at(index).setNumber == currentSetNumber
                               && index > 1 ? index-1 : index;
            if ( prevIndex >= 0 )
            {
                LOG_INFO(QString("Found %1").arg(mLegoSetDatabase.at(prevIndex).setNumber).toStdWString());
                sendSignals( mLegoSetDatabase.at(prevIndex) );
                return mLegoSetDatabase.at(prevIndex).setNumber;
            }
        }
    }
    emit setNumberNotFound();
    return 0;
}

LegoSetInfo LegoSetInfoGenerator::legoSetInfo() const
{
    return m_legoSetInfo;
}

void LegoSetInfoGenerator::fillDatabase(const QString& legoSetDatabaseFilePath)
{
    if (!mIsDataBaseReady)
    {
        LOG_INFO("fillDatabase");

        QFile cvsData(legoSetDatabaseFilePath);

        if ( !cvsData.open(QFile::ReadOnly | QIODevice::Text))
        {
            LOG_ERROR("Could not read from " << legoSetDatabaseFilePath.toStdWString());
            return;
        }
        QTextStream input(&cvsData);
        input.setCodec("UTF-8");
        const QChar sep = ';';
        const int expectedCols = 5;

        while (!input.atEnd()) {
            QString line = input.readLine();
            QStringList cols = line.split(sep);
            if (cols.size() != expectedCols)
                continue;

            int     setNumber = cols.at(0).toInt();
            QString description = cols.at(2);
            int     year = cols.at(3).toInt();
            double  recommendedRetailPrice = cols.at(4).toDouble();

            mLegoSetDatabase.push_back(LegoSetInfo(setNumber,description,year,recommendedRetailPrice) );
        }

        // Ref: https://en.cppreference.com/w/cpp/algorithm/unique
        // remove consecutive (adjacent) duplicates
        auto last = std::unique(mLegoSetDatabase.begin(), mLegoSetDatabase.end());
        mLegoSetDatabase.erase(last, mLegoSetDatabase.end());

        // sort followed by unique, to remove all duplicates
        std::sort(mLegoSetDatabase.begin(), mLegoSetDatabase.end()); // {1 1 2 3 4 4 5}
        last = std::unique(mLegoSetDatabase.begin(), mLegoSetDatabase.end());

        mLegoSetDatabase.erase(last, mLegoSetDatabase.end());

        mIsDataBaseReady = true;
    }
}

void LegoSetInfoGenerator::sendSignals(const LegoSetInfo &info)
{
    emit setNumber(info.setNumber);
    QString imageurl = QString("file:///%1/%2.jpg").arg(mLegoSetImages).arg(info.setNumber);
    emit imageUrl(imageurl);
    emit description(info.description);
    emit year(info.year);
    emit recommendedRetailPrice(info.recommendedRetailPrice);
    m_legoSetInfo = info;
    emit legoSetInfoChanged(info);
}

