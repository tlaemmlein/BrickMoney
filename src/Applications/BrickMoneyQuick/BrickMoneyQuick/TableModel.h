#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent= nullptr);


    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
signals:

public slots:

};


#endif

