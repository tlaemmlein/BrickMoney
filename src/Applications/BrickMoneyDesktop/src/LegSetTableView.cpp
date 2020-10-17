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
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::imageUrl, new ImageDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::setNumber, new LegoSetSpinBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::isSelected, new CheckBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::seller, new LineEditDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::purchaseDate, new CalendarDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::purchasingPrice, new DoubleSpinBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::retailPrice, new DoubleSpinBoxDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::saleDate, new CalendarDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::soldOver, new LineEditDelegate(this));
    ui->legoSetTableView->setItemDelegateForColumn(LegoSetProperty::buyer, new LineEditDelegate(this));

    QStandardItemModel* model = new QStandardItemModel(3, 1); // 3 rows, 1 col


    for (int r = 0; r < LegoSetProperty::COUNT; ++r)
    {
        QStandardItem* item = new QStandardItem(LegoSet::displayName( static_cast<LegoSetProperty>(r)));

        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);

        mColumnItems.push_back(item);

        model->setItem(r, 0, item);
    }

    connect(model, &QStandardItemModel::dataChanged, [&] (const QModelIndex& topLeft, const QModelIndex&) {
        qDebug() << "Item " << topLeft.row();
        QStandardItem* item = mColumnItems[topLeft.row()];
        if(item->checkState() == Qt::Unchecked)
        {
            qDebug() << "Unchecked!";
            ui->legoSetTableView->setColumnHidden(topLeft.row(), false);
        }
        else if(item->checkState() == Qt::Checked)
        {
            qDebug() << "Checked!";
            ui->legoSetTableView->setColumnHidden(topLeft.row(), true);
        }

    });

    ui->colVisibilityComboBox->setModel(model);



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

