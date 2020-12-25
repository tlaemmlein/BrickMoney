#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/MainWindow.h>


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

public slots:
	void updateInStockTitle();
	void updateForStockTitle();
	void updateSoldTitle();
	void updateImportTitle();

protected:

	void closeEvent(QCloseEvent *event) override;

	void changeEvent(QEvent* event);



private:
    Ui::MainWindow *ui;

	KDDockWidgets::DockWidget* m_InStockDock;
	KDDockWidgets::DockWidget* m_ForSaleDock;
	KDDockWidgets::DockWidget* m_SoldDock;
	KDDockWidgets::DockWidget* m_ImportDock;

	bool loadProjectFromSettings();
	QString postWindowTitle();
	QString inStockText() const;
	QString forSaleText() const;
	QString soldText() const;
	QString importText() const;
};
}
#endif // MAINWINDOW_H
