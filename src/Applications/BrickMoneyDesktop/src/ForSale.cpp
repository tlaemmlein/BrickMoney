#include "ForSale.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"
#include "Packages/BrickMoneyBackend/BrickMoneySettings.h"

#include <QPushButton>

ForSale::ForSale(QWidget *parent) : LegSetTableView(parent)
    , mSortModel(BrickMoneyProject::Inst()->getForSaleSortModel())
    , mModel(BrickMoneyProject::Inst()->getForSaleModel())
{
    LegSetTableView::init();

    mToSold = addPushButton(tr("Move to Sold"));
    mToSold->setVisible(mModel->selectionIsDirty());
    connect(mToSold, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getSoldModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getSoldModel());
        emit legoSetsMovedToSold();
    });

    mToInStock = addPushButton(tr("Move to In Stock"));
    mToInStock->setVisible(mModel->selectionIsDirty());
    connect(mToInStock, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getInStockModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getInStockModel());
        emit legoSetsMovedToInStock();
    });
}

ForSale::~ForSale()
{
}

LegoSetSortFilterTableModel *ForSale::getSortModel() const
{
    return mSortModel;
}

LegoSetTableModel *ForSale::getModel() const
{
    return mModel;
}

QString ForSale::title() const
{
    return tr("For Sale");
}

void ForSale::selectionIsDirty(bool isDirty)
{
    mToInStock->setVisible(isDirty);
    mToSold->setVisible(isDirty);
}



uint ForSale::getVisibilityFlags() const
{
    return BrickMoneySettings::Inst()->forSaleFlags();
}

void ForSale::setVisibilityFlags(uint flags)
{
    BrickMoneySettings::Inst()->setForSaleFlags(flags);
}
