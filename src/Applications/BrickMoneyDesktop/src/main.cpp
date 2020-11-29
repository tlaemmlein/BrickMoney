#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoneyDesktop.Main")

#include "MainWindow.h"

#include "Packages/BrickMoneyBackend/BrickMoneyDatabase.h"
#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"
#include "Packages/BrickMoneyBackend/BrickMoneyDataManager.h"
#include "Packages/BrickMoneyBackend/LegoSet.h"
#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"
#include "Packages/BrickMoneyBackend/LegoSetInfoGenerator.h"
#include "Packages/BrickMoneyBackend/LegoSetTableModel.h"

#include <kddockwidgets/Config.h>

#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QAbstractTableModel>
#include <QIcon>
#include <QDate>
#include <QStandardPaths>
#include <QMessageBox>
#include <QSplashScreen>

using namespace KDDockWidgets;
using namespace log4cplus;
using namespace log4cplus::helpers;

//Ref: https://stackoverflow.com/questions/11785157/replacing-winmain-with-main-function-in-win32-programs
#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char *argv[])
{
    // Initialization and deinitialization.
    log4cplus::Initializer initializer;

    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] %-5p %c - %m%n");

    SharedObjectPtr<Appender> console_appender(new ConsoleAppender(false, true));
    console_appender->setName(LOG4CPLUS_TEXT("BrickMoneyConsole"));
    console_appender->setLayout(std::unique_ptr<Layout>(new PatternLayout(pattern)));
    Logger::getRoot().addAppender(console_appender);

    Logger::getRoot().setLogLevel(DEBUG_LOG_LEVEL);

    LOG_INFO("Start BrickMoneyDesktop");

    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("BrickMoney");
	const QString appdataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);


	QPixmap pixmap(":/images/brickMoney_splashscreen.png");
	QSplashScreen splash(pixmap);

	if (!BrickMoneyDatabase::Inst()->prepareBrickMoneyDBLocale(appdataLoc) )
	{
		QMessageBox messageBox;
		QString msg = "Could not prepare locale database!\n";
		msg += QString("Please check %1 folder.\n").arg(appdataLoc);
		messageBox.critical(0, "Error with locale database", msg);
		return -1;
	}

	try
	{
		if (BrickMoneyDatabase::Inst()->isNewRemoteVersionAvailable()) {
			splash.show();
			app.processEvents();
			splash.showMessage("The database must be updated. That will take some time.");
			BrickMoneyDatabase::Inst()->updateBrickMoneyDBLocale();
		}
	}
	catch (const RemoteDBException& e)
	{
		QMessageBox messageBox;
		QString msg = "The remote database is not online!\n";
		msg += QString("Hint: Please check your internet connection.\n");
		msg += QString("Error message: %1.\n").arg(QString::fromStdString(e.what()));
		messageBox.warning(0, "Error with remote database", msg);
	}
	catch (const std::exception& e)
	{
		QMessageBox messageBox;
		QString msg = "Something is wrong with the database!\n";
		msg += QString("Error message: %1.\n").arg(QString::fromStdString(e.what()));
		messageBox.critical(0, "Error with database", msg);
		return -1;
	}

	if (BrickMoneyProject::Inst()->checkBrickMoneyProject(BrickMoneySettings::Inst()->brickMoneyFilePath()))
	{
		BrickMoneyProject::Inst()->load();
	}
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(false);

	auto flags = KDDockWidgets::Config::self().flags();
	flags |= KDDockWidgets::Config::Flag_AllowReorderTabs;
	flags |= KDDockWidgets::Config::Flag_TitleBarIsFocusable;
	flags |= KDDockWidgets::Config::Flag_AlwaysTitleBarWhenFloating;
	flags |= KDDockWidgets::Config::Flag_TabsHaveCloseButton;
	flags &= ~KDDockWidgets::Config::Flag_AeroSnapWithClientDecos;
	/*flags |= KDDockWidgets::Config::Flag_HideTitleBarWhenTabsVisible;
	flags |= KDDockWidgets::Config::Flag_TitleBarHasMaximizeButton;*/
	flags |= KDDockWidgets::Config::Flag_DoubleClickMaximizes;
	KDDockWidgets::Config::self().setFlags(flags);

    MainWindowOptions options = MainWindowOption_None;

    BrickMoney::MainWindow w(QStringLiteral("BrickMoneyMainWindow"), options);
	splash.showMessage("Setting up the main window.");
    w.show();
	splash.finish(&w);

    return app.exec();
}

