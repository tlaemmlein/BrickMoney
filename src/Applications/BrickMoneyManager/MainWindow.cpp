#include "MainWindow.h"
#include "ui_MainWindow.h"

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");

    QString connectString = QStringLiteral(
    "Driver={ODBC Driver 17 for SQL Server};"
    "Server=tcp:brickmoneyserver.database.windows.net,1433;"
    "Database=BrickMoneyDB;"
    "Uid=bmadmin;"
    "Pwd={YOURPW};"
    "Encrypt=yes;"
    "TrustServerCertificate=no;"
        "Connection Timeout=30;");
    db.setDatabaseName(connectString);
    if(!db.open())
    {
        qDebug() << "Can't Connect to DB !";
    }
    else
    {
        qDebug() << "Connected Successfully to DB !";

        //onlyQSqlTableModel();

        onlyQSqlQuery();

        fillTable();

        //mixedQueryAndModel();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onlyQSqlTableModel()
{
    QSqlTableModel* model = new QSqlTableModel(this);
    model->setTable("LegoSets");
    model->select();
    while (model->canFetchMore())
    {
        model->fetchMore();
    }
    int rows = model->rowCount();
    QString outext = "row Count:" + QString::number(rows);

    //qDebug() << "rows: " << model->rowCount();

    ui->tableView->setModel(model);

//    for (int i = 0; i < model.rowCount(); ++i) {
//        const auto& rec = model.record(i);
//        QString msg = rec.value("set_id").toString();
//        msg += ";" + rec.value("name_en").toString();
//        msg += ";" + rec.value("name_de").toString();
//        msg += ";" + QString::number(rec.value("year").toInt());
//        msg += ";" + QString::number(rec.value("rr_price").toDouble());
//        ui->textEdit->append(msg);
//        qDebug() << ".";
//    }
}

void MainWindow::onlyQSqlQuery()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT * FROM LegoSets");
    if(!query.exec())
    {
        qDebug() << "Can't Execute Query !";
    }
    else
    {
        qDebug() << "Query Executed Successfully !";
        while(query.next())
        {
            QString msg = query.value("set_id").toString();
            msg += ";" + query.value("name_en").toString();
            msg += ";" + query.value("name_de").toString();
            msg += ";" + QString::number(query.value("year").toInt());
            msg += ";" + QString::number(query.value("rr_price").toDouble());
            ui->textEdit->append(msg);
        }
    }
}

void MainWindow::fillTable()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT * FROM LegoSets");
    if(!query.exec())
    {
        qDebug() << "Can't Execute Query !";
    }
    else
    {
        qDebug() << "Query Executed Successfully !";

        qDebug() << "Num of rows : " << query.numRowsAffected();
        auto model= new QStandardItemModel(0, 5, this);
        qDebug() << "Model created !";

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

            qDebug() << i;
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

void MainWindow::mixedQueryAndModel()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT * FROM LegoSets");
    if(!query.exec())
    {
        qDebug() << "Can't Execute Query !";
    }
    else
    {
        qDebug() << "Query Executed Successfully !";
        QSqlQueryModel model;
        model.setQuery(query);

        for (int i = 0; i < model.rowCount(); ++i) {
            const auto& rec = model.record(i);
            QString msg = rec.value("set_id").toString();
            msg += ";" + rec.value("name_en").toString();
            msg += ";" + rec.value("name_de").toString();
            msg += ";" + QString::number(rec.value("year").toInt());
            msg += ";" + QString::number(rec.value("rr_price").toDouble());
            ui->textEdit->append(msg);
            qDebug() << ".";
        }
    }
}
