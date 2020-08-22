#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"

#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->inStockTableView->setModel(BrickMoneyProject::Inst()->getInStockSortModel());
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
	static const QString inStockTitle = tr("In Stock");
	connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		ui->tabWidget->setTabText(mInStockTabIndex, inStockTitle + " (" + QString::number(num) + ")");
		});
	ui->tabWidget->setTabText(mInStockTabIndex, inStockTitle + " (" + QString::number(BrickMoneyProject::Inst()->getInStockModel()->numberOfLegoSets()) + ")");
	connect(ui->inStockLineEdit, &QLineEdit::editingFinished, [&]() {
		BrickMoneyProject::Inst()->getInStockSortModel()->setFilterText(ui->inStockLineEdit->text());
		});
    connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromInStockToForSalePushButton, &QPushButton::setVisible);
    ui->fromInStockToForSalePushButton->setVisible(BrickMoneyProject::Inst()->getInStockModel()->selectionIsDirty());
    connect(ui->fromInStockToForSalePushButton, &QPushButton::clicked, [&]() {
		BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getInStockModel(), BrickMoneyProject::Inst()->getForSaleModel());
        ui->tabWidget->setCurrentIndex(mForSaleTabIndex);
		});

    ui->forSaleTableView->setModel(BrickMoneyProject::Inst()->getForSaleSortModel());
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
	static const QString forSaleTitle = tr("For Sale");
	connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		ui->tabWidget->setTabText(mForSaleTabIndex, forSaleTitle + " (" + QString::number(num) + ")");
		});
	ui->tabWidget->setTabText(mForSaleTabIndex, forSaleTitle + " (" + QString::number(BrickMoneyProject::Inst()->getForSaleModel()->numberOfLegoSets()) + ")");
	connect(ui->forSaleLineEdit, &QLineEdit::editingFinished, [&]() {
		BrickMoneyProject::Inst()->getForSaleSortModel()->setFilterText(ui->forSaleLineEdit->text());
        });
    connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromForSaleToInStockPushButton, &QPushButton::setVisible);
    ui->fromForSaleToInStockPushButton->setVisible(BrickMoneyProject::Inst()->getForSaleModel()->selectionIsDirty());
    connect(ui->fromForSaleToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getForSaleModel(), BrickMoneyProject::Inst()->getInStockModel());
        ui->tabWidget->setCurrentIndex(mInStockTabIndex);

    });
    connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromForSaleToSoldPushButton, &QPushButton::setVisible);
    ui->fromForSaleToSoldPushButton->setVisible(BrickMoneyProject::Inst()->getForSaleModel()->selectionIsDirty());
    connect(ui->fromForSaleToSoldPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getForSaleModel(), BrickMoneyProject::Inst()->getSoldModel());
        ui->tabWidget->setCurrentIndex(mSoldTabIndex);
    });


    ui->soldTableView->setModel(BrickMoneyProject::Inst()->getSoldSortModel());
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
	static const QString soldTitle = tr("Sold");
	connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		ui->tabWidget->setTabText(mSoldTabIndex, soldTitle + " (" + QString::number(num) + ")");
		});
	ui->tabWidget->setTabText(mSoldTabIndex, soldTitle + " (" + QString::number(BrickMoneyProject::Inst()->getSoldModel()->numberOfLegoSets()) +")");
	connect(ui->soldLineEdit, &QLineEdit::editingFinished, [&]() {
		BrickMoneyProject::Inst()->getSoldSortModel()->setFilterText(ui->soldLineEdit->text());
		});
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromSoldToInStockPushButton, &QPushButton::setVisible);
    ui->fromSoldToInStockPushButton->setVisible(BrickMoneyProject::Inst()->getSoldModel()->selectionIsDirty());
    connect(ui->fromSoldToInStockPushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getSoldModel(), BrickMoneyProject::Inst()->getInStockModel());
        ui->tabWidget->setCurrentIndex(mInStockTabIndex);
    });
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::selectionIsDirtyChanged, ui->fromSoldToForSalePushButton, &QPushButton::setVisible);
    ui->fromSoldToForSalePushButton->setVisible(BrickMoneyProject::Inst()->getSoldModel()->selectionIsDirty());
    connect(ui->fromSoldToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->moveSelectedLegoSets(BrickMoneyProject::Inst()->getSoldModel(), BrickMoneyProject::Inst()->getForSaleModel());
        ui->tabWidget->setCurrentIndex(mForSaleTabIndex);
    });

    m_postWindowTitel = tr(" - BrickMoney Vers. 0.2 - The software for LEGO Investment");

    connect(BrickMoneySettings::Inst(), &BrickMoneySettings::brickMoneyFilePathChanged, [&](QString filepath){
        setWindowTitle(filepath + m_postWindowTitel);
    });
    setWindowTitle(BrickMoneySettings::Inst()->brickMoneyFilePath() + m_postWindowTitel);

    connect(ui->actionLoad_Project, &QAction::triggered, [&]() {
        QString brickMoneyFilePath = QFileDialog::getOpenFileUrl(this, tr("Load BrickMoney Project"),
                                                                  BrickMoneySettings::Inst()->brickMoneyFilePath(),
                                                                  tr("JSON files (*.json)")).toString();

        if ( BrickMoneyProject::Inst()->checkBrickMoneyProject(brickMoneyFilePath))
        {
            BrickMoneySettings::Inst()->setBrickMoneyFilePath(brickMoneyFilePath);
            BrickMoneyProject::Inst()->load();
        }
    });
    
	connect(ui->actionSave_Project, &QAction::triggered, [&]() { BrickMoneyProject::Inst()->save(); });
	connect(BrickMoneyDataManager::Inst(), &BrickMoneyDataManager::brickMoneyIsDirtyChanged, ui->actionSave_Project, &QAction::setEnabled);
	ui->actionSave_Project->setEnabled(BrickMoneyDataManager::Inst()->brickMoneyIsDirty());

    connect(ui->actionSave_Project_as, &QAction::triggered, [&]() {
        QString brickMoneyFilePath = QFileDialog::getSaveFileUrl(this, tr("Save BrickMoney project as..."),
                                                                  BrickMoneySettings::Inst()->brickMoneyFilePath(),
                                                                  tr("JSON files (*.json)")).toString();
        if (!brickMoneyFilePath.isEmpty())
        {
            BrickMoneySettings::Inst()->setBrickMoneyFilePath(brickMoneyFilePath);
            BrickMoneyProject::Inst()->save();
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	if (BrickMoneyDataManager::Inst()->brickMoneyIsDirty())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, tr("The document has been modified."), tr("Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
			BrickMoneyProject::Inst()->save();
	}
	event->accept();
}

