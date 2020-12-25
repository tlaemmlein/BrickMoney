#ifndef LEGOSETINFOGENERATOR_H
#define LEGOSETINFOGENERATOR_H

#include <QObject>
#include <QString>


class LegoSetInfoGeneratorPrivate;

class LegoSetInfoGenerator : public QObject
{
    Q_OBJECT

public:
    explicit LegoSetInfoGenerator(QObject *parent = nullptr);

    Q_INVOKABLE void querySetNumber(int num);
    Q_INVOKABLE int nextSetNumber(int currentSetNumber);
    Q_INVOKABLE int previousSetNumber(int currentSetNumber);

signals:
    void setNumber(int setNumber);
    void imageKey(QString imageKey);
    void description(QString description);
    void year(int year);
    void recommendedRetailPrice(double recommendedRetailPrice);

private:
	LegoSetInfoGeneratorPrivate* d_ptr;
};


#endif // LEGOSETINFOGENERATOR_H
