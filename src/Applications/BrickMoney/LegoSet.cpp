#include "LegoSet.h"

LegoSet::LegoSet(QObject *parent) : QObject(parent)
    ,m_imageName("None")
    ,m_imageFilePath("None")
    ,m_setNumber(-1)
    ,m_description("None")
    ,m_year(0)
    ,m_recommendedRetailPrice(0)
    ,m_purchasingPrice(0)
    ,m_cheaperPercent(0)
    ,m_seller("None")
    ,m_purchaseDate("None")
    ,m_retailPrice(0)
{

}

LegoSet::LegoSet(const QString &imageName, const QString &imageFilePath, int setNumber, const QString &description,
                 int year, double recommendedRetailPrice, double purchasingPrice, double cheaperPercent,
                 const QString &seller, const QString &purchaseDate, double retailPrice, QObject *parent) : QObject(parent)
    ,m_imageName(imageName)
    ,m_imageFilePath(imageFilePath)
    ,m_setNumber(setNumber)
    ,m_description(description)
    ,m_year(year)
    ,m_recommendedRetailPrice(recommendedRetailPrice)
    ,m_purchasingPrice(purchasingPrice)
    ,m_cheaperPercent(cheaperPercent)
    ,m_seller(seller)
    ,m_purchaseDate(purchaseDate)
    ,m_retailPrice(retailPrice)
{

}

QString LegoSet::imageName() const
{
    return m_imageName;
}

QString LegoSet::imageFilePath() const
{
    return m_imageFilePath;
}

int LegoSet::setNumber() const
{
    return m_setNumber;
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

QString LegoSet::purchaseDate() const
{
    return m_purchaseDate;
}

double LegoSet::retailPrice() const
{
    return m_retailPrice;
}

void LegoSet::setImageName(QString imageName)
{
    if (m_imageName == imageName)
        return;

    m_imageName = imageName;
    emit imageNameChanged(m_imageName);
}

void LegoSet::setImageFilePath(QString imageFilePath)
{
    if (m_imageFilePath == imageFilePath)
        return;

    m_imageFilePath = imageFilePath;
    emit imageFilePathChanged(m_imageFilePath);
}

void LegoSet::setSetNumber(int setNumber)
{
    if (m_setNumber == setNumber)
        return;

    m_setNumber = setNumber;
    emit setNumberChanged(m_setNumber);
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
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_recommendedRetailPrice, recommendedRetailPrice))
        return;

    m_recommendedRetailPrice = recommendedRetailPrice;
    emit recommendedRetailPriceChanged(m_recommendedRetailPrice);
}

void LegoSet::setPurchasingPrice(double purchasingPrice)
{
    m_purchasingPrice = purchasingPrice;
}

void LegoSet::setCheaperPercent(double cheaperPercent)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_cheaperPercent, cheaperPercent))
        return;

    m_cheaperPercent = cheaperPercent;
    emit cheaperPercentChanged(m_cheaperPercent);
}

void LegoSet::setSeller(QString seller)
{
    if (m_seller == seller)
        return;

    m_seller = seller;
    emit sellerChanged(m_seller);
}

void LegoSet::setPurchaseDate(QString purchaseDate)
{
    if (m_purchaseDate == purchaseDate)
        return;

    m_purchaseDate = purchaseDate;
    emit purchaseDateChanged(m_purchaseDate);
}

void LegoSet::setRetailPrice(double retailPrice)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_retailPrice, retailPrice))
        return;

    m_retailPrice = retailPrice;
    emit retailPriceChanged(m_retailPrice);
}

