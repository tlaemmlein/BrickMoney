#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void onlyQSqlTableModel();
    void onlyQSqlQuery();
    void mixedQueryAndModel();
    void fillTable();
};

#endif // MAINWINDOW_H
