#ifndef LEGOSET_TABLE_DATA_H
#define LEGOSET_TABLE_DATA_H


#include <QString>
#include <QList>
#include <QVariant>
#include <QHash>
#include <QTextStream>

struct LegoSetRecord
{
    LegoSetRecord(QString new_imageName, QString new_imageFilePath, int new_setnumber, QString new_description, int new_year
                  ,double new_rrp, double new_purchasing_price);

    QString imageName; // ImageName + Extension
    QString imageFilePath; // Full file path
    int setnumber;
    QString description;
    int year;
    double rrp; // recommended retail price in euros
    double purchasingPrice; //purchase price in euros
    double cheaperPercent;

    static const int columnCountForGui = 7;
    static const int columnCountForInport = 6;
};

class LegoSetTableData
{
public:
    LegoSetTableData();

    int rowCount() const;

    int columnCount() const;

    QVariant getData(int row, int role) const;

    bool setData(int row, const QVariant &value, int role);

    void clear();

    void append(const LegoSetRecord& record);

    void removeAt(int row);

    QHash<int, QByteArray> roleNames() const;

    int roleID(const QString& roleName);

    void saveDataTo(const QChar& separator, QTextStream& out, const QString& projectFolder) const;

    void loadDataFrom(const QChar& separator, QTextStream& in, const QString& projectFolder);

private:
    enum TableRoles{
        ImageRole = Qt::UserRole +1,
        SetNumberRole,
        DescriptionRole,
        YearRole,
        RrpRole,
        PurchasingPriceRole,
        CheaperPercentRole
    };

    QList< LegoSetRecord > mData;
    QHash<int, QByteArray> mRoles;
};

#endif

