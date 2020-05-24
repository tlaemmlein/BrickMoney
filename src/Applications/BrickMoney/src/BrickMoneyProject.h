#ifndef BRICKMONEYPROJECT_H
#define BRICKMONEYPROJECT_H

#include "LegoSetDataSource.h"
#include "LegoSetTableModel.h"

#include <QObject>

class BrickMoneyProject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString brickMoneyFilePath READ brickMoneyFilePath WRITE setBrickMoneyFilePath NOTIFY brickMoneyFilePathChanged)

public:
    static BrickMoneyProject* Inst();
    BrickMoneyProject(const BrickMoneyProject&) = delete;            // No copy ctor
    BrickMoneyProject& operator=(const BrickMoneyProject&) = delete; //No assign op


    QString brickMoneyFilePath() const;
    Q_INVOKABLE bool checkBrickMoneyProject(const QString& brickMoneyFilePath);

	LegoSetDataSource* getDataSourceInStock() const;
	LegoSetTableModel* getInStockModel();
	LegoSetDataSource* getDataSourceForSale() const;
	LegoSetTableModel* getForSaleModel();
	LegoSetDataSource* getDataSourceSold() const;
	LegoSetTableModel* getSoldModel();

public slots:
    void load();
    void save();
    void setBrickMoneyFilePath(const QString& brickMoneyFilePath);

signals:
    void brickMoneyFilePathChanged(QString brickMoneyFilePath);

private:
    static std::unique_ptr<BrickMoneyProject> smInstance;
    BrickMoneyProject();
    static const QString BrickMoneyTitleKey;
    static const QString BrickMoneyTitleValue;
    static const QString BrickMoneyVersionKey;
    static const QString BrickMoneyVersionValue;
    static const QString BrickMoneyLinkKey;
    static const QString BrickMoneyLinkValue;
    static const QString BrickMoneyInStock;
    static const QString BrickMoneyForSale;
    static const QString BrickMoneySold;


    LegoSetDataSource* m_DataSourceInStock;
	LegoSetTableModel  m_InStockModel;
    LegoSetDataSource* m_DataSourceForSale;
	LegoSetTableModel  m_ForSaleModel;
    LegoSetDataSource* m_DataSourceSold;
	LegoSetTableModel  m_SoldModel;

    QString m_brickMoneyFilePath;

	QString toLocalFile(const QString& fileUrl);
};



#endif // BRICKMONEYPROJECT_H
