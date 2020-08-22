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

    mSortModel = BrickMoneyProject::Inst()->getSoldSortModel();
    mModel = BrickMoneyProject::Inst()->getSoldModel();


    ui->soldTableView->setModel(mSortModel);
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    connect(ui->soldLineEdit, &QLineEdit::editingFinished, [&]() {
        mSortModel->setFilterText(ui->soldLineEdit->text());
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, ui->copyAndPastePushButton, &QPushButton::setVisible);
    ui->copyAndPastePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->copyAndPastePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->copySelectedLegoSets(mModel, mModel);
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, ui->fromSoldToInStockPushButton, &QPushButton::setVisible);
    ui->fromSoldToInStockPushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromSoldToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });
    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, ui->fromSoldToForSalePushButton, &QPushButton::setVisible);
    ui->fromSoldToForSalePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromSoldToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
    });
}

Sold::~Sold()
{
    delete ui;
}
