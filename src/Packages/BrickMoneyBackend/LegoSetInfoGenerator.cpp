#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.LegoSetInfoGenerator")

#include "LegoSetInfoGenerator.h"
#include "BrickMoneyDatabase.h"
#include "BrickMoneyImages.h"
#include "BrickMoneySettings.h"

#include <QPixmap>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QVariant>


class LegoSetInfoGeneratorPrivate : public QObject
{
public:
	explicit LegoSetInfoGeneratorPrivate(QObject* parent = nullptr) : QObject(parent) {}

	void updateLegoSetImages(int legoset_id)
	{
		QPixmap pm;
		QString image_key = QString::number(legoset_id);
		if (!BrickMoneyImages::Inst()->contains(image_key))
		{
			auto pixmaps = BrickMoneyDatabase::Inst()->queryLegoSetImages(legoset_id);
			if (!pixmaps.isEmpty())
				BrickMoneyImages::Inst()->setImage(image_key, pixmaps.at(0));
		}
	}
};

LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent), d_ptr(new LegoSetInfoGeneratorPrivate(this))
{
    LOG_SCOPE_METHOD(L"");
}

void LegoSetInfoGenerator::querySetNumber(int num)
{
    LOG_SCOPE_METHOD(L"");
    LOG_TRACE(QString("querySetNumber %1").arg(num).toStdWString());

	auto info = BrickMoneyDatabase::Inst()->queryLegoSetInfo(num);

	emit setNumber(info.set_id);
	d_ptr->updateLegoSetImages(info.set_id);
	emit imageKey(info.is_valid ? QString::number(info.set_id) : "Null");
	if ("de" == BrickMoneySettings::Inst()->language() )
		emit description(info.name_de);
	else
		emit description(info.name_en);
	emit year(info.year);
	emit recommendedRetailPrice(info.rr_price);
}

int LegoSetInfoGenerator::nextSetNumber(int currentSetNumber)
{
    LOG_SCOPE_METHOD(L"");
    LOG_INFO(QString("nextSetNumber %1").arg(currentSetNumber).toStdWString());

	auto info = BrickMoneyDatabase::Inst()->nextLegoSetInfo(currentSetNumber);

	if (!info.is_valid)
		return currentSetNumber; // Remains at the current set number!
	emit setNumber(info.set_id);
	d_ptr->updateLegoSetImages(info.set_id);
	emit imageKey(QString::number(info.set_id));
	emit description(info.name_en);
	emit year(info.year);
	emit recommendedRetailPrice(info.rr_price);
	return info.set_id;
}

int LegoSetInfoGenerator::previousSetNumber(int currentSetNumber)
{
    LOG_SCOPE_METHOD(L"");
    LOG_INFO(QString("previousSetNumber %1").arg(currentSetNumber).toStdWString());

	auto info = BrickMoneyDatabase::Inst()->previousLegoSetInfo(currentSetNumber);

	if (!info.is_valid)
		return currentSetNumber; // Remains at the current set number!
	emit setNumber(info.set_id);
	d_ptr->updateLegoSetImages(info.set_id);
	emit imageKey(QString::number(info.set_id));
	emit description(info.name_en);
	emit year(info.year);
	emit recommendedRetailPrice(info.rr_price);
	return info.set_id;
}

