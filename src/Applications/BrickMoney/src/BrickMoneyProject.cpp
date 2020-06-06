#include "Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyProject")

#include "BrickMoneyProject.h"
#include "BrickMoneySettings.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>


std::unique_ptr<BrickMoneyProject> BrickMoneyProject::smInstance = nullptr;

const QString BrickMoneyProject::BrickMoneyTitleKey{"BrickMoneyTitle"};
const QString BrickMoneyProject::BrickMoneyTitleValue{"BrickMoney Project File"};
const QString BrickMoneyProject::BrickMoneyVersionKey{"BrickMoneyVersion"};
const QString BrickMoneyProject::BrickMoneyVersionValue{"0.2"};
const QString BrickMoneyProject::BrickMoneyLinkKey{"BrickMoneyLink"};
const QString BrickMoneyProject::BrickMoneyLinkValue{"https://brickmoney.de/"};
const QString BrickMoneyProject::BrickMoneyInStock{"InStock"};
const QString BrickMoneyProject::BrickMoneyForSale{"ForSale"};
const QString BrickMoneyProject::BrickMoneySold{"Sold"};




BrickMoneyProject *BrickMoneyProject::Inst()
{
    if(!smInstance)
    {
        smInstance = std::unique_ptr<BrickMoneyProject>(new BrickMoneyProject());
    }
    return smInstance.get();
}

void BrickMoneyProject::load()
{
	QString localeProjectFile = toLocalFile(m_brickMoneyFilePath);

    if (!checkBrickMoneyProject(localeProjectFile) )
        return;

    QFile projectFile(localeProjectFile);

    if (!projectFile.open(QIODevice::ReadOnly)) {
        LOG_ERROR("Couldn't open project file for reading.");
        return;
    }

    QByteArray jsonData = projectFile.readAll();

    QJsonDocument projectDoc( QJsonDocument::fromJson(jsonData) );

    const QJsonObject &jsonProject = projectDoc.object();

    if (jsonProject.contains(BrickMoneyTitleKey) && jsonProject[BrickMoneyTitleKey].isString())
	{
        if (jsonProject[BrickMoneyTitleKey].toString() != BrickMoneyTitleValue)
		{
			LOG_ERROR(m_brickMoneyFilePath.toStdWString() << " is not a BrickMoney Project File.");
			return;
		}
	}
	
    if (jsonProject.contains(BrickMoneyVersionKey) && jsonProject[BrickMoneyVersionKey].isString())
	{
        QString version = jsonProject[BrickMoneyVersionKey].toString();
        if (version != BrickMoneyVersionValue)
		{
            LOG_ERROR("Version mismatch: Expected: " << BrickMoneyVersionValue.toStdWString()<< " Actual: " << version.toStdWString() );
			return;
		}
	}
	QFileInfo fileInfo(localeProjectFile);

	QString projectPath = fileInfo.absolutePath();

    if (jsonProject.contains(BrickMoneyInStock) && jsonProject[BrickMoneyInStock].isArray())
	{
		m_InStockModel.clearAll();
		m_DataSourceInStock->read(jsonProject[BrickMoneyInStock].toArray());
	}

    if (jsonProject.contains(BrickMoneyForSale) && jsonProject[BrickMoneyForSale].isArray())
	{
		m_ForSaleModel.clearAll();
		m_DataSourceForSale->read(jsonProject[BrickMoneyForSale].toArray());
	}

    if (jsonProject.contains(BrickMoneySold) && jsonProject[BrickMoneySold].isArray())
	{
		m_SoldModel.clearAll();
		m_DataSourceSold->read(jsonProject[BrickMoneySold].toArray());
	}
}

void BrickMoneyProject::save()
{
	QFile projectFile(toLocalFile(m_brickMoneyFilePath));

	if (!projectFile.open(QIODevice::WriteOnly)) {
		LOG_ERROR("Couldn't open project file for writing.");
		return;
	}

	QJsonObject projectJObject;

    projectJObject[BrickMoneyTitleKey] = BrickMoneyTitleValue;
    projectJObject[BrickMoneyLinkKey] = BrickMoneyLinkValue;
    projectJObject[BrickMoneyVersionKey] = BrickMoneyVersionValue;

	QJsonArray inStock;
	m_DataSourceInStock->write(inStock);
	projectJObject[BrickMoneyInStock] = inStock;

	QJsonArray forSale;
	m_DataSourceForSale->write(forSale);
	projectJObject[BrickMoneyForSale] = forSale;

	QJsonArray sold;
	m_DataSourceSold->write(sold);
	projectJObject[BrickMoneySold] = sold;

	QJsonDocument projectDoc(projectJObject);
	projectFile.write(projectDoc.toJson());
}

BrickMoneyProject::BrickMoneyProject()
{
    m_DataSourceInStock = new LegoSetDataSource(this);
	m_InStockModel.setDataSource(m_DataSourceInStock);
    m_InStockSortModel = new LegoSetSortFilterTableModel(&m_InStockModel);

    m_DataSourceForSale = new LegoSetDataSource(this);
	m_ForSaleModel.setDataSource(m_DataSourceForSale);
    m_ForSaleSortModel = new LegoSetSortFilterTableModel(&m_ForSaleModel);

    m_DataSourceSold = new LegoSetDataSource(this);
	m_SoldModel.setDataSource(m_DataSourceSold);
    m_SoldSortModel = new LegoSetSortFilterTableModel(&m_SoldModel);

    connect(BrickMoneySettings::Inst(), &BrickMoneySettings::brickMoneyFilePathChanged, this, &BrickMoneyProject::setBrickMoneyFilePath);
	setBrickMoneyFilePath(BrickMoneySettings::Inst()->brickMoneyFilePath());
}


QString BrickMoneyProject::brickMoneyFilePath() const
{
    return m_brickMoneyFilePath;
}

bool BrickMoneyProject::checkBrickMoneyProject(const QString& brickMoneyFilePath)
{
    QFileInfo info(toLocalFile(brickMoneyFilePath));

    if ( !info.exists() )
    {
        LOG_ERROR(brickMoneyFilePath.toStdWString() << " doesn't exits.");
        return false;
    }

    return true;
}

LegoSetDataSource * BrickMoneyProject::getDataSourceInStock() const
{
	return m_DataSourceInStock;
}

LegoSetTableModel * BrickMoneyProject::getInStockModel()
{
    return &m_InStockModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getInStockSortModel()
{
    return m_InStockSortModel;
}

LegoSetDataSource * BrickMoneyProject::getDataSourceForSale() const
{
	return m_DataSourceForSale;
}

LegoSetTableModel * BrickMoneyProject::getForSaleModel()
{
    return &m_ForSaleModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getForSaleSortModel()
{
    return m_ForSaleSortModel;
}

LegoSetDataSource * BrickMoneyProject::getDataSourceSold() const
{
	return m_DataSourceSold;
}

LegoSetTableModel * BrickMoneyProject::getSoldModel()
{
    return &m_SoldModel;
}

LegoSetSortFilterTableModel *BrickMoneyProject::getSoldSortModel()
{
    return m_SoldSortModel;
}

void BrickMoneyProject::setBrickMoneyFilePath(const QString& brickMoneyFilePath)
{
    if (m_brickMoneyFilePath == brickMoneyFilePath)
        return;

    m_brickMoneyFilePath = brickMoneyFilePath;
    emit brickMoneyFilePathChanged(m_brickMoneyFilePath);
}

QString BrickMoneyProject::toLocalFile(const QString &fileUrl)
{
	LOG_INFO("fileUrl " << fileUrl.toStdWString());
	QFileInfo info(fileUrl);

	if (info.exists())
	{
		LOG_INFO("exists");
		return fileUrl;
	}

	QUrl url(fileUrl);

	return url.toLocalFile();
}


