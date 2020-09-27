#include "CalendarDelegate.h"

#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QPainter>
#include <QEvent>
#include <QCalendarWidget>

CalendarDelegate::CalendarDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *CalendarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new QCalendarWidget(parent);
}

void CalendarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto value = index.model()->data(index, int(LegoSetTableModel::Role::Sort)).toDate();
    auto c = static_cast<QCalendarWidget*>(editor);

    c->setSelectedDate(value);
}

void CalendarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto c = static_cast<QCalendarWidget*>(editor);
    //    lineEdit->returnPressed();
    QDate value = c->selectedDate();
    model->setData(index, value, int(LegoSetTableModel::Role::Sort));
}

void CalendarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //editor->setGeometry(option.rect);
    editor->move(option.rect.x(), editor->y());
}

