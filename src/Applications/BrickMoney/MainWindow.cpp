#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "LegoSetModel.h"

#include <QDebug>
#include <QPushButton>
#include <QIcon>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_LegoSetModel = new LegoSetModel(this);

    ui->legoset_tableView->setModel(m_LegoSetModel);

    setWindowTitle(tr("BrickMoney - Software für LEGO Investment"));

    ui->statusBar->showMessage(tr("BrickMoney gestartet"));

    updateActions();

    updateToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadProject()
{

}

void MainWindow::saveProject()
{

}


void MainWindow::updateActions()
{
    connect(ui->action_load, &QAction::triggered, this, &MainWindow::loadProject);
}

void MainWindow::updateToolBar()
{
    ui->mainToolBar->addAction(ui->action_load);
    ui->mainToolBar->addAction(ui->action_save);
    ui->mainToolBar->addAction(ui->action_close);
}


void MainWindow::on_legoset_new_pushButton_clicked()
{
    qDebug() << __FUNCTION__;
    m_LegoSetModel->addSet();
    ui->legoset_tableView->scrollToBottom();
    ui->legoset_tableView->setFocus();
    QModelIndex index = m_LegoSetModel->index(m_LegoSetModel->rowCount() - 1, LegoSetModel::SetNumber);
    ui->legoset_tableView->setCurrentIndex(index);
    ui->legoset_tableView->edit(index);
}

void MainWindow::on_legoset_delete_pushButton_clicked()
{
    qDebug() << __FUNCTION__;

    if ( m_LegoSetModel->rowCount() < 2 )
    {
        return;
    }

    QItemSelectionModel *selectionModel = ui->legoset_tableView->selectionModel();

    if (!selectionModel->hasSelection())
    {
        return;
    }

    QModelIndex index = selectionModel->currentIndex();

    if (!index.isValid())
    {
        return;
    }

    QModelIndex next_index = m_LegoSetModel->index(index.row() + 1, LegoSetModel::SetNumber);

    if (next_index.isValid())
    {
        ui->legoset_tableView->setCurrentIndex(next_index);
    }
    else
    {
        QModelIndex prev_index = m_LegoSetModel->index(index.row() - 1, LegoSetModel::SetNumber);

        if (prev_index.isValid())
        {
            ui->legoset_tableView->setCurrentIndex(prev_index);
        }
    }

    m_LegoSetModel->removeRow(index.row(), index.parent());
}
