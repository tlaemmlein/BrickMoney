#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetTableData")

#include "LegoSetTableData.h"
#include "LegoSetTableData_p.h"
#include "LegoSetRecord.h"

#include <QDir>
#include <QUrl>

LegoSetTableData::LegoSetTableData(QObject *parent) : QObject(parent), d_ptr(new LegoSetTableDataPrivate(this))
{
    LegoSetRecordInternal testRecord;

    d_ptr->mColumnCountForGui = 0;
    d_ptr->mColumnCountForIO = 0;
    for( auto item : testRecord.mRecord)
    {
        if ( item->role() >= 0 )
        {
            ++d_ptr->mColumnCountForGui;
            d_ptr->mRoles[item->role()] = item->roleName();
        }

        if ( item->shouldBeIO())
        {
            ++d_ptr->mColumnCountForIO;
        }
    }
}

LegoSetTableData::~LegoSetTableData()
{
	clear();
}

int LegoSetTableData::rowCount() const
{
    return d_ptr->mData.size();
}

int LegoSetTableData::columnCount() const
{
    return d_ptr->mColumnCountForGui;
}

QVariant LegoSetTableData::getData(int row, int role) const
{
    LOG_DEBUG("row: " << row << " role: " << role);

    if (row >= rowCount() || rowCount() < 0)
        return QVariant();

    for (LegoSetItem* item : d_ptr->mData.at(row)->mRecord)
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
    for (auto item : d_ptr->mData.at(row)->mRecord)
    {
        if (item->role() == role )
        {
            bool ret = item->setData(value);
            d_ptr->mData.at(row)->check(item);
			return ret;
        }
    }

    return false;
}

void LegoSetTableData::clear()
{
    for (auto e : d_ptr->mData)
	{
		delete e;
		e = nullptr;
	}

    d_ptr->mData.clear();
}

void LegoSetTableData::append(const LegoSetRecord& record)
{
    d_ptr->mData.append(new LegoSetRecordInternal(record, this));
}

void LegoSetTableData::removeAt(int row)
{
    auto e= d_ptr->mData.value(row);
	delete e;
	e = nullptr;
    d_ptr->mData.removeAt(row);
}

QHash<int, QByteArray> LegoSetTableData::roleNames() const
{
    return d_ptr->mRoles;
}

int LegoSetTableData::roleID(const QString &roleName)
{
    return d_ptr->mRoles.key(roleName.toUtf8());
}

void LegoSetTableData::saveDataTo(const QChar &separator, QTextStream &out, const QString &projectFolder) const
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

    for(auto legoSetRecord : d_ptr->mData)
    {
        auto record = legoSetRecord->mRecord;
		LegoSetItemRecord recordForExport;
		for (int i = 0; i < record.size(); ++i)
		{
			auto item = record.at(i);
			if (item->shouldBeIO())
			{
				recordForExport.push_back(item);
			}
		}

		size_t sizeForExport = recordForExport.size();

        for(int i = 0; i < sizeForExport; ++i)
        {
			auto item = recordForExport.at(i);
            if (item->shouldBeIO())
            {
                if ( LegoSetInt* int_item = dynamic_cast<LegoSetInt*>(item) )
                    out << int_item->getValue();
                else if ( LegoSetDouble* double_item = dynamic_cast<LegoSetDouble*>(item) )
                    out << double_item->getValue();
                else if ( LegoSetString* string_item = dynamic_cast<LegoSetString*>(item) )
                    out << string_item->getValue();

				if (i < sizeForExport - 1)
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
        if (columns.size() != d_ptr->mColumnCountForIO)
        {
            LOG_WARN("The number of the imported columns differs from the expected columns. Actual: "
                     << columns.size() << " Expexted: "<< d_ptr->mColumnCountForIO << ". Read the next line.");
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

        LegoSetRecord record(imageName, url.toString(), columns.at(1).toInt(), columns.at(2),
                             columns.at(3).toInt(), rrp, pPrice,columns.at(6),columns.at(7));

        d_ptr->mData.append(new LegoSetRecordInternal(record, this));
    }
}

