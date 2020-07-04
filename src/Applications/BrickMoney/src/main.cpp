#include "Logging.h"
SET_LOGGER("BrickMoney.Main")

#include "BrickMoneySettings.h"
#include "BrickMoneyProject.h"
#include "LegoSetInfoGenerator.h"
#include "LegoSetTableModel.h"
#include "LegoSet.h"

#include "QmlValuePreview.h"

#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>
#include <QIcon>

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

    LOG_INFO("Start BrickMoney");

    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QIcon appIcon(":/images/LogoBrickMoneyIcon_16.png");
    appIcon.addFile(":/images/LogoBrickMoneyIcon_32.png");
    appIcon.addFile(":/images/LogoBrickMoneyIcon_48.png");
    appIcon.addFile(":/images/LogoBrickMoneyIcon_64.png");
    app.setWindowIcon(appIcon);

    QCoreApplication::setApplicationName("BrickMoney");

    qRegisterMetaType<QDoubleValueArg *>("QDoubleValueArg *");
    qmlRegisterType<QmlDoubleValuePreview>("QmlUtils", 1, 0, "DoubleValuePreview");

    qmlRegisterType<LegoSet>("de.brickmoney.models", 0, 1, "LegoSet");
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("BrickMoneySettings", BrickMoneySettings::Inst());
    engine.rootContext()->setContextProperty("BrickMoneyProject", BrickMoneyProject::Inst());

    engine.rootContext()->setContextProperty("InStockLegoSetTableModel", BrickMoneyProject::Inst()->getInStockModel());
    engine.rootContext()->setContextProperty("ForSaleLegoSetTableModel", BrickMoneyProject::Inst()->getForSaleModel());
    engine.rootContext()->setContextProperty("SoldLegoSetTableModel", BrickMoneyProject::Inst()->getSoldModel());

    engine.rootContext()->setContextProperty("InStockLegoSetTableSortModel", BrickMoneyProject::Inst()->getInStockSortModel());
    engine.rootContext()->setContextProperty("ForSaleLegoSetTableSortModel", BrickMoneyProject::Inst()->getForSaleSortModel());
    engine.rootContext()->setContextProperty("SoldLegoSetTableSortModel", BrickMoneyProject::Inst()->getSoldSortModel());

    LegoSetInfoGenerator gen;
    engine.rootContext()->setContextProperty("LegoSetInfoGenerator", &gen);

    LegoSet legoSet;
    engine.rootContext()->setContextProperty("_LegoSet", &legoSet);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

	if (BrickMoneyProject::Inst()->checkBrickMoneyProject(BrickMoneySettings::Inst()->brickMoneyFilePath()))
	{
		BrickMoneyProject::Inst()->load();
	}
	BrickMoneySettings::Inst()->setBrickMoneyIsDirty(false);


    return app.exec();
}
