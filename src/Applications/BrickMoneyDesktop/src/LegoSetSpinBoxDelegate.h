#ifndef LegoSetSpinBoxDelegate_H
#define LegoSetSpinBoxDelegate_H

#include <QObject>

#include <QStyledItemDelegate>


class LegoSetSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    LegoSetSpinBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // BARGRAPHDELEGATE_H
