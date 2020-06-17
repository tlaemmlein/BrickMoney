#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QJsonArray>

class LegoSet;

class LegoSetDataSource : public QObject
{
    Q_OBJECT
public:
    explicit LegoSetDataSource(QObject *parent = nullptr);

    Q_INVOKABLE void addLegoSet(LegoSet* set);
    Q_INVOKABLE void removeLegoSet(int index);
    Q_INVOKABLE int legoSetCount();
    Q_INVOKABLE LegoSet *legoSetAt(int index);
    Q_INVOKABLE void clearLegoSets();

	bool read(const QJsonArray& legoSetArray);
	bool write(QJsonArray& legoSetArray);

signals:
    void preLegoSetAdded();
    void postLegoSetAdded();
    void preLegoSetRemoved(int index);
    void postLegoSetRemoved();
	void resetLegoSets();

private:
    QList<LegoSet*> m_legoSets;

	static const QString SetNumberName;
	static const QString PurchasingPriceName;
	static const QString SellerName;
	static const QString PurchaseDateName;
	static const QString RetailPriceName;
	static const QString SaleDateName;
	static const QString SoldOverName;
	static const QString BuyerName;
};



#endif // DATASOURCE_H
