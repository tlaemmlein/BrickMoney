#include "InStock.h"
#include "ui_InStock.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

InStock::InStock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InStock)
{
    ui->setupUi(this);

    ui->inStockTableView->setModel(BrickMoneyProject::Inst()->getInStockSortModel());
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    connect(ui->inStockLineEdit, &QLineEdit::editingFinished, [&]() {
        BrickMoneyProject::Inst()->getInStockSortModel()->setFilterText(ui->inStockLineEdit->text());
    });
    connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromInStockToForSalePushButton, &QPushButton::setVisible);
    ui->fromInStockToForSalePushButton->setVisible(BrickMoneyProject::Inst()->getInStockModel()->selectionIsDirty());
    connect(ui->fromInStockToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getInStockModel(), BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
    });
}

InStock::~InStock()
{
    delete ui;
}
