
#include "MainWindow.h"

#include <QApplication>

//Ref: https://stackoverflow.com/questions/11785157/replacing-winmain-with-main-function-in-win32-programs
#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "universal");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("StoreAndRetrieveImageInSQLite");

    MainWindow w;
    w.show();

    return a.exec();
}
