#ifndef LEGOSET_TABLE_MODEL_H
#define LEGOSET_TABLE_MODEL_H

#include "LegoSet.h"
#include "DataSource.h"

#include <QAbstractTableModel>
#include <QQmlListProperty>

class LegoSetTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(DataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
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

    void setDataSource(DataSource* dataSource);

    DataSource *dataSource() const;

    QQmlListProperty<LegoSet> legoSets();

    Q_INVOKABLE void addLegoSet();
    Q_INVOKABLE void removeLegoSet(int rowIndex);
    Q_INVOKABLE void clearAll();

signals:
    void dataSourceChanged(DataSource* dataSource);

    void legoSetsChanged(QQmlListProperty<LegoSet> legoSets);

private:
    static void appendLegoSet(QQmlListProperty<LegoSet>*, LegoSet*);
    static int legoSetCount(QQmlListProperty<LegoSet>*);
    static LegoSet* legoSet(QQmlListProperty<LegoSet>*, int);
    static void clearLegoSets(QQmlListProperty<LegoSet>*);

    DataSource* m_dataSource;
    bool m_signalConnected;
    QQmlListProperty<LegoSet> m_legoSets;
    QHash<int, QByteArray> m_roles;
};


#endif

