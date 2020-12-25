#ifndef BRICKMONEYPROJECT_H
#define BRICKMONEYPROJECT_H


#include "Packages/BrickMoneyBackend/LegoSetDataSource.h"
#include "Packages/BrickMoneyBackend/LegoSetTableModel.h"
#include "Packages/BrickMoneyBackend/LegoSetSortFilterTableModel.h"

#include <QObject>

class ProjectLoadException : public std::exception
{
public:
	explicit ProjectLoadException(const char* message) :
		msg_(message) {
	}

	explicit ProjectLoadException(const std::string& message) :
		msg_(message) {}

	virtual ~ProjectLoadException() noexcept {}

	virtual const char* what() const noexcept {
		return msg_.c_str();
	}

protected:
	std::string msg_;
};

class BrickMoneyProject : public QObject
{
    Q_OBJECT

public:
    static BrickMoneyProject* Inst();
    BrickMoneyProject(const BrickMoneyProject&) = delete;            // No copy ctor
    BrickMoneyProject& operator=(const BrickMoneyProject&) = delete; //No assign op

	/// Loads the project from BrickMoneySettings
	/// \exception ProjectLoadException is something is wrong with loading.
	void loadProjectFromSettings();

	/// Save the project from BrickMoneySettings
	void saveProjectFromSettings();

	QString temporaryProjectFilePath() const;

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


private:
    static std::unique_ptr<BrickMoneyProject> smInstance;
    BrickMoneyProject();
    static const QString TitleKey;
    static const QString TitleValue;
    static const QString ProjectVersionKey;
    static const int     ProjectVersionValue;
    static const QString BrickMoneyLinkKey;
    static const QString BrickMoneyLinkValue;
    static const QString InStockKey;
    static const QString ForSaleKey;
    static const QString SoldKey;


    LegoSetTableModel  m_InStockModel;
    LegoSetSortFilterTableModel* m_InStockSortModel;

    LegoSetTableModel  m_ForSaleModel;
    LegoSetSortFilterTableModel* m_ForSaleSortModel;

    LegoSetTableModel  m_SoldModel;
    LegoSetSortFilterTableModel* m_SoldSortModel;

    LegoSetTableModel  m_ImportModel;
    LegoSetSortFilterTableModel* m_ImportSortModel;

	QString m_tempBrickMoneyFilePath;

	QString toLocalFile(const QString& fileUrl);
};



#endif // BRICKMONEYPROJECT_H
