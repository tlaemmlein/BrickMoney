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
    QString title() const override;
    void selectionIsDirty(bool isDirty) override;

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
    QString mTitle;
    QPushButton* mToInStock;
    QPushButton* mToSold;
};

#endif // ForSale_H