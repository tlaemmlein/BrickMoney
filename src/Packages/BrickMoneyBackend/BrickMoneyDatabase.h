#ifndef BRICKMONEY_DATABASE_H
#define BRICKMONEY_DATABASE_H

#include <QPixmap>
#include <QString>
#include <QVector>

#include <optional>

struct LegoSetInfo {

	LegoSetInfo() : is_valid(false), set_id(0), name_en("Null"), name_de("Null"), year(-1), rr_price(0.0)
	{}
	
	LegoSetInfo(bool other_info_is_valid, int other_set_id, const QString& other_name_en, const QString& other_name_de,  int other_year, double  other_rr_price
	) : is_valid(other_info_is_valid)
		, set_id(other_set_id)
		, name_en(other_name_en)
		, name_de(other_name_de)
		, year(other_year)
		, rr_price(other_rr_price)
	{}

	bool    is_valid;
	int     set_id;
	QString name_en;
	QString name_de;
	int     year;
	double  rr_price; // recommended retail price
};

class RemoteDBException : public std::exception
{
public:
	explicit RemoteDBException(const char* message) :
		msg_(message)	{
	}

	explicit RemoteDBException(const std::string& message) :
		msg_(message){}

	virtual ~RemoteDBException() noexcept {}

	virtual const char* what() const noexcept {
		return msg_.c_str();
	}

protected:
	std::string msg_;
};

class BrickMoneyDatabasePrivate;

class BrickMoneyDatabase
{
public:
	static BrickMoneyDatabase* Inst();
	~BrickMoneyDatabase();
	BrickMoneyDatabase(const BrickMoneyDatabase&) = delete;            // No copy ctor
	BrickMoneyDatabase& operator=(const BrickMoneyDatabase&) = delete; //No assign op

	bool prepareBrickMoneyDBLocale(const QString& legoSetDatabasePath);

	int remoteDBVersion() const;

	int localeDBVersion() const;

	/// \exception RemoteDBException if the db is not online.  
	bool isNewRemoteVersionAvailable();

	/// \exception RemoteDBException if the db is not online.  
	bool updateBrickMoneyDBLocale();

	QVector<QPixmap> queryLegoSetImages(const int& legoset_id);
	LegoSetInfo queryLegoSetInfo(const int& set_id);

	LegoSetInfo nextLegoSetInfo(const int& set_id);
	LegoSetInfo previousLegoSetInfo(const int& set_id);


	QString calcMD5Sum(const QString& imageFilePath);
	QByteArray calcBlobData(const QString& imageFilePath);

private:
	static std::unique_ptr<BrickMoneyDatabase> smInstance;
	BrickMoneyDatabase();

	std::unique_ptr<BrickMoneyDatabasePrivate> d_ptr;
};

#endif
