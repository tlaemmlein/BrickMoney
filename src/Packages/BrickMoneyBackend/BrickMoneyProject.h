#ifndef BRICKMONEYPROJECT_H
#define BRICKMONEYPROJECT_H


#include "Packages/BrickMoneyBackend/LegoSetDataSource.h"
#include "Packages/BrickMoneyBackend/LegoSetTableModel.h"
#include "Packages/BrickMoneyBackend/LegoSetSortFilterTableModel.h"

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

    Q_INVOKABLE bool moveSelectedLegoSets(LegoSetTableModel* from, LegoSetTableModel* to);

    Q_INVOKABLE bool copySelectedLegoSets(LegoSetTableModel* from, LegoSetTableModel* to);

    Q_INVOKABLE bool loadDataFrom(const QString& pathToCsvFile);


    LegoSetTableModel* getInStockModel();
    LegoSetSortFilterTableModel* getInStockSortModel();

    LegoSetTableModel* getForSaleModel();
    LegoSetSortFilterTableModel* getForSaleSortModel();

    LegoSetTableModel* getSoldModel();
    LegoSetSortFilterTableModel* getSoldSortModel();

    LegoSetTableModel* getImportModel();
    LegoSetSortFilterTableModel* getImportSortModel();

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


    LegoSetTableModel  m_InStockModel;
    LegoSetSortFilterTableModel* m_InStockSortModel;

    LegoSetTableModel  m_ForSaleModel;
    LegoSetSortFilterTableModel* m_ForSaleSortModel;

    LegoSetTableModel  m_SoldModel;
    LegoSetSortFilterTableModel* m_SoldSortModel;

    LegoSetTableModel  m_ImportModel;
    LegoSetSortFilterTableModel* m_ImportSortModel;

    QString m_brickMoneyFilePath;

	QString toLocalFile(const QString& fileUrl);
};



#endif // BRICKMONEYPROJECT_H
