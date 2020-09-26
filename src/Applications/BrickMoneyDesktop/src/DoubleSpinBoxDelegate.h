#ifndef DoubleSpinBoxDelegate_H
#define DoubleSpinBoxDelegate_H

#include <QObject>

#include <QStyledItemDelegate>


class DoubleSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DoubleSpinBoxDelegate(QObject *parent = 0);

//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // BARGRAPHDELEGATE_H
