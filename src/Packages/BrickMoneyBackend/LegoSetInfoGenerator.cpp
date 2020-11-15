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

bool LegoSetInfoGenerator::mIsDataBaseReady = false;
std::vector<LegoSetInfo> LegoSetInfoGenerator::mLegoSetDatabase {};




class LegoSetInfoGeneratorPrivate : public QObject
{
public:
	explicit LegoSetInfoGeneratorPrivate(QObject* parent = nullptr) : QObject(parent) {}

	void prepareBrickMoneyImagesDB(const QString& legoSetDatabasePath)
	{
		if (!mBrickMoneyImagesDB.isOpen())
		{
			LOG_INFO("Prepare BrickMoneyUserImagesDB.");

			QString dbName(legoSetDatabasePath + "/BrickMoneyImagesDB.db3");
			auto names = mBrickMoneyImagesDB.connectionNames();
			if (!mBrickMoneyImagesDB.contains("BrickMoneyImagesDB"))
				mBrickMoneyImagesDB = QSqlDatabase::addDatabase("QSQLITE", "BrickMoneyImagesDB");
			mBrickMoneyImagesDB.setDatabaseName(dbName);
			if (!mBrickMoneyImagesDB.open())
			{
				LOG_ERROR("Could not open " << dbName.toStdWString());
				return;
			}
			mBrickMoneyImagesDBQuery = QSqlQuery(mBrickMoneyImagesDB);
			if (!mBrickMoneyImagesDBQuery.exec(
				"CREATE TABLE IF NOT EXISTS Images ( [set_id] bigint, [name] varchar(8), [md5sum] TEXT, [image_data] BLOB )"
				//"Das ist kein sql"
			))
			{
				LOG_ERROR("Could not create Images table. Last error: " << mBrickMoneyImagesDBQuery.lastError().text().toStdWString());
				mBrickMoneyImagesDB.close();
			}
		}
	}

	static QSqlDatabase mBrickMoneyImagesDB;
	static QSqlQuery mBrickMoneyImagesDBQuery;
};

QSqlDatabase LegoSetInfoGeneratorPrivate::mBrickMoneyImagesDB{};
QSqlQuery LegoSetInfoGeneratorPrivate::mBrickMoneyImagesDBQuery{};


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
    mLegoSetImages = legoSetDatabasePath + "/images";
    //createMD5Sum(mLegoSetImages);
	d_ptr->prepareBrickMoneyImagesDB(legoSetDatabasePath);
    fillDatabase();
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

void LegoSetInfoGenerator::fillDatabase()
{
    if (!mIsDataBaseReady)
    {
        LOG_INFO("Start filling the database");

        auto db = QSqlDatabase::addDatabase("QODBC3");
        QString connectString  = "Driver={ODBC Driver 17 for SQL Server};";
        connectString += "Server=tcp:brickmoneyserver.database.windows.net,1433;";
        connectString += "Database=BrickMoneyDB;";
        connectString += "Uid=readonlyuser;";
		QString wan = QString(QByteArray::fromBase64("VW50ZXIyMndlZ3MzNA=="));
        connectString += "Pwd={"+ wan +"};";
        connectString += "Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;";

        db.setDatabaseName(connectString);
        if(!db.open())
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
        if(!query.exec())
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
                               ,query.value("name_de").toString()
                               ,query.value("year").toInt()
                               ,query.value("rr_price").toDouble()) );
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

void LegoSetInfoGenerator::sendSignals(const LegoSetInfo &info)
{
    emit setNumber(info.setNumber);
    QString imageurl = QString("file:///%1/%2.jpg").arg(mLegoSetImages).arg(info.setNumber);
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
    m_legoSetInfo = info;
    emit legoSetInfoChanged(info);
}


