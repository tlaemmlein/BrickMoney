#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.Main")

#include "BrickMoneySettings.h"
#include "BrickMoneyProject.h"
#include "BrickMoneyTrans.h"

#include "Packages/BrickMoneyData/BrickMoneyDataManager.h"
#include "Packages/BrickMoneyData/LegoSet.h"
#include "Packages/LegoSetInfoGenerator/LegoSetInfoGenerator.h"
#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include "QmlValuePreview.h"


#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>
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

    LOG_INFO("Start BrickMoney");

    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("BrickMoney");

    qRegisterMetaType<QDoubleValueArg *>("QDoubleValueArg *");
    qmlRegisterType<QmlDoubleValuePreview>("QmlUtils", 1, 0, "DoubleValuePreview");

    qmlRegisterSingletonType(QUrl("qrc:/qml/helper/BrickMoneyStyle.qml"), "de.brickmoney.styles", 1, 0, "BrickMoneyStyle");

    qmlRegisterType<LegoSet>("de.brickmoney.models", 0, 1, "LegoSet");
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("BrickMoneySettings", BrickMoneySettings::Inst());
    engine.rootContext()->setContextProperty("BrickMoneyDataManager", BrickMoneyDataManager::Inst());
    engine.rootContext()->setContextProperty("BrickMoneyProject", BrickMoneyProject::Inst());

    engine.rootContext()->setContextProperty("InStockLegoSetTableModel", BrickMoneyProject::Inst()->getInStockModel());
    engine.rootContext()->setContextProperty("ForSaleLegoSetTableModel", BrickMoneyProject::Inst()->getForSaleModel());
    engine.rootContext()->setContextProperty("SoldLegoSetTableModel", BrickMoneyProject::Inst()->getSoldModel());
    engine.rootContext()->setContextProperty("ImportLegoSetTableModel", BrickMoneyProject::Inst()->getImportModel());

    engine.rootContext()->setContextProperty("InStockLegoSetTableSortModel", BrickMoneyProject::Inst()->getInStockSortModel());
    engine.rootContext()->setContextProperty("ForSaleLegoSetTableSortModel", BrickMoneyProject::Inst()->getForSaleSortModel());
    engine.rootContext()->setContextProperty("SoldLegoSetTableSortModel", BrickMoneyProject::Inst()->getSoldSortModel());
    engine.rootContext()->setContextProperty("ImportLegoSetTableSortModel", BrickMoneyProject::Inst()->getImportSortModel());

    LegoSetInfoGenerator gen;
    engine.rootContext()->setContextProperty("LegoSetInfoGenerator", &gen);

    LegoSet legoSet;
    engine.rootContext()->setContextProperty("_LegoSet", &legoSet);

    BrickMoneyTrans bmTrans(&engine);
    bmTrans.selectLanguage(BrickMoneySettings::Inst()->language());
    engine.rootContext()->setContextProperty("BrickMoneyTrans", &bmTrans);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

	if (BrickMoneyProject::Inst()->checkBrickMoneyProject(BrickMoneySettings::Inst()->brickMoneyFilePath()))
	{
		BrickMoneyProject::Inst()->load();
	}
    BrickMoneyDataManager::Inst()->setBrickMoneyIsDirty(false);


    return app.exec();
}

