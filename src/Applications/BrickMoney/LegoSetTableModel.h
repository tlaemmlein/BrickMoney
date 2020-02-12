#ifndef LEGOSET_TABLE_MODEL_H
#define LEGOSET_TABLE_MODEL_H

#include "LegoSetTableData.h"

#include <QAbstractTableModel>

#include <QLinkedList>
#include <QTextStream>

class LegoSetTableModel : public QAbstractTableModel
{
    Q_OBJECT

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

    Q_INVOKABLE int roleID(const QString& roleName);

    void saveDataTo(const QChar& separator, QTextStream& out, const QString& projectFolder) const;

    void loadDataFrom(const QChar& separator, QTextStream& in, const QString& projectFolder);

public slots:

    void newEntry();
    void deleteEntry(int rowIndex);
    void clearAll();

private:

    LegoSetTableData mLegoSetTableData;
};


#endif

