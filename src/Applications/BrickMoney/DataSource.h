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
    Q_INVOKABLE int legoSetCount();
    Q_INVOKABLE LegoSet *legoSetAt(int index);
    Q_INVOKABLE void clearLegoSets();

    Q_INVOKABLE void saveLegoSets(const QString& fileUrlPath);

signals:
    void preLegoSetAdded();
    void postLegoSetAdded();
    void preLegoSetRemoved(int index);
    void postLegoSetRemoved();

private:
    QList<LegoSet*> m_legoSets;

    QString toLocalFile(const QString& fileUrl);
};



#endif // DATASOURCE_H
