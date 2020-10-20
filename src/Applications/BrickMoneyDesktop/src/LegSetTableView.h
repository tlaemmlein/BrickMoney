#ifndef LegSetTableView_H
#define LegSetTableView_H

#include "Packages/BrickMoneyBackend/LegoSet.h"

#include <QWidget>
#include <QVector>

#include <bitset>


QT_BEGIN_NAMESPACE
class QPushButton;
class QTableView;
class QLineEdit;
class QStandardItem;
QT_END_NAMESPACE

class LegoSetSortFilterTableModel;
class LegoSetTableModel;


namespace Ui {
class LegSetTableView;
}

class LegSetTableView : public QWidget
{
    Q_OBJECT

public:
    explicit LegSetTableView(QWidget *parent = nullptr);
    ~LegSetTableView();

    virtual void init();

protected:
    Ui::LegSetTableView *ui;

    QPushButton* addPushButton(const QString& title);

    QTableView* tableView();
    QLineEdit* filterLineEdit();

    virtual LegoSetSortFilterTableModel* getSortModel() const = 0;
    virtual LegoSetTableModel* getModel() const = 0;

    virtual QString title() const = 0;

    virtual void selectionIsDirty(bool isDirty) = 0;
    virtual uint getVisibilityFlags() const = 0;
	virtual void setVisibilityFlags(uint flags) = 0;


private:
	std::bitset<LegoSetProperty::COUNT> mVisiblityFlags;
};

#endif // LegSetTableView_H
