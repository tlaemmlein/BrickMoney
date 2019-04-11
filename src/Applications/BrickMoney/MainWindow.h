#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LegoSetModel;
class LegoSetList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadProject();

private:
    Ui::MainWindow *ui;

    LegoSetModel* m_LegoSetModel;
    LegoSetList* m_LegoSetList;

    void updateActions();

    void updateToolBar();
};

#endif // MAINWINDOW_H
