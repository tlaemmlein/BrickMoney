#ifndef Sold_H
#define Sold_H

#include <QWidget>

class LegoSetSortFilterTableModel;
class LegoSetTableModel;


namespace Ui {
class Sold;
}

class Sold : public QWidget
{
    Q_OBJECT

public:
    explicit Sold(QWidget *parent = nullptr);
    ~Sold();

signals:
    void legoSetsMovedToInStock();
    void legoSetsMovedToForSale();

private:
    Ui::Sold *ui;
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;

};

#endif // Sold_H
