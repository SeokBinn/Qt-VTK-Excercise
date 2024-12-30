/********************************************************************************
** Form generated from reading UI file 'QtVTKProject.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTVTKPROJECT_H
#define UI_QTVTKPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qvtkopenglnativewidget.h"

QT_BEGIN_NAMESPACE

class Ui_QtVTKProjectClass
{
public:
    QWidget *centralWidget;
    QPushButton *loadDICOM_button;
    QVTKOpenGLNativeWidget *openGLWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *QtVTKProjectClass)
    {
        if (QtVTKProjectClass->objectName().isEmpty())
            QtVTKProjectClass->setObjectName(QString::fromUtf8("QtVTKProjectClass"));
        QtVTKProjectClass->resize(861, 522);
        centralWidget = new QWidget(QtVTKProjectClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        loadDICOM_button = new QPushButton(centralWidget);
        loadDICOM_button->setObjectName(QString::fromUtf8("loadDICOM_button"));
        loadDICOM_button->setGeometry(QRect(130, 40, 80, 23));
        openGLWidget = new QVTKOpenGLNativeWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(345, 38, 471, 410));
        QtVTKProjectClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtVTKProjectClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 861, 21));
        QtVTKProjectClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(QtVTKProjectClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtVTKProjectClass->setStatusBar(statusBar);
        mainToolBar = new QToolBar(QtVTKProjectClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtVTKProjectClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(QtVTKProjectClass);

        QMetaObject::connectSlotsByName(QtVTKProjectClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtVTKProjectClass)
    {
        QtVTKProjectClass->setWindowTitle(QApplication::translate("QtVTKProjectClass", "QtVTKProject", nullptr));
        loadDICOM_button->setText(QApplication::translate("QtVTKProjectClass", "Load DICOM", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtVTKProjectClass: public Ui_QtVTKProjectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTVTKPROJECT_H
