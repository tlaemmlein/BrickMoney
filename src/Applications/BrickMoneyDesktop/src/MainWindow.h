#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kddockwidgets/MainWindow.h>

namespace Ui {
class MainWindow;
}

namespace  BrickMoney {
class MainWindow : public KDDockWidgets::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &uniqueName, KDDockWidgets::MainWindowOptions options,
                        QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

protected:

	void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    QString m_postWindowTitel;
};
}
#endif // MAINWINDOW_H
