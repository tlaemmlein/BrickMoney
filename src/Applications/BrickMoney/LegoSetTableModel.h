#ifndef LEGOSET_TABLE_MODEL_H
#define LEGOSET_TABLE_MODEL_H

#include "LegoSetTableData.h"
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
        ImageFilePathRole,
        SetNumberRole,
        DescriptionRole,
        YearRole,
        RecommendedRetailPriceRole,
        PurchasingPriceRole,
        CheaperPercentRole,
        SellerRole,
        PurchaseDateRole,
        RetailPrice
    };

public:
    explicit LegoSetTableModel(QObject *parent= nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // QAbstractItemModel interface
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;

    void saveDataTo(const QChar& separator, QTextStream& out, const QString& projectFolder) const;

    void loadDataFrom(const QChar& separator, QTextStream& in, const QString& projectFolder);

    DataSource *dataSource() const;

    QQmlListProperty<LegoSet> legoSets() const;

public slots:

    void newEntry();
    void deleteEntry(int rowIndex);
    void clearAll();

    void setDataSource(DataSource* dataSource);

signals:
    void dataSourceChanged(DataSource* dataSource);

    void legoSetsChanged(QQmlListProperty<LegoSet> legoSets);

private:

    LegoSetTableData mLegoSetTableData;
    DataSource* m_dataSource;
    bool m_signalConnected;
    QQmlListProperty<LegoSet> m_legoSets;
};


#endif

