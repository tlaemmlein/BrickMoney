#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.LegoSetInfoGenerator")

#include "LegoSetInfoGenerator.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QPixmapCache>
#include <QStandardPaths>
#include <QTextStream>
#include <QSql>
#include <QSqlError>
#include <QUrl>
#include <QSqlDatabase>
#include <QSqlQuery>





class LegoSetInfoGeneratorPrivate : public QObject
{
public:
	explicit LegoSetInfoGeneratorPrivate(QObject* parent = nullptr) : QObject(parent) {}

	bool prepareBrickMoneyDBLocale(const QString& legoSetDatabasePath)
	{
		if (mBrickMoneyDBLocale.isOpen())
		{
			return true;
		}
		LOG_INFO("Prepare BrickMoneyDBLocale.");

		QString dbName(legoSetDatabasePath + "/BrickMoneyDBLocale.db3");
		if (!mBrickMoneyDBLocale.contains("BrickMoneyDBLocale"))
			mBrickMoneyDBLocale = QSqlDatabase::addDatabase("QSQLITE", "BrickMoneyDBLocale");
		mBrickMoneyDBLocale.setDatabaseName(dbName);
		if (!mBrickMoneyDBLocale.open())
		{
			auto error = mBrickMoneyDBLocale.lastError();
			LOG_ERROR("Could not open " << dbName.toStdWString());
			LOG_ERROR(error.databaseText().toStdWString());
			LOG_ERROR(error.driverText().toStdWString());
			LOG_ERROR(error.nativeErrorCode().toStdWString());
			return false;
		}
		mBrickMoneyDBLocaleQuery = QSqlQuery(mBrickMoneyDBLocale);

		QStringList sql_querys;
		sql_querys << "CREATE TABLE IF NOT EXISTS LegoSets (set_id bigint PRIMARY KEY, name_en text NOT NULL, name_de text NOT NULL, year INTEGER NOT NULL, rr_price float)"
			<< "CREATE TABLE IF NOT EXISTS Images (legoset_id bigint REFERENCES LegoSets (set_id), name text, md5sum text, image_data BLOB)"
			<< "CREATE TABLE IF NOT EXISTS Version (id int PRIMARY KEY, version bigint)"
			<< "INSERT OR IGNORE INTO Version (id) VALUES (0)"
			<< "CREATE UNIQUE INDEX IF NOT EXISTS filename ON Images ( legoset_id, name )";

		for (const auto& sql_query : sql_querys)
		{
			if (!mBrickMoneyDBLocaleQuery.exec(sql_query))
			{
				LOG_ERROR("Could not exec query. Last error: " << mBrickMoneyDBLocaleQuery.lastError().text().toStdWString());
				mBrickMoneyDBLocale.close();
				return false;
			}
		}
		LOG_INFO("BrickMoneyDBLocale is ready.");

		return true;
	}
	
	void updateBrickMoneyDBLocale()
	{
		if (!mBrickMoneyDBLocale.isOpen() || !mTryToUpdateBrickMoneyDBLocale)
			return;

		LOG_INFO("Update BrickMoneyDBLocale.");
		
		QSqlDatabase remoteDB = QSqlDatabase::addDatabase("QODBC3", "BrickMoneyDBRemote");

		QString connectString = "Driver={ODBC Driver 17 for SQL Server};";
		connectString += "Server=tcp:brickmoneyserver.database.windows.net,1433;";
		connectString += "Database=BrickMoneyDB;";
        connectString += "Uid=readonlyuser;";
		QString wan = QString(QByteArray::fromBase64("VW50ZXIyMndlZ3MzNA=="));
		connectString += "Pwd={" + wan + "};";
		connectString += "Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;";

		remoteDB.setDatabaseName(connectString);
		if (!remoteDB.open())
		{
			auto error = remoteDB.lastError();
			LOG_ERROR("Could not open remote db.");
			LOG_ERROR(error.databaseText().toStdWString());
			LOG_ERROR(error.driverText().toStdWString());
			LOG_ERROR(error.nativeErrorCode().toStdWString());
			mTryToUpdateBrickMoneyDBLocale = false;
			return;
		}

		QSqlQuery remoteQuery(remoteDB);
		remoteQuery.setForwardOnly(true);

		int remoteVersion = 0;
		remoteQuery.exec("SELECT version FROM Version");
		while (remoteQuery.next())
			remoteVersion = remoteQuery.value("version").toInt();

		int localeVersion = 0;
		mBrickMoneyDBLocaleQuery.exec("SELECT version FROM Version");
		while (mBrickMoneyDBLocaleQuery.next())
			localeVersion = mBrickMoneyDBLocaleQuery.value("version").toInt();

		if (localeVersion == remoteVersion)
		{
			LOG_INFO("The locale db is up to date.");
			mTryToUpdateBrickMoneyDBLocale = false;
			return;
		}

		mBrickMoneyDBLocaleQuery.prepare("UPDATE Version SET version=:version WHERE ID=0");
		mBrickMoneyDBLocaleQuery.bindValue(":version", remoteVersion);
		mBrickMoneyDBLocaleQuery.exec();

		QSqlDatabase::database().transaction();
		remoteQuery.prepare("SELECT * FROM LegoSets");
		if (!remoteQuery.exec())
		{
			LOG_ERROR("Can't execute query the remote LegoSets table!");
			mTryToUpdateBrickMoneyDBLocale = false;
			remoteDB.close();
			return;
		}

		LOG_INFO("Remote LegoSets query successful!");

		while (remoteQuery.next())
		{
			// https://stackoverflow.com/questions/3634984/insert-if-not-exists-else-update
			mBrickMoneyDBLocaleQuery.prepare("INSERT OR REPLACE INTO LegoSets (set_id, name_en, name_de, year, rr_price) "
				"VALUES (:set_id, :name_en, :name_de, :year, :rr_price)");
			mBrickMoneyDBLocaleQuery.bindValue(":set_id", remoteQuery.value("set_id").toInt());
			mBrickMoneyDBLocaleQuery.bindValue(":name_en", remoteQuery.value("name_en").toString());
			mBrickMoneyDBLocaleQuery.bindValue(":name_de", remoteQuery.value("name_de").toString());
			mBrickMoneyDBLocaleQuery.bindValue(":year", remoteQuery.value("year").toInt());
			mBrickMoneyDBLocaleQuery.bindValue(":rr_price", remoteQuery.value("rr_price").toDouble());
			mBrickMoneyDBLocaleQuery.exec();
		}
		QSqlDatabase::database().commit();
		remoteDB.close();
		mTryToUpdateBrickMoneyDBLocale = false;

		LOG_INFO("BrickMoneyDBLocale updated.");
	}

	void fillLegoSetsDatabase()
	{
		if (mIsDataBaseReady)
			return;
			
		mBrickMoneyDBLocaleQuery.prepare("SELECT * FROM LegoSets");
		if (!mBrickMoneyDBLocaleQuery.exec())
		{
			LOG_ERROR("Can't execute query for selecting the LegoSets table!");
			return;
		}

		LOG_INFO("Query Executed Successfully !");

		while (mBrickMoneyDBLocaleQuery.next())
		{
			mLegoSetDatabase.push_back(
				LegoSetInfo(mBrickMoneyDBLocaleQuery.value("set_id").toInt()
					, mBrickMoneyDBLocaleQuery.value("name_de").toString()
					, mBrickMoneyDBLocaleQuery.value("year").toInt()
					, mBrickMoneyDBLocaleQuery.value("rr_price").toDouble()));
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
	
	QString mLegoSetImages;
	LegoSetInfo mLegoSetInfo;

	static bool mIsDataBaseReady;
	static std::vector<LegoSetInfo> mLegoSetDatabase;
	static QSqlDatabase mBrickMoneyDBLocale;
	static QSqlQuery mBrickMoneyDBLocaleQuery;
	static bool mTryToUpdateBrickMoneyDBLocale;
};

bool LegoSetInfoGeneratorPrivate::mIsDataBaseReady = false;
std::vector<LegoSetInfo> LegoSetInfoGeneratorPrivate::mLegoSetDatabase{};
QSqlDatabase LegoSetInfoGeneratorPrivate::mBrickMoneyDBLocale{};
QSqlQuery LegoSetInfoGeneratorPrivate::mBrickMoneyDBLocaleQuery{};
bool LegoSetInfoGeneratorPrivate::mTryToUpdateBrickMoneyDBLocale = true;


void createMD5Sum(const QString& legoSetImages)
{
	static bool alreadyCreated = false;

	if (!alreadyCreated)
	{
		QDir directory(legoSetImages);
		QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG", QDir::Files);
		foreach(QString filename, images) {
			QPixmap pixmap;
			QByteArray inByteArray;
			QBuffer inBuffer(&inByteArray);
			inBuffer.open(QIODevice::WriteOnly);
			pixmap.save(&inBuffer, "JPG");
			QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
			hash.addData(inByteArray);
			QString md5sum = hash.result().toHex();
			QFile md5sumFile(legoSetImages + "/" + filename + ".md5");
			if (md5sumFile.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QTextStream stream(&md5sumFile);
				stream << md5sum;
				md5sumFile.close();
			}
		}
		alreadyCreated = true;
	}
}



LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent), d_ptr(new LegoSetInfoGeneratorPrivate(this))
{
    LOG_SCOPE_METHOD(L"");
    qRegisterMetaType<LegoSetInfo>();
    const QString legoSetDatabasePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/LegoDatabase";
	d_ptr->mLegoSetImages = legoSetDatabasePath + "/images";
    //createMD5Sum(mLegoSetImages);
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

LegoSetInfo LegoSetInfoGenerator::legoSetInfo() const
{
    return d_ptr->mLegoSetInfo;
}

void LegoSetInfoGenerator::sendSignals(const LegoSetInfo &info)
{
    emit setNumber(info.setNumber);

    QPixmap pm;
    QString imageKey = QString::number(info.setNumber);
    if (!QPixmapCache::find(imageKey, &pm)) {
        QString imageurl = QString("file:///%1/%2.jpg").arg(d_ptr->mLegoSetImages).arg(info.setNumber);
        QFileInfo fileInfo(imageurl);
        QString local;

        if (fileInfo.exists())
            local=imageurl;
        else
        {
            QUrl url(imageurl);
            local=url.toLocalFile();
        }
        pm.load(local);
        if (!QPixmapCache::insert(imageKey, pm))
            LOG_ERROR("Could not insert image " << imageKey.toStdWString());
        LOG_TRACE("Key: " << imageKey.toStdWString() << " imageUrl: " << local.toStdWString());
    }

    emit imageUrl(imageKey);
    emit description(info.description);
    emit year(info.year);
    emit recommendedRetailPrice(info.recommendedRetailPrice);
	d_ptr->mLegoSetInfo = info;
    emit legoSetInfoChanged(info);
}


