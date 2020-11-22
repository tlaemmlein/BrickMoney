
#include "BrickMoneyDatabaseImages.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QDir>
#include <QPixmap>


QString BrickMoneyDatabaseImages::calcMD5Sum(const QString & imageFilePath)
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

QByteArray BrickMoneyDatabaseImages::calcBlobData(const QString & imageFilePath)
{
	QPixmap pm(imageFilePath);
	QByteArray inByteArray;
	QBuffer inBuffer(&inByteArray);
	inBuffer.open(QIODevice::WriteOnly);
	pm.save(&inBuffer, "JPG");
	return inByteArray;
}
