#include "InStock.h"
#include "ui_InStock.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"
#include "SpinBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

#include <QMessageBox>


InStock::InStock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InStock)
{
    ui->setupUi(this);

    mSortModel = BrickMoneyProject::Inst()->getInStockSortModel();
    mModel = BrickMoneyProject::Inst()->getInStockModel();

    ui->inStockTableView->setModel(mModel);
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::purchasingPrice, new SpinBoxDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::retailPrice, new SpinBoxDelegate(this));
    connect(ui->inStockLineEdit, &QLineEdit::editingFinished, [&]() {
        mSortModel->setFilterText(ui->inStockLineEdit->text());
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, [&] (bool isDirty) {
        ui->numOfSelectedLabel->setVisible(isDirty);
        ui->copyAndPastePushButton->setVisible(isDirty);
        ui->fromInStockToForSalePushButton->setVisible(isDirty);
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

    ui->fromInStockToForSalePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromInStockToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getForSaleModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
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

InStock::~InStock()
{
    delete ui;
}
