#ifndef Sold_H
#define Sold_H

#include "LegSetTableView.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class LegoSetSortFilterTableModel;
class LegoSetTableModel;


class Sold : public LegSetTableView
{
    Q_OBJECT

public:
    explicit Sold(QWidget *parent = nullptr);
    ~Sold();

signals:
    void legoSetsMovedToInStock();
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
    QPushButton* mToInStock;
    QPushButton* mToForSale;
};

#endif // Sold_H