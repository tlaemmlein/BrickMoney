#include "ForSale.h"
#include "ui_ForSale.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

ForSale::ForSale(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForSale)
{
    ui->setupUi(this);

    ui->forSaleTableView->setModel(BrickMoneyProject::Inst()->getForSaleSortModel());
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    connect(ui->forSaleLineEdit, &QLineEdit::editingFinished, [&]() {
        BrickMoneyProject::Inst()->getForSaleSortModel()->setFilterText(ui->forSaleLineEdit->text());
    });
    connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromForSaleToInStockPushButton, &QPushButton::setVisible);
    ui->fromForSaleToInStockPushButton->setVisible(BrickMoneyProject::Inst()->getForSaleModel()->selectionIsDirty());
    connect(ui->fromForSaleToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getForSaleModel(), BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });
    connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromForSaleToSoldPushButton, &QPushButton::setVisible);
    ui->fromForSaleToSoldPushButton->setVisible(BrickMoneyProject::Inst()->getForSaleModel()->selectionIsDirty());
    connect(ui->fromForSaleToSoldPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getForSaleModel(), BrickMoneyProject::Inst()->getSoldModel());
        emit legoSetsMovedToSold();
    });

}

ForSale::~ForSale()
{
    delete ui;
}
