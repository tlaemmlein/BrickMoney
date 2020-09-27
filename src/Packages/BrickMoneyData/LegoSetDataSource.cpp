#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.DataSource")

#include "LegoSetDataSource.h"

#include "LegoSet.h"
#include "BrickMoneyDataManager.h"

#include "Packages/LegoSetInfoGenerator/LegoSetInfoGenerator.h"

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
    , m_SelectedLegoSets(0)
{

}

void LegoSetDataSource::addLegoSet(LegoSet *set)
{
    emit preLegoSetAdded();
    m_legoSets.append(set);
    connectSelection(set);
    emit postLegoSetAdded();
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

QVector<LegoSet *> LegoSetDataSource::getSelectedLegoSets()
{
    QVector<LegoSet*> selectedSets;

    for (const auto& set : m_legoSets)
        if (set->isSelected())
            selectedSets.push_back(set);

    return selectedSets;
}

void LegoSetDataSource::selectAllSets()
{
    for (const auto& set : m_legoSets)
        set->setIsSelected(true);
}

void LegoSetDataSource::selectNoneSets()
{
    for (const auto& set : m_legoSets)
        set->setIsSelected(false);
}

int LegoSetDataSource::getNumberSelectedLegoSets()
{
    return m_SelectedLegoSets;
}


void LegoSetDataSource::removeSelectedLegoSets()
{
    for (const auto& set : getSelectedLegoSets())
    {
        int index = m_legoSets.indexOf(set);
        emit preLegoSetRemoved(index);
        m_legoSets.removeAt(index);
        emit postLegoSetRemoved();
    }
    setNumberSelectedLegoSets(0);
    emit selectionIsDirtyChanged(false);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
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
    setNumberSelectedLegoSets(0);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSetDataSource::loadDataFrom(const QChar &sep, QTextStream &in)
{
    static const int NumberOfNeededParams = 8; // All except IsSelectedName;

    QLocale locale;

    const QString dateFormat = "dd.MM.yyyy"; // locale.dateFormat();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList row = line.split(sep);
        if (row.size() != NumberOfNeededParams)
            continue;

        const int setNum = row.at(0).toInt();
        const double purchasingPrice = locale.toDouble(row.at(1));
        const QString seller = row.at(2);
        const QDate purchaseDate = row.at(3).isEmpty() ? QDate::currentDate() : QDate::fromString(row.at(3), dateFormat);
        const double retailPrice = locale.toDouble(row.at(4));
        const QDate saleDate = row.at(5).isEmpty() ? QDate::currentDate() : QDate::fromString(row.at(5), dateFormat);
        const QString soldOver = row.at(6);
        const QString buyer = row.at(7);

        LegoSet* set = new LegoSet(this);
        set->setSetNumber(setNum);

        set->setIsSelected(true);
        set->setPurchasingPrice(purchasingPrice);
        set->setSeller(seller);
        set->setPurchaseDate(purchaseDate);
        set->setRetailPrice(retailPrice);
        set->setSaleDate(saleDate);
        set->setSoldOver(soldOver);
        set->setBuyer(buyer);

        addLegoSet(set);
    }
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

    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(false);
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
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(false);
    return true;
}

bool LegoSetDataSource::selectionIsDirty()
{
    return getNumberSelectedLegoSets() > 0;
}

void LegoSetDataSource::setNumberSelectedLegoSets(int num)
{
    if (num == m_SelectedLegoSets)
        return;
    m_SelectedLegoSets = num;
    emit numberSelectedLegoSetsChanged(m_SelectedLegoSets);
}

void LegoSetDataSource::connectSelection(LegoSet *set)
{
    connect(set, &LegoSet::isSelectedChanged, [&](bool selected) {
        const int previous = getNumberSelectedLegoSets();
        int newNumber = getNumberSelectedLegoSets();
        setNumberSelectedLegoSets(selected ? ++newNumber : --newNumber);
        if (previous == 0)
            emit selectionIsDirtyChanged(true);
        if (previous == 1 && getNumberSelectedLegoSets() == 0)
            emit selectionIsDirtyChanged(false);
    } );

    if (set->isSelected())
    {
        int newNumber = getNumberSelectedLegoSets();
        setNumberSelectedLegoSets(++newNumber);
        emit selectionIsDirtyChanged(true);
    }
}
