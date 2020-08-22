#ifndef INSTOCK_H
#define INSTOCK_H

#include <QWidget>

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
};

#endif // INSTOCK_H
