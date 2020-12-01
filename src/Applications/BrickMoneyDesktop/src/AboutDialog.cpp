#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include "BrickMoneyBackend/BrickMoneyDatabase.h"

AboutDialog::AboutDialog(QWidget *parent,  Qt::WindowFlags f) :
    QDialog(parent,f),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	int remoteDBVersion = -1;
	try
	{
		remoteDBVersion = BrickMoneyDatabase::Inst()->remoteDBVersion();
	}
	catch (...)
	{

	}

	ui->versionValueLabel->setText(QString::number(0.2));
	ui->remoteDBVersionValueLabel->setText(QString::number(remoteDBVersion));
	ui->localeDBVersionValueLabel->setText(QString::number(BrickMoneyDatabase::Inst()->localeDBVersion()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
