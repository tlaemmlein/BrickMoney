#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableData")

#include "LegoSetTableData.h"
#include "LegoSetRecord.h"

#include <QDir>
#include <QUrl>

LegoSetTableData::LegoSetTableData()
{
	LegoSetRecord test("Empty.svg", "qrc:/images/Empty.svg", 1,"Egal",2018, 10.0, 5.0);
	mColumnCountForGui = test.columnCountForGui();
	mColumnCountForIO = test.columnCountForIO();
    mRoles = test.roleNames();
}

LegoSetTableData::~LegoSetTableData()
{
	clear();
}

int LegoSetTableData::rowCount() const
{
    return mData.size();
}

int LegoSetTableData::columnCount() const
{
    return mColumnCountForGui;
}

QVariant LegoSetTableData::getData(int row, int role) const
{
    LOG_DEBUG("row: " << row << " role: " << role);

    if (row >= rowCount() || rowCount() < 0)
        return QVariant();

    for (LegoSetItem* item : mData.at(row)->record())
    {
        if (item->role() == role )
        {
            return item->getData();
        }
    }

    return QVariant();
}

bool LegoSetTableData::setData(int row, const QVariant &value, int role)
{
    for (auto item : mData.at(row)->record())
    {
        if (item->role() == role )
        {
			bool ret = item->setData(value);
			mData.at(row)->check(item);
			return ret;
        }
    }

    return false;
}

void LegoSetTableData::clear()
{
	for (auto e : mData)
	{
		delete e;
		e = nullptr;
	}

    mData.clear();
}

void LegoSetTableData::append(LegoSetRecord* record)
{
    mData.append(record);
}

void LegoSetTableData::removeAt(int row)
{
	auto e= mData.value(row);
	delete e;
	e = nullptr;
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

    for(auto legoSetRecord : mData)
    {
		auto record = legoSetRecord->record();
		auto size = record.size();
        for(int i = 0; i < size; ++i)
        {
			auto item = record.at(i);
            if (item->shouldBeIO())
            {
                if ( LegoSetInt* int_item = dynamic_cast<LegoSetInt*>(item) )
                    out << int_item->getValue();
                else if ( LegoSetDouble* double_item = dynamic_cast<LegoSetDouble*>(item) )
                    out << double_item->getValue();
                else if ( LegoSetString* string_item = dynamic_cast<LegoSetString*>(item) )
                    out << string_item->getValue();

				if (i < size - 2)
					out << separator;
            }
        }
        out << "\n";
    }
}

void LegoSetTableData::loadDataFrom(const QChar &separator, QTextStream &in, const QString &projectFolder)
{
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList columns = line.split(separator);
        if (columns.size() != mColumnCountForIO)
        {
            LOG_WARN("The number of the imported columns differs from the expected columns. Actual: "
                     << columns.size() << " Expexted: "<< mColumnCountForIO << ". Read the next line.");
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

        mData.append(new LegoSetRecord(imageName, url.toString(), columns.at(1).toInt(), columns.at(2),
                                               columns.at(3).toInt(), rrp, pPrice));
    }
}

