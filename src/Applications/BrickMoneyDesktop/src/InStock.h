#ifndef INSTOCK_H
#define INSTOCK_H

#include "LegSetTableView.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE


class LegoSetSortFilterTableModel;
class LegoSetTableModel;

class InStock : public LegSetTableView
{
    Q_OBJECT

public:
    explicit InStock(QWidget *parent = nullptr);
    ~InStock();

signals:
    void legoSetsMovedToForSale();

    // LegSetTableView interface
protected:
    LegoSetSortFilterTableModel *getSortModel() const override;
    LegoSetTableModel *getModel() const override;
    QString title() const override;
    void selectionIsDirty(bool isDirty) override;

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
    QString mTitle;
    QPushButton* mAddLegoSetButton;
    QPushButton* mFromInStockToForSalePushButton;
};


#endif // INSTOCK_H
