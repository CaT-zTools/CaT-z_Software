/********************************************************************************
** Form generated from reading UI file 'selectbehaviorlabels.ui'
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


#ifndef UI_SELECTBEHAVIORLABELS_H
#define UI_SELECTBEHAVIORLABELS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectBehaviorLabels
{
public:
    QFrame *frame;
    QLabel *label_26;
    QLabel *lblVideo2_5;
    QLabel *label_14;
    QLabel *label_7;
    QLabel *lblVideo2;
    QLabel *lblVideo2_2;
    QSpinBox *sbNumberOfEvents;
    QPushButton *btnOK;
    QTableWidget *tbTableEvents;
    QPushButton *btnFinish;
    QLabel *label_16;
    QLabel *label_15;

    void setupUi(QDialog *SelectBehaviorLabels)
    {
        if (SelectBehaviorLabels->objectName().isEmpty())
            SelectBehaviorLabels->setObjectName(QStringLiteral("SelectBehaviorLabels"));
        SelectBehaviorLabels->resize(336, 383);
        SelectBehaviorLabels->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        frame = new QFrame(SelectBehaviorLabels);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 370, 51));
        QFont font;
        font.setFamily(QStringLiteral("Helvetica Neue"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        frame->setFont(font);
        frame->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 127);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_26 = new QLabel(frame);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(780, 20, 201, 41));
        label_26->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_26->setScaledContents(true);
        lblVideo2_5 = new QLabel(frame);
        lblVideo2_5->setObjectName(QStringLiteral("lblVideo2_5"));
        lblVideo2_5->setGeometry(QRect(10, 10, 331, 31));
        QFont font1;
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        lblVideo2_5->setFont(font1);
        lblVideo2_5->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        lblVideo2_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2_5->setWordWrap(true);
        label_14 = new QLabel(SelectBehaviorLabels);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(230, 362, 101, 20));
        QFont font2;
        font2.setPointSize(8);
        font2.setBold(false);
        font2.setWeight(50);
        label_14->setFont(font2);
        label_14->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_14->setFrameShape(QFrame::NoFrame);
        label_14->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(SelectBehaviorLabels);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 60, 211, 16));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setWeight(75);
        label_7->setFont(font3);
        label_7->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        lblVideo2 = new QLabel(SelectBehaviorLabels);
        lblVideo2->setObjectName(QStringLiteral("lblVideo2"));
        lblVideo2->setGeometry(QRect(10, 88, 151, 31));
        lblVideo2->setFont(font1);
        lblVideo2->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        lblVideo2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2->setWordWrap(true);
        lblVideo2_2 = new QLabel(SelectBehaviorLabels);
        lblVideo2_2->setObjectName(QStringLiteral("lblVideo2_2"));
        lblVideo2_2->setGeometry(QRect(23, 120, 91, 21));
        lblVideo2_2->setFont(font2);
        lblVideo2_2->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        lblVideo2_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2_2->setWordWrap(true);
        sbNumberOfEvents = new QSpinBox(SelectBehaviorLabels);
        sbNumberOfEvents->setObjectName(QStringLiteral("sbNumberOfEvents"));
        sbNumberOfEvents->setGeometry(QRect(160, 90, 61, 31));
        sbNumberOfEvents->setFont(font1);
        sbNumberOfEvents->setAlignment(Qt::AlignCenter);
        sbNumberOfEvents->setMinimum(2);
        sbNumberOfEvents->setMaximum(14);
        sbNumberOfEvents->setValue(7);
        btnOK = new QPushButton(SelectBehaviorLabels);
        btnOK->setObjectName(QStringLiteral("btnOK"));
        btnOK->setGeometry(QRect(230, 95, 41, 21));
        QFont font4;
        font4.setBold(true);
        font4.setWeight(75);
        btnOK->setFont(font4);
        btnOK->setStyleSheet(QLatin1String("background-color: rgb(0, 85, 127);\n"
"color: rgb(255, 255, 255);\n"
" border-radius:10px;"));
        tbTableEvents = new QTableWidget(SelectBehaviorLabels);
        tbTableEvents->setObjectName(QStringLiteral("tbTableEvents"));
        tbTableEvents->setGeometry(QRect(10, 150, 191, 192));
        tbTableEvents->setStyleSheet(QStringLiteral("background-color: rgb(220, 236, 255);"));
        tbTableEvents->setFrameShape(QFrame::StyledPanel);
        tbTableEvents->setFrameShadow(QFrame::Plain);
        tbTableEvents->setLineWidth(3);
        tbTableEvents->horizontalHeader()->setVisible(false);
        tbTableEvents->horizontalHeader()->setCascadingSectionResizes(false);
        tbTableEvents->verticalHeader()->setVisible(false);
        btnFinish = new QPushButton(SelectBehaviorLabels);
        btnFinish->setObjectName(QStringLiteral("btnFinish"));
        btnFinish->setGeometry(QRect(210, 310, 61, 31));
        QFont font5;
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        btnFinish->setFont(font5);
        btnFinish->setStyleSheet(QLatin1String("background-color: rgb(0, 85, 127);\n"
"color: rgb(255, 255, 255);\n"
" border-radius:10px;"));
        label_16 = new QLabel(SelectBehaviorLabels);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(148, 361, 81, 21));
        QFont font6;
        font6.setPointSize(13);
        font6.setBold(false);
        font6.setWeight(50);
        label_16->setFont(font6);
        label_16->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_16->setFrameShape(QFrame::NoFrame);
        label_16->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(SelectBehaviorLabels);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(70, 360, 81, 21));
        QFont font7;
        font7.setPointSize(15);
        font7.setBold(true);
        font7.setWeight(75);
        label_15->setFont(font7);
        label_15->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_15->setFrameShape(QFrame::NoFrame);
        label_15->setAlignment(Qt::AlignCenter);

        retranslateUi(SelectBehaviorLabels);

        QMetaObject::connectSlotsByName(SelectBehaviorLabels);
    } // setupUi

    void retranslateUi(QDialog *SelectBehaviorLabels)
    {
        SelectBehaviorLabels->setWindowTitle(QApplication::translate("SelectBehaviorLabels", "Behaviors' selection - CaT - z", Q_NULLPTR));
        label_26->setText(QString());
        lblVideo2_5->setText(QApplication::translate("SelectBehaviorLabels", "RGB-D DATA VISUALIZATION AND ANNOTATION GUI", Q_NULLPTR));
        label_14->setText(QApplication::translate("SelectBehaviorLabels", " by AG & PA. 2017", Q_NULLPTR));
        label_7->setText(QApplication::translate("SelectBehaviorLabels", "Behaviors' selection:", Q_NULLPTR));
        lblVideo2->setText(QApplication::translate("SelectBehaviorLabels", "Define number of behaviors to annotate:", Q_NULLPTR));
        lblVideo2_2->setText(QApplication::translate("SelectBehaviorLabels", "(min:2   max: 14)", Q_NULLPTR));
        btnOK->setText(QApplication::translate("SelectBehaviorLabels", "OK", Q_NULLPTR));
        btnFinish->setText(QApplication::translate("SelectBehaviorLabels", "SAVE", Q_NULLPTR));
        label_16->setText(QApplication::translate("SelectBehaviorLabels", "software", Q_NULLPTR));
        label_15->setText(QApplication::translate("SelectBehaviorLabels", "CaT - z", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SelectBehaviorLabels: public Ui_SelectBehaviorLabels {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTBEHAVIORLABELS_H
