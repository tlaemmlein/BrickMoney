#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.LegoSetInfoGenerator")

#include "LegoSetInfoGenerator.h"
#include "BrickMoneyDatabase.h"
#include "BrickMoneyImages.h"

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
			auto pixmaps = BrickMoneyDatabase::queryLegoSetImages(legoset_id);
			if (!pixmaps.isEmpty())
				BrickMoneyImages::Inst()->setImage(image_key, pixmaps.at(0));
		}
	}
};

LegoSetInfoGenerator::LegoSetInfoGenerator(QObject *parent) : QObject(parent), d_ptr(new LegoSetInfoGeneratorPrivate(this))
{
    LOG_SCOPE_METHOD(L"");

	if (!BrickMoneyDatabase::prepareBrickMoneyDBLocale(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
		return;

	static bool mTryToUpdateBrickMoneyDBLocale = true;

	if (mTryToUpdateBrickMoneyDBLocale)
		BrickMoneyDatabase::updateBrickMoneyDBLocale();

	mTryToUpdateBrickMoneyDBLocale = false;
}

bool LegoSetInfoGenerator::querySetNumber(int num)
{
    LOG_SCOPE_METHOD(L"");
    LOG_TRACE(QString("querySetNumber %1").arg(num).toStdWString());

	auto info = BrickMoneyDatabase::queryLegoSetInfo(num);

	if (!info.set_id.has_value())
	{
		emit setNumberNotFound();
		return false;
	}
	const auto set_id = info.set_id.value();
	emit setNumber(set_id);
	d_ptr->updateLegoSetImages(set_id);
	emit imageKey(QString::number(set_id));
	emit description(info.name_en);
	emit year(info.year);
	emit recommendedRetailPrice(info.rr_price);
	return true;
}

int LegoSetInfoGenerator::nextSetNumber(int currentSetNumber)
{
    LOG_SCOPE_METHOD(L"");
    LOG_INFO(QString("nextSetNumber %1").arg(currentSetNumber).toStdWString());

	auto info = BrickMoneyDatabase::nextLegoSetInfo(currentSetNumber);

	if (!info.set_id.has_value())
	{
		emit setNumberNotFound();
		return currentSetNumber; // Remains at the current set number!
	}
	const auto set_id = info.set_id.value();
	emit setNumber(set_id);
	d_ptr->updateLegoSetImages(set_id);
	emit imageKey(QString::number(set_id));
	emit description(info.name_en);
	emit year(info.year);
	emit recommendedRetailPrice(info.rr_price);
	return set_id;
}

int LegoSetInfoGenerator::previousSetNumber(int currentSetNumber)
{
    LOG_SCOPE_METHOD(L"");
    LOG_INFO(QString("previousSetNumber %1").arg(currentSetNumber).toStdWString());

	auto info = BrickMoneyDatabase::previousLegoSetInfo(currentSetNumber);

	if (!info.set_id.has_value())
	{
		emit setNumberNotFound();
		return currentSetNumber; // Remains at the current set number!
	}
	const auto set_id = info.set_id.value();
	emit setNumber(set_id);
	d_ptr->updateLegoSetImages(set_id);
	emit imageKey(QString::number(set_id));
	emit description(info.name_en);
	emit year(info.year);
	emit recommendedRetailPrice(info.rr_price);
	return set_id;
}

