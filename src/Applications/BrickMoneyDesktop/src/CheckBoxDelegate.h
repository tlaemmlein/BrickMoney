#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QObject>

#include <QItemDelegate>
#include <QCheckBox>
#include <QHBoxLayout>


class CheckBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CheckBoxDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // BARGRAPHDELEGATE_H
