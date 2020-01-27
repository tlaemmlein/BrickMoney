#include "LegoSetTableModel.h"
#include "LegoSetIOManager.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/consoleappender.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>


int main(int argc, char *argv[])
{
//    // Initialization and deinitialization.
    log4cplus::Initializer initializer;

    log4cplus::BasicConfigurator config;
    config.configure();

    log4cplus::Logger logger = log4cplus::Logger::getInstance(
        LOG4CPLUS_TEXT("main"));
    LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Hello, World!"));

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

