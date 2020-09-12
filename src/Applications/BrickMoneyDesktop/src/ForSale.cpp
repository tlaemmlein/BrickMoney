#include "ForSale.h"
#include "ui_ForSale.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"
#include "SpinBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

#include <QMessageBox>

ForSale::ForSale(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForSale)
{
    ui->setupUi(this);

    mSortModel = BrickMoneyProject::Inst()->getForSaleSortModel();
    mModel = BrickMoneyProject::Inst()->getForSaleModel();

    ui->forSaleTableView->setModel(mSortModel);
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::purchasingPrice, new SpinBoxDelegate(this));
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::retailPrice, new SpinBoxDelegate(this));
    connect(ui->forSaleLineEdit, &QLineEdit::editingFinished, [&]() {
        mSortModel->setFilterText(ui->forSaleLineEdit->text());
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, [&] (bool isDirty) {
        ui->numOfSelectedLabel->setVisible(isDirty);
        ui->copyAndPastePushButton->setVisible(isDirty);
        ui->fromForSaleToInStockPushButton->setVisible(isDirty);
        ui->fromForSaleToSoldPushButton->setVisible(isDirty);
        ui->deletePushButton->setVisible(isDirty);
    });

    ui->numOfSelectedLabel->setVisible(mModel->selectionIsDirty());
    static const QString selectedText = tr("selected");
    connect(mModel, &LegoSetTableModel::numberOfSelectedLegoSetsChanged, [&] (int num) {
        ui->numOfSelectedLabel->setText(QString::number(num) + " " + selectedText);
    });
    ui->numOfSelectedLabel->setText(QString::number(mModel->numberOfSelectedLegoSets()) + " " + selectedText);

    ui->copyAndPastePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->copyAndPastePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->copySelectedLegoSets(mModel, mModel);
    });

    ui->fromForSaleToInStockPushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromForSaleToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getInStockModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });

    ui->fromForSaleToSoldPushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromForSaleToSoldPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getSoldModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getSoldModel());
        emit legoSetsMovedToSold();
    });

    ui->deletePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->deletePushButton, &QPushButton::clicked, [&]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Do you want to delete the LegoSet(s)?"), "ID(s): " + mModel->getSelectedLegoSetIDs(),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            mModel->removeSelectedLegoSets();
    });

}

ForSale::~ForSale()
{
    delete ui;
}
