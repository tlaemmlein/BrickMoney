#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

    enum TableRoles{
        TableDataRole = Qt::UserRole +1,
        HeadingRole
    };

public:
    explicit TableModel(QObject *parent= nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // QAbstractItemModel interface
    bool insertRows(int row, int count, const QModelIndex &parent) override;
//    bool removeRows(int row, int count, const QModelIndex &parent) override;
signals:

public slots:

    void newEntry();

private:
    QVector<QVector<QString>> table;

    int mIndex;
};


#endif

