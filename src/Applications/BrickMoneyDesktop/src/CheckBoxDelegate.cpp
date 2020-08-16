#include "CheckBoxDelegate.h"

#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QPainter>
#include <QEvent>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if (index.data(int(LegoSetTableModel::Role::Type)).toString() == QLatin1String("selection")) {
        drawCheck(painter, option, option.rect, index.data().toBool() ? Qt::Checked : Qt::Unchecked);
        drawFocus(painter, option, option.rect);
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}


bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease){
        model->setData(index, !model->data(index).toBool());
        event->accept();
    }
    return QItemDelegate::editorEvent(event, model, option, index);

}
