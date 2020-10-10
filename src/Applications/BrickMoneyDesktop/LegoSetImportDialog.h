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

private:
    Ui::LegoSetImportDialog *ui;
};

#endif // LEGOSETIMPORTDIALOG_H
