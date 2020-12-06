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

    std::function<void(const QString,
                       QWidget* tableView,
                       const QIcon icon,
                       KDDockWidgets::Location loc,
                       LegoSetTableModel *model)>
        tableCreator = [&] (const QString title,
                          QWidget* tableView,
                          QIcon icon,
                          KDDockWidgets::Location loc,
                          LegoSetTableModel *model) {
        auto dock = new KDDockWidgets::DockWidget(title);
        dock->setWidget(tableView);
        dock->setIcon(icon);
        addDockWidget(dock, loc);
        m_dockwidgets.push_back(dock);
        dock->setTitle(title + " (" + QString::number(model->numberOfLegoSets()) + ")");
    };

    static const QString inStockTitle = tr("In Stock");
    tableCreator(inStockTitle, new InStock, QIcon(":/images/inStock.svg"), KDDockWidgets::Location_OnLeft,
                 BrickMoneyProject::Inst()->getInStockModel());
    connect(BrickMoneyProject::Inst()->getInStockModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
        m_dockwidgets[0]->setTitle(inStockTitle + " (" + QString::number(num) + ")");
    });
    connect(ui->actionIn_Stock, &QAction::triggered, [&]() { m_dockwidgets[0]->show(); });

    static const QString forSaleTitle = tr("For Sale");
    tableCreator(forSaleTitle, new ForSale, QIcon(":/images/forSale.svg"), KDDockWidgets::Location_OnRight,
                 BrickMoneyProject::Inst()->getForSaleModel());
    connect(BrickMoneyProject::Inst()->getForSaleModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
        m_dockwidgets[1]->setTitle(forSaleTitle + " (" + QString::number(num) + ")");
    });
    connect(ui->actionFor_Sale, &QAction::triggered, [&]() { m_dockwidgets[1]->show(); });

    static const QString soldTitle = tr("Sold");
    tableCreator(soldTitle, new Sold, QIcon(":/images/sold.svg"), KDDockWidgets::Location_OnRight,
                 BrickMoneyProject::Inst()->getSoldModel());
    connect(BrickMoneyProject::Inst()->getSoldModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
		m_dockwidgets[2]->setTitle(soldTitle + " (" + QString::number(num) + ")");
        });
    connect(ui->actionSold, &QAction::triggered, [&]() { m_dockwidgets[2]->show(); });

    static const QString importTitle = tr("Import");
    tableCreator(importTitle, new LegoSetImportDialog, QIcon(), KDDockWidgets::Location_OnTop,
                 BrickMoneyProject::Inst()->getImportModel());
    m_dockwidgets[3]->close();
    connect(BrickMoneyProject::Inst()->getImportModel(), &LegoSetTableModel::numberOfLegoSetsChanged, [&](int num) {
        m_dockwidgets[3]->setTitle(importTitle + " (" + QString::number(num) + ")");
    });
    connect(ui->actionImport, &QAction::triggered, [&]() { m_dockwidgets[3]->show(); });

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
    qDeleteAll(m_dockwidgets);
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
	}
	QMainWindow::changeEvent(event);
}

QString BrickMoney::MainWindow::postWindowTitle()
{
	return tr(" - BrickMoney Vers. 0.2 - The software for LEGO Investment");
}

