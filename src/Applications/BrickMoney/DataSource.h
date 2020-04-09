#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

class LegoSet;

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);

    Q_INVOKABLE void addLegoSet(LegoSet* set);
    Q_INVOKABLE void removeLegoSet(int index);

    int legoSetCount();

    LegoSet *legoSetAt(int index);

    void clearLegoSets();

signals:
    void preLegoSetAdded();
    void postLegoSetAdded();
    void preLegoSetRemoved(int index);
    void postLegoSetRemoved();

private:
    QList<LegoSet*> m_legoSets;

};

#endif // DATASOURCE_H
