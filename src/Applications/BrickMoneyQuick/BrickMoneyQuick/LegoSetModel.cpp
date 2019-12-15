#include "LegoSetModel.h"

LegoSetModel::LegoSetModel(QObject *parent)
    : QStandardItemModel(parent)
{
    reset();
}

void LegoSetModel::reset()
{
    clear();
    addSet();
}

void LegoSetModel::addSet()
{
    QStandardItem *pSetNumberItem = new QStandardItem;
    pSetNumberItem->setData(0, Qt::EditRole);
    QList<QStandardItem*> legoset;
    legoset << pSetNumberItem;
    appendRow(legoset);
}

void LegoSetModel::initialize()
{
    setHorizontalHeaderLabels(QStringList() << tr("Set Nummer"));
}

void LegoSetModel::clear()
{
    QStandardItemModel::clear();
    initialize();
}
