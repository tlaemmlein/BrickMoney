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
    void saveProject();

private slots:
    void on_legoset_new_pushButton_clicked();

    void on_legoset_delete_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    LegoSetModel* m_LegoSetModel;
    LegoSetList* m_LegoSetList;

    void updateActions();

    void updateToolBar();
};

#endif // MAINWINDOW_H
