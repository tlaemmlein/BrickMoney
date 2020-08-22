#ifndef INSTOCK_H
#define INSTOCK_H

#include <QWidget>

class LegoSetSortFilterTableModel;
class LegoSetTableModel;

namespace Ui {
class InStock;
}

class InStock : public QWidget
{
    Q_OBJECT

public:
    explicit InStock(QWidget *parent = nullptr);
    ~InStock();

signals:
    void legoSetsMovedToForSale();

private:
    Ui::InStock *ui;
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
};

#endif // INSTOCK_H
