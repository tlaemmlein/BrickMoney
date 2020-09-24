#ifndef CALENDARDELEGATE_H
#define CALENDARDELEGATE_H

#include <QObject>

#include <QItemDelegate>
#include <QCheckBox>
#include <QHBoxLayout>


class CalendarDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CalendarDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // BARGRAPHDELEGATE_H
