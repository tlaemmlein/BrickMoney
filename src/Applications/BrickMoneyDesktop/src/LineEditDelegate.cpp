#include "LineEditDelegate.h"

#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QDoubleSpinBox>
#include <QPainter>
#include <QEvent>
#include <QApplication>
#include <QLineEdit>

LineEditDelegate::LineEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    qDebug() << __FUNCTION__;

}

QWidget *LineEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex &/* index */) const
{
    qDebug() << __FUNCTION__;
    return new QLineEdit(parent);
}

void LineEditDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;

    QString value = index.model()->data(index, int(LegoSetTableModel::Role::Sort)).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    //    lineEdit->returnPressed();
    QString value = lineEdit->text();

    model->setData(index, value, int(LegoSetTableModel::Role::Sort));
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    qDebug() << __FUNCTION__;

    editor->setGeometry(option.rect);
}
