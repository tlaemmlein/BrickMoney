#ifndef ForSale_H
#define ForSale_H

#include <QWidget>

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
};

#endif // ForSale_H
