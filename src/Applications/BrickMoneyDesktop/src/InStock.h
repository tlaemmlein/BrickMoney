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
    uint getVisibilityFlags() const override;
    void setVisibilityFlags(uint flags) override;

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
    QPushButton* mAddLegoSetButton;
    QPushButton* mFromInStockToForSalePushButton;

    // LegSetTableView interface
protected:
    void retranslateUi() const override;
};






#endif // INSTOCK_H
