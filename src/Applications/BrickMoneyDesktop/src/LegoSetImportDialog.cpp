#include "LegoSetImportDialog.h"
#include "ui_LegoSetImportDialog.h"

LegoSetImportDialog::LegoSetImportDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LegoSetImportDialog)
{
    ui->setupUi(this);
}

LegoSetImportDialog::~LegoSetImportDialog()
{
    delete ui;
}

void LegoSetImportDialog::on_closePushButton_clicked()
{
    close();
}
