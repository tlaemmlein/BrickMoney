#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageDelegate.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->inStockTableView->setModel(BrickMoneyProject::Inst()->getInStockSortModel());
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));

    ui->forSaleTableView->setModel(BrickMoneyProject::Inst()->getForSaleSortModel());
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));

    ui->soldTableView->setModel(BrickMoneyProject::Inst()->getSoldSortModel());
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
