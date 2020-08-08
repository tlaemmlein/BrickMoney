#ifndef LEGOSET_SORTFILTER_TABLE_MODEL_H
#define LEGOSET_SORTFILTER_TABLE_MODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class LegoSetTableModel;

class LegoSetSortFilterTableModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    LegoSetSortFilterTableModel(LegoSetTableModel* model, QObject *parent = nullptr);
    Q_INVOKABLE void toggleAllNoneSelection(bool selection);

    QString filterText() const  {return m_filterText;}
    void setFilterText(QString filterText);
    Q_INVOKABLE void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnWidth(int c, const QFont *font = nullptr);
    Q_INVOKABLE Qt::SortOrder initialSortOrder(int column) const;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;


signals:
    void filterTextChanged(QString filterText);

private:
    LegoSetTableModel* m_LegoSetTableModel;

    //QVector<fields> m_fields;

    QString m_filterText;
};

#endif
