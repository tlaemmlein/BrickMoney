#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyProject")

#include "BrickMoneyProject.h"
#include "BrickMoneySettings.h"
#include "BrickMoneyDataManager.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QUrl>


std::unique_ptr<BrickMoneyProject> BrickMoneyProject::smInstance = nullptr;

const QString BrickMoneyProject::TitleKey{"BrickMoneyTitle"};
const QString BrickMoneyProject::TitleValue{"BrickMoney Project File"};
const QString BrickMoneyProject::ProjectVersionKey{"ProjectVersion"};
const int     BrickMoneyProject::ProjectVersionValue = 1;
const QString BrickMoneyProject::BrickMoneyLinkKey{"BrickMoneyLink"};
const QString BrickMoneyProject::BrickMoneyLinkValue{"https://brickmoney.de/"};
const QString BrickMoneyProject::InStockKey{"InStock"};
const QString BrickMoneyProject::ForSaleKey{"ForSale"};
const QString BrickMoneyProject::SoldKey{"Sold"};




BrickMoneyProject *BrickMoneyProject::Inst()
{
    if(!smInstance)
    {
        smInstance = std::unique_ptr<BrickMoneyProject>(new BrickMoneyProject());
    }
    return smInstance.get();
}

void BrickMoneyProject::loadProjectFromSettings()
{
	QString localeProjectFile = toLocalFile(BrickMoneySettings::Inst()->brickMoneyFilePath());

	QFileInfo info(toLocalFile(localeProjectFile));

	if (!info.exists()) {
		QString msg = tr("The project file doesn't exits: ") + localeProjectFile;
		throw ProjectLoadException(msg.toStdString().c_str());
	}

	QFile projectFile(localeProjectFile);

	if (!projectFile.open(QIODevice::ReadOnly)) {
		QString msg = tr("Could not open project file for reading: ") + localeProjectFile;
		throw ProjectLoadException(msg.toStdString().c_str());
	}

	QByteArray jsonData = projectFile.readAll();

	QJsonDocument projectDoc(QJsonDocument::fromJson(jsonData));

	const QJsonObject &jsonProject = projectDoc.object();

	if (!jsonProject.contains(TitleKey) || !jsonProject[TitleKey].isString() ) {
		QString msg = tr("This it not a valid project file because of missing or invalid title: ") + localeProjectFile;
		throw ProjectLoadException(msg.toStdString().c_str());
	}

	auto actualTitle = jsonProject[TitleKey].toString();

	if (actualTitle != TitleValue)	{
		QString msg = tr("This it not a valid project file because of title mismatch: ") + localeProjectFile;
		msg += tr("\nExpected: '") + TitleValue + tr("' Actual: '") + actualTitle +"'";
		throw ProjectLoadException(msg.toStdString().c_str());
	}

	if (!jsonProject.contains(ProjectVersionKey)) {
		QString msg = tr("This it not a valid project file because of missing project version: ") + localeProjectFile;
		throw ProjectLoadException(msg.toStdString().c_str());
	}

	auto actualVersion = jsonProject[ProjectVersionKey].toInt();

	if (actualVersion != ProjectVersionValue) {
		QString msg = tr("This it not a valid project file because of project version mismatch: ") + localeProjectFile;
		msg += tr("\nExpected: ") + QString::number(ProjectVersionValue) + tr(" Actual: ") + QString::number(actualVersion);
		throw ProjectLoadException(msg.toStdString().c_str());
	}

	if (jsonProject.contains(InStockKey) && jsonProject[InStockKey].isArray()) {
		m_InStockModel.clearAll();
		m_InStockModel.dataSource()->read(jsonProject[InStockKey].toArray());
	}

	if (jsonProject.contains(ForSaleKey) && jsonProject[ForSaleKey].isArray())	{
		m_ForSaleModel.clearAll();
		m_ForSaleModel.dataSource()->read(jsonProject[ForSaleKey].toArray());
	}

	if (jsonProject.contains(SoldKey) && jsonProject[SoldKey].isArray()) {
		m_SoldModel.clearAll();
		m_SoldModel.dataSource()->read(jsonProject[SoldKey].toArray());
	}
}

void BrickMoneyProject::saveProjectFromSettings()
{
	const QString localeFile = toLocalFile(BrickMoneySettings::Inst()->brickMoneyFilePath());
	QFile projectFile(localeFile);

	if (!projectFile.open(QIODevice::WriteOnly)) {
		QString msg = tr("Could not open project file for writing: ") + localeFile;
		LOG_ERROR(msg.toStdWString());
		return;
	}

	QJsonObject projectJObject;

    projectJObject[TitleKey] = TitleValue;
    projectJObject[BrickMoneyLinkKey] = BrickMoneyLinkValue;
    projectJObject[ProjectVersionKey] = ProjectVersionValue;

    QJsonArray inStock;
    m_InStockModel.dataSource()->write(inStock);
	projectJObject[InStockKey] = inStock;

	QJsonArray forSale;
    m_ForSaleModel.dataSource()->write(forSale);
	projectJObject[ForSaleKey] = forSale;

	QJsonArray sold;
    m_SoldModel.dataSource()->write(sold);
	projectJObject[SoldKey] = sold;

	QJsonDocument projectDoc(projectJObject);
	projectFile.write(projectDoc.toJson());
}

BrickMoneyProject::BrickMoneyProject()
{
    m_InStockModel.setDataSource(new LegoSetDataSource(this));
    m_InStockSortModel = new LegoSetSortFilterTableModel(&m_InStockModel);

    m_ForSaleModel.setDataSource(new LegoSetDataSource(this));
    m_ForSaleSortModel = new LegoSetSortFilterTableModel(&m_ForSaleModel);

    m_SoldModel.setDataSource(new LegoSetDataSource(this));
    m_SoldSortModel = new LegoSetSortFilterTableModel(&m_SoldModel);

    m_ImportModel.setDataSource(new LegoSetDataSource(this));
    m_ImportSortModel = new LegoSetSortFilterTableModel(&m_ImportModel);

	const QString temp_loc = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	m_tempBrickMoneyFilePath = temp_loc + "/TempBrickMoney.json";

    //connect(BrickMoneySettings::Inst(), &BrickMoneySettings::brickMoneyFilePathChanged, this, &BrickMoneyProject::reloadProject);
}


QString BrickMoneyProject::temporaryProjectFilePath() const
{
	return m_tempBrickMoneyFilePath;
}



bool BrickMoneyProject::moveSelectedLegoSets(LegoSetTableModel *from, LegoSetTableModel *to)
{
    if (from == to)
    {
        LOG_ERROR("It makes no sense to move the selected LegoSets to the self model");
        return false;
    }

    LegoSetDataSource* fromDS = from->dataSource();
    LegoSetDataSource* toDS = to->dataSource();

    for (auto set : fromDS->getSelectedLegoSets())
    {
        LegoSet* clone = new LegoSet();
        *clone = *set;
        toDS->addLegoSet(clone);
    }

	fromDS->removeSelectedLegoSets();

    return true;
}

bool BrickMoneyProject::copySelectedLegoSets(LegoSetTableModel *from, LegoSetTableModel *to)
{
    LegoSetDataSource* fromDS = from->dataSource();
    LegoSetDataSource* toDS = to->dataSource();

    for (auto set : fromDS->getSelectedLegoSets())
    {
        LegoSet* clone = new LegoSet();
        *clone = *set;
        toDS->addLegoSet(clone);
        set->setIsSelected(false);
    }

    return true;
}

bool BrickMoneyProject::loadDataFrom(const QString &pathToCsvFile)
{
    const QString local = toLocalFile(pathToCsvFile);
    QFile cvsData(local);

    if ( !cvsData.open(QFile::ReadOnly | QIODevice::Text))
    {
        LOG_ERROR("Could not read from " << local.toStdWString());
        return false;
    }

    QTextStream input(&cvsData);
    input.setCodec("UTF-8");

    m_ImportModel.clearAll();

    m_ImportModel.dataSource()->loadDataFrom(';', input);

    return m_ImportModel.numberOfLegoSets() > 0;
}


LegoSetTableModel * BrickMoneyProject::getInStockModel()
{
    return &m_InStockModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getInStockSortModel()
{
    return m_InStockSortModel;
}


LegoSetTableModel * BrickMoneyProject::getForSaleModel()
{
    return &m_ForSaleModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getForSaleSortModel()
{
    return m_ForSaleSortModel;
}


LegoSetTableModel * BrickMoneyProject::getSoldModel()
{
    return &m_SoldModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getSoldSortModel()
{
    return m_SoldSortModel;
}

LegoSetTableModel *BrickMoneyProject::getImportModel()
{
    return &m_ImportModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getImportSortModel()
{
    return m_ImportSortModel;
}

QString BrickMoneyProject::toLocalFile(const QString &fileUrl)
{
	QFileInfo info(fileUrl);

	if (info.exists())
	{
		return fileUrl;
	}

	QUrl url(fileUrl);

	return url.toLocalFile();
}


