#ifndef LEGOSET_H
#define LEGOSET_H

#include <QObject>

class LegoSet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageName READ imageName WRITE setImageName NOTIFY imageNameChanged)
    Q_PROPERTY(QString imageFilePath READ imageFilePath WRITE setImageFilePath NOTIFY imageFilePathChanged)
    Q_PROPERTY(int setNumber READ setNumber WRITE setSetNumber NOTIFY setNumberChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(double recommendedRetailPrice READ recommendedRetailPrice WRITE setRecommendedRetailPrice NOTIFY recommendedRetailPriceChanged)
    Q_PROPERTY(double purchasingPrice READ purchasingPrice WRITE setPurchasingPrice NOTIFY purchasingPriceChanged)
    Q_PROPERTY(double cheaperPercent READ cheaperPercent WRITE setCheaperPercent NOTIFY cheaperPercentChanged)
    Q_PROPERTY(QString seller READ seller WRITE setSeller NOTIFY sellerChanged)
    Q_PROPERTY(QString purchaseDate READ purchaseDate WRITE setPurchaseDate NOTIFY purchaseDateChanged)
    Q_PROPERTY(double retailPrice READ retailPrice WRITE setRetailPrice NOTIFY retailPriceChanged)

public:
    explicit LegoSet(QObject *parent = nullptr);
    LegoSet(const QString& imageName, const QString& imageFilePath, int setNumber,
            const QString& description, int year, double recommendedRetailPrice,
            double purchasingPrice, double cheaperPercent, const QString& seller,
            const QString& purchaseDate, double retailPrice, QObject *parent = nullptr);


    QString imageName() const;
    QString imageFilePath() const;
    int setNumber() const;
    QString description() const;
    int year() const;
    double recommendedRetailPrice() const;
    double purchasingPrice() const;
    double cheaperPercent() const;
    QString seller() const;
    QString purchaseDate() const;
    double retailPrice() const;

public slots:
    void setImageName(QString imageName);
    void setImageFilePath(QString imageFilePath);
    void setSetNumber(int setNumber);
    void setDescription(QString description);
    void setYear(int year);
    void setRecommendedRetailPrice(double recommendedRetailPrice);
    void setPurchasingPrice(double purchasingPrice);
    void setCheaperPercent(double cheaperPercent);
    void setSeller(QString seller);
    void setPurchaseDate(QString purchaseDate);
    void setRetailPrice(double retailPrice);

signals:
    void imageNameChanged(QString imageName);
    void imageFilePathChanged(QString imageFilePath);
    void setNumberChanged(int setNumber);
    void descriptionChanged(QString description);
    void yearChanged(int year);
    void recommendedRetailPriceChanged(double recommendedRetailPrice);
    void purchasingPriceChanged(double purchasingPrice);
    void cheaperPercentChanged(double cheaperPercent);
    void sellerChanged(QString seller);
    void purchaseDateChanged(QString purchaseDate);
    void retailPriceChanged(double retailPrice);

private:
    QString m_imageName;
    QString m_imageFilePath;
    int     m_setNumber;
    QString m_description;
    int     m_year;
    double  m_recommendedRetailPrice;
    double  m_purchasingPrice;
    double  m_cheaperPercent;
    QString m_seller;
    QString m_purchaseDate;
    double  m_retailPrice;
};

#endif // LEGOSET_H
