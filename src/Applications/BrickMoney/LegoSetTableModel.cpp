#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableModel")

#include "LegoSetTableModel.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QResource>
#include <QTextCodec>

LegoSetTableModel::LegoSetTableModel(QObject *parent) : QAbstractTableModel (parent)
{
    setObjectName("TableModel");
}

int LegoSetTableModel::rowCount(const QModelIndex &) const
{
    return mLegoSetTableData.size();
}

int LegoSetTableModel::columnCount(const QModelIndex &) const
{
    return LegoSetTableData::countForGui;
}

QVariant LegoSetTableModel::data(const QModelIndex &index, int role) const
{
	LOG_SCOPE_METHOD(L"");

    if (!index.isValid())
        return QVariant();

	LOG_DEBUG("index.row(): " << index.row() << " mLegoSetTableData.size(): " << mLegoSetTableData.size());

    if (index.row() >= mLegoSetTableData.size() || mLegoSetTableData.size() < 0)
        return QVariant();

	LOG_DEBUG("role: " << role);

    LegoSetTableData row = mLegoSetTableData.at(index.row());
    LOG_DEBUG("row.countForGui: " << row.countForGui);

    switch (role) {
    case ImageRole:
    {
        return QVariant(row.imageData.imageFilePath);
    }
    case SetNumberRole:
    {
        return QVariant(row.setnumber);
    }
    case DescriptionRole:
    {
        return QVariant(row.description);
    }
    case YearRole:
    {
        return QVariant(row.year);
    }
    case RrpRole:
    {
        return QVariant(row.rrp);
    }
    case PurchasingPriceRole:
    {
        return QVariant(row.purchasingPrice);
    }
    case CheaperPercentRole:
    {
        return QVariant(row.cheaperPercent);
    }
    default:
        break;
    }

    return QVariant();
}

QVariant LegoSetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	LOG_SCOPE_METHOD(L"");

    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Bild");

        case 1:
            return tr("Set Nummer");

        case 2:
            return tr("Beschreibung");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool LegoSetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	LOG_SCOPE_METHOD(L"");

	LOG_DEBUG("role: " << role << " r: " << index.row() << " c: " << index.column());
    if (!index.isValid() )
        return false;

    if ( index.row() < 0 || index.row() >= mLegoSetTableData.size() ||
        index.column() < 0 || index.column() >= LegoSetTableData::countForGui)
        return false;

    switch (role) {
    case ImageRole:
    {
        return false;
    }
    case SetNumberRole:
    {
        mLegoSetTableData[index.row()].setnumber = value.toInt();
        break;
    }
    case DescriptionRole:
    {
        mLegoSetTableData[index.row()].description = value.toString();
        break;
    }
    case YearRole:
    {
        mLegoSetTableData[index.row()].year = value.toInt();
        break;
    }
	case RrpRole:
	{
		mLegoSetTableData[index.row()].rrp = value.toDouble();
        mLegoSetTableData[index.row()].cheaperPercent =
            calcCheaperPercent(mLegoSetTableData[index.row()].rrp, mLegoSetTableData[index.row()].purchasingPrice);
        break;
    }
    case PurchasingPriceRole:
    {
        mLegoSetTableData[index.row()].purchasingPrice = value.toDouble();
        mLegoSetTableData[index.row()].cheaperPercent =
            calcCheaperPercent(mLegoSetTableData[index.row()].rrp, mLegoSetTableData[index.row()].purchasingPrice);
        break;
    }
    case CheaperPercentRole:
    {
        mLegoSetTableData[index.row()].cheaperPercent = value.toDouble();
        break;
    }
    default:
        break;
    }

    emit dataChanged(index,index);

    return true;
}

void LegoSetTableModel::newEntry()
{
	LOG_SCOPE_METHOD(L"");

    if (insertRow(rowCount()))
    {
		LOG_DEBUG("Row inserted");
    }
}

void LegoSetTableModel::deleteEntry(int rowIndex)
{
	LOG_SCOPE_METHOD(L"");

    if (removeRow(rowIndex))
    {
		LOG_DEBUG("Row " << rowIndex << " deleted.");
    }
}

void LegoSetTableModel::clearAll()
{
	LOG_SCOPE_METHOD(L"");

    beginResetModel();
    mLegoSetTableData.clear();
    endResetModel();
}

double LegoSetTableModel::calcCheaperPercent(double rrp, double purchasingPrice)
{
  return (purchasingPrice == 0.0) ? 0.0 : ( (1.0 - purchasingPrice/rrp) * 100.0);
}


bool LegoSetTableModel::insertRows(int row, int count, const QModelIndex &)
{
	LOG_SCOPE_METHOD(L"");
	LOG_DEBUG("count: " << count);
	LOG_DEBUG("row: " << row);
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
		mLegoSetTableData.append(LegoSetTableData({ "Empty.svg", "qrc:/images/Empty.svg" }, mLegoSetTableData.size(),
                                                  QString("Beschreibung %1").arg(mLegoSetTableData.size())
                                                  ,2018, 10.0, 5.0, 100.0));
		LOG_DEBUG("row +i: " << row +i);
    }

    endInsertRows();
    return true;
}

bool LegoSetTableModel::removeRows(int row, int count, const QModelIndex &)
{
	LOG_SCOPE_METHOD(L"");
	LOG_DEBUG("count: " << count);
	LOG_DEBUG("row: " << row);

    if ( row >= mLegoSetTableData.size())
    {
        return false;
    }

    if ( count > 1)
    {
		LOG_ERROR("Sorry. Only one row can be removed.");
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    mLegoSetTableData.removeAt(row);

    endRemoveRows();
    return true;
}


QHash<int, QByteArray> LegoSetTableModel::roleNames() const
{
	LOG_SCOPE_METHOD(L"");
	QHash<int, QByteArray> roles;
    roles[ImageRole] = "image";
    roles[SetNumberRole] = "setnumber";
    roles[DescriptionRole] = "description";
	roles[YearRole] = "year";
	roles[RrpRole] = "rrp";
    roles[PurchasingPriceRole] = "purchasePrice";
    roles[CheaperPercentRole] = "cheaperPercent";
	return roles;
}

int LegoSetTableModel::roleID(QString roleName)
{
	LOG_SCOPE_METHOD(L"");

    if (roleName == "image" )
    {
        return ImageRole;
    }

    if (roleName == "setnumber") {
        return SetNumberRole;
    }

    if ( roleName =="description" )
    {
        return DescriptionRole;
    }

    if ( roleName =="year" )
    {
        return YearRole;
    }

	if (roleName == "rrp")
	{
		return RrpRole;
	}

    if (roleName == "purchasePrice")
    {
        return PurchasingPriceRole;
    }

    if ( roleName == "cheaperPercent")
    {
        return CheaperPercentRole;
    }

    return -1;
}

void LegoSetTableModel::saveDataTo(const QChar &sep, QTextStream &out, const QString& projectFolder) const
{
	LOG_SCOPE_METHOD(L"");

	QString imageFolderString = projectFolder + "/images";

	if (!QDir(imageFolderString).exists())
	{
		if (!QDir().mkdir(imageFolderString))
		{
			LOG_ERROR("Could not create " << imageFolderString.toStdWString());
			return;
		}
	}
	
	QString imageFilePathString = imageFolderString + "/Empty.svg";
	if (!QFileInfo::exists(imageFilePathString))
	{
		QFile emptySvgFile(":/images/Empty.svg");

		if (!emptySvgFile.copy(imageFilePathString))
		{
			LOG_ERROR("Could not copy from " << emptySvgFile.fileName().toStdWString()
				<< " to " << imageFilePathString.toStdWString() );
		}
	}

	for(const auto & entry : mLegoSetTableData)
    {
        out << entry.imageData.imageName << sep << entry.setnumber
            << sep << entry.description << sep << entry.year << sep << entry.rrp
            << sep << entry.purchasingPrice << "\n";
    }
}

void LegoSetTableModel::loadDataFrom(const QChar &sep, QTextStream &in, const QString &projectFolder)
{
    LOG_SCOPE_METHOD(L"");
    beginResetModel();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList row = line.split(sep);
        if (row.size() != LegoSetTableData::countForIO)
		{
			LOG_WARN("The number of the imported columns differs from the expected columns. Actual: "
                     << row.size() << " Expexted: "<< LegoSetTableData::countForIO << ". Read the next line.");
  		   continue;
		}

        QString imageName = row.at(0);
		QUrl url = QUrl(imageName);
		QString localeFile = url.toLocalFile();
		QString nativeLocaleFile = QDir::toNativeSeparators(localeFile);
		if (QFileInfo::exists(nativeLocaleFile))
		{
			LOG_WARN("A complete file path to the image is not expected. ImageFilePath: " << imageName.toStdWString()
				<< "Read the next line.");
			continue;
		}

		QString imageFilePathString = projectFolder + "/images/" + imageName;
		if (!QFileInfo::exists(imageFilePathString))
		{
			LOG_DEBUG(imageFilePathString.toStdWString() << " doesn't exists. Read the next line.");
			continue;
		}
		url = QUrl::fromLocalFile(QFileInfo(imageFilePathString).canonicalFilePath());

        LOG_TRACE(row.at(2).toStdWString());

        const double rrp = row.at(4).toDouble();
        const double pPrice = row.at(5).toDouble();
        const double cheaperPercent = calcCheaperPercent(rrp, pPrice);

        mLegoSetTableData.append(LegoSetTableData({ imageName, url.toString() }, row.at(1).toInt(), row.at(2),
                                                  row.at(3).toInt(), rrp, pPrice, cheaperPercent));
    }

    if ( 0 == mLegoSetTableData.size())
        LOG_ERROR("LegoSetTableData is empty. Nothing to load.");

    endResetModel();
}

