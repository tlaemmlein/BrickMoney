#ifndef LEGOSET_TABLE_DATA_P_H
#define LEGOSET_TABLE_DATA_P_H

#include "LegoSetRecord.h"

#include <QObject>
#include <QHash>
#include <QByteArray>
#include <QString>
#include <QVariant>

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

class LegoSetSeller : public LegoSetString
{
public:
    LegoSetSeller(const QString& value) : LegoSetString(value) {}
    ~LegoSetSeller() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "seller"; }
    bool shouldBeIO() override { return true;}
private:
    static const int mRole = Qt::UserRole +8;
};

class LegoSetPurchaseDate : public LegoSetString
{
public:
    LegoSetPurchaseDate(const QString& value) : LegoSetString(value) {}
    ~LegoSetPurchaseDate() = default;
    int role() const override { return mRole;}
    QByteArray roleName() override { return "purchaseDate"; }
    bool shouldBeIO() override { return true;}
private:
    static const int mRole = Qt::UserRole +9;
};

using LegoSetItemRecord = std::vector<LegoSetItem*>;

class LegoSetRecordInternal : public QObject
{
public:
    explicit LegoSetRecordInternal(QObject *parent= nullptr) : QObject(parent),
          mImageName( new LegoSetImage("imageName"))
        , mImageFilePath( new LegoSetImageFilePath("imageFilePath"))
        , mSetNumber(new LegoSetNumber(123))
        , mDescription(new LegoSetDescription("description"))
        , mYear(new LegoSetYear(2018))
        , mRRetailPrice( new LegoSetRecommendedRetailPrice(100.0))
        , mPurchasingPrice(new LegoSetPurchasingPrice(10.0))
        , mCheaperPercent(new LegoSetCheaperPercent(calcCheaperPercent(100.0, 10.0)))
        , mSeller(new LegoSetSeller("Seller"))
        , mPurchaseDate(new LegoSetPurchaseDate("07.0.72018"))
    {
        init();
    }

    LegoSetRecordInternal(const LegoSetRecord& record, QObject *parent = nullptr): QObject(parent)
        , mImageName( new LegoSetImage( std::get<0>(record) ) )
        , mImageFilePath( new LegoSetImageFilePath(std::get<1>(record) ) )
        , mSetNumber(new LegoSetNumber(std::get<2>(record) ) )
        , mDescription(new LegoSetDescription(std::get<3>(record) ) )
        , mYear(new LegoSetYear(std::get<4>(record) ) )
        , mRRetailPrice( new LegoSetRecommendedRetailPrice(std::get<5>(record) ) )
        , mPurchasingPrice(new LegoSetPurchasingPrice(std::get<6>(record) ) )
        , mCheaperPercent(new LegoSetCheaperPercent(calcCheaperPercent(std::get<5>(record), std::get<6>(record)) ))
        , mSeller(new LegoSetSeller(std::get<7>(record)))
        , mPurchaseDate( new LegoSetPurchaseDate(std::get<8>(record)))
    {
        init();
    }

    ~LegoSetRecordInternal() {
        for (auto item : mRecord)
        {
            delete item;
            item = nullptr;
        }
    }

    // implicit
    LegoSetRecordInternal(const LegoSetRecordInternal&) = delete;
    LegoSetRecordInternal& operator=(const LegoSetRecordInternal&) = delete;


    void check(LegoSetItem* item)
    {
        if (item == mRRetailPrice || item == mPurchasingPrice)
        {
            mCheaperPercent->setValue(calcCheaperPercent(mRRetailPrice->getValue(), mPurchasingPrice->getValue()));
        }
    }

    double calcCheaperPercent(double rrp, double purchasingPrice)
    {
        return (purchasingPrice == 0.0) ? 0.0 : ( (1.0 - purchasingPrice/rrp) * 100.0);
    }

    LegoSetImage* mImageName;
    LegoSetImageFilePath* mImageFilePath;
    LegoSetNumber* mSetNumber;
    LegoSetDescription* mDescription;
    LegoSetYear* mYear;
    LegoSetRecommendedRetailPrice* mRRetailPrice;
    LegoSetPurchasingPrice* mPurchasingPrice;
    LegoSetCheaperPercent* mCheaperPercent;
    LegoSetSeller* mSeller;
    LegoSetPurchaseDate* mPurchaseDate;

    LegoSetItemRecord mRecord;

private:
    void init(){
        mRecord.push_back(mImageName);
        mRecord.push_back(mImageFilePath);
        mRecord.push_back(mSetNumber);
        mRecord.push_back(mDescription);
        mRecord.push_back(mYear);
        mRecord.push_back(mRRetailPrice);
        mRecord.push_back(mPurchasingPrice);
        mRecord.push_back(mCheaperPercent);
        mRecord.push_back(mSeller);
        mRecord.push_back(mPurchaseDate);
    }
};


class LegoSetTableDataPrivate : public QObject
{
public:
    explicit LegoSetTableDataPrivate(QObject* parent = nullptr): QObject(parent) {}

    QList< LegoSetRecordInternal* > mData;
    QHash<int, QByteArray> mRoles;
    int mColumnCountForGui;
    int mColumnCountForIO;
};

#endif

