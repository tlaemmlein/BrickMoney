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
    uint getVisibilityFlags() const override;
    void setVisibilityFlags(uint flags) override;

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
    QPushButton* mToInStock;
    QPushButton* mToForSale;



    // LegSetTableView interface
protected:
    void retranslateUi() const override;
};







#endif // Sold_H
