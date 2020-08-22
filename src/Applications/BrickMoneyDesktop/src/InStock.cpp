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

    mSortModel = BrickMoneyProject::Inst()->getInStockSortModel();
    mModel = BrickMoneyProject::Inst()->getInStockModel();

    ui->inStockTableView->setModel(mSortModel);
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    connect(ui->inStockLineEdit, &QLineEdit::editingFinished, [&]() {
        mSortModel->setFilterText(ui->inStockLineEdit->text());
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, ui->copyAndPastePushButton, &QPushButton::setVisible);
    ui->copyAndPastePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->copyAndPastePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->copySelectedLegoSets(mModel, mModel);
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, ui->fromInStockToForSalePushButton, &QPushButton::setVisible);
    ui->fromInStockToForSalePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromInStockToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
    });


}

InStock::~InStock()
{
    delete ui;
}
