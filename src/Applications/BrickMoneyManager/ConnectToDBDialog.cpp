#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoneyManager.ConnectToDBDialog")

#include "ConnectToDBDialog.h"
#include "ui_ConnectToDBDialog.h"

#include <QDebug>
#include <QSqlError>


ConnectToDBDialog::ConnectToDBDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectToDBDialog)
{
    ui->setupUi(this);
    setModal(true);

    if (!m_Database.contains("QODBC3") )
        m_Database = QSqlDatabase::addDatabase("QODBC3");

    ui->driverLineEdit->setText("{ODBC Driver 17 for SQL Server}");
    ui->serverLineEdit->setText("tcp:brickmoneyserver.database.windows.net,1433");
    ui->databaseLineEdit->setText("BrickMoneyDB");

    m_ConnectString  = "Driver="   + ui->driverLineEdit->text() +";";
    m_ConnectString += "Server="   + ui->serverLineEdit->text() +";";
    m_ConnectString += "Database=" + ui->databaseLineEdit->text() +";";

    ui->uidLineEdit->setText("bmadmin");

    connect(ui->cancelPushButton, &QPushButton::clicked, this, [&]() { reject(); close();});

    connect(ui->togglePwToolButton, &QToolButton::toggled, this, [&] (bool checked) {
        ui->passwordLineEdit->setEchoMode( checked ? QLineEdit::EchoMode::Normal : QLineEdit::EchoMode::Password);
        ui->togglePwToolButton->setText( checked ? tr("Hide") : tr("Show"));
    });

    connect(ui->connectPushButton, &QPushButton::clicked, this, [&]() {
        QString connectString = m_ConnectString;
        connectString += "Uid=" + ui->uidLineEdit->text() + ";";
        connectString += "Pwd={" + ui->passwordLineEdit->text() + "};";
        connectString += "Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;";
        m_Database.setDatabaseName(connectString);
        if(m_Database.open())
            accept();
        else
        {
            auto error = m_Database.lastError();
            LOG_ERROR("Could not open database with connection string.");
            LOG_ERROR(error.databaseText().toStdWString());
            LOG_ERROR(error.driverText().toStdWString());
            LOG_ERROR(error.nativeErrorCode().toStdWString());
        }
    });
}

ConnectToDBDialog::~ConnectToDBDialog()
{
    delete ui;
}

QSqlDatabase ConnectToDBDialog::database()
{
    return m_Database;
}
