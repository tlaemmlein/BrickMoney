#include "InStock.h"

#include "Packages/BrickMoneyBackend/BrickMoneyProject.h"

#include <QPushButton>


InStock::InStock(QWidget *parent) : LegSetTableView(parent)
    , mSortModel(BrickMoneyProject::Inst()->getInStockSortModel())
    , mModel(BrickMoneyProject::Inst()->getInStockModel())
    , mTitle(tr("In Stock"))
{
    LegSetTableView::init();

    mFromInStockToForSalePushButton = addPushButton(tr("Move to For Sale"));

    mFromInStockToForSalePushButton->setVisible(mModel->selectionIsDirty());
    connect(mFromInStockToForSalePushButton, &QPushButton::clicked, [&]() {
        BrickMoneyProject::Inst()->getForSaleModel()->removeSelectedLegoSets();
        BrickMoneyProject::Inst()->moveSelectedLegoSets(mModel, BrickMoneyProject::Inst()->getForSaleModel());
        emit legoSetsMovedToForSale();
    });
}

InStock::~InStock()
{
}

LegoSetSortFilterTableModel *InStock::getSortModel() const
{
    return mSortModel;
}


LegoSetTableModel *InStock::getModel() const
{
    return mModel;
}

QString InStock::title() const
{
    return mTitle;
}


void InStock::selectionIsDirty(bool isDirty)
{
    mFromInStockToForSalePushButton->setVisible(isDirty);
}
