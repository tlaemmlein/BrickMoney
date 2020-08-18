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

