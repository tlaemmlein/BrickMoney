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
    QSharedPointer<LegoSetTableModel> lego_table_model = QSharedPointer<LegoSetTableModel>(new LegoSetTableModel);
    LegoSetIOManager lego_io_manager;
    lego_io_manager.setLegoSetTableModel(lego_table_model);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_LegoSetTableModel", lego_table_model.get());
    engine.rootContext()->setContextProperty("_LegoSetIOManager", &lego_io_manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}

