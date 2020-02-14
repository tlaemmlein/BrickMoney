#ifndef LEGOSET_RECORD_H
#define LEGOSET_RECORD_H

#include <QString>
#include <QVariant>

#include <vector>

class LegoSetItem
{
public:
    LegoSetItem(){}
    virtual ~LegoSetItem() = default;

    virtual int role() const = 0;
    virtual QByteArray roleName() = 0;
    virtual QVariant getData() = 0;
    virtual bool setData(const QVariant &value) = 0;
    virtual bool shouldBeIO() = 0;
};

class LegoSetInt : public LegoSetItem
{
public:
    LegoSetInt(int value) : mValue(value) {}
    ~LegoSetInt() = default;

    QVariant getData() override
    {
        return QVariant(mValue);
    }
    bool setData(const QVariant &value) override
    {
        bool ok;
        int temp = value.toInt(&ok);
        if (!ok)
            return false;
        mValue = temp;
        return true;
    }
    int getValue() { return mValue;}
	void setValue(int value) { mValue = value; }
private:
    int mValue;
};

class LegoSetDouble : public LegoSetItem
{
public:
    LegoSetDouble(double value) : mValue(value) {}
    ~LegoSetDouble() = default;

    QVariant getData() override
    {
        return QVariant(mValue);
    }
    bool setData(const QVariant &value) override
    {
        bool ok;
        int temp = value.toDouble(&ok);
        if (!ok)
            return false;
        mValue = temp;
        return true;
    }
    double getValue() { return mValue;}
	void setValue(double value) { mValue = value; }
private:
    double mValue;
};

class LegoSetString : public LegoSetItem
{
public:
    LegoSetString(const QString& value) : mValue(value) {}
    ~LegoSetString() = default;

    QVariant getData() override
    {
        return QVariant(mValue);
    }
    bool setData(const QVariant &value) override
    {
        mValue = value.toString();
        return true;
    }
    QString getValue() { return mValue;}
	void setValue(const QString& value) { mValue = value; }
private:
    QString mValue;
};

class LegoSetImage : public LegoSetString
{
public:
    LegoSetImage(const QString& value) : LegoSetString(value) {}
    ~LegoSetImage() = default;
    int role() const override { return -1;}
    QByteArray roleName() override { return "imageFilePath"; }
    bool shouldBeIO() override { return true;}
};

class LegoSetImageFilePath : public LegoSetString
{
public:
    LegoSetImageFilePath(const QString& value) : LegoSetString(value) {}
    ~LegoSetImageFilePath() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "image"; }
    bool shouldBeIO() override { return false;}
private:
    static const int mRole = Qt::UserRole +1;
};

class LegoSetNumber : public LegoSetInt
{
public:
    LegoSetNumber(int value) : LegoSetInt(value) {}
    ~LegoSetNumber() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "setnumber"; }
    bool shouldBeIO() override { return true;}
private:
    static const int mRole = Qt::UserRole +2;
};

class LegoSetDescription : public LegoSetString
{
public:
    LegoSetDescription(const QString& value) : LegoSetString(value) {}
    ~LegoSetDescription() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "description"; }
    bool shouldBeIO() override { return true;}
private:
    static const int mRole = Qt::UserRole +3;
};

class LegoSetYear : public LegoSetInt
{
public:
    LegoSetYear(int value) : LegoSetInt(value) {}
    ~LegoSetYear() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "year"; }
    bool shouldBeIO() override { return true;}
private:
    static const int mRole = Qt::UserRole +4;
};

class LegoSetRecommendedRetailPrice : public LegoSetDouble
{
public:
    LegoSetRecommendedRetailPrice(double value) : LegoSetDouble(value) {}
    ~LegoSetRecommendedRetailPrice() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "recommendedRetailPrice"; }
    bool shouldBeIO() override { return true;}
private:
    static constexpr double mRole = Qt::UserRole +5;
};

class LegoSetPurchasingPrice : public LegoSetDouble
{
public:
    LegoSetPurchasingPrice(double value) : LegoSetDouble(value) {}
    ~LegoSetPurchasingPrice() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "purchasingPrice"; }
    bool shouldBeIO() override { return true;}
private:
    static constexpr double mRole = Qt::UserRole +6;
};

class LegoSetCheaperPercent : public LegoSetDouble
{
public:
    LegoSetCheaperPercent(double value) : LegoSetDouble(value) {}
    ~LegoSetCheaperPercent() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "cheaperPercent"; }
    bool shouldBeIO() override { return false;}
private:
    static constexpr double mRole = Qt::UserRole +7;
};

class LegoSetRecord
{
public:
    LegoSetRecord(QString new_imageName, QString new_imageFilePath, int new_setnumber, QString new_description, int new_year
                            ,double new_rrp, double new_purchasing_price);

	~LegoSetRecord();

	// implicit
	LegoSetRecord(const LegoSetRecord&) = delete;
	LegoSetRecord& operator=(const LegoSetRecord&) = delete;

    int columnCountForGui() const;

    int columnCountForIO() const;

    std::vector<LegoSetItem*> record() const;

    QHash<int, QByteArray> roleNames() const;

	void check(LegoSetItem* item);

private:
    LegoSetImage* mImageName;
    LegoSetImageFilePath* mImageFilePath;
    LegoSetNumber* mSetNumber;
    LegoSetDescription* mDescription;
    LegoSetYear* mYear;
    LegoSetRecommendedRetailPrice* mRRetailPrice;
    LegoSetPurchasingPrice* mPurchasingPrice;
    LegoSetCheaperPercent* mCheaperPercent;

    std::vector<LegoSetItem*> mRecord;
    QHash<int, QByteArray> mRoles;

    int mColumnCountForGui;
    int mColumnCountForIO;
};


#endif

