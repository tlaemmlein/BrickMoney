#ifndef LEGOSET_H
#define LEGOSET_H

#include <QObject>
#include <QDate>

class LegoSetInfoGenerator;

class LegoSet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageName READ imageName NOTIFY imageNameChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(int setNumber READ setNumber WRITE setSetNumber NOTIFY setNumberChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(double recommendedRetailPrice READ recommendedRetailPrice WRITE setRecommendedRetailPrice NOTIFY recommendedRetailPriceChanged)
    Q_PROPERTY(double purchasingPrice READ purchasingPrice WRITE setPurchasingPrice NOTIFY purchasingPriceChanged)
    Q_PROPERTY(double cheaperPercent READ cheaperPercent NOTIFY cheaperPercentChanged)
    Q_PROPERTY(QString seller READ seller WRITE setSeller NOTIFY sellerChanged)
    Q_PROPERTY(QDate purchaseDate READ purchaseDate WRITE setPurchaseDate NOTIFY purchaseDateChanged)
    Q_PROPERTY(double retailPrice READ retailPrice WRITE setRetailPrice NOTIFY retailPriceChanged)
    Q_PROPERTY(double profitEuros READ profitEuros NOTIFY profitEurosChanged)
    Q_PROPERTY(double profitPercent READ profitPercent NOTIFY profitPercentChanged)
    Q_PROPERTY(QDate saleDate READ saleDate WRITE setSaleDate NOTIFY saleDateChanged)
    Q_PROPERTY(QString soldOver READ soldOver WRITE setSoldOver NOTIFY soldOverChanged)
    Q_PROPERTY(QString buyer READ buyer WRITE setBuyer NOTIFY buyerChanged)

public:
    explicit LegoSet(QObject *parent = nullptr);


    QString imageName() const;
    QString imageUrl() const;
    int setNumber() const;
    QString description() const;
    int year() const;
    double recommendedRetailPrice() const;
    double purchasingPrice() const;
    double cheaperPercent() const;
    QString seller() const;
    QDate purchaseDate() const;
    double retailPrice() const;
    double profitEuros() const;
    double profitPercent() const;
    QDate saleDate() const;
    QString soldOver() const;
    QString buyer() const;

public slots:
    void setImageUrl(QString imageUrl);
    void setSetNumber(int setNumber);
    void setDescription(QString description);
    void setYear(int year);
    void setRecommendedRetailPrice(double recommendedRetailPrice);
    void setPurchasingPrice(double purchasingPrice);
    void setSeller(QString seller);
    void setPurchaseDate(QDate purchaseDate);
    void setRetailPrice(double retailPrice);
    void setSaleDate(QDate saleDate);
    void setSoldOver(QString soldOver);
    void setBuyer(QString buyer);

signals:
    void imageNameChanged(QString imageName);
    void imageUrlChanged(QString imageUrl);
    void setNumberChanged(int setNumber);
    void descriptionChanged(QString description);
    void yearChanged(int year);
    void recommendedRetailPriceChanged(double recommendedRetailPrice);
    void purchasingPriceChanged(double purchasingPrice);
    void cheaperPercentChanged(double cheaperPercent);
    void sellerChanged(QString seller);
    void purchaseDateChanged(QDate purchaseDate);
    void retailPriceChanged(double retailPrice);
    void profitEurosChanged(double profitEuros);
    void profitPercentChanged(double profitPercent);
    void saleDateChanged(QDate saleDate);
    void soldOverChanged(QString soldOver);
    void buyerChanged(QString buyer);

private slots:
    void setImageName(QString imageName);


private:
    LegoSetInfoGenerator* m_LegoSetInfoGenerator;
    QString m_imageName;
    QString m_imageUrl;
    int     m_setNumber;
    QString m_description;
    int     m_year;
    double  m_recommendedRetailPrice;
    double  m_purchasingPrice;
    double  m_cheaperPercent;
    QString m_seller;
    QDate m_purchaseDate;
    double  m_retailPrice;
    double  m_profitEuros;
    double  m_profitPercent;
    QDate m_saleDate;
    QString m_soldOver;
    QString m_buyer;

    double calcCheaperPercent(double rrp, double purchasingPrice);
    void setCheaperPercent(double cheaperPercent);
    double calcProfitEuros(double retailPrice, double purchasingPrice);
    void setProfitEuros(double profitEuros);
    double calcProfitPercent(double retailPrice, double purchasingPrice);
    void setProfitPercent(double profitPercent);
};


#endif // LEGOSET_H
