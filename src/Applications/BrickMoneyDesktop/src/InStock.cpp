#include "InStock.h"
#include "ui_InStock.h"
#include "ImageDelegate.h"
#include "CalendarDelegate.h"
#include "CheckBoxDelegate.h"
#include "DoubleSpinBoxDelegate.h"
#include "LineEditDelegate.h"
#include "LegoSetSpinBoxDelegate.h"

#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"
#include "Packages/BrickMoneyBackend/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"

#include <QMessageBox>


InStock::InStock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InStock)
{
    ui->setupUi(this);

    mSortModel = BrickMoneyProject::Inst()->getInStockSortModel();
    mModel = BrickMoneyProject::Inst()->getInStockModel();

    ui->inStockTableView->setModel(mSortModel);
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::setNumber, new LegoSetSpinBoxDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::seller, new LineEditDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::purchaseDate, new CalendarDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::purchasingPrice, new DoubleSpinBoxDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::retailPrice, new DoubleSpinBoxDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::saleDate, new CalendarDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::soldOver, new LineEditDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::buyer, new LineEditDelegate(this));

    ui->inStockTableView->resizeColumnsToContents();

    connect(ui->selectAllPushButton, &QPushButton::clicked, [&] {
        mModel->dataSource()->selectAllSets();
        ui->inStockTableView->setFocus();
        ui->inStockTableView->update();
    });

    connect(ui->selectNonePushButton, &QPushButton::clicked, [&] {
        mModel->dataSource()->selectNoneSets();
        ui->inStockTableView->setFocus();
        ui->inStockTableView->update();
    });

    connect(ui->addPushButton, &QPushButton::clicked, [&] {
		ui->inStockLineEdit->setText("");
		mSortModel->setFilterText("");
		auto set = new LegoSet(41599, mSortModel);
        auto source = mModel->dataSource();
        set->setIsSelected(true);
        source->addLegoSet( set );
        ui->inStockTableView->scrollToBottom();
        ui->inStockTableView->setFocus();
        QModelIndex index = mSortModel->index(mSortModel->rowCount() - 1, LegoSetProperty::setNumber);
        ui->inStockTableView->setCurrentIndex(index);
        ui->inStockTableView->edit(index);
    });

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
