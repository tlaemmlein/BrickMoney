#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoneyDesktop.Main")

#include "MainWindow.h"

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
#include <QPixmapCache>

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

    QPixmapCache::clear();
    const int cacheLimit200MB = 20 * 10240;
    QPixmapCache::setCacheLimit(cacheLimit200MB);

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
    w.show();

    return app.exec();
}

