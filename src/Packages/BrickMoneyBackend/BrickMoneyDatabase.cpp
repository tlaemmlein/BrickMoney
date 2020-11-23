#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyDatabase")


#include "BrickMoneyDatabase.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QDir>
#include <QPixmap>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>


QSqlDatabase BrickMoneyDatabase::prepareBrickMoneyDBLocale(const QString & legoSetDatabasePath)
{
	LOG_INFO("Prepare BrickMoneyDBLocale.");
	QSqlDatabase localeDB;

	QString dbName(legoSetDatabasePath + "/BrickMoneyDBLocale.db3");
	if (!localeDB.contains("BrickMoneyDBLocale"))
		localeDB = QSqlDatabase::addDatabase("QSQLITE", "BrickMoneyDBLocale");
	localeDB.setDatabaseName(dbName);
	if (!localeDB.open())
	{
		auto error = localeDB.lastError();
		LOG_ERROR("Could not open " << dbName.toStdWString());
		LOG_ERROR(error.databaseText().toStdWString());
		LOG_ERROR(error.driverText().toStdWString());
		LOG_ERROR(error.nativeErrorCode().toStdWString());
		return localeDB;
	}
	QSqlQuery localeDBQuery(localeDB);

	QStringList sql_querys;
	sql_querys << "CREATE TABLE IF NOT EXISTS LegoSets (set_id bigint PRIMARY KEY, name_en text NOT NULL, name_de text NOT NULL, year INTEGER NOT NULL, rr_price float)"
		<< "CREATE TABLE IF NOT EXISTS Images (legoset_id bigint REFERENCES LegoSets (set_id), name text, md5sum text, image_data BLOB)"
		<< "CREATE TABLE IF NOT EXISTS Version (id int PRIMARY KEY, version bigint)"
		<< "INSERT OR IGNORE INTO Version (id) VALUES (0)"
		<< "CREATE UNIQUE INDEX IF NOT EXISTS filename ON Images ( legoset_id, name )";

	for (const auto& sql_query : sql_querys)
	{
		if (!localeDBQuery.exec(sql_query))
		{
			LOG_ERROR("Could not exec query. Last error: " << localeDBQuery.lastError().text().toStdWString());
			localeDB.close();
			return localeDB;
		}
	}
	LOG_INFO("BrickMoneyDBLocale is ready.");

	return localeDB;
}

bool BrickMoneyDatabase::updateBrickMoneyDBLocale(QSqlDatabase & localeDB)
{
	if (!localeDB.isOpen())
		return false;

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
		return false;
	}

	QSqlQuery remoteQuery(remoteDB);
	remoteQuery.setForwardOnly(true);

	int remoteVersion = 0;
	remoteQuery.exec("SELECT version FROM Version");
	while (remoteQuery.next())
		remoteVersion = remoteQuery.value("version").toInt();

	int localeVersion = 0;
	QSqlQuery localeDBQuery(localeDB);
	localeDBQuery.exec("SELECT version FROM Version");
	while (localeDBQuery.next())
		localeVersion = localeDBQuery.value("version").toInt();

	if (localeVersion == remoteVersion)
	{
		LOG_INFO("The locale db is up to date.");
		return true;
	}

	localeDBQuery.prepare("UPDATE Version SET version=:version WHERE ID=0");
	localeDBQuery.bindValue(":version", remoteVersion);
	localeDBQuery.exec();

	localeDB.transaction();
	remoteQuery.prepare("SELECT * FROM LegoSets");
	if (!remoteQuery.exec())
	{
		LOG_ERROR("Can't execute query the remote LegoSets table!");
		remoteDB.close();
		return false;
	}

	LOG_INFO("Remote LegoSets query successful!");

	while (remoteQuery.next())
	{
		// https://stackoverflow.com/questions/3634984/insert-if-not-exists-else-update
		localeDBQuery.prepare("INSERT OR REPLACE INTO LegoSets (set_id, name_en, name_de, year, rr_price) "
			"VALUES (:set_id, :name_en, :name_de, :year, :rr_price)");
		localeDBQuery.bindValue(":set_id", remoteQuery.value("set_id").toInt());
		localeDBQuery.bindValue(":name_en", remoteQuery.value("name_en").toString());
		localeDBQuery.bindValue(":name_de", remoteQuery.value("name_de").toString());
		localeDBQuery.bindValue(":year", remoteQuery.value("year").toInt());
		localeDBQuery.bindValue(":rr_price", remoteQuery.value("rr_price").toDouble());
		localeDBQuery.exec();
	}
	localeDB.commit();
	remoteDB.close();
	LOG_INFO("BrickMoneyDBLocale updated.");
	return true;
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
