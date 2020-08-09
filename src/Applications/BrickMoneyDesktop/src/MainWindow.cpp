#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->inStockTableView->setModel(BrickMoneyProject::Inst()->getInStockSortModel());
    ui->forSaleTableView->setModel(BrickMoneyProject::Inst()->getForSaleSortModel());
    ui->soldTableView->setModel(BrickMoneyProject::Inst()->getSoldSortModel());
}

MainWindow::~MainWindow()
{
    delete ui;
}
