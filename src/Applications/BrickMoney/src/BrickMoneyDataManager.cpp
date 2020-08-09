
#include "BrickMoneyDataManager.h"


std::unique_ptr<BrickMoneyDataManager> BrickMoneyDataManager::smInstance = nullptr;



BrickMoneyDataManager *BrickMoneyDataManager::Inst()
{
    if(!smInstance)
    {
        smInstance = std::unique_ptr<BrickMoneyDataManager>(new BrickMoneyDataManager());
    }
    return smInstance.get();
}

BrickMoneyDataManager::BrickMoneyDataManager() : m_brickMoneyIsDirty(false)
{
}


BrickMoneyDataManager::~BrickMoneyDataManager()
{
}


bool BrickMoneyDataManager::brickMoneyIsDirty() const
{
    return m_brickMoneyIsDirty;
}

void BrickMoneyDataManager::setBrickMoneyIsDirty(bool brickMoneyIsDirty)
{
    if (m_brickMoneyIsDirty == brickMoneyIsDirty)
        return;

    m_brickMoneyIsDirty = brickMoneyIsDirty;
    emit brickMoneyIsDirtyChanged(m_brickMoneyIsDirty);
}
