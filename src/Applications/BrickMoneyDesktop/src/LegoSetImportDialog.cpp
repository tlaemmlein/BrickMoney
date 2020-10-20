#include "LegoSetImportDialog.h"
#include "ui_LegoSetImportDialog.h"

#include "LegSetTableView.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"
#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"

#include <QPushButton>
#include <QFileDialog>


class ImportTable  : public LegSetTableView
{
    Q_OBJECT

public:
    explicit ImportTable(QWidget *parent = nullptr) : LegSetTableView(parent)
            , mSortModel(BrickMoneyProject::Inst()->getImportSortModel())
            , mModel(BrickMoneyProject::Inst()->getImportModel())
            , mTitle(tr("Import"))
    {
        LegSetTableView::init();

        mToSold = addPushButton(tr("Move to Sold"));
        mToSold->setVisible(mModel->selectionIsDirty());
        connect(mToSold, &QPushButton::clicked, [&]() {
            BrickMoneyProject::Inst()->getSoldModel()->removeSelectedLegoSets();
            BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getSoldModel());
            emit legoSetsMovedToSold();
        });

        mToForSale = addPushButton(tr("Move to For Sale"));
        mToForSale->setVisible(mModel->selectionIsDirty());
        connect(mToForSale, &QPushButton::clicked, [&]() {
            BrickMoneyProject::Inst()->getForSaleModel()->removeSelectedLegoSets();
            BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
            emit legoSetsMovedToForSale();
        });

        mToInStock = addPushButton(tr("Move to In Stock"));
        mToInStock->setVisible(mModel->selectionIsDirty());
        connect(mToInStock, &QPushButton::clicked, [&]() {
            BrickMoneyProject::Inst()->getInStockModel()->removeSelectedLegoSets();
            BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
            emit legoSetsMovedToInStock();
        });
    }

    ~ImportTable() {}

signals:
    void legoSetsMovedToInStock();
    void legoSetsMovedToForSale();
    void legoSetsMovedToSold();

    // LegSetTableView interface
protected:
    LegoSetSortFilterTableModel *getSortModel() const override { return mSortModel;}
    LegoSetTableModel *getModel() const override { return mModel;}
    QString title() const override { return mTitle;}
    void selectionIsDirty(bool isDirty) override
    {
        mToInStock->setVisible(isDirty);
        mToForSale->setVisible(isDirty);
        mToSold->setVisible(isDirty);
    }

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
    QString mTitle;
    QPushButton* mToInStock;
    QPushButton* mToForSale;
    QPushButton* mToSold;


    // LegSetTableView interface
protected:
    uint getVisibilityFlags() const override
    {
        return ~static_cast<uint>(0);
    }

    void setVisibilityFlags(uint) override  { }
};



LegoSetImportDialog::LegoSetImportDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LegoSetImportDialog)
{
    ui->setupUi(this);

    ui->importLegoTableView->parentWidget()->layout()->replaceWidget(ui->importLegoTableView, new ImportTable(this));
}

LegoSetImportDialog::~LegoSetImportDialog()
{
    delete ui;
}

void LegoSetImportDialog::on_importCsvPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import from csv file..."), BrickMoneySettings::Inst()->brickMoneyFilePath()
                                                        , tr("CSV files (*.csv)"));
    if (!fileName.isEmpty())
    {
        BrickMoneyProject::Inst()->loadDataFrom(fileName);
    }
}

#include "LegoSetImportDialog.moc"

