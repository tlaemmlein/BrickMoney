#include "Sold.h"
#include "ui_Sold.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"
#include "CalendarDelegate.h"
#include "DoubleSpinBoxDelegate.h"
#include "LineEditDelegate.h"
#include "LegoSetSpinBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

#include <QMessageBox>

Sold::Sold(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sold)
{
    ui->setupUi(this);

    mSortModel = BrickMoneyProject::Inst()->getSoldSortModel();
    mModel = BrickMoneyProject::Inst()->getSoldModel();


    ui->soldTableView->setModel(mSortModel);
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::setNumber, new LegoSetSpinBoxDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::seller, new LineEditDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::purchaseDate, new CalendarDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::purchasingPrice, new DoubleSpinBoxDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::retailPrice, new DoubleSpinBoxDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::saleDate, new CalendarDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::soldOver, new LineEditDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::buyer, new LineEditDelegate(this));

    connect(ui->selectAllPushButton, &QPushButton::clicked, [&] {
        mModel->dataSource()->selectAllSets();
        ui->soldTableView->setFocus();
        ui->soldTableView->update();
    });

    connect(ui->selectNonePushButton, &QPushButton::clicked, [&] {
        mModel->dataSource()->selectNoneSets();
        ui->soldTableView->setFocus();
        ui->soldTableView->update();
    });


    connect(ui->soldLineEdit, &QLineEdit::editingFinished, [&]() {
        mSortModel->setFilterText(ui->soldLineEdit->text());
    });

    connect(mModel, &LegoSetTableModel::selectionIsDirtyChanged, [&] (bool isDirty) {
        ui->numOfSelectedLabel->setVisible(isDirty);
        ui->copyAndPastePushButton->setVisible(isDirty);
        ui->fromSoldToInStockPushButton->setVisible(isDirty);
        ui->fromSoldToForSalePushButton->setVisible(isDirty);
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

    ui->fromSoldToInStockPushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromSoldToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getInStockModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });

    ui->fromSoldToForSalePushButton->setVisible(mModel->selectionIsDirty());
    connect(ui->fromSoldToForSalePushButton, &QPushButton::clicked, [&]() {
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

Sold::~Sold()
{
    delete ui;
}
