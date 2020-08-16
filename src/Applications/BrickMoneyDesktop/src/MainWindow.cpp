#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"

#include "Packages/BrickMoneyProject/BrickMoneyProject.h"
#include "Packages/BrickMoneyData/BrickMoneySettings.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->inStockTableView->setModel(BrickMoneyProject::Inst()->getInStockSortModel());
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->inStockTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));

    ui->forSaleTableView->setModel(BrickMoneyProject::Inst()->getForSaleSortModel());
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->forSaleTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));

    ui->soldTableView->setModel(BrickMoneyProject::Inst()->getSoldSortModel());
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->soldTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));

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

