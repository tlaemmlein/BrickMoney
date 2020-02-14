#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetRecord")

#include "LegoSetRecord.h"

static double calcCheaperPercent(double rrp, double purchasingPrice)
{
   return (purchasingPrice == 0.0) ? 0.0 : ( (1.0 - purchasingPrice/rrp) * 100.0);
}


LegoSetRecord::LegoSetRecord(QString new_imageName, QString new_imageFilePath,
                             int new_setnumber, QString new_description,
                             int new_year, double new_rrp, double new_purchasing_price):
      mImageName( new LegoSetImage(new_imageName)), mImageFilePath( new LegoSetImageFilePath(new_imageFilePath))
	, mSetNumber(new LegoSetNumber(new_setnumber)), mDescription(new LegoSetDescription(new_description))
	, mYear(new LegoSetYear(new_year)), mRRetailPrice( new LegoSetRecommendedRetailPrice(new_rrp))
	, mPurchasingPrice(new LegoSetPurchasingPrice(new_purchasing_price))
    , mCheaperPercent(new LegoSetCheaperPercent(calcCheaperPercent(new_rrp, new_purchasing_price)) )
{
    mRecord.push_back(mImageName);
    mRecord.push_back(mImageFilePath);
    mRecord.push_back(mSetNumber);
    mRecord.push_back(mDescription);
    mRecord.push_back(mYear);
    mRecord.push_back(mRRetailPrice);
    mRecord.push_back(mPurchasingPrice);
    mRecord.push_back(mCheaperPercent);

    mColumnCountForGui = 0;
    mColumnCountForIO = 0;
    for( auto item : mRecord)
    {
        if ( item->role() >= 0 )
        {
            ++mColumnCountForGui;
            mRoles[item->role()] = item->roleName();
        }

        if ( item->shouldBeIO())
        {
            ++mColumnCountForIO;
        }
    }
}

LegoSetRecord::~LegoSetRecord()
{
	for (auto item : mRecord)
	{
		delete item;
		item = nullptr;
	}
}

int LegoSetRecord::columnCountForGui() const
{
    return mColumnCountForGui;
}

int LegoSetRecord::columnCountForIO() const
{
    return mColumnCountForIO;
}

std::vector<LegoSetItem *> LegoSetRecord::record() const
{
    return mRecord;
}

QHash<int, QByteArray> LegoSetRecord::roleNames() const
{
    return mRoles;
}

void LegoSetRecord::check(LegoSetItem * item)
{
	if (item == mRRetailPrice || item == mPurchasingPrice)
	{
		mCheaperPercent->setValue(calcCheaperPercent(mRRetailPrice->getValue(), mPurchasingPrice->getValue()));
	}
}
