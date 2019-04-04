#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
