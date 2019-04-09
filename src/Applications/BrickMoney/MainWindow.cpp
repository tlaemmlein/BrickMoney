#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "LegoSetModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_LegoSetModel = new LegoSetModel(this);

    ui->legoset_tableView->setModel(m_LegoSetModel);

    m_LegoSetModel->addSet();
    m_LegoSetModel->addSet();
    m_LegoSetModel->addSet();
    m_LegoSetModel->addSet();

    setWindowTitle(tr("BrickMoney - Software für LEGO Investment"));

    ui->statusBar->showMessage(tr("BrickMoney gestartet"));

    updateActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateActions()
{
    connect(ui->action_exit, &QAction::triggered, this, &MainWindow::close);
}
