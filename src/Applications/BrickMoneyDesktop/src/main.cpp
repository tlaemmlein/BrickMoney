#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoneyDesktop.Main")

#include "MainWindow.h"

#include "Packages/BrickMoneyData/BrickMoneySettings.h"
#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"
#include "Packages/BrickMoneyData/LegoSet.h"
#include "Packages/BrickMoneyProject/BrickMoneyProject.h"
#include "Packages/LegoSetInfoGenerator/LegoSetInfoGenerator.h"
#include "Packages/LegoSetModels/LegoSetTableModel.h"


#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QAbstractTableModel>
#include <QIcon>
#include <QDate>

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

	if (BrickMoneyProject::Inst()->checkBrickMoneyProject(BrickMoneySettings::Inst()->brickMoneyFilePath()))
	{
		BrickMoneyProject::Inst()->load();
	}
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(false);

    MainWindow w;
    w.show();

    return app.exec();
}

