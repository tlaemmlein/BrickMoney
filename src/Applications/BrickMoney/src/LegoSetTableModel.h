#ifndef LEGOSET_TABLE_MODEL_H
#define LEGOSET_TABLE_MODEL_H

#include "LegoSet.h"
#include "LegoSetDataSource.h"

#include <QAbstractTableModel>

class LegoSetTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(LegoSetDataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)

    Q_CLASSINFO("DefaultProperty", "legoSets")


public:
    enum class Role {
        Sort = Qt::UserRole,
        Number,
        Type
    };

    explicit LegoSetTableModel(QObject *parent= nullptr);
    ~LegoSetTableModel();

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    Q_INVOKABLE int columnWidth(int c, const QFont *font = nullptr);


    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void setDataSource(LegoSetDataSource* dataSource);

    LegoSetDataSource *dataSource() const;


    Q_INVOKABLE LegoSet* addLegoSet(int setNumber);
    Q_INVOKABLE void removeLegoSet(int rowIndex);
    Q_INVOKABLE void clearAll();

signals:
    void dataSourceChanged(LegoSetDataSource* dataSource);
	void modelReset();


private:

    LegoSetDataSource* m_dataSource;
    bool m_signalConnected;

    QVector<int> m_columnWidths = QVector<int>(LegoSetProperty::COUNT);
    QHash<int, QByteArray> m_roleNames;
};


#endif

