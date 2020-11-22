#ifndef BRICKMONEY_DATABASE_IMAGES_H
#define BRICKMONEY_DATABASE_IMAGES_H

#include <QObject>

class BrickMoneyDatabaseImages : public QObject
{
public:
    BrickMoneyDatabaseImages(const BrickMoneyDatabaseImages&) = delete;            // No copy ctor
    BrickMoneyDatabaseImages& operator=(const BrickMoneyDatabaseImages&) = delete; //No assign op

	static QString calcMD5Sum(const QString& imageFilePath);
	static QByteArray calcBlobData(const QString& imageFilePath);
};

#endif
