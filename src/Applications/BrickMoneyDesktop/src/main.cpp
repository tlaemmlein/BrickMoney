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
#include <log4cplus/fileappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QAbstractTableModel>
#include <QDir>
#include <QIcon>
#include <QDate>
#include <QStandardPaths>
#include <QMessageBox>
#include <QSplashScreen>

using namespace KDDockWidgets;
//using namespace log4cplus;
//using namespace log4cplus::helpers;

//Ref: https://stackoverflow.com/questions/11785157/replacing-winmain-with-main-function-in-win32-programs
#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("BrickMoney");
	const QString app_data_loc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	if (!QDir(app_data_loc).exists())
	{
		QDir().mkdir(app_data_loc);
	}

    // Initialization and deinitialization.
    log4cplus::Initializer initializer;

    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] %-5p %c - %m%n");

	log4cplus::helpers::SharedObjectPtr<log4cplus::Appender> console_appender(new log4cplus::ConsoleAppender(false, true));
    console_appender->setName(LOG4CPLUS_TEXT("BrickMoneyConsole"));
    console_appender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));
	log4cplus::Logger::getRoot().addAppender(console_appender);

	log4cplus::tstring log_file_name = (QString(app_data_loc) + "/BrickMoney.log").toStdWString();
	log4cplus::helpers::SharedObjectPtr<log4cplus::Appender> rolling_file_appender(new log4cplus::RollingFileAppender(log_file_name));
	rolling_file_appender->setName(LOG4CPLUS_TEXT("BrickMoneyRollingFile"));
	rolling_file_appender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));
	log4cplus::Logger::getRoot().addAppender(rolling_file_appender);

	log4cplus::Logger::getRoot().setLogLevel(log4cplus::DEBUG_LOG_LEVEL);

    LOG_INFO("Start " << QCoreApplication::applicationName().toStdWString() << " 0.2" );


	QPixmap pixmap(":/images/brickMoney_splashscreen.png");
	QSplashScreen splash(pixmap);

	if (!BrickMoneyDatabase::Inst()->prepareBrickMoneyDBLocale(app_data_loc) )
	{
		QMessageBox messageBox;
		QString msg = QCoreApplication::translate("main", "Could not prepare locale database!\n");
		msg += QCoreApplication::translate("main", "Please check %1 folder.\n").arg(app_data_loc);
		LOG_ERROR(msg.toStdWString());
		messageBox.critical(0, "Error with locale database", msg);
		return -1;
	}

	try
	{
		if (BrickMoneyDatabase::Inst()->isNewRemoteVersionAvailable()) {
			splash.show();
			app.processEvents();
			QString msg = "The database must be updated. That will take some time.";
			splash.showMessage(msg);
			LOG_INFO(msg.toStdWString());
			BrickMoneyDatabase::Inst()->updateBrickMoneyDBLocale();
		}
	}
	catch (const RemoteDBException& e)
	{
		QMessageBox messageBox;
		QString msg = QCoreApplication::translate("main", "The remote database is not online!\n");
		msg += QCoreApplication::translate("main", "Hint: Please check your internet connection.\n");
		msg += QCoreApplication::translate("main", "Error message: %1.\n").arg(QString::fromStdString(e.what()));
		LOG_ERROR(msg.toStdWString());
		messageBox.warning(0, "Error with remote database", msg);
	}
	catch (const std::exception& e)
	{
		QMessageBox messageBox;
		QString msg = QCoreApplication::translate("main", "Something is wrong with the database!\n");
		msg += QCoreApplication::translate("main", "Error message: %1.\n").arg(QString::fromStdString(e.what()));
		LOG_ERROR(msg.toStdWString());
		messageBox.critical(0, "Error with database", msg);
		return -1;
	}

	if ( !BrickMoneyProject::Inst()->prepareBrickMoneyProject() )
	{
		LOG_WARN("Could not prepare BrickMoneyProject.");
	}

	auto flags = KDDockWidgets::Config::self().flags();
	flags |= KDDockWidgets::Config::Flag_AllowReorderTabs;
	flags |= KDDockWidgets::Config::Flag_TitleBarIsFocusable;
	flags |= KDDockWidgets::Config::Flag_AlwaysTitleBarWhenFloating;
	flags |= KDDockWidgets::Config::Flag_TabsHaveCloseButton;
    flags |= KDDockWidgets::Config::Flag_TitleBarHasMinimizeButton;
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

