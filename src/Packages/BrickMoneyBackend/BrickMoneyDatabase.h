#ifndef BRICKMONEY_DATABASE_IMAGES_H
#define BRICKMONEY_DATABASE_IMAGES_H

#include <QObject>
#include <QSqlDatabase>

#include <optional>

struct LegoSetInfo {

	LegoSetInfo() : set_id{}, name_en(""), name_de(""), year(-1), rr_price(0.0)
	{}

	LegoSetInfo(int other_set_id, const QString& other_name_en, const QString& other_name_de,  int other_year, double  other_rr_price
	) : set_id(other_set_id)
		, name_en(other_name_en)
		, name_de(other_name_de)
		, year(other_year)
		, rr_price(other_rr_price)
	{}

	bool operator==(const LegoSetInfo& other) const
	{
		return (set_id == other.set_id && name_en == other.name_en
			&& name_de == other.name_de
			&& year == other.year && rr_price == other.rr_price);
	}

	bool operator!=(const LegoSetInfo& other) const
	{
		return (!operator==(other));
	}

	bool operator < (const LegoSetInfo &info) const {
		return set_id < info.set_id;
	}

	std::optional<int>     set_id;
	QString name_en;
	QString name_de;
	int     year;
	double  rr_price; // recommended retail price
};

Q_DECLARE_METATYPE(LegoSetInfo)


class BrickMoneyDatabase : public QObject
{
public:
    BrickMoneyDatabase(const BrickMoneyDatabase&) = delete;            // No copy ctor
    BrickMoneyDatabase& operator=(const BrickMoneyDatabase&) = delete; //No assign op


	static bool prepareBrickMoneyDBLocale(const QString& legoSetDatabasePath);
	static bool updateBrickMoneyDBLocale();

	static QVector<QPixmap> queryLegoSetImages(const int& legoset_id);
	static LegoSetInfo queryLegoSetInfo(const int& set_id);

	static LegoSetInfo nextLegoSetInfo(const int& set_id);
	static LegoSetInfo previousLegoSetInfo(const int& set_id);


	static QString calcMD5Sum(const QString& imageFilePath);
	static QByteArray calcBlobData(const QString& imageFilePath);

private:
	static QSqlDatabase mBrickMoneyDBLocale;

};

#endif
