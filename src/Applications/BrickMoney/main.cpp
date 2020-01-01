#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>

#include "LegoSetTableModel.h"
#include "LegoSetIOManager.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("BrickMoney");
    QCoreApplication::setOrganizationName("Spielolamm");

    qmlRegisterType<LegoSetTableModel>("LegoSetTableModel", 0, 1, "LegoSetTableModel");
    qmlRegisterType<LegoSetIOManager>("LegoSetIOManager", 0, 1, "LegoSetIOManager");
    LegoSetTableModel lego_table_model;
    LegoSetIOManager lego_io_manager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_LegoSetTableModel", &lego_table_model);
    engine.rootContext()->setContextProperty("_LegoSetIOManager", &lego_io_manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}

