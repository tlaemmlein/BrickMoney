#ifndef LEGOSETINFOGENERATOR_H
#define LEGOSETINFOGENERATOR_H

#include <QObject>
#include <QString>

#include <vector>

struct LegoSetInfo{

    LegoSetInfo() : setNumber(-1), description(""), year(-1), recommendedRetailPrice(0.0)
    {}

    LegoSetInfo(int otherSetNumber, QString otherDescription, int otherYear, double  otherRecommendedRetailPrice
                ) : setNumber(otherSetNumber)
                    , description(otherDescription)
                    , year(otherYear)
                    , recommendedRetailPrice(otherRecommendedRetailPrice)
    {}

    bool operator==(const LegoSetInfo& other) const
    {
        return ( setNumber == other.setNumber && description == other.description
             && year == other.year && recommendedRetailPrice == other.recommendedRetailPrice);
    }

    bool operator!=(const LegoSetInfo& other) const
    {
        return ( !operator==(other) );
    }

    bool operator < (const LegoSetInfo &info) const {
        return setNumber < info.setNumber;
    }

    int     setNumber;
    QString description;
    int     year;
    double  recommendedRetailPrice;
};

Q_DECLARE_METATYPE(LegoSetInfo)

class LegoSetInfoGenerator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LegoSetInfo legoSetInfo READ legoSetInfo NOTIFY legoSetInfoChanged)

public:
    explicit LegoSetInfoGenerator(QObject *parent = nullptr);

public:
    Q_INVOKABLE bool querySetNumber(int num);
    Q_INVOKABLE int nextSetNumber(int currentSetNumber);
    Q_INVOKABLE int previousSetNumber(int currentSetNumber);

    LegoSetInfo legoSetInfo() const;

signals:
    void setNumberNotFound();
    void setNumber(int setNumber);
    void imageName(QString imageName);
    void imageUrl(QString imageUrl);
    void description(QString description);
    void year(int year);
    void recommendedRetailPrice(double recommendedRetailPrice);

    void legoSetInfoChanged(LegoSetInfo legoSetInfo);

private:
    void fillDatabase(const QString& legoSetDatabaseFilePath);
    void sendSignals(const LegoSetInfo& info);
    QString mLegoSetImages;
    static bool mIsDataBaseReady;
    static std::vector<LegoSetInfo> mLegoSetDatabase;
    LegoSetInfo m_legoSetInfo;
};


#endif // LEGOSETINFOGENERATOR_H