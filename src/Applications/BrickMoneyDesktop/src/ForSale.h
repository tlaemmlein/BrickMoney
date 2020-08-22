#ifndef ForSale_H
#define ForSale_H

#include <QWidget>

class LegoSetSortFilterTableModel;
class LegoSetTableModel;

namespace Ui {
class ForSale;
}

class ForSale : public QWidget
{
    Q_OBJECT

public:
    explicit ForSale(QWidget *parent = nullptr);
    ~ForSale();

signals:
    void legoSetsMovedToInStock();
    void legoSetsMovedToSold();

private:
    Ui::ForSale *ui;
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
};

#endif // ForSale_H
