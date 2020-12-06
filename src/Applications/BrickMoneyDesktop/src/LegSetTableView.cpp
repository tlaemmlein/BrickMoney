#include "LegSetTableView.h"
#include "ui_LegSetTableView.h"
#include "ImageDelegate.h"
#include "CheckBoxDelegate.h"
#include "CalendarDelegate.h"
#include "DoubleSpinBoxDelegate.h"
#include "LineEditDelegate.h"
#include "LegoSetSpinBoxDelegate.h"

#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"
#include "Packages/BrickMoneyBackend/BrickMoneyDataManager.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"

#include <QMessageBox>
#include <QPushButton>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QMenu>
#include <QWidgetAction>

LegSetTableView::LegSetTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LegSetTableView)
{
    ui->setupUi(this);
    ui->legoSetTableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color:'wheat' }");
}

LegSetTableView::~LegSetTableView()
{
    delete ui;
}

void LegSetTableView::init()
{
    ui->legSetTableViewTitle->setText(title());

    ui->legoSetTableView->setModel(getSortModel());
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::imageKey, new ImageDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::setNumber, new LegoSetSpinBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::seller, new LineEditDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::purchaseDate, new CalendarDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::purchasingPrice, new DoubleSpinBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::retailPrice, new DoubleSpinBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::saleDate, new CalendarDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::soldOver, new LineEditDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::buyer, new LineEditDelegate(this));

    ui->colVisibilityToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu* menu = new QMenu(ui->colVisibilityToolButton);
    mVisiblityFlags  = getVisibilityFlags();
    for (int index = 0; index < LegoSetProperty::COUNT; ++index)
    {
        QCheckBox *checkBox = new QCheckBox(LegoSet::displayName( static_cast<LegoSetProperty>(index)), menu);
		checkBox->setChecked(mVisiblityFlags.test(index));
		checkBox->setProperty("ColIndex", index);
        QWidgetAction *checkableAction = new QWidgetAction(menu);
        checkableAction->setDefaultWidget(checkBox);
        menu->addAction(checkableAction);
        connect(checkBox, &QCheckBox::toggled, [checkBox, this] (bool checked) {
            int index = checkBox->property("ColIndex").toInt();
            //qDebug() << "Column: " << index;
			ui->legoSetTableView->setColumnHidden(index, !checked);
			mVisiblityFlags.set(index, checked);
            setVisibilityFlags(mVisiblityFlags.to_ulong());
        });
		ui->legoSetTableView->setColumnHidden(index, !checkBox->isChecked());
	}
    ui->colVisibilityToolButton->setMenu(menu);


    ui->legoSetTableView->resizeColumnsToContents();

    connect(ui->selectAllPushButton, &QPushButton::clicked, [&] {
        getModel()->dataSource()->selectAllSets();
        ui->legoSetTableView->setFocus();
        ui->legoSetTableView->update();
    });

    connect(ui->selectNonePushButton, &QPushButton::clicked, [&] {
        getModel()->dataSource()->selectNoneSets();
        ui->legoSetTableView->setFocus();
        ui->legoSetTableView->update();
    });


    connect(ui->filterLineEdit, &QLineEdit::editingFinished, [&]() {
        getSortModel()->setFilterText(ui->filterLineEdit->text());
    });

    connect(getModel(), &LegoSetTableModel::selectionIsDirtyChanged, [&] (bool isDirty) {
        ui->numOfSelectedLabel->setVisible(isDirty);
        ui->copyAndPastePushButton->setVisible(isDirty);
        ui->deletePushButton->setVisible(isDirty);
        selectionIsDirty(isDirty);
    });

    ui->numOfSelectedLabel->setVisible(getModel()->selectionIsDirty());
    static const QString selectedText = tr("selected");
    connect(getModel(), &LegoSetTableModel::numberOfSelectedLegoSetsChanged, [&] (int num) {
        ui->numOfSelectedLabel->setText(QString::number(num) + " " + selectedText);
    });
    ui->numOfSelectedLabel->setText(QString::number(getModel()->numberOfSelectedLegoSets()) + " " + selectedText);

    ui->copyAndPastePushButton->setVisible(getModel()->selectionIsDirty());
    connect(ui->copyAndPastePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->copySelectedLegoSets(getModel(), getModel());
    });

    ui->deletePushButton->setVisible(getModel()->selectionIsDirty());
    connect(ui->deletePushButton, &QPushButton::clicked, [&]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Do you want to delete the LegoSet(s)?"), "ID(s): " + getModel()->getSelectedLegoSetIDs(),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            getModel()->removeSelectedLegoSets();
    });
}

void LegSetTableView::changeEvent(QEvent *event)
{
	if (event && QEvent::LanguageChange == event->type()) {
		// this event is send if a translator is loaded
		ui->retranslateUi(this);
		ui->legSetTableViewTitle->setText(title());
	}
	QWidget::changeEvent(event);
}

QPushButton* LegSetTableView::addPushButton(const QString &title)
{
    QPushButton* button = new QPushButton(title, ui->LegSetTableViewFrame);

    ui->horizontalLayout->insertWidget(5, button);

    return button;
}

QTableView *LegSetTableView::tableView()
{
    return ui->legoSetTableView;
}

QLineEdit *LegSetTableView::filterLineEdit()
{
    return ui->filterLineEdit;
}

