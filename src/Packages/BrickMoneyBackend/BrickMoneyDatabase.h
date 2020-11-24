#ifndef BRICKMONEY_DATABASE_IMAGES_H
#define BRICKMONEY_DATABASE_IMAGES_H

#include <QObject>
#include <QSqlDatabase>

struct LegoSetInfo {

	LegoSetInfo() : setNumber(-1), description(""), year(-1), recommendedRetailPrice(0.0)
	{}

	LegoSetInfo(int otherSetNumber, QString otherDescription, int otherYear, double  otherRecommendedRetailPrice
	) : setNumber(otherSetNumber)
		, description(otherDescription)
		, year(otherYear)
		, recommendedRetailPrice(otherRecommendedRetailPrice)
	{}

	bool operator==(const LegoSetInfo& other) const
	{
		return (setNumber == other.setNumber && description == other.description
			&& year == other.year && recommendedRetailPrice == other.recommendedRetailPrice);
	}

	bool operator!=(const LegoSetInfo& other) const
	{
		return (!operator==(other));
	}

	bool operator < (const LegoSetInfo &info) const {
		return setNumber < info.setNumber;
	}

	int     setNumber;
	QString description;
	int     year;
	double  recommendedRetailPrice;
};

Q_DECLARE_METATYPE(LegoSetInfo)


class BrickMoneyDatabase : public QObject
{
public:
    BrickMoneyDatabase(const BrickMoneyDatabase&) = delete;            // No copy ctor
    BrickMoneyDatabase& operator=(const BrickMoneyDatabase&) = delete; //No assign op


	static QSqlDatabase prepareBrickMoneyDBLocale(const QString& legoSetDatabasePath);
	static bool updateBrickMoneyDBLocale(QSqlDatabase& localeDB);

	static QVector<QPixmap> queryLegoSetImages(QSqlDatabase& localeDB, const int& legoset_id);

	static QString calcMD5Sum(const QString& imageFilePath);
	static QByteArray calcBlobData(const QString& imageFilePath);
};

#endif
