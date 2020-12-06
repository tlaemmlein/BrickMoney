#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/MainWindow.h>

#include <QTranslator>


namespace Ui {
class MainWindow;
}

class InStock;

namespace  BrickMoney {
class MainWindow : public KDDockWidgets::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &uniqueName, KDDockWidgets::MainWindowOptions options,
                        QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

protected:

	void closeEvent(QCloseEvent *event) override;

	void changeEvent(QEvent* event);

private:
    Ui::MainWindow *ui;

    KDDockWidgets::DockWidget::List m_dockwidgets;

    QString m_postWindowTitel;

	QTranslator m_Translator;

	QString postWindowTitle();
};
}
#endif // MAINWINDOW_H
