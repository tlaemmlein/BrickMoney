#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class ImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ImageDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // BARGRAPHDELEGATE_H
