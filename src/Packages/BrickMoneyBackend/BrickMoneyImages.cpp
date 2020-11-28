#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyImages")

#include <QHash>

#include "BrickMoneyImages.h"

std::unique_ptr<BrickMoneyImages> BrickMoneyImages::smInstance = nullptr;

class BrickMoneyImagesPrivate {
public:

	QHash<QString, QPixmap> pixmapCache;
};


BrickMoneyImages *BrickMoneyImages::Inst()
{
    if(!smInstance)
    {
        smInstance = std::unique_ptr<BrickMoneyImages>(new BrickMoneyImages());
    }
    return smInstance.get();
}

BrickMoneyImages::BrickMoneyImages() : d_ptr( new BrickMoneyImagesPrivate)
{
}


BrickMoneyImages::~BrickMoneyImages()
{
}

bool BrickMoneyImages::contains(const QString & imageKey)
{
	return d_ptr->pixmapCache.contains(imageKey);
}

QPixmap BrickMoneyImages::image(const QString & imageKey)
{
	return d_ptr->pixmapCache.value(imageKey);
}

void BrickMoneyImages::setImage(const QString & imageKey, const QPixmap& image)
{
	d_ptr->pixmapCache[imageKey] = image;
}


