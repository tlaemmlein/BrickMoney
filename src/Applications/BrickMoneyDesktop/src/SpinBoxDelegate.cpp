#include "SpinBoxDelegate.h"

#include "Packages/LegoSetModels/LegoSetTableModel.h"

#include <QFileInfo>
#include <QDoubleSpinBox>
#include <QPainter>
#include <QEvent>
#include <QApplication>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    qDebug() << __FUNCTION__;
}

//void SpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    qDebug() << __FUNCTION__;
//    if (index.data(int(LegoSetTableModel::Role::Type)).toString() == QLatin1String("double")) {
//        int percent = qRound(index.data(int(LegoSetTableModel::Role::Number)).toDouble());

//        QStyleOptionProgressBar progressBarOption;
//        progressBarOption.rect = option.rect;
//        progressBarOption.minimum = 0;
//        progressBarOption.maximum = 100;
//        progressBarOption.progress = percent;
//        progressBarOption.text = index.data().toString();
//        progressBarOption.textVisible = true;

//        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
//    } else {
//        QStyledItemDelegate::paint(painter, option, index);
//    }
//}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    qDebug() << __FUNCTION__;

    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(-1000000);
    editor->setMaximum( 1000000);

    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;

    double value = index.model()->data(index, int(LegoSetTableModel::Role::Number)).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBox->setDecimals(2);
    spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    qDebug() << __FUNCTION__;

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, int(LegoSetTableModel::Role::Number));
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    qDebug() << __FUNCTION__;

    editor->setGeometry(option.rect);
}
