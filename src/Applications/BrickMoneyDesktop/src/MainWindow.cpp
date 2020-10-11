#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"
#include "LegoSetImportDialog.h"
#include "InStock.h"
#include "ForSale.h"
#include "Sold.h"

#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"
#include "Packages/BrickMoneyBackend/BrickMoneyDataManager.h"
#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"

#include <kddockwidgets/DockWidget.h>

#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>

using namespace BrickMoney;


MainWindow::MainWindow(const QString &uniqueName, KDDockWidgets::MainWindowOptions options, QWidget *parent, Qt::WindowFlags flags) :
    KDDockWidgets::MainWindow(uniqueName, options, parent, flags),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    static const QString inStockTitle = tr("In Stock");
    auto dockInStock = new KDDockWidgets::DockWidget(inStockTitle);
    dockInStock->setWidget(new InStock);
    dockInStock->setIcon(QIcon(":/images/inStock.svg"));
    addDockWidget(dockInStock, KDDockWidgets::Location_OnLeft);
    m_dockwidgets.push_back(dockInStock);
    connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		m_dockwidgets[0]->setTitle(inStockTitle + " (" + QString::number(num) + ")");
        });
    dockInStock->setTitle(inStockTitle + " (" + QString::number(BrickMoneyProject::Inst()->getInStockModel()->numberOfLegoSets()) + ")");
    connect(ui->actionIn_Stock, &QAction::triggered, [&]() { m_dockwidgets[0]->show(); });

    static const QString forSaleTitle = tr("For Sale");
    auto dockForSale = new KDDockWidgets::DockWidget(forSaleTitle);
    dockForSale->setWidget(new ForSale());
    dockForSale->setIcon(QIcon(":/images/forSale.svg"));
    addDockWidget(dockForSale, KDDockWidgets::Location_OnRight);
    m_dockwidgets.push_back(dockForSale);
    connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		m_dockwidgets[1]->setTitle(forSaleTitle + " (" + QString::number(num) + ")");
        });
    dockForSale->setTitle(forSaleTitle + " (" + QString::number(BrickMoneyProject::Inst()->getForSaleModel()->numberOfLegoSets()) + ")");
    connect(ui->actionFor_Sale, &QAction::triggered, [&]() { m_dockwidgets[1]->show(); });

    static const QString soldTitle = tr("Sold");
    auto dockSold = new KDDockWidgets::DockWidget(soldTitle);
    dockSold->setWidget(new Sold());
    dockSold->setIcon(QIcon(":/images/sold.svg"));
    addDockWidget(dockSold, KDDockWidgets::Location_OnRight);
    m_dockwidgets.push_back(dockSold);
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		m_dockwidgets[2]->setTitle(soldTitle + " (" + QString::number(num) + ")");
        });
    dockSold->setTitle(soldTitle + " (" + QString::number(BrickMoneyProject::Inst()->getSoldModel()->numberOfLegoSets()) +")");
    connect(ui->actionSold, &QAction::triggered, [&]() { m_dockwidgets[2]->show(); });

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

    connect(ui->actionImport, &QAction::triggered, [&]() {
        LegoSetImportDialog* d = new LegoSetImportDialog();
        d->resize(200, 200);
        d->show();
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
    qDeleteAll(m_dockwidgets);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	if (BrickMoneyDataManager::Inst()->brickMoneyIsDirty())
	{
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            BrickMoneyProject::Inst()->save();
            event->accept();
            return;
        }
        if (ret == QMessageBox::Discard)
        {
            event->accept();
            return;
        }
        if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
	}
	event->accept();
}

