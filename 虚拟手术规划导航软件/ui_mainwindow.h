/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pb_logo;
    QLabel *lb_title;
    QSpacerItem *horizontalSpacer;
    QPushButton *pb_Minimize;
    QPushButton *pb_Normal;
    QPushButton *pb_Maximize;
    QPushButton *pb_Close;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QVTKWidget *qvtkWidget_4;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_9;
    QVTKWidget *qvtkWidget;
    QSpacerItem *horizontalSpacer_22;
    QVTKWidget *qvtkWidget_2;
    QSpacerItem *horizontalSpacer_23;
    QVTKWidget *qvtkWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pb_Delimiter1;
    QToolButton *pb_Menu_File;
    QPushButton *pb_Menu_Edit;
    QPushButton *pb_Menu_Tools;
    QPushButton *pb_Menu_View;
    QPushButton *pb_Menu_Help;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pb_Delimiter1_2;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_6;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_7;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_8;
    QSpacerItem *horizontalSpacer_8;
    QLabel *lb_Delimiter2;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label;
    QSpacerItem *horizontalSpacer_10;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_14;
    QPushButton *pb_start_Diction;
    QPushButton *pb_stop_Diction;
    QSpacerItem *horizontalSpacer_11;
    QLabel *lb_Delimiter2_1;
    QSpacerItem *horizontalSpacer_12;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_13;
    QSlider *horizontalSlider;
    QSpacerItem *horizontalSpacer_15;
    QLabel *lb_SliderNum;
    QSpacerItem *horizontalSpacer_16;
    QLabel *lb_Delimiter2_2;
    QSpacerItem *horizontalSpacer_17;
    QLabel *label_3;
    QComboBox *comboBox_2;
    QSpacerItem *horizontalSpacer_18;
    QPushButton *pb_start_cartoon;
    QPushButton *pb_stop_cartoon;
    QSpacerItem *horizontalSpacer_19;
    QLabel *lb_Delimiter2_3;
    QSpacerItem *horizontalSpacer_21;
    QLabel *label_4;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer_20;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_3;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1263, 768);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1024, 768));
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setEnabled(true);
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setStyleSheet(QStringLiteral("background-color: rgb(83, 83, 83);"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pb_logo = new QPushButton(widget);
        pb_logo->setObjectName(QStringLiteral("pb_logo"));
        pb_logo->setMinimumSize(QSize(20, 20));
        pb_logo->setMaximumSize(QSize(20, 20));
        pb_logo->setFlat(true);

        horizontalLayout->addWidget(pb_logo);

        lb_title = new QLabel(widget);
        lb_title->setObjectName(QStringLiteral("lb_title"));
        lb_title->setLineWidth(1);
        lb_title->setMidLineWidth(0);
        lb_title->setTextFormat(Qt::PlainText);
        lb_title->setScaledContents(false);
        lb_title->setIndent(10);

        horizontalLayout->addWidget(lb_title);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pb_Minimize = new QPushButton(widget);
        pb_Minimize->setObjectName(QStringLiteral("pb_Minimize"));
        pb_Minimize->setMinimumSize(QSize(29, 19));
        pb_Minimize->setMaximumSize(QSize(29, 19));
        pb_Minimize->setCheckable(false);
        pb_Minimize->setChecked(false);
        pb_Minimize->setAutoRepeat(false);
        pb_Minimize->setAutoExclusive(false);
        pb_Minimize->setFlat(true);

        horizontalLayout->addWidget(pb_Minimize, 0, Qt::AlignTop);

        pb_Normal = new QPushButton(widget);
        pb_Normal->setObjectName(QStringLiteral("pb_Normal"));
        pb_Normal->setEnabled(true);
        pb_Normal->setMinimumSize(QSize(26, 19));
        pb_Normal->setMaximumSize(QSize(26, 19));
        pb_Normal->setAutoFillBackground(false);
        pb_Normal->setFlat(true);

        horizontalLayout->addWidget(pb_Normal, 0, Qt::AlignTop);

        pb_Maximize = new QPushButton(widget);
        pb_Maximize->setObjectName(QStringLiteral("pb_Maximize"));
        pb_Maximize->setMinimumSize(QSize(26, 19));
        pb_Maximize->setMaximumSize(QSize(26, 19));
        pb_Maximize->setAutoFillBackground(false);
        pb_Maximize->setFlat(true);

        horizontalLayout->addWidget(pb_Maximize, 0, Qt::AlignTop);

        pb_Close = new QPushButton(widget);
        pb_Close->setObjectName(QStringLiteral("pb_Close"));
        pb_Close->setMinimumSize(QSize(48, 19));
        pb_Close->setMaximumSize(QSize(48, 19));
        pb_Close->setFlat(true);

        horizontalLayout->addWidget(pb_Close, 0, Qt::AlignTop);

        horizontalSpacer_2 = new QSpacerItem(3, 30, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        qvtkWidget_4 = new QVTKWidget(centralwidget);
        qvtkWidget_4->setObjectName(QStringLiteral("qvtkWidget_4"));
        qvtkWidget_4->setFocusPolicy(Qt::StrongFocus);
        qvtkWidget_4->setAutoFillBackground(false);
        qvtkWidget_4->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/vtk00.jpg);"));

        horizontalLayout_2->addWidget(qvtkWidget_4);


        verticalLayout_4->addLayout(horizontalLayout_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));

        verticalLayout_4->addLayout(verticalLayout_2);

        widget_4 = new QWidget(centralwidget);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMinimumSize(QSize(0, 2));
        widget_4->setMaximumSize(QSize(16777215, 0));
        widget_4->setStyleSheet(QStringLiteral("color: rgb(236, 255, 20);"));

        verticalLayout_4->addWidget(widget_4);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        qvtkWidget = new QVTKWidget(centralwidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        qvtkWidget->setStyleSheet(QLatin1String("image: url(:/Resource/ico/vtk01.jpg);\n"
"width:100%;\n"
"height:100%;"));

        horizontalLayout_9->addWidget(qvtkWidget);

        horizontalSpacer_22 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_22);

        qvtkWidget_2 = new QVTKWidget(centralwidget);
        qvtkWidget_2->setObjectName(QStringLiteral("qvtkWidget_2"));
        qvtkWidget_2->setStyleSheet(QLatin1String("image: url(:/Resource/ico/vtk01.jpg);\n"
"width:100%;\n"
"height:100%;"));

        horizontalLayout_9->addWidget(qvtkWidget_2);

        horizontalSpacer_23 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_23);

        qvtkWidget_3 = new QVTKWidget(centralwidget);
        qvtkWidget_3->setObjectName(QStringLiteral("qvtkWidget_3"));
        qvtkWidget_3->setAutoFillBackground(false);
        qvtkWidget_3->setStyleSheet(QLatin1String("image: url(:/Resource/ico/vtk01.jpg);\n"
"width:100%;\n"
"height:100%;"));

        horizontalLayout_9->addWidget(qvtkWidget_3);


        verticalLayout_4->addLayout(horizontalLayout_9);

        verticalLayout_4->setStretch(0, 2);
        verticalLayout_4->setStretch(3, 1);

        horizontalLayout_8->addLayout(verticalLayout_4);


        gridLayout_2->addLayout(horizontalLayout_8, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(2, -1, 2, -1);
        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(0, 35));
        widget_2->setMaximumSize(QSize(16777215, 35));
        widget_2->setStyleSheet(QStringLiteral("background-color: rgb(83, 83, 83);"));
        horizontalLayout_4 = new QHBoxLayout(widget_2);
        horizontalLayout_4->setSpacing(2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        pb_Delimiter1 = new QPushButton(widget_2);
        pb_Delimiter1->setObjectName(QStringLiteral("pb_Delimiter1"));
        pb_Delimiter1->setMinimumSize(QSize(11, 35));
        pb_Delimiter1->setMaximumSize(QSize(11, 35));
        pb_Delimiter1->setFlat(true);

        horizontalLayout_4->addWidget(pb_Delimiter1);

        pb_Menu_File = new QToolButton(widget_2);
        pb_Menu_File->setObjectName(QStringLiteral("pb_Menu_File"));
        pb_Menu_File->setMinimumSize(QSize(60, 20));
        pb_Menu_File->setMaximumSize(QSize(60, 20));
        pb_Menu_File->setFocusPolicy(Qt::StrongFocus);
        pb_Menu_File->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;"));
        pb_Menu_File->setPopupMode(QToolButton::DelayedPopup);
        pb_Menu_File->setAutoRaise(true);
        pb_Menu_File->setArrowType(Qt::NoArrow);

        horizontalLayout_4->addWidget(pb_Menu_File);

        pb_Menu_Edit = new QPushButton(widget_2);
        pb_Menu_Edit->setObjectName(QStringLiteral("pb_Menu_Edit"));
        pb_Menu_Edit->setMinimumSize(QSize(60, 20));
        pb_Menu_Edit->setMaximumSize(QSize(60, 20));
        pb_Menu_Edit->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;"));
        pb_Menu_Edit->setFlat(true);

        horizontalLayout_4->addWidget(pb_Menu_Edit);

        pb_Menu_Tools = new QPushButton(widget_2);
        pb_Menu_Tools->setObjectName(QStringLiteral("pb_Menu_Tools"));
        pb_Menu_Tools->setMinimumSize(QSize(60, 20));
        pb_Menu_Tools->setMaximumSize(QSize(60, 20));
        pb_Menu_Tools->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;"));
        pb_Menu_Tools->setFlat(true);

        horizontalLayout_4->addWidget(pb_Menu_Tools);

        pb_Menu_View = new QPushButton(widget_2);
        pb_Menu_View->setObjectName(QStringLiteral("pb_Menu_View"));
        pb_Menu_View->setMinimumSize(QSize(60, 20));
        pb_Menu_View->setMaximumSize(QSize(60, 20));
        pb_Menu_View->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;"));
        pb_Menu_View->setFlat(true);

        horizontalLayout_4->addWidget(pb_Menu_View);

        pb_Menu_Help = new QPushButton(widget_2);
        pb_Menu_Help->setObjectName(QStringLiteral("pb_Menu_Help"));
        pb_Menu_Help->setMinimumSize(QSize(60, 20));
        pb_Menu_Help->setMaximumSize(QSize(60, 20));
        pb_Menu_Help->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;"));
        pb_Menu_Help->setFlat(true);

        horizontalLayout_4->addWidget(pb_Menu_Help);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        pb_Delimiter1_2 = new QPushButton(widget_2);
        pb_Delimiter1_2->setObjectName(QStringLiteral("pb_Delimiter1_2"));
        pb_Delimiter1_2->setMinimumSize(QSize(11, 35));
        pb_Delimiter1_2->setMaximumSize(QSize(11, 35));
        pb_Delimiter1_2->setFlat(true);

        horizontalLayout_4->addWidget(pb_Delimiter1_2);

        horizontalSpacer_7 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        pushButton_6 = new QPushButton(widget_2);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setMinimumSize(QSize(20, 20));
        pushButton_6->setMaximumSize(QSize(20, 20));
        pushButton_6->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;\n"
"    border:2px groove gray;\n"
"    border-radius:5px;\n"
""));
        pushButton_6->setFlat(false);

        horizontalLayout_4->addWidget(pushButton_6);

        horizontalSpacer_5 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        pushButton_7 = new QPushButton(widget_2);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setMinimumSize(QSize(20, 20));
        pushButton_7->setMaximumSize(QSize(20, 20));
        pushButton_7->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;\n"
"    border:2px groove gray;\n"
"    border-radius:5px;\n"
""));
        pushButton_7->setFlat(false);

        horizontalLayout_4->addWidget(pushButton_7);

        horizontalSpacer_6 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        pushButton_8 = new QPushButton(widget_2);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setMinimumSize(QSize(20, 20));
        pushButton_8->setMaximumSize(QSize(20, 20));
        pushButton_8->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;\n"
"    border:2px groove gray;\n"
"    border-radius:5px;\n"
""));
        pushButton_8->setFlat(false);

        horizontalLayout_4->addWidget(pushButton_8);

        horizontalSpacer_8 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        lb_Delimiter2 = new QLabel(widget_2);
        lb_Delimiter2->setObjectName(QStringLiteral("lb_Delimiter2"));

        horizontalLayout_4->addWidget(lb_Delimiter2);

        horizontalSpacer_9 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setFrameShape(QFrame::WinPanel);

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        comboBox = new QComboBox(widget_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMinimumSize(QSize(80, 25));
        comboBox->setMaximumSize(QSize(80, 25));

        horizontalLayout_4->addWidget(comboBox);

        horizontalSpacer_14 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_14);

        pb_start_Diction = new QPushButton(widget_2);
        pb_start_Diction->setObjectName(QStringLiteral("pb_start_Diction"));
        pb_start_Diction->setMinimumSize(QSize(20, 20));
        pb_start_Diction->setMaximumSize(QSize(20, 20));
        pb_start_Diction->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/start.png);"));
        pb_start_Diction->setFlat(false);

        horizontalLayout_4->addWidget(pb_start_Diction);

        pb_stop_Diction = new QPushButton(widget_2);
        pb_stop_Diction->setObjectName(QStringLiteral("pb_stop_Diction"));
        pb_stop_Diction->setMinimumSize(QSize(20, 20));
        pb_stop_Diction->setMaximumSize(QSize(20, 20));
        pb_stop_Diction->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/stop.png);"));

        horizontalLayout_4->addWidget(pb_stop_Diction);

        horizontalSpacer_11 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);

        lb_Delimiter2_1 = new QLabel(widget_2);
        lb_Delimiter2_1->setObjectName(QStringLiteral("lb_Delimiter2_1"));

        horizontalLayout_4->addWidget(lb_Delimiter2_1);

        horizontalSpacer_12 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_12);

        label_6 = new QLabel(widget_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setStyleSheet(QLatin1String("    color: #f0f0f0;\n"
"    font: 13.5px;"));

        horizontalLayout_4->addWidget(label_6);

        horizontalSpacer_13 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_13);

        horizontalSlider = new QSlider(widget_2);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMinimumSize(QSize(120, 20));
        horizontalSlider->setMaximumSize(QSize(120, 20));
        horizontalSlider->setMouseTracking(false);
        horizontalSlider->setMinimum(-2000);
        horizontalSlider->setMaximum(10000);
        horizontalSlider->setValue(8000);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setInvertedAppearance(false);
        horizontalSlider->setInvertedControls(false);

        horizontalLayout_4->addWidget(horizontalSlider);

        horizontalSpacer_15 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_15);

        lb_SliderNum = new QLabel(widget_2);
        lb_SliderNum->setObjectName(QStringLiteral("lb_SliderNum"));
        lb_SliderNum->setMinimumSize(QSize(18, 20));
        lb_SliderNum->setMaximumSize(QSize(18, 20));

        horizontalLayout_4->addWidget(lb_SliderNum);

        horizontalSpacer_16 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_16);

        lb_Delimiter2_2 = new QLabel(widget_2);
        lb_Delimiter2_2->setObjectName(QStringLiteral("lb_Delimiter2_2"));

        horizontalLayout_4->addWidget(lb_Delimiter2_2);

        horizontalSpacer_17 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_17);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(30, 30));
        label_3->setMaximumSize(QSize(30, 30));

        horizontalLayout_4->addWidget(label_3);

        comboBox_2 = new QComboBox(widget_2);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setMinimumSize(QSize(90, 25));
        comboBox_2->setMaximumSize(QSize(90, 25));

        horizontalLayout_4->addWidget(comboBox_2);

        horizontalSpacer_18 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_18);

        pb_start_cartoon = new QPushButton(widget_2);
        pb_start_cartoon->setObjectName(QStringLiteral("pb_start_cartoon"));
        pb_start_cartoon->setMinimumSize(QSize(20, 20));
        pb_start_cartoon->setMaximumSize(QSize(20, 20));
        pb_start_cartoon->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/start.png);"));

        horizontalLayout_4->addWidget(pb_start_cartoon);

        pb_stop_cartoon = new QPushButton(widget_2);
        pb_stop_cartoon->setObjectName(QStringLiteral("pb_stop_cartoon"));
        pb_stop_cartoon->setMinimumSize(QSize(20, 20));
        pb_stop_cartoon->setMaximumSize(QSize(20, 20));
        pb_stop_cartoon->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/stop.png);"));

        horizontalLayout_4->addWidget(pb_stop_cartoon);

        horizontalSpacer_19 = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_19);

        lb_Delimiter2_3 = new QLabel(widget_2);
        lb_Delimiter2_3->setObjectName(QStringLiteral("lb_Delimiter2_3"));

        horizontalLayout_4->addWidget(lb_Delimiter2_3);

        horizontalSpacer_21 = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_21);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(35, 30));
        label_4->setMaximumSize(QSize(35, 30));

        horizontalLayout_4->addWidget(label_4);

        pushButton_4 = new QPushButton(widget_2);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/cut01.png);"));

        horizontalLayout_4->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(widget_2);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setStyleSheet(QStringLiteral("image: url(:/Resource/ico/cut02.png);"));

        horizontalLayout_4->addWidget(pushButton_5);

        horizontalSpacer_20 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_20);


        horizontalLayout_3->addWidget(widget_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);

        horizontalLayout_3->addLayout(horizontalLayout_5);


        gridLayout_2->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(0, 25));
        widget_3->setMaximumSize(QSize(16777215, 25));
        widget_3->setStyleSheet(QStringLiteral("background-color: rgb(83, 83, 83);"));
        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 4, 421, 16));

        gridLayout_2->addWidget(widget_3, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(pb_Close, SIGNAL(clicked()), MainWindow, SLOT(close()));
        QObject::connect(pb_Minimize, SIGNAL(clicked()), MainWindow, SLOT(showMinimized()));
        QObject::connect(pb_Maximize, SIGNAL(clicked()), MainWindow, SLOT(showMaximized()));
        QObject::connect(pb_Maximize, SIGNAL(clicked()), pb_Normal, SLOT(show()));
        QObject::connect(pushButton_8, SIGNAL(clicked()), MainWindow, SLOT(brain3()));
        QObject::connect(pb_start_Diction, SIGNAL(clicked()), pb_start_Diction, SLOT(hide()));
        QObject::connect(pb_Normal, SIGNAL(clicked()), pb_Normal, SLOT(hide()));
        QObject::connect(pb_Normal, SIGNAL(clicked()), pb_Maximize, SLOT(show()));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), MainWindow, SLOT(test3()));
        QObject::connect(pb_Normal, SIGNAL(clicked()), MainWindow, SLOT(showNormal()));
        QObject::connect(pb_start_cartoon, SIGNAL(clicked()), pb_stop_cartoon, SLOT(show()));
        QObject::connect(pb_stop_Diction, SIGNAL(clicked()), pb_stop_Diction, SLOT(hide()));
        QObject::connect(pb_stop_cartoon, SIGNAL(clicked()), pb_stop_cartoon, SLOT(hide()));
        QObject::connect(comboBox, SIGNAL(activated(QString)), MainWindow, SLOT(test3()));
        QObject::connect(pushButton_6, SIGNAL(clicked()), MainWindow, SLOT(brain1()));
        QObject::connect(pb_Maximize, SIGNAL(clicked()), pb_Maximize, SLOT(hide()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), MainWindow, SLOT(test2()));
        QObject::connect(pb_stop_cartoon, SIGNAL(clicked()), pb_start_cartoon, SLOT(show()));
        QObject::connect(pushButton_7, SIGNAL(clicked()), MainWindow, SLOT(brain2()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), MainWindow, SLOT(test()));
        QObject::connect(pb_stop_Diction, SIGNAL(clicked()), pb_start_Diction, SLOT(show()));
        QObject::connect(pb_start_cartoon, SIGNAL(clicked()), pb_start_cartoon, SLOT(hide()));
        QObject::connect(pb_start_Diction, SIGNAL(clicked()), pb_stop_Diction, SLOT(show()));
        QObject::connect(comboBox_2, SIGNAL(activated(QString)), MainWindow, SLOT(test5()));
        QObject::connect(pb_Menu_File, SIGNAL(clicked()), MainWindow, SLOT(load()));

        pb_Normal->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pb_logo->setText(QString());
        lb_title->setText(QApplication::translate("MainWindow", "\350\231\232\346\213\237\346\211\213\346\234\257\350\247\204\345\210\222\345\257\274\350\210\252\350\275\257\344\273\266 V1.0", Q_NULLPTR));
        pb_Minimize->setText(QString());
        pb_Normal->setText(QString());
        pb_Maximize->setText(QString());
        pb_Close->setText(QString());
        pb_Delimiter1->setText(QString());
        pb_Menu_File->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266(F)", Q_NULLPTR));
        pb_Menu_Edit->setText(QApplication::translate("MainWindow", "\347\274\226\350\276\221(E)", Q_NULLPTR));
        pb_Menu_Tools->setText(QApplication::translate("MainWindow", "\345\267\245\345\205\267(T)", Q_NULLPTR));
        pb_Menu_View->setText(QApplication::translate("MainWindow", "\350\247\206\345\233\276(V)", Q_NULLPTR));
        pb_Menu_Help->setText(QApplication::translate("MainWindow", "\345\270\256\345\212\251(H)", Q_NULLPTR));
        pb_Delimiter1_2->setText(QString());
        pushButton_6->setText(QApplication::translate("MainWindow", "\346\250\252", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("MainWindow", "\347\237\242", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("MainWindow", "\345\206\240", Q_NULLPTR));
        lb_Delimiter2->setText(QString());
        label->setText(QApplication::translate("MainWindow", "\346\270\262\346\237\223", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\346\270\262\346\237\22301", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\270\262\346\237\22302", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\270\262\346\237\223\345\212\250\347\224\273", Q_NULLPTR)
        );
        pb_start_Diction->setText(QString());
        pb_stop_Diction->setText(QString());
        lb_Delimiter2_1->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "\345\217\214\347\234\274\350\267\235", Q_NULLPTR));
        lb_SliderNum->setText(QApplication::translate("MainWindow", "40", Q_NULLPTR));
        lb_Delimiter2_2->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "\345\212\250\347\224\273", Q_NULLPTR));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\350\267\257\345\276\204\345\212\250\347\224\27301", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\350\267\257\345\276\204\345\212\250\347\224\27302", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\351\205\215\345\207\206", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\270\262\346\237\223\345\212\250\347\224\273", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\271\263\347\247\273+\346\227\213\350\275\254STL", Q_NULLPTR)
         << QApplication::translate("MainWindow", "pickpoint", Q_NULLPTR)
        );
        pb_start_cartoon->setText(QString());
        pb_stop_cartoon->setText(QString());
        lb_Delimiter2_3->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "\345\210\207\345\211\262", Q_NULLPTR));
        pushButton_4->setText(QString());
        pushButton_5->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "D:/project/brain", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
