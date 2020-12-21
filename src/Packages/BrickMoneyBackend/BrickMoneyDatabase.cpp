#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyDatabase")


#include "BrickMoneyDatabase.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QDir>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

std::unique_ptr<BrickMoneyDatabase> BrickMoneyDatabase::smInstance = nullptr;

class BrickMoneyDatabasePrivate {
public:

	QSqlDatabase getRemoteDB() {
		QSqlDatabase remoteDB = QSqlDatabase::addDatabase("QODBC3", "BrickMoneyDBRemote");

		QString connectString = "Driver={ODBC Driver 17 for SQL Server};";
		connectString += "Server=tcp:brickmoneyserver.database.windows.net,1433;";
		connectString += "Database=BrickMoneyDB;";
		connectString += "Uid=readonlyuser;";
		QString wan = QString(QByteArray::fromBase64("VW50ZXIyMndlZ3MzNA=="));
		connectString += "Pwd={" + wan + "};";
		connectString += "Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;";

		remoteDB.setDatabaseName(connectString);
		return remoteDB;
	}

	void openDatabase(QSqlDatabase& db) {
		if (!db.open())
		{
			auto error = db.lastError();
			LOG_ERROR("Could not open db.");
			LOG_ERROR(error.databaseText().toStdWString());
			LOG_ERROR(error.driverText().toStdWString());
			LOG_ERROR(error.nativeErrorCode().toStdWString());
			throw RemoteDBException("Could not open db.");
		}
	}


	QSqlDatabase mBrickMoneyDBLocale;

	int mRemoteVersion = 0;
};


BrickMoneyDatabase * BrickMoneyDatabase::Inst()
{
	if (!smInstance)
	{
		smInstance = std::unique_ptr<BrickMoneyDatabase>(new BrickMoneyDatabase());
	}
	return smInstance.get();
}

BrickMoneyDatabase::~BrickMoneyDatabase()
{
}

BrickMoneyDatabase::BrickMoneyDatabase() : d_ptr(new BrickMoneyDatabasePrivate)
{

}

bool BrickMoneyDatabase::prepareBrickMoneyDBLocale(const QString & legoSetDatabasePath)
{
	if (d_ptr->mBrickMoneyDBLocale.isOpen())
		return true;

	LOG_INFO("Prepare BrickMoneyDBLocale.");

	QString dbName(legoSetDatabasePath + "/BrickMoneyDBLocale.db3");
	if (!d_ptr->mBrickMoneyDBLocale.contains("BrickMoneyDBLocale"))
		d_ptr->mBrickMoneyDBLocale = QSqlDatabase::addDatabase("QSQLITE", "BrickMoneyDBLocale");
	d_ptr->mBrickMoneyDBLocale.setDatabaseName(dbName);
	if (!d_ptr->mBrickMoneyDBLocale.open())
	{
		auto error = d_ptr->mBrickMoneyDBLocale.lastError();
		LOG_ERROR("Could not open " << dbName.toStdWString());
		LOG_ERROR(error.databaseText().toStdWString());
		LOG_ERROR(error.driverText().toStdWString());
		LOG_ERROR(error.nativeErrorCode().toStdWString());
		return false;
	}
	QSqlQuery brickmoney_locale_query(d_ptr->mBrickMoneyDBLocale);

	QStringList sql_querys;
	sql_querys << "CREATE TABLE IF NOT EXISTS LegoSets (set_id bigint PRIMARY KEY, name_en text NOT NULL, name_de text NOT NULL, year INTEGER NOT NULL, rr_price float)"
		<< "CREATE TABLE IF NOT EXISTS Images (legoset_id bigint REFERENCES LegoSets (set_id), name text, md5sum text, image_data BLOB)"
		<< "CREATE TABLE IF NOT EXISTS Version (id int PRIMARY KEY, version bigint)"
		<< "INSERT OR IGNORE INTO Version (id) VALUES (0)"
		<< "CREATE UNIQUE INDEX IF NOT EXISTS filename ON Images ( legoset_id, name )";

	for (const auto& sql_query : sql_querys)
	{
		if (!brickmoney_locale_query.exec(sql_query))
		{
			LOG_ERROR("Could not exec query. Last error: " << brickmoney_locale_query.lastError().text().toStdWString());
			d_ptr->mBrickMoneyDBLocale.close();
			return false;
		}
	}
	LOG_INFO("BrickMoneyDBLocale version: " << localeDBVersion());

	return true;
}

int BrickMoneyDatabase::remoteDBVersion() const
{
	QSqlDatabase remoteDB = d_ptr->getRemoteDB();
	d_ptr->openDatabase(remoteDB);

	QSqlQuery remoteQuery(remoteDB);
	remoteQuery.setForwardOnly(true);

	int remoteVersion = 0;

	remoteQuery.exec("SELECT version FROM Version");
	while (remoteQuery.next())
		remoteVersion = remoteQuery.value("version").toInt();

	return remoteVersion;
}

int BrickMoneyDatabase::localeDBVersion() const
{
	if (!d_ptr->mBrickMoneyDBLocale.isOpen())
		return 0;

	int localeVersion = 0;
	QSqlQuery brickmoney_locale_query(d_ptr->mBrickMoneyDBLocale);
	brickmoney_locale_query.exec("SELECT version FROM Version");
	while (brickmoney_locale_query.next())
		localeVersion = brickmoney_locale_query.value("version").toInt();

	return localeVersion;
}


bool BrickMoneyDatabase::isNewRemoteVersionAvailable()
{
	d_ptr->mRemoteVersion = remoteDBVersion();

	int localeVersion = localeDBVersion();

	return d_ptr->mRemoteVersion > localeVersion;
}

bool BrickMoneyDatabase::updateBrickMoneyDBLocale()
{
	if (!d_ptr->mBrickMoneyDBLocale.isOpen())
		return false;

	LOG_INFO("Update BrickMoneyDBLocale version from " << localeDBVersion() << " to " << remoteDBVersion() );

	if (!isNewRemoteVersionAvailable()) {
		LOG_INFO("The locale db is up to date.");
		return true;
	}

	QSqlDatabase remoteDB = d_ptr->getRemoteDB();
	d_ptr->openDatabase(remoteDB);

	QSqlQuery remoteQuery(remoteDB);
	remoteQuery.setForwardOnly(true);

	QSqlQuery brickmoney_locale_query(d_ptr->mBrickMoneyDBLocale);

	// Update LegoSets table
	d_ptr->mBrickMoneyDBLocale.transaction();
	remoteQuery.prepare("SELECT * FROM LegoSets");
	if (!remoteQuery.exec())
	{
		LOG_ERROR("Can't execute query the remote LegoSets table!");
		remoteDB.close();
		return false;
	}

    while (remoteQuery.next())
	{
		// https://stackoverflow.com/questions/3634984/insert-if-not-exists-else-update
		brickmoney_locale_query.prepare("INSERT OR REPLACE INTO LegoSets (set_id, name_en, name_de, year, rr_price) "
			"VALUES (:set_id, :name_en, :name_de, :year, :rr_price)");
		brickmoney_locale_query.bindValue(":set_id", remoteQuery.value("set_id").toInt());
		brickmoney_locale_query.bindValue(":name_en", remoteQuery.value("name_en").toString());
		brickmoney_locale_query.bindValue(":name_de", remoteQuery.value("name_de").toString());
		brickmoney_locale_query.bindValue(":year", remoteQuery.value("year").toInt());
		brickmoney_locale_query.bindValue(":rr_price", remoteQuery.value("rr_price").toDouble());
		brickmoney_locale_query.exec();
	}
	d_ptr->mBrickMoneyDBLocale.commit();

	// Update Images table
	d_ptr->mBrickMoneyDBLocale.transaction();
	remoteQuery.prepare("SELECT * FROM Images");
	if (!remoteQuery.exec())
	{
		LOG_ERROR("Can't execute query the remote Images table!");
		remoteDB.close();
		return false;
	}

	while (remoteQuery.next())
	{
		// https://stackoverflow.com/questions/3634984/insert-if-not-exists-else-update
		int remote_legoset_id = remoteQuery.value("legoset_id").toInt();
		QString remote_name = remoteQuery.value("name").toString();
		QString remote_md5sum = remoteQuery.value("md5sum").toString();
		QByteArray remote_image_data = remoteQuery.value("image_data").toByteArray();
		brickmoney_locale_query.prepare("INSERT OR REPLACE INTO Images (legoset_id, name, md5sum, image_data) "
			"VALUES (:legoset_id, :name, :md5sum, :image_data)");
		brickmoney_locale_query.bindValue(":legoset_id", remote_legoset_id);
		brickmoney_locale_query.bindValue(":name", remote_name);
		brickmoney_locale_query.bindValue(":md5sum", remote_md5sum);
		brickmoney_locale_query.bindValue(":image_data", remote_image_data);
		brickmoney_locale_query.exec();


		//mBrickMoneyDBLocaleQuery.prepare("SELECT md5sum FROM Images WHERE legoset_id=:legoset_id AND name=':name'");
		//mBrickMoneyDBLocaleQuery.bindValue(":legoset_id", remote_legoset_id);
		//mBrickMoneyDBLocaleQuery.bindValue(":name", remote_name);

		//QString locale_md5sum = "";
		//if (mBrickMoneyDBLocaleQuery.exec())
		//{
		//	while (mBrickMoneyDBLocaleQuery.next()) {
		//		locale_md5sum = mBrickMoneyDBLocaleQuery.value("md5sum").toString();
		//	}

		//	if (locale_md5sum != remote_md5sum)
		//	{
		//		QSqlQuery remoteImageQuery(remoteDB);
		//		//remoteImageQuery.setForwardOnly(true);
		//		remoteImageQuery.prepare("SELECT image_data FROM Images WHERE legoset_id=:rlegoset_id AND name=':rname'");
		//		remoteImageQuery.bindValue(":rlegoset_id", remote_legoset_id);
		//		remoteImageQuery.bindValue(":rname", remote_name);
		//		if (!remoteImageQuery.exec())
		//		{
		//			LOG_ERROR("Could not read the image data from remote Images table for " << remote_legoset_id << "|" << remote_name.toStdWString());
		//			continue;
		//		}

		//		QByteArray remote_image_data;
		//		while (remoteImageQuery.next()) {
		//			remote_image_data = remoteImageQuery.value("image_data").toByteArray();
		//		}

		//		mBrickMoneyDBLocaleQuery.prepare("INSERT OR REPLACE INTO Images (legoset_id, name, md5sum, image_data) "
		//			"VALUES (:legoset_id, :name, :md5sum, :image_data)");
		//		mBrickMoneyDBLocaleQuery.bindValue(":legoset_id", remote_legoset_id);
		//		mBrickMoneyDBLocaleQuery.bindValue(":name", remote_name);
		//		mBrickMoneyDBLocaleQuery.bindValue(":md5sum", remote_md5sum);
		//		mBrickMoneyDBLocaleQuery.bindValue(":image_data", remote_image_data);
		//		mBrickMoneyDBLocaleQuery.exec();
		//	}
		//}
	

		
		
	}
	d_ptr->mBrickMoneyDBLocale.commit();

	brickmoney_locale_query.prepare("UPDATE Version SET version=:version WHERE ID=0");
	brickmoney_locale_query.bindValue(":version", d_ptr->mRemoteVersion);
	brickmoney_locale_query.exec();

	remoteDB.close();
	LOG_INFO("BrickMoneyDBLocale updated.");
	return true;
}

QVector<QPixmap> BrickMoneyDatabase::queryLegoSetImages(const int & legoset_id)
{
	 QVector<QPixmap> images;
	 QSqlQuery brickmoney_locale_query(d_ptr->mBrickMoneyDBLocale);

	 brickmoney_locale_query.prepare("SELECT * FROM Images WHERE legoset_id=:legoset_id");
	 brickmoney_locale_query.bindValue(":legoset_id", legoset_id);
	if (!brickmoney_locale_query.exec())
	{
		LOG_ERROR("Can't execute query the locate Images table!");
		return images;
	}

	while (brickmoney_locale_query.next()) {
		QByteArray outByteArray = brickmoney_locale_query.value("image_data").toByteArray();
		QPixmap outPixmap = QPixmap();
		outPixmap.loadFromData(outByteArray);
		images.append(outPixmap);
	}

	return images;
}

LegoSetInfo BrickMoneyDatabase::queryLegoSetInfo(const int & set_id)
{
	LegoSetInfo info;

	QSqlQuery brickmoney_locale_query(d_ptr->mBrickMoneyDBLocale);

	brickmoney_locale_query.prepare("SELECT * FROM LegoSets WHERE set_id=:set_id");
	brickmoney_locale_query.bindValue(":set_id", set_id);
	if (!brickmoney_locale_query.exec())
	{
		LOG_ERROR("Can't execute query the locate LegoSets table!");
		return info;
	}

	while (brickmoney_locale_query.next()) {
		info = LegoSetInfo(brickmoney_locale_query.value("set_id").toInt()
			, brickmoney_locale_query.value("name_en").toString()
			, brickmoney_locale_query.value("name_de").toString()
			, brickmoney_locale_query.value("year").toInt()
			, brickmoney_locale_query.value("rr_price").toDouble());
	}

	return info;
}

LegoSetInfo BrickMoneyDatabase::nextLegoSetInfo(const int & set_id)
{
	LegoSetInfo info;

	QSqlQuery brickmoney_locale_query(d_ptr->mBrickMoneyDBLocale);

	brickmoney_locale_query.prepare("SELECT * FROM LegoSets WHERE set_id > :set_id ORDER BY set_id ASC LIMIT 1");
	brickmoney_locale_query.bindValue(":set_id", set_id);
	if (!brickmoney_locale_query.exec())
	{
		LOG_ERROR("Can't execute query the locate LegoSets table!");
		return info;
	}

	while (brickmoney_locale_query.next()) {
		info = LegoSetInfo(brickmoney_locale_query.value("set_id").toInt()
			, brickmoney_locale_query.value("name_en").toString()
			, brickmoney_locale_query.value("name_de").toString()
			, brickmoney_locale_query.value("year").toInt()
			, brickmoney_locale_query.value("rr_price").toDouble());
	}

	return info;
}

LegoSetInfo BrickMoneyDatabase::previousLegoSetInfo(const int & set_id)
{
	LegoSetInfo info;

	QSqlQuery mBrickMoneyDBLocaleQuery(d_ptr->mBrickMoneyDBLocale);

	mBrickMoneyDBLocaleQuery.prepare("SELECT * FROM LegoSets WHERE set_id < :set_id ORDER BY set_id DESC LIMIT 1");
	mBrickMoneyDBLocaleQuery.bindValue(":set_id", set_id);
	if (!mBrickMoneyDBLocaleQuery.exec())
	{
		LOG_ERROR("Can't execute query the locate LegoSets table!");
		return info;
	}

	while (mBrickMoneyDBLocaleQuery.next()) {
		info = LegoSetInfo(mBrickMoneyDBLocaleQuery.value("set_id").toInt()
			, mBrickMoneyDBLocaleQuery.value("name_en").toString()
			, mBrickMoneyDBLocaleQuery.value("name_de").toString()
			, mBrickMoneyDBLocaleQuery.value("year").toInt()
			, mBrickMoneyDBLocaleQuery.value("rr_price").toDouble());
	}

	return info;
}

QString BrickMoneyDatabase::calcMD5Sum(const QString & imageFilePath)
{
	//Ref: https://stackoverflow.com/questions/16383392/how-to-get-the-sha-1-md5-checksum-of-a-file-with-qt
	QFile f(imageFilePath);
	if (f.open(QFile::ReadOnly)) {
		QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
		if (hash.addData(&f)) {
			return hash.result().toHex();
		}
	}
	return QByteArray();
}

QByteArray BrickMoneyDatabase::calcBlobData(const QString & imageFilePath)
{
	QPixmap pm(imageFilePath);
	QByteArray inByteArray;
	QBuffer inBuffer(&inByteArray);
	inBuffer.open(QIODevice::WriteOnly);
	pm.save(&inBuffer, "JPG");
	return inByteArray;
}
