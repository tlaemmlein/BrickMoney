#include "LegoSetModel.h"

LegoSetModel::LegoSetModel(QObject *parent)
    : QStandardItemModel(parent)
{
    reset();
}

void LegoSetModel::reset()
{
    clear();
    m_MaxSetID = 0;
    addSet();
}

void LegoSetModel::addSet()
{
    QStandardItem *pSetIdItem = new QStandardItem;
    pSetIdItem->setData(m_MaxSetID, Qt::EditRole);
    pSetIdItem->setEditable(false);
    pSetIdItem->setEnabled(false);
    m_MaxSetID++;
    QStandardItem *pSetNumberItem = new QStandardItem;
    pSetNumberItem->setData(0, Qt::EditRole);
    QList<QStandardItem*> cascade;
    cascade << pSetIdItem << pSetNumberItem;
    appendRow(cascade);

}

void LegoSetModel::initialize()
{
    setHorizontalHeaderLabels(QStringList() << tr("SetID") << tr("Set number"));
}

void LegoSetModel::clear()
{
    QStandardItemModel::clear();
    initialize();
}
