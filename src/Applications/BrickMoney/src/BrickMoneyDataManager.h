#ifndef BRICKMONEY_DATAMANAGER_H
#define BRICKMONEY_DATAMANAGER_H

#include <QObject>

class BrickMoneyDataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool brickMoneyIsDirty READ brickMoneyIsDirty WRITE setBrickMoneyIsDirty NOTIFY brickMoneyIsDirtyChanged)
public:
    static BrickMoneyDataManager* Inst();
    ~BrickMoneyDataManager();
    BrickMoneyDataManager(const BrickMoneyDataManager&) = delete;            // No copy ctor
    BrickMoneyDataManager& operator=(const BrickMoneyDataManager&) = delete; //No assign op

    bool brickMoneyIsDirty() const;

public slots:
    void setBrickMoneyIsDirty(bool brickMoneyIsDirty);

signals:
    void brickMoneyIsDirtyChanged(bool brickMoneyIsDirty);

private:
    static std::unique_ptr<BrickMoneyDataManager> smInstance;
    BrickMoneyDataManager();

    bool m_brickMoneyIsDirty;
};

#endif
