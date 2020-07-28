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

    void addLegoSet(LegoSet* set);
    QString getSelectedLegoSetIDs();
    void removeSelectedLegoSets();
    int legoSetCount();
    LegoSet *legoSetAt(int index);
    void clearLegoSets();

	bool read(const QJsonArray& legoSetArray);
	bool write(QJsonArray& legoSetArray);

    bool selectionIsDirty();

signals:
    void preLegoSetAdded();
    void postLegoSetAdded();
    void preLegoSetRemoved(int index);
    void postLegoSetRemoved();
	void resetLegoSets();

    void selectionIsDirtyChanged(bool);

private:
    QList<LegoSet*> m_legoSets;

    static const QString IsSelectedName;
    static const QString SetNumberName;
    static const QString PurchasingPriceName;
	static const QString SellerName;
	static const QString PurchaseDateName;
	static const QString RetailPriceName;
	static const QString SaleDateName;
	static const QString SoldOverName;
	static const QString BuyerName;

    int m_SelectedLegoSets;

    void connectSelection(LegoSet *set);

};

#endif // DATASOURCE_H
