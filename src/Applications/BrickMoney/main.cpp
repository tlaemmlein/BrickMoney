#include "Logging.h"
SET_LOGGER("BrickMoney.Main")

#include "LegoSetTableModel.h"

#include "LegoSetIOManager.h"

#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>

using namespace log4cplus;
using namespace log4cplus::helpers;


int main(int argc, char *argv[])
{
//    // Initialization and deinitialization.
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
    QCoreApplication::setApplicationName("BrickMoney");
    QCoreApplication::setOrganizationName("Spielolamm");

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

