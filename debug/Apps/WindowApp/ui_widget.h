/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "imageview.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *GL_form;
    QListWidget *LW_images;
    QPushButton *PB_Open;
    ImageView *GV_main;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(961, 641);
        gridLayoutWidget = new QWidget(Widget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 0, 951, 631));
        GL_form = new QGridLayout(gridLayoutWidget);
        GL_form->setSpacing(6);
        GL_form->setContentsMargins(11, 11, 11, 11);
        GL_form->setObjectName(QStringLiteral("GL_form"));
        GL_form->setContentsMargins(0, 0, 0, 0);
        LW_images = new QListWidget(gridLayoutWidget);
        LW_images->setObjectName(QStringLiteral("LW_images"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LW_images->sizePolicy().hasHeightForWidth());
        LW_images->setSizePolicy(sizePolicy);

        GL_form->addWidget(LW_images, 1, 0, 1, 1);

        PB_Open = new QPushButton(gridLayoutWidget);
        PB_Open->setObjectName(QStringLiteral("PB_Open"));

        GL_form->addWidget(PB_Open, 0, 0, 1, 1);

        GV_main = new ImageView(gridLayoutWidget);
        GV_main->setObjectName(QStringLiteral("GV_main"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(GV_main->sizePolicy().hasHeightForWidth());
        GV_main->setSizePolicy(sizePolicy1);
        GV_main->setFocusPolicy(Qt::ClickFocus);
        GV_main->setDragMode(QGraphicsView::ScrollHandDrag);
        GV_main->setResizeAnchor(QGraphicsView::AnchorViewCenter);

        GL_form->addWidget(GV_main, 1, 1, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        PB_Open->setText(QApplication::translate("Widget", "Open", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
