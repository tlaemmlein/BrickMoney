#include "Logging.h"
SET_LOGGER("BrickMoney.Main")

#include "LegoSetTableModel.h"

#include "QmlValuePreview.h"

#include "LegoSetIOManager.h"

#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>
#include <QIcon>

#define URI "QmlUtils"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

using namespace log4cplus;
using namespace log4cplus::helpers;



int main(int argc, char *argv[])
{
    // Initialization and deinitialization.
    log4cplus::Initializer initializer;

    SharedObjectPtr<Appender> console_appender(new ConsoleAppender(false, true));
	console_appender->setName(LOG4CPLUS_TEXT("BrickMoneyConsole"));

	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] %-5p %c - %m%n");
	console_appender->setLayout(std::unique_ptr<Layout>(new PatternLayout(pattern)));
	Logger::getRoot().addAppender(console_appender);

    Logger::getRoot().setLogLevel(TRACE_LOG_LEVEL);

    LOG_INFO("Start BrickMoney");

    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

	QIcon icon(":/images/LogoBrickMoneyIcon_16.png");
	icon.addFile(":/images/LogoBrickMoneyIcon_32.png");
	icon.addFile(":/images/LogoBrickMoneyIcon_48.png");
	icon.addFile(":/images/LogoBrickMoneyIcon_64.png");

	app.setWindowIcon(icon);

    QCoreApplication::setApplicationName("BrickMoney");
    QCoreApplication::setOrganizationName("Spielolamm");

    qRegisterMetaType<QDoubleValueArg *>("QDoubleValueArg *");
    qmlRegisterType<QmlDoubleValuePreview>(URI, VERSION_MAJOR, VERSION_MINOR, "DoubleValuePreview");

    qmlRegisterType<LegoSetTableModel>("LegoSetTableModel", 0, 1, "LegoSetTableModel");
    qmlRegisterType<LegoSetIOManager>("LegoSetIOManager", 0, 1, "LegoSetIOManager");
    QSharedPointer<LegoSetTableModel> lego_table_model = QSharedPointer<LegoSetTableModel>(new LegoSetTableModel);
    LegoSetIOManager lego_io_manager;
    lego_io_manager.setLegoSetTableModel(lego_table_model);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_LegoSetTableModel", lego_table_model.get());
    engine.rootContext()->setContextProperty("_LegoSetIOManager", &lego_io_manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

