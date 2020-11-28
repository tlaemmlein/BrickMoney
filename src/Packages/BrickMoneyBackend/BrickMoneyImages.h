#ifndef BrickMoneyImages_H
#define BrickMoneyImages_H

#include <memory>
#include <QPixmap>


class BrickMoneyImagesPrivate;

class BrickMoneyImages
{
public:
    static BrickMoneyImages* Inst();
    ~BrickMoneyImages();
    BrickMoneyImages(const BrickMoneyImages&) = delete;            // No copy ctor
    BrickMoneyImages& operator=(const BrickMoneyImages&) = delete; //No assign op

	bool contains(const QString& imageKey);
	QPixmap image(const QString& imageKey);
	void setImage(const QString& imageKey, const QPixmap& image);


private:
    static std::unique_ptr<BrickMoneyImages> smInstance;
    BrickMoneyImages();
    
	std::unique_ptr<BrickMoneyImagesPrivate> d_ptr;

};

#endif // BrickMoneyImages_H
