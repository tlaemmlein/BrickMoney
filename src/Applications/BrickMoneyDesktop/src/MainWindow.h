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

    QString m_postWindowTitel;

	static const int mInStockTabIndex = 1;
	static const int mForSaleTabIndex = 2;
	static const int mSoldTabIndex = 3;
};

#endif // MAINWINDOW_H
