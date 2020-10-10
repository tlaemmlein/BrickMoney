/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#ifndef KD_MULTISPLITTER_SEPARATORQWIDGET_P_H
#define KD_MULTISPLITTER_SEPARATORQWIDGET_P_H

#include "multisplitter_export.h"
#include "Separator_p.h"
#include "Widget_qwidget.h"

#include <QWidget>

namespace Layouting {

class MULTISPLITTER_EXPORT SeparatorWidget
        : public QWidget
        , public Layouting::Separator
        , public Layouting::Widget_qwidget
{
    Q_OBJECT
public:
    explicit SeparatorWidget(Layouting::Widget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    Widget* createRubberBand(Widget *parent) override;
    Widget *asWidget() override;
};

}

#endif
