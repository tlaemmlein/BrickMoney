#ifndef LEGOSET_TABLE_DATA_H
#define LEGOSET_TABLE_DATA_H


#include <QString>
#include <QList>
#include <QVariant>
#include <QHash>
#include <QTextStream>

class LegoSetRecord;

class LegoSetTableData
{
public:
    LegoSetTableData();
	~LegoSetTableData();

    int rowCount() const;

    int columnCount() const;

    QVariant getData(int row, int role) const;

    bool setData(int row, const QVariant &value, int role);

    void clear();

    void append(LegoSetRecord* record);

    void removeAt(int row);

    QHash<int, QByteArray> roleNames() const;

    int roleID(const QString& roleName);

    void saveDataTo(const QChar& separator, QTextStream& out, const QString& projectFolder) const;

    void loadDataFrom(const QChar& separator, QTextStream& in, const QString& projectFolder);

private:
    QList< LegoSetRecord* > mData;
    QHash<int, QByteArray> mRoles;
	int mColumnCountForGui;
	int mColumnCountForIO;
};

#endif

