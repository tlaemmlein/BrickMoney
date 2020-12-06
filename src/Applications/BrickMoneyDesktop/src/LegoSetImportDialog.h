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

protected:
	void changeEvent(QEvent* event);

private slots:
    void on_importCsvPushButton_clicked();

private:
    Ui::LegoSetImportDialog *ui;
};

#endif // LEGOSETIMPORTDIALOG_H
