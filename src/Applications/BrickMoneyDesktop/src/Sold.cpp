#include "Sold.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"
#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"

#include <QPushButton>

Sold::Sold(QWidget *parent) : LegSetTableView(parent)
    , mSortModel(BrickMoneyProject::Inst()->getSoldSortModel())
    , mModel(BrickMoneyProject::Inst()->getSoldModel())
{
    LegSetTableView::init();

    mToForSale = addPushButton();
    connect(mToForSale, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getForSaleModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
    });

    mToInStock = addPushButton();
    connect(mToInStock, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getInStockModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });
}

Sold::~Sold()
{
}


LegoSetSortFilterTableModel *Sold::getSortModel() const
{
    return mSortModel;
}

LegoSetTableModel *Sold::getModel() const
{
    return mModel;
}

uint Sold::getVisibilityFlags() const
{
    return BrickMoneySettings::Inst()->soldFlags();
}

void Sold::setVisibilityFlags(uint flags)
{
    BrickMoneySettings::Inst()->setSoldFlags(flags);
}

void Sold::retranslateUi() const
{
    mToForSale->setText(moveToForSaleText());
    mToInStock->setText(moveToInStockText());
}
