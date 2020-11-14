#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSqlQuery;

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

	void addImgToTable(const QString& name, QSqlQuery& query);

	void getImgFromTable(QSqlQuery& query);
};

#endif // MAINWINDOW_H
