#ifndef LEGOSET_TABLE_MODEL_H
#define LEGOSET_TABLE_MODEL_H

#include <QAbstractTableModel>

#include <QLinkedList>
#include <QTextStream>

class LegoSetTableModel : public QAbstractTableModel
{
    Q_OBJECT

    enum TableRoles{
        ImageRole = Qt::UserRole +1,
        SetNumberRole,
        DescriptionRole,
        YearRole
    };


public:
    explicit LegoSetTableModel(QObject *parent= nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // QAbstractItemModel interface
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int roleID(QString roleName);

    void saveDataTo(const QChar& separator, QTextStream& out, const QString& projectFolder) const;

    void loadDataFrom(const QChar& separator, QTextStream& in, const QString& projectFolder);

public slots:

    void newEntry();
    void deleteEntry(int rowIndex);
    void clearAll();

private:
	struct ImageData
	{
		ImageData(QString const& new_imageName, QString const& new_imageFilePath ) : imageName(new_imageName), imageFilePath(new_imageFilePath)
		{}

		QString imageName; // ImageName + Extension
		QString imageFilePath; // Full file path
	};

    struct LegoSetTableData
    {
        LegoSetTableData(ImageData new_imageData, int new_setnumber, QString new_description, int new_year): imageData(new_imageData),
            setnumber(new_setnumber), description(new_description), year(new_year)
        {}

		ImageData imageData;
        int setnumber;
        QString description;
        int year;

        static const int count = 4;
    };


    QList< LegoSetTableData > mLegoSetTableData;
};


#endif

