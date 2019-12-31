#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>

#include "TableModel.h"
#include "LegoSetIOManager.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("BrickMoney");
    QCoreApplication::setOrganizationName("Spielolamm");

    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");
    qmlRegisterType<TableModel>("LegoSetIOManager", 0, 1, "LegoSetIOManager");
    TableModel tm;
    LegoSetIOManager lego_io_manager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_TableModel", &tm);
    engine.rootContext()->setContextProperty("_LegoSetIOManager", &lego_io_manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}

