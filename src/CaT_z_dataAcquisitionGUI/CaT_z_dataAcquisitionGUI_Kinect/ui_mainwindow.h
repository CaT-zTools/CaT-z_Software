/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

/* *****************************************************************************************************
This file is part of the CaT-Z software. 
2020, Porto, Portugal

Authors: Ana Gerós (anafgeros@ineb.up.pt), Paulo Aguiar (pauloaguiar@ineb.up.pt)

If you use this software, please acknowledge our work published in: 
“Gerós, A., Magalhães, A. &  Aguiar, P. Improved 3D tracking and automatic classification of rodent’s behavioral activity using depth-sensing cameras. Behav Res (2020)”

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

***************************************************************************************************** */


#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *lblVideo2_5;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_4;
    QFrame *line_4;
    QPushButton *btnConnect;
    QLabel *label_3;
    QLabel *label_5;
    QFrame *line_5;
    QPushButton *btnSaveReal;
    QLineEdit *edtFileVideo;
    QLabel *label_6;
    QLabel *lblVideo2;
    QPushButton *btnOpenDir;
    QFrame *frame_2;
    QLabel *label_28;
    QLabel *label_16;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(335, 310);
        QPalette palette;
        QBrush brush(QColor(231, 231, 231, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(59, 59, 59, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(3, 7, 35, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        MainWindow->setPalette(palette);
        QFont font;
        font.setPointSize(1);
        MainWindow->setFont(font);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(59, 59, 59);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setAutoFillBackground(false);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 751, 51));
        QFont font1;
        font1.setFamily(QStringLiteral("Helvetica Neue"));
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        frame->setFont(font1);
        frame->setStyleSheet(QStringLiteral("background-color: rgb(0, 68, 100);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_25 = new QLabel(frame);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(10, 4, 81, 56));
        label_25->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-10.jpg")));
        label_25->setScaledContents(true);
        label_26 = new QLabel(frame);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(780, 20, 201, 41));
        label_26->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_26->setScaledContents(true);
        lblVideo2_5 = new QLabel(frame);
        lblVideo2_5->setObjectName(QStringLiteral("lblVideo2_5"));
        lblVideo2_5->setGeometry(QRect(10, 10, 501, 31));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        lblVideo2_5->setFont(font2);
        lblVideo2_5->setStyleSheet(QLatin1String("color: rgb(199, 199, 199);\n"
""));
        lblVideo2_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2_5->setWordWrap(true);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(210, 273, 111, 20));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        label_14->setFont(font3);
        label_14->setStyleSheet(QStringLiteral("color: rgb(140, 140, 140);"));
        label_14->setFrameShape(QFrame::NoFrame);
        label_14->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(12, 269, 101, 21));
        QFont font4;
        font4.setPointSize(15);
        font4.setBold(true);
        font4.setWeight(75);
        label_15->setFont(font4);
        label_15->setStyleSheet(QStringLiteral("color: rgb(140, 140, 140);"));
        label_15->setFrameShape(QFrame::NoFrame);
        label_15->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 60, 161, 21));
        QFont font5;
        font5.setFamily(QStringLiteral("Helvetica Neue"));
        font5.setPointSize(11);
        font5.setBold(true);
        font5.setWeight(75);
        label_4->setFont(font5);
        label_4->setStyleSheet(QStringLiteral("color: rgb(199, 199, 199);"));
        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(170, 70, 504, 2));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line_4->sizePolicy().hasHeightForWidth());
        line_4->setSizePolicy(sizePolicy);
        line_4->setBaseSize(QSize(0, 0));
        line_4->setStyleSheet(QStringLiteral("background:rgb(230, 230, 230)"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        btnConnect = new QPushButton(centralWidget);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(107, 98, 51, 21));
        QFont font6;
        font6.setBold(true);
        font6.setWeight(75);
        btnConnect->setFont(font6);
        btnConnect->setStyleSheet(QLatin1String("background-color: rgb(0, 68, 100);\n"
" border-radius:10px;\n"
"color: rgb(255, 255, 255);"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(12, 91, 81, 31));
        label_3->setStyleSheet(QStringLiteral("color: rgb(229, 229, 229);"));
        label_3->setWordWrap(true);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 142, 171, 21));
        label_5->setFont(font5);
        label_5->setStyleSheet(QStringLiteral("color: rgb(199, 199, 199);"));
        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(170, 152, 504, 2));
        sizePolicy.setHeightForWidth(line_5->sizePolicy().hasHeightForWidth());
        line_5->setSizePolicy(sizePolicy);
        line_5->setBaseSize(QSize(0, 0));
        line_5->setStyleSheet(QStringLiteral("background:rgb(230, 230, 230)"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        btnSaveReal = new QPushButton(centralWidget);
        btnSaveReal->setObjectName(QStringLiteral("btnSaveReal"));
        btnSaveReal->setGeometry(QRect(143, 218, 51, 21));
        QFont font7;
        font7.setBold(true);
        font7.setItalic(false);
        font7.setWeight(75);
        btnSaveReal->setFont(font7);
        btnSaveReal->setStyleSheet(QLatin1String("background-color: rgb(0, 68, 100);\n"
" border-radius:10px;\n"
"color: rgb(255, 255, 255);"));
        edtFileVideo = new QLineEdit(centralWidget);
        edtFileVideo->setObjectName(QStringLiteral("edtFileVideo"));
        edtFileVideo->setEnabled(false);
        edtFileVideo->setGeometry(QRect(101, 182, 101, 20));
        QFont font8;
        font8.setFamily(QStringLiteral("Calibri"));
        edtFileVideo->setFont(font8);
        edtFileVideo->setStyleSheet(QLatin1String("background-color: rgb(57, 57, 57);\n"
"border-color: rgb(0, 0, 0);"));
        edtFileVideo->setInputMethodHints(Qt::ImhDigitsOnly);
        edtFileVideo->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(17, 212, 121, 31));
        label_6->setStyleSheet(QStringLiteral("color: rgb(229, 229, 229);"));
        label_6->setWordWrap(true);
        lblVideo2 = new QLabel(centralWidget);
        lblVideo2->setObjectName(QStringLiteral("lblVideo2"));
        lblVideo2->setGeometry(QRect(17, 176, 81, 31));
        lblVideo2->setStyleSheet(QStringLiteral("color: rgb(229, 229, 229);"));
        lblVideo2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2->setWordWrap(true);
        btnOpenDir = new QPushButton(centralWidget);
        btnOpenDir->setObjectName(QStringLiteral("btnOpenDir"));
        btnOpenDir->setGeometry(QRect(208, 181, 27, 23));
        QFont font9;
        font9.setFamily(QStringLiteral("Calibri"));
        font9.setPointSize(13);
        font9.setBold(true);
        font9.setWeight(75);
        btnOpenDir->setFont(font9);
        btnOpenDir->setStyleSheet(QLatin1String("background-color: rgb(121, 121, 121);\n"
"  border-radius:10px;\n"
"color: rgb(84, 84, 84);"));
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 293, 751, 51));
        frame_2->setFont(font1);
        frame_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 68, 100);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_28 = new QLabel(frame_2);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(780, 20, 201, 41));
        label_28->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_28->setScaledContents(true);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(100, 271, 81, 21));
        QFont font10;
        font10.setPointSize(13);
        font10.setBold(false);
        font10.setWeight(50);
        label_16->setFont(font10);
        label_16->setStyleSheet(QStringLiteral("color: rgb(140, 140, 140);"));
        label_16->setFrameShape(QFrame::NoFrame);
        label_16->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        line_5->raise();
        line_4->raise();
        frame->raise();
        label_14->raise();
        label_15->raise();
        label_4->raise();
        btnConnect->raise();
        label_3->raise();
        label_5->raise();
        btnSaveReal->raise();
        edtFileVideo->raise();
        label_6->raise();
        lblVideo2->raise();
        btnOpenDir->raise();
        frame_2->raise();
        label_16->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CaT-z : DATA ACQUISITION GUI", Q_NULLPTR));
        label_25->setText(QString());
        label_26->setText(QString());
        lblVideo2_5->setText(QApplication::translate("MainWindow", "RGB-D DATA ACQUISITION GUI", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", " by AG & PA. 2017", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "CaT - z", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Camera Connection", Q_NULLPTR));
        btnConnect->setText(QApplication::translate("MainWindow", "ON", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Connect camera", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Real time acquisition", Q_NULLPTR));
        btnSaveReal->setText(QApplication::translate("MainWindow", "START", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Start real time recording:", Q_NULLPTR));
        lblVideo2->setText(QApplication::translate("MainWindow", "Select directory:", Q_NULLPTR));
        btnOpenDir->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        label_28->setText(QString());
        label_16->setText(QApplication::translate("MainWindow", "software", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
