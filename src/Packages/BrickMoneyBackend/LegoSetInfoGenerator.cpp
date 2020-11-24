#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.LegoSetInfoGenerator")

#include "LegoSetInfoGenerator.h"
#include "BrickMoneyDatabase.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QPixmapCache>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QTextStream>
#include <QUrl>

class LegoSetInfoGeneratorPrivate : public QObject
{
public:
	explicit LegoSetInfoGeneratorPrivate(QObject* parent = nullptr) : QObject(parent) {}

	bool prepareBrickMoneyDBLocale(const QString& legoSetDatabasePath)
	{
		if (mBrickMoneyDBLocale.isOpen())
			return true;

		mBrickMoneyDBLocale = BrickMoneyDatabase::prepareBrickMoneyDBLocale(legoSetDatabasePath);

		if (!mBrickMoneyDBLocale.isOpen())
			return false;

		return true;
	}
	
	void updateBrickMoneyDBLocale()
	{
		if (!mBrickMoneyDBLocale.isOpen() || !mTryToUpdateBrickMoneyDBLocale)
			return;

		BrickMoneyDatabase::updateBrickMoneyDBLocale(mBrickMoneyDBLocale);

		mTryToUpdateBrickMoneyDBLocale = false;
	}

	void fillLegoSetsDatabase()
	{
		if (!mBrickMoneyDBLocale.isOpen() ||  mIsDataBaseReady)
			return;

		QSqlQuery brickMoneyDBLocaleQuery(mBrickMoneyDBLocale);

			
		brickMoneyDBLocaleQuery.prepare("SELECT * FROM LegoSets");
		if (!brickMoneyDBLocaleQuery.exec())
		{
			LOG_ERROR("Can't execute query for selecting the LegoSets table!");
			return;
		}

		LOG_INFO("Query Executed Successfully !");

		while (brickMoneyDBLocaleQuery.next())
		{
			mLegoSetDatabase.push_back(
				LegoSetInfo(brickMoneyDBLocaleQuery.value("set_id").toInt()
					, brickMoneyDBLocaleQuery.value("name_de").toString()
					, brickMoneyDBLocaleQuery.value("year").toInt()
					, brickMoneyDBLocaleQuery.value("rr_price").toDouble()));
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

		LOG_INFO("Completed database filling");
	}
	
	static bool mIsDataBaseReady;
	static std::vector<LegoSetInfo> mLegoSetDatabase;
	static QSqlDatabase mBrickMoneyDBLocale;
	static bool mTryToUpdateBrickMoneyDBLocale;
};

bool LegoSetInfoGeneratorPrivate::mIsDataBaseReady = false;
std::vector<LegoSetInfo> LegoSetInfoGeneratorPrivate::mLegoSetDatabase{};
QSqlDatabase LegoSetInfoGeneratorPrivate::mBrickMoneyDBLocale{};
bool LegoSetInfoGeneratorPrivate::mTryToUpdateBrickMoneyDBLocale = true;

LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent), d_ptr(new LegoSetInfoGeneratorPrivate(this))
{
    LOG_SCOPE_METHOD(L"");
    qRegisterMetaType<LegoSetInfo>();

	if (!d_ptr->prepareBrickMoneyDBLocale(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
		return;

	d_ptr->updateBrickMoneyDBLocale();
	d_ptr->fillLegoSetsDatabase();
}

bool LegoSetInfoGenerator::querySetNumber(int num)
{
    LOG_SCOPE_METHOD(L"");
    LOG_TRACE(QString("querySetNumber %1").arg(num).toStdWString());

    for(const LegoSetInfo& info : d_ptr->mLegoSetDatabase)
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

    for( size_t index = 0; index < d_ptr->mLegoSetDatabase.size(); ++index )
    {
        if (d_ptr->mLegoSetDatabase.at(index).setNumber >= currentSetNumber)
        {
            size_t nextIndex = d_ptr->mLegoSetDatabase.at(index).setNumber == currentSetNumber
                               && index < d_ptr->mLegoSetDatabase.size() -1 ? index+1 : index;
            if ( nextIndex <= d_ptr->mLegoSetDatabase.size() )
            {
                LOG_INFO(QString("Found %1").arg(d_ptr->mLegoSetDatabase.at(nextIndex).setNumber).toStdWString());
                sendSignals(d_ptr->mLegoSetDatabase.at(nextIndex) );
                return d_ptr->mLegoSetDatabase.at(nextIndex).setNumber;
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

    for( size_t index = d_ptr->mLegoSetDatabase.size()-1; index >= 0 ; --index )
    {
        if (d_ptr->mLegoSetDatabase.at(index).setNumber <= currentSetNumber)
        {
            size_t prevIndex = d_ptr->mLegoSetDatabase.at(index).setNumber == currentSetNumber
                               && index > 1 ? index-1 : index;
            if ( prevIndex >= 0 )
            {
                LOG_INFO(QString("Found %1").arg(d_ptr->mLegoSetDatabase.at(prevIndex).setNumber).toStdWString());
                sendSignals(d_ptr->mLegoSetDatabase.at(prevIndex) );
                return d_ptr->mLegoSetDatabase.at(prevIndex).setNumber;
            }
        }
    }
    emit setNumberNotFound();
    return 0;
}


void LegoSetInfoGenerator::sendSignals(const LegoSetInfo &info)
{
    emit setNumber(info.setNumber);

    QPixmap pm;
    QString imageKey = QString::number(info.setNumber);
    if (!QPixmapCache::find(imageKey, &pm)) {
		auto pixmaps = BrickMoneyDatabase::queryLegoSetImages(d_ptr->mBrickMoneyDBLocale, info.setNumber);
		if (!pixmaps.isEmpty()) {
			pm = pixmaps.at(0);
			if (!QPixmapCache::insert(imageKey, pm))
				LOG_ERROR("Could not insert image " << imageKey.toStdWString());
		}

    }

    emit imageUrl(imageKey);
    emit description(info.description);
    emit year(info.year);
    emit recommendedRetailPrice(info.recommendedRetailPrice);
}


