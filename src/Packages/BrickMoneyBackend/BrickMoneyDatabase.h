#ifndef BRICKMONEY_DATABASE_IMAGES_H
#define BRICKMONEY_DATABASE_IMAGES_H

#include <QObject>
#include <QSqlDatabase>

class BrickMoneyDatabase : public QObject
{
public:
    BrickMoneyDatabase(const BrickMoneyDatabase&) = delete;            // No copy ctor
    BrickMoneyDatabase& operator=(const BrickMoneyDatabase&) = delete; //No assign op


	static QSqlDatabase prepareBrickMoneyDBLocale(const QString& legoSetDatabasePath);
	static bool updateBrickMoneyDBLocale(QSqlDatabase& localeDB);

	static QString calcMD5Sum(const QString& imageFilePath);
	static QByteArray calcBlobData(const QString& imageFilePath);
};

#endif
