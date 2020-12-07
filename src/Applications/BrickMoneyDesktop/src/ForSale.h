#ifndef ForSale_H
#define ForSale_H

#include "LegSetTableView.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class LegoSetSortFilterTableModel;
class LegoSetTableModel;


class ForSale : public LegSetTableView
{
    Q_OBJECT

public:
    explicit ForSale(QWidget *parent = nullptr);
    ~ForSale();

signals:
    void legoSetsMovedToInStock();
    void legoSetsMovedToSold();

    // LegSetTableView interface
protected:
    LegoSetSortFilterTableModel *getSortModel() const override;
    LegoSetTableModel *getModel() const override;

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
    QPushButton* mToInStock;
    QPushButton* mToSold;


    // LegSetTableView interface
protected:
    uint getVisibilityFlags() const override;
    void setVisibilityFlags(uint flags) override;

    // LegSetTableView interface
protected:
    void retranslateUi() const override;
};







#endif // ForSale_H
