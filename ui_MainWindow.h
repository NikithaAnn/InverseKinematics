/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *s_description;
    QGridLayout *s_MainGridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *m_load;
    QPushButton *m_CCD;
    QCheckBox *m_wireframe;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        s_description = new QWidget(MainWindow);
        s_description->setObjectName(QStringLiteral("s_description"));
        s_MainGridLayout = new QGridLayout(s_description);
        s_MainGridLayout->setObjectName(QStringLiteral("s_MainGridLayout"));
        groupBox = new QGroupBox(s_description);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        m_load = new QPushButton(groupBox);
        m_load->setObjectName(QStringLiteral("m_load"));

        gridLayout->addWidget(m_load, 0, 0, 1, 1);

        m_CCD = new QPushButton(groupBox);
        m_CCD->setObjectName(QStringLiteral("m_CCD"));

        gridLayout->addWidget(m_CCD, 1, 0, 1, 1);

        m_wireframe = new QCheckBox(groupBox);
        m_wireframe->setObjectName(QStringLiteral("m_wireframe"));
        m_wireframe->setChecked(true);

        gridLayout->addWidget(m_wireframe, 2, 0, 1, 1);


        s_MainGridLayout->addWidget(groupBox, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_MainGridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        s_MainGridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        MainWindow->setCentralWidget(s_description);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CCDIKSolver", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Controls:", 0));
        m_load->setText(QApplication::translate("MainWindow", "Load File", 0));
        m_CCD->setText(QApplication::translate("MainWindow", "Reach Target", 0));
        m_wireframe->setText(QApplication::translate("MainWindow", "Solidframe", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
