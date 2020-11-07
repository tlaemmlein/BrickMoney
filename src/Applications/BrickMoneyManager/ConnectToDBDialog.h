#ifndef CONNECTTODBDIALOG_H
#define CONNECTTODBDIALOG_H

#include <QDialog>

#include <QSql>
#include <QSqlDatabase>

namespace Ui {
class ConnectToDBDialog;
}

class ConnectToDBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectToDBDialog(QWidget *parent = nullptr);
    ~ConnectToDBDialog();

    QSqlDatabase database();

private:
    Ui::ConnectToDBDialog *ui;
    QSqlDatabase m_Database;
    QString m_ConnectString;
};

#endif // CONNECTTODBDIALOG_H
