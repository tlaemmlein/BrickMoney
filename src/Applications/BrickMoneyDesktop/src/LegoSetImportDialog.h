#ifndef LEGOSETIMPORTDIALOG_H
#define LEGOSETIMPORTDIALOG_H

#include <QWidget>

namespace Ui {
class LegoSetImportDialog;
}

class LegoSetImportDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LegoSetImportDialog(QWidget *parent = nullptr);
    ~LegoSetImportDialog();

private slots:
    void on_closePushButton_clicked();

private:
    Ui::LegoSetImportDialog *ui;
};

#endif // LEGOSETIMPORTDIALOG_H
