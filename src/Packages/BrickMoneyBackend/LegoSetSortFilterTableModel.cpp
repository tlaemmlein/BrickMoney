#include "LegoSetSortFilterTableModel.h"
#include "LegoSetTableModel.h"

LegoSetSortFilterTableModel::LegoSetSortFilterTableModel(LegoSetTableModel* model, QObject *parent)
  : QSortFilterProxyModel (parent)
{
    m_LegoSetTableModel = model;
    setSourceModel(model);
    setSortRole(int(LegoSetTableModel::Role::Sort));
    setFilterKeyColumn(-1); //With -1: Include all columns to filter
}

void LegoSetSortFilterTableModel::setFilterText(QString filterText)
{
    if (m_filterText == filterText)
        return;

    m_filterText = filterText;
    setFilterRegularExpression(filterText);
    emit filterTextChanged(m_filterText);
}


void LegoSetSortFilterTableModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);
}

int LegoSetSortFilterTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_LegoSetTableModel->columnCount();
}

int LegoSetSortFilterTableModel::columnWidth(int c, const QFont *font)
{
    return m_LegoSetTableModel->columnWidth(c, font);
}

Qt::SortOrder LegoSetSortFilterTableModel::initialSortOrder(int column) const
{
    bool ok = false;
    if (column < 0 || column >= m_LegoSetTableModel->columnCount())
        return Qt::AscendingOrder;
    int ret = m_LegoSetTableModel->data(m_LegoSetTableModel->index(0, column), Qt::InitialSortOrderRole).toInt(&ok);
    if (ok)
        return Qt::SortOrder(ret);
    else
        return Qt::AscendingOrder;
}

QModelIndex LegoSetSortFilterTableModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    int row = QSortFilterProxyModel::mapFromSource(sourceIndex).row();
    int c =sourceIndex.column();
    return m_LegoSetTableModel->index(row, c);
}

QModelIndex LegoSetSortFilterTableModel::mapToSource(const QModelIndex &proxyIndex) const
{
    QModelIndex rowIndex = QSortFilterProxyModel::mapToSource(proxyIndex);
    int col = -1;
    if (proxyIndex.column() >= 0 && proxyIndex.column() < m_LegoSetTableModel->columnCount())
        col = proxyIndex.column();
    return m_LegoSetTableModel->index(rowIndex.row(), col);
}

