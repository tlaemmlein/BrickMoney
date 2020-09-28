#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"

#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"
#include "Packages/BrickMoneyBackend/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"

#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	static const QString inStockTitle = tr("In Stock");
	connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		ui->tabWidget->setTabText(mInStockTabIndex, inStockTitle + " (" + QString::number(num) + ")");
		});
	ui->tabWidget->setTabText(mInStockTabIndex, inStockTitle + " (" + QString::number(BrickMoneyProject::Inst()->getInStockModel()->numberOfLegoSets()) + ")");
    connect(ui->inStockWidget, &InStock::legoSetsMovedToForSale, [&]() { ui->tabWidget->setCurrentIndex(mForSaleTabIndex); });

	static const QString forSaleTitle = tr("For Sale");
	connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		ui->tabWidget->setTabText(mForSaleTabIndex, forSaleTitle + " (" + QString::number(num) + ")");
		});
	ui->tabWidget->setTabText(mForSaleTabIndex, forSaleTitle + " (" + QString::number(BrickMoneyProject::Inst()->getForSaleModel()->numberOfLegoSets()) + ")");
    connect(ui->forSaleWidget, &ForSale::legoSetsMovedToInStock, [&]() { ui->tabWidget->setCurrentIndex(mInStockTabIndex); });
    connect(ui->forSaleWidget, &ForSale::legoSetsMovedToSold, [&]() { ui->tabWidget->setCurrentIndex(mSoldTabIndex); });


	static const QString soldTitle = tr("Sold");
	connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		ui->tabWidget->setTabText(mSoldTabIndex, soldTitle + " (" + QString::number(num) + ")");
		});
    ui->tabWidget->setTabText(mSoldTabIndex, soldTitle + " (" + QString::number(BrickMoneyProject::Inst()->getSoldModel()->numberOfLegoSets()) +")");
    connect(ui->soldWidget, &Sold::legoSetsMovedToInStock, [&]() { ui->tabWidget->setCurrentIndex(mInStockTabIndex); });
    connect(ui->soldWidget, &Sold::legoSetsMovedToForSale, [&]() { ui->tabWidget->setCurrentIndex(mForSaleTabIndex); });

    ui->tabWidget->setCurrentIndex(1);


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

    if ( BrickMoneySettings::Inst()->mainIsMaximized() )
    {
        showMaximized();
    }
    else
    {
        setGeometry(BrickMoneySettings::Inst()->mainWindow());
    }
}

MainWindow::~MainWindow()
{
    BrickMoneySettings::Inst()->setMainWindow(geometry());
    BrickMoneySettings::Inst()->setMainIsMaximized( isMaximized());
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

