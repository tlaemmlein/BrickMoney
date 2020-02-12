#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableData")

#include "LegoSetTableData.h"

#include <QDir>
#include <QUrl>


static double calcCheaperPercent(double rrp, double purchasingPrice)
{
   return (purchasingPrice == 0.0) ? 0.0 : ( (1.0 - purchasingPrice/rrp) * 100.0);
}


LegoSetRecord::LegoSetRecord(QString new_imageName, QString new_imageFilePath, int new_setnumber,
                                               QString new_description, int new_year, double new_rrp, double new_purchasing_price):
    imageName(new_imageName),imageFilePath(new_imageFilePath), setnumber(new_setnumber), description(new_description)
    , year(new_year),rrp(new_rrp), purchasingPrice(new_purchasing_price), cheaperPercent( calcCheaperPercent(rrp, purchasingPrice) )
{

}


LegoSetTableData::LegoSetTableData()
{
    mRoles[ImageRole] = "image";
    mRoles[SetNumberRole] = "setnumber";
    mRoles[DescriptionRole] = "description";
    mRoles[YearRole] = "year";
    mRoles[RrpRole] = "rrp";
    mRoles[PurchasingPriceRole] = "purchasePrice";
    mRoles[CheaperPercentRole] = "cheaperPercent";
}

int LegoSetTableData::rowCount() const
{
    return mData.size();
}

int LegoSetTableData::columnCount() const
{
    return LegoSetRecord::columnCountForGui;
}

QVariant LegoSetTableData::getData(int row, int role) const
{
    LOG_DEBUG("row: " << row << " role: " << role);

    if (row >= rowCount() || rowCount() < 0)
        return {};

    LegoSetRecord record = mData.at(row);

    switch (role) {
    case ImageRole:
    {
        return QVariant(record.imageFilePath);
    }
    case SetNumberRole:
    {
        return QVariant(record.setnumber);
    }
    case DescriptionRole:
    {
        return QVariant(record.description);
    }
    case YearRole:
    {
        return QVariant(record.year);
    }
    case RrpRole:
    {
        return QVariant(record.rrp);
    }
    case PurchasingPriceRole:
    {
        return QVariant(record.purchasingPrice);
    }
    case CheaperPercentRole:
    {
        return QVariant(record.cheaperPercent);
    }
    default:
        break;
    }

    return QVariant();
}

bool LegoSetTableData::setData(int row, const QVariant &value, int role)
{
    switch (role) {
    case ImageRole:
    {
        return false;
    }
    case SetNumberRole:
    {
        mData[row].setnumber = value.toInt();
        break;
    }
    case DescriptionRole:
    {
        mData[row].description = value.toString();
        break;
    }
    case YearRole:
    {
        mData[row].year = value.toInt();
        break;
    }
    case RrpRole:
    {
        mData[row].rrp = value.toDouble();
        mData[row].cheaperPercent =
            calcCheaperPercent(mData[row].rrp, mData[row].purchasingPrice);
        break;
    }
    case PurchasingPriceRole:
    {
        mData[row].purchasingPrice = value.toDouble();
        mData[row].cheaperPercent =
            calcCheaperPercent(mData[row].rrp, mData[row].purchasingPrice);
        break;
    }
    case CheaperPercentRole:
    {
        mData[row].cheaperPercent = value.toDouble();
        break;
    }
    default:
        break;
    }

    return true;
}

void LegoSetTableData::clear()
{
    mData.clear();
}

void LegoSetTableData::append(const LegoSetRecord &record)
{
    mData.append(record);
}

void LegoSetTableData::removeAt(int row)
{
    mData.removeAt(row);
}

QHash<int, QByteArray> LegoSetTableData::roleNames() const
{
    return mRoles;
}

int LegoSetTableData::roleID(const QString &roleName)
{
    return mRoles.key(roleName.toUtf8());
}

void LegoSetTableData::saveDataTo(const QChar &separator, QTextStream &out, const QString &projectFolder) const
{
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

    for(const auto & entry : mData)
    {
        out << entry.imageName << separator << entry.setnumber
            << separator << entry.description << separator << entry.year << separator << entry.rrp
            << separator << entry.purchasingPrice << "\n";
    }
}

void LegoSetTableData::loadDataFrom(const QChar &separator, QTextStream &in, const QString &projectFolder)
{
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList columns = line.split(separator);
        if (columns.size() != LegoSetRecord::columnCountForInport)
        {
            LOG_WARN("The number of the imported columns differs from the expected columns. Actual: "
                     << columns.size() << " Expexted: "<< LegoSetRecord::columnCountForInport << ". Read the next line.");
            continue;
        }

        QString imageName = columns.at(0);
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

        const double rrp = columns.at(4).toDouble();
        const double pPrice = columns.at(5).toDouble();

        mData.append(LegoSetRecord(imageName, url.toString(), columns.at(1).toInt(), columns.at(2),
                                               columns.at(3).toInt(), rrp, pPrice));
    }
}
