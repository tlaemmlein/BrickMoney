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
    {
        LegSetTableView::init();

        mToSold = addActionButton(":/images/move_to_Sold.svg");
        connect(mToSold, &QPushButton::clicked, [&]() {
            BrickMoneyProject::Inst()->getSoldModel()->removeSelectedLegoSets();
            BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getSoldModel());
            emit legoSetsMovedToSold();
        });

        mToForSale = addActionButton(":/images/move_to_forSale.svg");
        connect(mToForSale, &QPushButton::clicked, [&]() {
            BrickMoneyProject::Inst()->getForSaleModel()->removeSelectedLegoSets();
            BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
            emit legoSetsMovedToForSale();
        });

        mToInStock = addActionButton(":/images/move_to_inStock.svg");
        connect(mToInStock, &QPushButton::clicked, [&]() {
            BrickMoneyProject::Inst()->getInStockModel()->removeSelectedLegoSets();
            BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
            emit legoSetsMovedToInStock();
        });
        retranslateUi();
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

private:
    LegoSetSortFilterTableModel* mSortModel;
    LegoSetTableModel* mModel;
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

    void retranslateUi() const override
    {
        mToSold->setToolTip(moveToSoldText());
        mToForSale->setToolTip(moveToForSaleText());
        mToInStock->setToolTip(moveToInStockText());
    }
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

void LegoSetImportDialog::changeEvent(QEvent * event)
{
	if (event && QEvent::LanguageChange == event->type()) {
		// this event is send if a translator is loaded
		ui->retranslateUi(this);
	}
	QWidget::changeEvent(event);
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

