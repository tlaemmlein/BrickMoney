#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.MainWindow")

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AboutDialog.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"
#include "LegoSetImportDialog.h"
#include "InStock.h"
#include "ForSale.h"
#include "Sold.h"

#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"
#include "Packages/BrickMoneyBackend/BrickMoneyDataManager.h"
#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"
#include "Packages/BrickMoneyBackend/BrickMoneyImages.h"

#include <kddockwidgets/DockWidget.h>

#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>


using namespace BrickMoney;


MainWindow::MainWindow(const QString &uniqueName, KDDockWidgets::MainWindowOptions options, QWidget *parent, Qt::WindowFlags flags) :
    KDDockWidgets::MainWindow(uniqueName, options, parent, flags),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QActionGroup *languageActionGroup = new QActionGroup(this);
    QAction* lang_de_action = new QAction(QIcon(":/images/lang_de.svg"), "DE", languageActionGroup);
    QAction* lang_en_action = new QAction(QIcon(":/images/lang_en.svg"), "EN", languageActionGroup);
    lang_de_action->setCheckable(true);
    lang_en_action->setCheckable(true);

	// https://forum.qt.io/topic/74995/load-qm-file-from-resource/8
	// https://wiki.qt.io/How_to_create_a_multi_language_application
    connect(lang_de_action, &QAction::toggled, [&](bool isChecked) {
		if (!isChecked)
			return;
		qApp->removeTranslator(&m_Translator);
        QString language = "de";
		QString resourecePath = QString(":/translations/bm_%1.qm").arg(language);
        if (!m_Translator.load(resourecePath))	{
            LOG_ERROR("Could not load translation from " << resourecePath.toStdWString());
        }
        qApp->installTranslator(&m_Translator);
		BrickMoneySettings::Inst()->setLanguage("de");
		});

    connect(lang_en_action, &QAction::toggled, [&](bool isChecked) {
		if (!isChecked)
			return;
		qApp->removeTranslator(&m_Translator);
		BrickMoneySettings::Inst()->setLanguage("en");
	});

	auto lang = BrickMoneySettings::Inst()->language();
	if (lang == "de"){ 
		lang_de_action->setChecked(true);
	}
	else {
		lang_en_action->setChecked(true);
	}

    ui->toolBar->addActions(languageActionGroup->actions());

    QPixmap emptyPixmap(":/images/empty.svg");
	BrickMoneyImages::Inst()->setImage("None", emptyPixmap);

	std::function<KDDockWidgets::DockWidget* (const QString, QWidget* tableView, const QIcon icon, 	KDDockWidgets::Location loc, LegoSetTableModel *model)>
		dockCreator = [&](const QString uniqueID, QWidget* tableView, QIcon icon, KDDockWidgets::Location loc, LegoSetTableModel *model) -> KDDockWidgets::DockWidget* {
				auto dock = new KDDockWidgets::DockWidget(uniqueID);
				dock->setWidget(tableView);
				dock->setIcon(icon);
				addDockWidget(dock, loc);
				return dock;
	};

	m_InStockDock = dockCreator("InStockID", new InStock, QIcon(":/images/inStock.svg"), KDDockWidgets::Location_OnLeft, BrickMoneyProject::Inst()->getInStockModel());
	connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::numberOfLegoSetsChanged, this, &BrickMoney::MainWindow::updateInStockTitle);
    connect(ui->actionIn_Stock, &QAction::triggered, [&]() { m_InStockDock->show(); });
	updateInStockTitle();

	m_ForSaleDock = dockCreator("ForSaleID", new ForSale, QIcon(":/images/forSale.svg"), KDDockWidgets::Location_OnRight, BrickMoneyProject::Inst()->getForSaleModel());
	connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::numberOfLegoSetsChanged, this, &BrickMoney::MainWindow::updateForStockTitle);
	connect(ui->actionFor_Sale, &QAction::triggered, [&]() { m_ForSaleDock->show(); });
	updateForStockTitle();

	m_SoldDock = dockCreator("SoldID", new Sold, QIcon(":/images/sold.svg"), KDDockWidgets::Location_OnRight, BrickMoneyProject::Inst()->getSoldModel());
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::numberOfLegoSetsChanged, this, &BrickMoney::MainWindow::updateSoldTitle);
    connect(ui->actionSold, &QAction::triggered, [&]() { m_SoldDock->show(); });
	updateSoldTitle();

	m_ImportDock = dockCreator("ImportID", new LegoSetImportDialog, QIcon(), KDDockWidgets::Location_OnTop, BrickMoneyProject::Inst()->getImportModel());
	m_ImportDock->close();
    connect(BrickMoneyProject::Inst()->getImportModel(), &LegoSetTableModel::numberOfLegoSetsChanged, this, &BrickMoney::MainWindow::updateImportTitle);
    connect(ui->actionImport, &QAction::triggered, [&]() { m_ImportDock->show(); });
	updateImportTitle();

    connect(BrickMoneySettings::Inst(), &BrickMoneySettings::brickMoneyFilePathChanged, [&](QString filepath){
        setWindowTitle(filepath + postWindowTitle());
    });
    setWindowTitle(BrickMoneySettings::Inst()->brickMoneyFilePath() + postWindowTitle());

    connect(ui->actionLoad_Project, &QAction::triggered, [&]() {
        QString brickMoneyFilePath = QFileDialog::getOpenFileUrl(this, tr("Load BrickMoney Project"),
                                                                  BrickMoneySettings::Inst()->brickMoneyFilePath(),
                                                                  tr("JSON files (*.json)")).toString();

        if ( BrickMoneyProject::Inst()->checkBrickMoneyProject(brickMoneyFilePath))
        {
            BrickMoneySettings::Inst()->setBrickMoneyFilePath(brickMoneyFilePath);
            BrickMoneyProject::Inst()->load();
        }
    });
    
	connect(ui->actionSave_Project, &QAction::triggered, [&]() { BrickMoneyProject::Inst()->save(); });
	connect(BrickMoneyDataManager::Inst(), &BrickMoneyDataManager::brickMoneyIsDirtyChanged, ui->actionSave_Project, &QAction::setEnabled);
	ui->actionSave_Project->setEnabled(BrickMoneyDataManager::Inst()->brickMoneyIsDirty());

    connect(ui->actionSave_Project_as, &QAction::triggered, [&]() {
        QString brickMoneyFilePath = QFileDialog::getSaveFileUrl(this, tr("Save BrickMoney project as..."),
                                                                  BrickMoneySettings::Inst()->brickMoneyFilePath(),
                                                                  tr("JSON files (*.json)")).toString();
        if (!brickMoneyFilePath.isEmpty())
        {
            BrickMoneySettings::Inst()->setBrickMoneyFilePath(brickMoneyFilePath);
            BrickMoneyProject::Inst()->save();
        }
    });

	if (BrickMoneyProject::Inst()->isTemporaryProject()) {
		QMessageBox messageBox;
		QString msg = tr("Welcome to BrickMoney!\n");
		msg += tr("Please save the BrickMoney project to another location.\n");
		LOG_INFO(msg.toStdWString());
		messageBox.information(0, "BrickMoney project file location", msg);
	}

    connect(ui->actionExit, &QAction::triggered, [&]() { close();});

    if ( BrickMoneySettings::Inst()->mainIsMaximized() )
    {
        showMaximized();
    }
    else
    {
        setGeometry(BrickMoneySettings::Inst()->mainWindow());
    }

    connect(ui->actionAbout, &QAction::triggered, [&]() {
        AboutDialog* dlg = new AboutDialog(this);
        dlg->setModal(true);
        dlg->show();
    });
}

MainWindow::~MainWindow()
{
    BrickMoneySettings::Inst()->setMainWindow(geometry());
    BrickMoneySettings::Inst()->setMainIsMaximized( isMaximized());
    delete ui;
	delete m_InStockDock;
	delete m_ForSaleDock;
	delete m_SoldDock;
	delete m_ImportDock;
}


void MainWindow::closeEvent(QCloseEvent * event)
{
	if (BrickMoneyDataManager::Inst()->brickMoneyIsDirty())
	{
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            BrickMoneyProject::Inst()->save();
            event->accept();
            return;
        }
        if (ret == QMessageBox::Discard)
        {
            event->accept();
            return;
        }
        if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
	}
	event->accept();
}

void MainWindow::changeEvent(QEvent * event)
{
	if (event && QEvent::LanguageChange == event->type()) {
		// this event is send if a translator is loaded
		ui->retranslateUi(this);
		setWindowTitle(BrickMoneySettings::Inst()->brickMoneyFilePath() + postWindowTitle());
		updateInStockTitle();
		updateForStockTitle();
		updateSoldTitle();
		updateImportTitle();

		bool reloadProject = true;

		if (BrickMoneyDataManager::Inst()->brickMoneyIsDirty())
		{
			QMessageBox msgBox;
			msgBox.setText(tr("The document has been modified."));
			msgBox.setInformativeText(tr("Do you want to save your changes before switching the language?"));
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Discard);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();

			if (ret == QMessageBox::Save)
				BrickMoneyProject::Inst()->save();

			if (ret == QMessageBox::Discard)
				reloadProject = false;
		}

		if (reloadProject)
		{
			if (BrickMoneyProject::Inst()->checkBrickMoneyProject(BrickMoneySettings::Inst()->brickMoneyFilePath()))
			{
				BrickMoneyProject::Inst()->load();
			}
			BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(false);
		}

	}
	QMainWindow::changeEvent(event);
}

void BrickMoney::MainWindow::updateInStockTitle()
{
	m_InStockDock->setTitle(inStockText() + " (" + QString::number(BrickMoneyProject::Inst()->getInStockModel()->numberOfLegoSets()) + ")");
}

void BrickMoney::MainWindow::updateForStockTitle()
{
	m_ForSaleDock->setTitle(forSaleText() + " (" + QString::number(BrickMoneyProject::Inst()->getForSaleModel()->numberOfLegoSets()) + ")");
}

void BrickMoney::MainWindow::updateSoldTitle()
{
	m_SoldDock->setTitle(soldText() + " (" + QString::number(BrickMoneyProject::Inst()->getSoldModel()->numberOfLegoSets()) + ")");
}

void BrickMoney::MainWindow::updateImportTitle()
{
	m_ImportDock->setTitle(importText() + " (" + QString::number(BrickMoneyProject::Inst()->getImportModel()->numberOfLegoSets()) + ")");
}


QString BrickMoney::MainWindow::postWindowTitle()
{
	return tr(" - BrickMoney Vers. 0.2 - The software for LEGO Investment");
}

QString BrickMoney::MainWindow::inStockText() const
{
	return tr("In Stock");
}

QString BrickMoney::MainWindow::forSaleText() const
{
	return tr("For Sale");
}

QString BrickMoney::MainWindow::soldText() const
{
	return tr("Sold");
}

QString BrickMoney::MainWindow::importText() const
{
	return tr("Import");
}

