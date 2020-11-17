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
		if (!mBrickMoneyDBLocale.isOpen())
		{
			LOG_INFO("Prepare BrickMoneyUserImagesDB.");

			QString dbName(legoSetDatabasePath + "/BrickMoneyDBLocale.db3");
			if (!mBrickMoneyDBLocale.contains("BrickMoneyDBLocale"))
				mBrickMoneyDBLocale = QSqlDatabase::addDatabase("QSQLITE", "BrickMoneyDBLocale");
			mBrickMoneyDBLocale.setDatabaseName(dbName);
			if (!mBrickMoneyDBLocale.open())
			{
				LOG_ERROR("Could not open " << dbName.toStdWString());
				return false;
			}
			mBrickMoneyDBLocaleQuery = QSqlQuery(mBrickMoneyDBLocale);

			QStringList sql_querys;
			sql_querys << "CREATE TABLE IF NOT EXISTS LegoSets (set_id bigint PRIMARY KEY, name_en text NOT NULL, name_de text NOT NULL, year INTEGER NOT NULL, rr_price float)"
				<< "CREATE TABLE IF NOT EXISTS Images (legoset_id bigint REFERENCES LegoSets (set_id), name text, md5sum text, image_data BLOB)"
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
		}
		return mBrickMoneyDBLocale.isOpen();
	}

	void fillDatabase()
	{
		if (!mIsDataBaseReady)
		{
			LOG_INFO("Start filling the database");

			auto db = QSqlDatabase::addDatabase("QODBC3");
			QString connectString = "Driver={ODBC Driver 17 for SQL Server};";
			connectString += "Server=tcp:brickmoneyserver.database.windows.net,1433;";
			connectString += "Database=BrickMoneyDB;";
			connectString += "Uid=readonlyuser;";
			QString wan = QString(QByteArray::fromBase64("VW50ZXIyMndlZ3MzNA=="));
			connectString += "Pwd={" + wan + "};";
			connectString += "Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;";

			db.setDatabaseName(connectString);
			if (!db.open())
			{
				auto error = db.lastError();
				LOG_ERROR("Could not open database with connection string.");
				LOG_ERROR(error.databaseText().toStdWString());
				LOG_ERROR(error.driverText().toStdWString());
				LOG_ERROR(error.nativeErrorCode().toStdWString());
				return;
			}

			LOG_INFO("Database opend");

			QSqlQuery query;
			query.setForwardOnly(true);
			query.prepare("SELECT * FROM LegoSets");
			if (!query.exec())
			{
				LOG_ERROR("Can't Execute Query !");
			}
			else
			{
				LOG_INFO("Query Executed Successfully !");

				while (query.next())
				{
					mLegoSetDatabase.push_back(
						LegoSetInfo(query.value("set_id").toInt()
							, query.value("name_de").toString()
							, query.value("year").toInt()
							, query.value("rr_price").toDouble()));
				}
			}

			db.close();

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
	}
	QString mLegoSetImages;
	static bool mIsDataBaseReady;
	static std::vector<LegoSetInfo> mLegoSetDatabase;
	LegoSetInfo mLegoSetInfo;
	static QSqlDatabase mBrickMoneyDBLocale;
	static QSqlQuery mBrickMoneyDBLocaleQuery;
};

bool LegoSetInfoGeneratorPrivate::mIsDataBaseReady = false;
std::vector<LegoSetInfo> LegoSetInfoGeneratorPrivate::mLegoSetDatabase{};
QSqlDatabase LegoSetInfoGeneratorPrivate::mBrickMoneyDBLocale{};
QSqlQuery LegoSetInfoGeneratorPrivate::mBrickMoneyDBLocaleQuery{};


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
	d_ptr->prepareBrickMoneyDBLocale(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	d_ptr->fillDatabase();
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
    QPixmap pm;
    QString imageKey = QString::number(info.setNumber);
    if (!QPixmapCache::find(imageKey, &pm)) {
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


