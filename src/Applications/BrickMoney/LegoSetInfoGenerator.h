#ifndef LEGOSETINFOGENERATOR_H
#define LEGOSETINFOGENERATOR_H

#include <QObject>
#include <QString>

class LegoSetInfoGenerator : public QObject
{
    Q_OBJECT
public:
    explicit LegoSetInfoGenerator(QObject *parent = nullptr);

public:
    void querySetNumber(int num);

signals:
    void setNumberNotFound();
    void imageName(QString imageName);
    void imageUrl(QString imageUrl);
    void description(QString description);
    void year(int year);
    void recommendedRetailPrice(double recommendedRetailPrice);
};


#endif // LEGOSETINFOGENERATOR_H
