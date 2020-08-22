#include "Sold.h"
#include "ui_Sold.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

Sold::Sold(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sold)
{
    ui->setupUi(this);

    ui->soldTableView->setModel(BrickMoneyProject::Inst()->getSoldSortModel());
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    connect(ui->soldLineEdit, &QLineEdit::editingFinished, [&]() {
        BrickMoneyProject::Inst()->getSoldSortModel()->setFilterText(ui->soldLineEdit->text());
    });
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromSoldToInStockPushButton, &QPushButton::setVisible);
    ui->fromSoldToInStockPushButton->setVisible(BrickMoneyProject::Inst()->getSoldModel()->selectionIsDirty());
    connect(ui->fromSoldToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getSoldModel(), BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromSoldToForSalePushButton, &QPushButton::setVisible);
    ui->fromSoldToForSalePushButton->setVisible(BrickMoneyProject::Inst()->getSoldModel()->selectionIsDirty());
    connect(ui->fromSoldToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getSoldModel(), BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
    });
}

Sold::~Sold()
{
    delete ui;
}
