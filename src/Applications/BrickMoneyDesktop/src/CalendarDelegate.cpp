#include "CalendarDelegate.h"

#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QPainter>
#include <QEvent>
#include <QCalendarWidget>

CalendarDelegate::CalendarDelegate(QObject *parent) : QItemDelegate(parent)
{
    qDebug() << __FUNCTION__;
}

QWidget *CalendarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;
    return new QCalendarWidget(parent);
}

void CalendarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;
    auto value = index.model()->data(index, int(LegoSetTableModel::Role::Sort)).toDate();
    auto c = static_cast<QCalendarWidget*>(editor);

    c->setSelectedDate(value);
}

void CalendarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;
    auto c = static_cast<QCalendarWidget*>(editor);
    //    lineEdit->returnPressed();
    QDate value = c->selectedDate();
    model->setData(index, value, int(LegoSetTableModel::Role::Sort));
}

void CalendarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;

    //editor->setGeometry(option.rect);
    editor->move(option.rect.x(), editor->y());
}

