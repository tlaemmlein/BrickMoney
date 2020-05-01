#ifndef LEGOSET_TABLE_MODEL_H
#define LEGOSET_TABLE_MODEL_H

#include "LegoSet.h"
#include "LegoSetDataSource.h"

#include <QAbstractTableModel>
#include <QQmlListProperty>

class LegoSetTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(LegoSetDataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(QQmlListProperty<LegoSet> legoSets READ legoSets NOTIFY legoSetsChanged)

    Q_CLASSINFO("DefaultProperty", "legoSets")

    enum LegoSetRoles {
        ImageNameRole = Qt::UserRole + 1,
        ImageUrl,
        SetNumberRole,
        DescriptionRole,
        YearRole,
        RecommendedRetailPriceRole,
        PurchasingPriceRole,
        CheaperPercentRole,
        SellerRole,
        PurchaseDateRole,
        RetailPrice,
        ProfitEuros,
        ProfitPercent,
        SaleDate,
        SoldOver,
        Buyer
    };

public:
    explicit LegoSetTableModel(QObject *parent= nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    void setDataSource(LegoSetDataSource* dataSource);

    LegoSetDataSource *dataSource() const;

    QQmlListProperty<LegoSet> legoSets();

    Q_INVOKABLE LegoSet* addLegoSet(int setNumber);
    Q_INVOKABLE void removeLegoSet(int rowIndex);
    Q_INVOKABLE void clearAll();

signals:
    void dataSourceChanged(LegoSetDataSource* dataSource);

    void legoSetsChanged(QQmlListProperty<LegoSet> legoSets);

private:
    static void appendLegoSet(QQmlListProperty<LegoSet>*, LegoSet*);
    static int legoSetCount(QQmlListProperty<LegoSet>*);
    static LegoSet* legoSet(QQmlListProperty<LegoSet>*, int);
    static void clearLegoSets(QQmlListProperty<LegoSet>*);

    LegoSetDataSource* m_dataSource;
    bool m_signalConnected;
    QHash<int, QByteArray> m_roles;
};


#endif

