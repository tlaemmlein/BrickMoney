#include "Logging.h"
SET_LOGGER("BrickMoney.DataSource")

#include "LegoSetDataSource.h"
#include "LegoSetInfoGenerator.h"
#include "LegoSet.h"
#include "BrickMoneySettings.h"

#include <QDate>
#include <QJsonObject>


const QString LegoSetDataSource::IsSelectedName = "IsSelected";
const QString LegoSetDataSource::SetNumberName = "SetNumber";
const QString LegoSetDataSource::PurchasingPriceName = "PurchasingPrice";
const QString LegoSetDataSource::SellerName = "Seller";
const QString LegoSetDataSource::PurchaseDateName = "PurchaseDate";
const QString LegoSetDataSource::RetailPriceName = "RetailPrice";
const QString LegoSetDataSource::SaleDateName = "SaleDate";
const QString LegoSetDataSource::SoldOverName = "SolderOver";
const QString LegoSetDataSource::BuyerName = "Buyer";


LegoSetDataSource::LegoSetDataSource(QObject *parent) : QObject(parent)
{

}

void LegoSetDataSource::addLegoSet(LegoSet *set)
{
    emit preLegoSetAdded();
    m_legoSets.append(set);
    emit postLegoSetAdded();
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSetDataSource::removeLegoSet(int index)
{
    emit preLegoSetRemoved(index);
    m_legoSets.removeAt(index);
    emit postLegoSetRemoved();
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(true);
}

int LegoSetDataSource::legoSetCount()
{
    return m_legoSets.count();
}

LegoSet *LegoSetDataSource::legoSetAt(int index)
{
    return m_legoSets[index];
}

void LegoSetDataSource::clearLegoSets()
{
    m_legoSets.clear();
    BrickMoneySettings::Inst()->setBrickMoneyIsDirty(true);
}

bool LegoSetDataSource::read(const QJsonArray& legoSetArray)
{
	LOG_SCOPE_METHOD(L"");

	clearLegoSets();
	
	LegoSetInfoGenerator gen;

	for (int index = 0; index < legoSetArray.size(); ++index) {

		QJsonObject obj = legoSetArray[index].toObject();
		
		if (!obj.contains(SetNumberName))
		{
			LOG_ERROR("We need a set number!");
			continue;
		}

		const int setNum = obj[SetNumberName].toInt();

		if (!gen.querySetNumber(setNum))
		{
			LOG_ERROR("Could not found the set number " << setNum << " in the database.");
			continue;
		}
			

		LegoSet* set = new LegoSet(this);
		set->setSetNumber(setNum);

        if (obj.contains(IsSelectedName) && obj[IsSelectedName].isBool())
            set->setIsSelected(obj[IsSelectedName].toBool());

        if (obj.contains(PurchasingPriceName) && obj[PurchasingPriceName].isDouble())
			set->setPurchasingPrice(obj[PurchasingPriceName].toDouble());

		if (obj.contains(SellerName) && obj[SellerName].isString())
			set->setSeller(obj[SellerName].toString());

		if (obj.contains(PurchaseDateName) && obj[PurchaseDateName].isString())
			set->setPurchaseDate(QDate::fromString(obj[PurchaseDateName].toString()));

		if (obj.contains(RetailPriceName) && obj[RetailPriceName].isDouble())
			set->setRetailPrice(obj[RetailPriceName].toDouble());

		if (obj.contains(SaleDateName) && obj[SaleDateName].isString())
			set->setSaleDate(QDate::fromString(obj[SaleDateName].toString()));

		if (obj.contains(SoldOverName) && obj[SoldOverName].isString())
			set->setSoldOver((obj[SoldOverName].toString()));

		if (obj.contains(BuyerName) && obj[BuyerName].isString())
			set->setBuyer((obj[BuyerName].toString()));

		addLegoSet(set);
	}

	BrickMoneySettings::Inst()->setBrickMoneyIsDirty(false);
	emit resetLegoSets();
	return true;
}

bool LegoSetDataSource::write(QJsonArray &legoSetArray)
{
    LOG_SCOPE_METHOD(L"");

	for (const auto& set : m_legoSets)
	{
		QJsonObject obj;
        obj[SetNumberName] = set->setNumber();

        obj[IsSelectedName] = set->isSelected();

		obj[PurchasingPriceName] = set->purchasingPrice();
		if (!set->seller().isEmpty())
			obj[SellerName] = set->seller();

		obj[PurchaseDateName] = set->purchaseDate().toString();
		obj[RetailPriceName] = set->retailPrice();
		obj[SaleDateName] = set->saleDate().toString();

		if (!set->soldOver().isEmpty())
			obj[SoldOverName] = set->soldOver();

		if (!set->buyer().isEmpty())
			obj[BuyerName] = set->buyer();

		legoSetArray.append(obj);
	}
	BrickMoneySettings::Inst()->setBrickMoneyIsDirty(false);
	return true;
}


