#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.ImageDelegate")

#include "ImageDelegate.h"

#include "Packages/BrickMoneyBackend/LegoSetTableModel.h"
#include "Packages/BrickMoneyBackend/BrickMoneyImages.h"

#include <QFileInfo>
#include <QPainter>

ImageDelegate::ImageDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if (index.data(int(LegoSetTableModel::Role::Type)).toString() == QLatin1String("image")) {
        QString imageKey = index.data().toString();

		QPixmap pm = BrickMoneyImages::Inst()->image(imageKey);

        //Scaled size that will be used to set draw aera to QPainter, with aspect ratio preserved
        QSize size = pm.size().scaled(option.rect.size(), Qt::KeepAspectRatio);

        //Draw the pixmap inside the scaled area, with aspect ratio preserved
        painter->drawPixmap(option.rect.x(), option.rect.y(), size.width(), size.height(), pm);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data(int(LegoSetTableModel::Role::Type)).toString() == QLatin1String("image")) {
        return QSize(100,100);
    }
    return QStyledItemDelegate::sizeHint(option, index);
}
