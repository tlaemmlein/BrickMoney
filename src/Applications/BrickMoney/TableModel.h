#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include <QAbstractTableModel>

#include <QLinkedList>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

    enum TableRoles{
        ImageRole = Qt::UserRole +1,
        SetNumberRole,
        DescriptionRole
    };


public:
    explicit TableModel(QObject *parent= nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // QAbstractItemModel interface
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;
signals:

public slots:

    void newEntry();
    void deleteEntry(int rowIndex);

private:

    QList< QVector<QVariant> > table;
};


#endif

