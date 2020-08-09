#include "LegoSet.h"
#include "magic_enum.h"

#include "Packages/LegoSetInfoGenerator/LegoSetInfoGenerator.h"
#include "BrickMoneyDataManager.h"

int LegoSet::IDCOUNTER = 0;


QVariant LegoSet::data(LegoSetProperty prop)
{
    return this->property(magic_enum::enum_name(prop).data());
}

bool LegoSet::setData(LegoSetProperty prop, const QVariant& data)
{
	if (this->property(magic_enum::enum_name(prop).data()) == data)
		return false;

    return this->setProperty(magic_enum::enum_name(prop).data(), data);
}

QString LegoSet::displayName(LegoSetProperty prop)
{
    switch (prop) {
    case LegoSetProperty::id:
        return tr("ID");
    case LegoSetProperty::isSelected:
        return tr("Sel");
    case LegoSetProperty::setNumber:
        return tr("Set");
    case LegoSetProperty::imageUrl:
        return tr("Image");
    case LegoSetProperty::description:
        return tr("Description");
    case LegoSetProperty::year:
        return tr("Year");
    case LegoSetProperty::recommendedRetailPrice:
        return tr("RRPrice €");
    case LegoSetProperty::purchasingPrice:
        return tr("Purchasing Price €");
    case LegoSetProperty::cheaperPercent:
        return tr("Cheaper %");
    case LegoSetProperty::seller:
        return tr("Seller");
    case LegoSetProperty::purchaseDate:
        return tr("Purchase Date");
    case LegoSetProperty::retailPrice:
        return tr("Retail Price €");
    case LegoSetProperty::profitEuros:
        return tr("Profit €");
    case LegoSetProperty::profitPercent:
        return tr("Profit %");
    case LegoSetProperty::saleDate:
        return tr("Sale Date");
    case LegoSetProperty::soldOver:
        return tr("Sold over");
    case LegoSetProperty::buyer:
        return tr("Buyer");
    case COUNT:
        return "Count";
    }

    return "";
}

LegoSet::LegoSet(QObject *parent) : QObject(parent)
{
    initParams();
    createConnections();
    setSetNumber(0);
    m_id = LegoSet::IDCOUNTER;
    ++LegoSet::IDCOUNTER;
}

LegoSet::LegoSet(int setNumber, QObject *parent) : QObject(parent)
{
    initParams();
    createConnections();
    setSetNumber(setNumber);
    m_id = LegoSet::IDCOUNTER;
    ++LegoSet::IDCOUNTER;
}

LegoSet::~LegoSet()
{
}

LegoSet &LegoSet::operator=(const LegoSet &other)
{
	// check for self-assignment
	if (&other == this)
		return *this;

	this->setIsSelected(other.isSelected());
	this->setSetNumber(other.setNumber());
	this->setIsSelected(other.isSelected());
	this->setPurchasingPrice(other.purchasingPrice());
	this->setPurchaseDate(other.purchaseDate());
	this->setSeller(other.seller());
	this->setRetailPrice(other.retailPrice());
	this->setSaleDate(other.saleDate());
	this->setSoldOver(other.soldOver());
	this->setBuyer(other.buyer());

	return *this;
}

int LegoSet::id() const
{
    return m_id;
}

void LegoSet::initParams()
{
    m_isSelected = false;
    m_setNumber=-1;
    m_imageUrl="None";
    m_description="None";
    m_year=0;
    m_recommendedRetailPrice=0;
    m_purchasingPrice=0;
    m_cheaperPercent=0;
    m_seller="None";
    m_purchaseDate = QDate::currentDate();
    m_retailPrice =0;
    m_profitEuros =0;
    m_profitPercent= 0;
    m_saleDate = QDate::currentDate();
    m_soldOver="";
    m_buyer="";
}

void LegoSet::createConnections()
{
    m_LegoSetInfoGenerator = new LegoSetInfoGenerator(this);
    connect(m_LegoSetInfoGenerator, &LegoSetInfoGenerator::imageUrl, this, &LegoSet::setImageUrl);
    connect(m_LegoSetInfoGenerator, &LegoSetInfoGenerator::description, this, &LegoSet::setDescription);
    connect(m_LegoSetInfoGenerator, &LegoSetInfoGenerator::year, this, &LegoSet::setYear);
    connect(m_LegoSetInfoGenerator, &LegoSetInfoGenerator::recommendedRetailPrice, this, &LegoSet::setRecommendedRetailPrice);
}

bool LegoSet::isSelected() const
{
    return m_isSelected;
}


int LegoSet::setNumber() const
{
    return m_setNumber;
}


QString LegoSet::imageUrl() const
{
    return m_imageUrl;
}

QString LegoSet::description() const
{
    return m_description;
}

int LegoSet::year() const
{
    return m_year;
}

double LegoSet::recommendedRetailPrice() const
{
    return m_recommendedRetailPrice;
}

double LegoSet::purchasingPrice() const
{
    return m_purchasingPrice;
}

double LegoSet::cheaperPercent() const
{
    return m_cheaperPercent;
}

QString LegoSet::seller() const
{
    return m_seller;
}

QDate LegoSet::purchaseDate() const
{
    return m_purchaseDate;
}

double LegoSet::retailPrice() const
{
    return m_retailPrice;
}

double LegoSet::profitEuros() const
{
    return m_profitEuros;
}

double LegoSet::profitPercent() const
{
    return m_profitPercent;
}

QDate LegoSet::saleDate() const
{
    return m_saleDate;
}

QString LegoSet::soldOver() const
{
    return m_soldOver;
}

QString LegoSet::buyer() const
{
    return m_buyer;
}

double LegoSet::calcCheaperPercent(double rrp, double purchasingPrice)
{
    return (purchasingPrice == 0.0) ? 0.0 : ( (1.0 - purchasingPrice/rrp) * 100.0);
}

void LegoSet::setIsSelected(bool isSelected)
{
    if (m_isSelected == isSelected)
        return;

    m_isSelected = isSelected;
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
    emit isSelectedChanged(m_isSelected);
}

void LegoSet::setSetNumber(int setNumber)
{
    if (m_setNumber == setNumber)
        return;

    m_LegoSetInfoGenerator->querySetNumber(setNumber);

    m_setNumber = setNumber;
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
    emit setNumberChanged(m_setNumber);
}


void LegoSet::setPurchasingPrice(double purchasingPrice)
{
    if (qFuzzyCompare(m_purchasingPrice, purchasingPrice))
        return;

    m_purchasingPrice = purchasingPrice;
    emit purchasingPriceChanged(m_purchasingPrice);

    setCheaperPercent( calcCheaperPercent(m_recommendedRetailPrice, m_purchasingPrice));
    setProfitEuros( calcProfitEuros(m_retailPrice, m_purchasingPrice));
    setProfitPercent( calcProfitPercent(m_retailPrice, m_purchasingPrice));
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSet::setSeller(QString seller)
{
    if (m_seller == seller)
        return;

    m_seller = seller;
    emit sellerChanged(m_seller);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSet::setPurchaseDate(QDate purchaseDate)
{
    if (m_purchaseDate == purchaseDate)
        return;

    m_purchaseDate = purchaseDate;
    emit purchaseDateChanged(m_purchaseDate);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSet::setRetailPrice(double retailPrice)
{
    if (qFuzzyCompare(m_retailPrice, retailPrice))
        return;

    m_retailPrice = retailPrice;
    emit retailPriceChanged(m_retailPrice);

    setProfitEuros( calcProfitEuros(m_retailPrice, m_purchasingPrice));
    setProfitPercent( calcProfitPercent(m_retailPrice, m_purchasingPrice));
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSet::setSaleDate(QDate saleDate)
{
    if (m_saleDate == saleDate)
        return;

    m_saleDate = saleDate;
    emit saleDateChanged(m_saleDate);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSet::setSoldOver(QString soldOver)
{
    if (m_soldOver == soldOver)
        return;

    m_soldOver = soldOver;
    emit soldOverChanged(m_soldOver);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}

void LegoSet::setBuyer(QString buyer)
{
    if (m_buyer == buyer)
        return;

    m_buyer = buyer;
    emit buyerChanged(m_buyer);
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(true);
}



void LegoSet::setImageUrl(QString imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged(m_imageUrl);
}

void LegoSet::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void LegoSet::setYear(int year)
{
    if (m_year == year)
        return;

    m_year = year;
    emit yearChanged(m_year);
}

void LegoSet::setRecommendedRetailPrice(double recommendedRetailPrice)
{
    if (qFuzzyCompare(m_recommendedRetailPrice, recommendedRetailPrice))
        return;

    m_recommendedRetailPrice = recommendedRetailPrice;
    emit recommendedRetailPriceChanged(m_recommendedRetailPrice);

    setCheaperPercent( calcCheaperPercent(m_recommendedRetailPrice, m_purchasingPrice));
}


void LegoSet::setCheaperPercent(double cheaperPercent)
{
    if (qFuzzyCompare(m_cheaperPercent, cheaperPercent))
        return;

    m_cheaperPercent = cheaperPercent;
    emit cheaperPercentChanged(m_cheaperPercent);
}

double LegoSet::calcProfitEuros(double retailPrice, double purchasingPrice)
{
    return (retailPrice <= 0.0) ? 0.0 : (retailPrice - purchasingPrice);
}

void LegoSet::setProfitEuros(double profitEuros)
{
    if (qFuzzyCompare(m_profitEuros, profitEuros))
        return;

    m_profitEuros = profitEuros;
    emit profitEurosChanged(m_profitEuros);
}

double LegoSet::calcProfitPercent(double retailPrice, double purchasingPrice)
{
    if ( retailPrice <= 0.0 || purchasingPrice <= 0.0)
        return 0.0;
    return ( (retailPrice/purchasingPrice) -1.0) *100.0;
}

void LegoSet::setProfitPercent(double profitPercent)
{
    if (qFuzzyCompare(m_profitPercent, profitPercent))
        return;

    m_profitPercent = profitPercent;
    emit profitPercentChanged(m_profitPercent);
}

