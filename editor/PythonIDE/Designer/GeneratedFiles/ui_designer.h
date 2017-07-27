/********************************************************************************
** Form generated from reading UI file 'designer.ui'
**
** Created: Tue 1. Apr 17:20:51 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DESIGNER_H
#define UI_DESIGNER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMdiArea>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DesignerClass
{
public:
    QAction *actionPreview;
    QAction *actionSaveForm;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *toolBarLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *fileName;
    QLabel *modifyLbl;
    QSpacerItem *horizontalSpacer;
    QPushButton *editWidgetsBtn;
    QPushButton *editTabOrderBtn;
    QPushButton *editBuddiesBtn;
    QSpacerItem *horizontalSpacer_3;
    QSplitter *splitter_2;
    QWidget *widgetsPlace;
    QVBoxLayout *verticalLayout_4;
    QMdiArea *formPlace;
    QSplitter *splitter;
    QWidget *objectsPlace;
    QVBoxLayout *verticalLayout;
    QWidget *propertyPlace;
    QVBoxLayout *verticalLayout_2;
    QWidget *signalPlace;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *slotTabPLace;
    QVBoxLayout *verticalLayout_7;
    QWidget *actionTabPlace;
    QVBoxLayout *verticalLayout_8;

    void setupUi(QWidget *DesignerClass)
    {
        if (DesignerClass->objectName().isEmpty())
            DesignerClass->setObjectName(QString::fromUtf8("DesignerClass"));
        DesignerClass->resize(790, 675);
        actionPreview = new QAction(DesignerClass);
        actionPreview->setObjectName(QString::fromUtf8("actionPreview"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Designer/images/application_form_magnify.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPreview->setIcon(icon);
        actionSaveForm = new QAction(DesignerClass);
        actionSaveForm->setObjectName(QString::fromUtf8("actionSaveForm"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Designer/images/save_form.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveForm->setIcon(icon1);
        verticalLayout_5 = new QVBoxLayout(DesignerClass);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        toolBarLayout = new QVBoxLayout();
        toolBarLayout->setSpacing(6);
        toolBarLayout->setObjectName(QString::fromUtf8("toolBarLayout"));

        verticalLayout_5->addLayout(toolBarLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        fileName = new QLabel(DesignerClass);
        fileName->setObjectName(QString::fromUtf8("fileName"));
        fileName->setMaximumSize(QSize(16777215, 28));

        horizontalLayout->addWidget(fileName);

        modifyLbl = new QLabel(DesignerClass);
        modifyLbl->setObjectName(QString::fromUtf8("modifyLbl"));
        modifyLbl->setEnabled(true);
        modifyLbl->setMaximumSize(QSize(16777215, 28));

        horizontalLayout->addWidget(modifyLbl);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        editWidgetsBtn = new QPushButton(DesignerClass);
        editWidgetsBtn->setObjectName(QString::fromUtf8("editWidgetsBtn"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Designer/images/widgettool.png"), QSize(), QIcon::Normal, QIcon::Off);
        editWidgetsBtn->setIcon(icon2);
        editWidgetsBtn->setIconSize(QSize(16, 16));
        editWidgetsBtn->setCheckable(true);

        horizontalLayout->addWidget(editWidgetsBtn);

        editTabOrderBtn = new QPushButton(DesignerClass);
        editTabOrderBtn->setObjectName(QString::fromUtf8("editTabOrderBtn"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Designer/images/tabordertool.png"), QSize(), QIcon::Normal, QIcon::Off);
        editTabOrderBtn->setIcon(icon3);
        editTabOrderBtn->setIconSize(QSize(16, 16));
        editTabOrderBtn->setCheckable(true);

        horizontalLayout->addWidget(editTabOrderBtn);

        editBuddiesBtn = new QPushButton(DesignerClass);
        editBuddiesBtn->setObjectName(QString::fromUtf8("editBuddiesBtn"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Designer/images/buddytool.png"), QSize(), QIcon::Normal, QIcon::Off);
        editBuddiesBtn->setIcon(icon4);
        editBuddiesBtn->setIconSize(QSize(16, 16));
        editBuddiesBtn->setCheckable(true);

        horizontalLayout->addWidget(editBuddiesBtn);

        horizontalSpacer_3 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout_5->addLayout(horizontalLayout);

        splitter_2 = new QSplitter(DesignerClass);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setFrameShape(QFrame::NoFrame);
        splitter_2->setOrientation(Qt::Horizontal);
        widgetsPlace = new QWidget(splitter_2);
        widgetsPlace->setObjectName(QString::fromUtf8("widgetsPlace"));
        widgetsPlace->setAcceptDrops(false);
        verticalLayout_4 = new QVBoxLayout(widgetsPlace);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        splitter_2->addWidget(widgetsPlace);
        formPlace = new QMdiArea(splitter_2);
        formPlace->setObjectName(QString::fromUtf8("formPlace"));
        QBrush brush(QColor(150, 150, 150, 255));
        brush.setStyle(Qt::Dense7Pattern);
        formPlace->setBackground(brush);
        formPlace->setTabsClosable(true);
        formPlace->setTabsMovable(true);
        formPlace->setTabShape(QTabWidget::Triangular);
        splitter_2->addWidget(formPlace);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setFrameShape(QFrame::NoFrame);
        splitter->setFrameShadow(QFrame::Plain);
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(true);
        splitter->setChildrenCollapsible(true);
        objectsPlace = new QWidget(splitter);
        objectsPlace->setObjectName(QString::fromUtf8("objectsPlace"));
        verticalLayout = new QVBoxLayout(objectsPlace);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter->addWidget(objectsPlace);
        propertyPlace = new QWidget(splitter);
        propertyPlace->setObjectName(QString::fromUtf8("propertyPlace"));
        verticalLayout_2 = new QVBoxLayout(propertyPlace);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter->addWidget(propertyPlace);
        signalPlace = new QWidget(splitter);
        signalPlace->setObjectName(QString::fromUtf8("signalPlace"));
        verticalLayout_3 = new QVBoxLayout(signalPlace);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget = new QTabWidget(signalPlace);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        slotTabPLace = new QWidget();
        slotTabPLace->setObjectName(QString::fromUtf8("slotTabPLace"));
        verticalLayout_7 = new QVBoxLayout(slotTabPLace);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        tabWidget->addTab(slotTabPLace, QString());
        actionTabPlace = new QWidget();
        actionTabPlace->setObjectName(QString::fromUtf8("actionTabPlace"));
        verticalLayout_8 = new QVBoxLayout(actionTabPlace);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        tabWidget->addTab(actionTabPlace, QString());

        verticalLayout_3->addWidget(tabWidget);

        splitter->addWidget(signalPlace);
        splitter_2->addWidget(splitter);

        verticalLayout_5->addWidget(splitter_2);


        retranslateUi(DesignerClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DesignerClass);
    } // setupUi

    void retranslateUi(QWidget *DesignerClass)
    {
        DesignerClass->setWindowTitle(QApplication::translate("DesignerClass", "Designer", 0, QApplication::UnicodeUTF8));
        actionPreview->setText(QApplication::translate("DesignerClass", "Preview", 0, QApplication::UnicodeUTF8));
        actionPreview->setShortcut(QApplication::translate("DesignerClass", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionSaveForm->setText(QApplication::translate("DesignerClass", "SaveForm", 0, QApplication::UnicodeUTF8));
        actionSaveForm->setShortcut(QApplication::translate("DesignerClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        fileName->setText(QString());
        modifyLbl->setText(QApplication::translate("DesignerClass", "*", 0, QApplication::UnicodeUTF8));
        editWidgetsBtn->setText(QString());
        editTabOrderBtn->setText(QString());
        editBuddiesBtn->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(slotTabPLace), QApplication::translate("DesignerClass", "Slot Editor", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(actionTabPlace), QApplication::translate("DesignerClass", "Action Editor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DesignerClass: public Ui_DesignerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DESIGNER_H
