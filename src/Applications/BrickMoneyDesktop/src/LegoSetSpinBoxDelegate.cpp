#include "LegoSetSpinBoxDelegate.h"

#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QSpinBox>
#include <QPainter>
#include <QEvent>
#include <QApplication>
#include <QSignalMapper>

LegoSetSpinBoxDelegate::LegoSetSpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *LegoSetSpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    auto sb = new QSpinBox(parent);
    sb->setMinimum(0);
    sb->setMaximum(2147483647);

    QSignalMapper * mapper = new QSignalMapper(sb);
    mapper->setMapping(sb, sb);
    QObject::connect(sb, SIGNAL(valueChanged(int)), mapper, SLOT(map()));
    QObject::connect(mapper, SIGNAL(mapped(QWidget *)), this, SIGNAL(commitData(QWidget *)));

    return sb;
}

void LegoSetSpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    auto value = index.model()->data(index, int(LegoSetTableModel::Role::Sort)).toInt();

    auto e = static_cast<QSpinBox*>(editor);
    //e->setButtonSymbols(QAbstractSpinBox::NoButtons);
    e->setValue(value);
}

void LegoSetSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    auto e = static_cast<QSpinBox*>(editor);
    e->interpretText();
    int value = e->value();

    model->setData(index, value, int(LegoSetTableModel::Role::Sort));
}

void LegoSetSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
