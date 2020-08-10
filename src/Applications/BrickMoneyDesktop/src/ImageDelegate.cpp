#include "ImageDelegate.h"

//#include "../qtquick/processmodel.h"
#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QPainter>

ImageDelegate::ImageDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if (index.data(int(LegoSetTableModel::Role::Type)).toString() == QLatin1String("image")) {
        QString imageUrl = index.data().toString();

        QFileInfo info(imageUrl);
        QString local;

        if (info.exists())
        {
            local=imageUrl;
        }
        else
        {
            QUrl url(imageUrl);

            local=url.toLocalFile();
        }
        //qDebug() << "Paint the image " << imageUrl << " " << local;

        QPixmap image(local);
        //Scaled size that will be used to set draw aera to QPainter, with aspect ratio preserved
        QSize size = image.size().scaled(option.rect.size(), Qt::KeepAspectRatio);

        //Draw the pixmap inside the scaled area, with aspect ratio preserved
        painter->drawPixmap(option.rect.x(), option.rect.y(), size.width(), size.height(), image);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
