/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/


#ifndef KD_FRAME_WIDGET_P_H
#define KD_FRAME_WIDGET_P_H

#include "../Frame_p.h"

QT_BEGIN_NAMESPACE
class QTabBar;
QT_END_NAMESPACE

namespace KDDockWidgets {

class TestDocks;
class TabWidget;

/**
 * @brief The GUI counterpart of Frame. Inherits Frame and implements paintEvent().
 */
class DOCKS_EXPORT FrameWidget : public Frame
{
    Q_OBJECT
public:
    explicit FrameWidget(QWidget *parent = nullptr, FrameOptions = FrameOption_None);
    ~FrameWidget();
    QTabBar *tabBar() const;
    TabWidget *tabWidget() const;

protected:
    void paintEvent(QPaintEvent *) override;
    QSize maxSizeHint() const override;
    void detachTab_impl(DockWidgetBase *) override;
    int indexOfDockWidget_impl(DockWidgetBase *) override;
    void setCurrentDockWidget_impl(DockWidgetBase *) override;
    int currentIndex_impl() const override;
    void insertDockWidget_impl(DockWidgetBase *, int index) override;
    void removeWidget_impl(DockWidgetBase *) override;
    void setCurrentTabIndex_impl(int) override;
    DockWidgetBase *currentDockWidget_impl() const override;
    DockWidgetBase *dockWidgetAt_impl(int index) const override;
    int dockWidgetCount_impl() const override;
    QRect dragRect() const override;
    void renameTab(int index, const QString &) override;
private:
    friend class TestDocks;
    TabWidget *const m_tabWidget;
};


}

#endif
