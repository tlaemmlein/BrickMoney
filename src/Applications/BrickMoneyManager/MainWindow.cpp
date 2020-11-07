#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoneyManager.MainWindow")

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ConnectToDBDialog.h"

#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("BrickMoneyMoney Vers. 0.2 - The software for LEGO Investment"));

    connect(ui->actionExit, &QAction::triggered, [&]() { close();});

    connect(ui->actionBrickMoneyDB, &QAction::triggered, [&] () {
        ConnectToDBDialog dlg;
        dlg.show();
        const auto ret = dlg.exec();
        if(ret != QDialog::Rejected)
        {
            m_Database = dlg.database();
            if(!m_Database.open())
            {
                const QString msg = "Can't connect to BrickMoneyDB.";
                ui->statusbar->showMessage(msg);
                LOG_ERROR(msg.toStdWString());
            }
            else
            {
                const QString msg = "Connected successfully to BrickMoneyDB.";
                ui->statusbar->showMessage(msg);
                LOG_INFO(msg.toStdWString());
                fillTable();
            }
        }
         });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::fillTable()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT * FROM LegoSets");
    if(!query.exec())
    {
        LOG_ERROR("Can't Execute Query !");
    }
    else
    {
        LOG_INFO("Query Executed Successfully !");

        auto model= new QStandardItemModel(0, 5, this);

        int i = 0;
        while (query.next())
        {
            QList<QStandardItem *> row;
            auto set_id = new QStandardItem(query.value("set_id").toString());
            row.append(set_id);

            auto name_en = new QStandardItem(query.value("name_en").toString());
            row.append(name_en);

            auto name_de = new QStandardItem(query.value("name_de").toString());
            row.append(name_de);

            auto year = new QStandardItem(QString::number(query.value("year").toInt()));
            row.append(year);

            auto rr_price = new QStandardItem(QString::number(query.value("rr_price").toDouble()));
            row.append(rr_price);

            model->appendRow(row);
            i++;
        }
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Set number"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name en"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Name de"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Year"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("RRPrice €"));
        ui->tableView->setModel(model);
    }
}
