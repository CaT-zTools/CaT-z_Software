/********************************************************************************
** Form generated from reading UI file 'selectdiagonal.ui'
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

#ifndef UI_SELECTDIAGONAL_H
#define UI_SELECTDIAGONAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <imagepointsevent.h>

QT_BEGIN_NAMESPACE

class Ui_selectDiagonal
{
public:
    imagePointsEvent *imagePoints;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *lblY;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *lblX;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QLineEdit *edtPoint1;
    QLabel *label_4;
    QLineEdit *edtPoint2;
    QLabel *lblErrorPoint;
    QFrame *frame;
    QLabel *label_26;
    QLabel *lblTitle;
    QFrame *frame_2;
    QLabel *label_27;

    void setupUi(QDialog *selectDiagonal)
    {
        if (selectDiagonal->objectName().isEmpty())
            selectDiagonal->setObjectName(QStringLiteral("selectDiagonal"));
        selectDiagonal->resize(785, 765);
        selectDiagonal->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        imagePoints = new imagePointsEvent(selectDiagonal);
        imagePoints->setObjectName(QStringLiteral("imagePoints"));
        imagePoints->setGeometry(QRect(8, 36, 768, 636));
        imagePoints->setMouseTracking(true);
        imagePoints->setFrameShape(QFrame::Box);
        imagePoints->setAlignment(Qt::AlignCenter);
        groupBox = new QGroupBox(selectDiagonal);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(8, 675, 181, 51));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        groupBox->setFont(font);
        groupBox->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(100, 23, 71, 17));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);

        lblY = new QLabel(layoutWidget);
        lblY->setObjectName(QStringLiteral("lblY"));
        lblY->setFrameShape(QFrame::Box);

        horizontalLayout_2->addWidget(lblY);

        layoutWidget_2 = new QWidget(groupBox);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 23, 71, 17));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        lblX = new QLabel(layoutWidget_2);
        lblX->setObjectName(QStringLiteral("lblX"));
        lblX->setFrameShape(QFrame::Box);

        horizontalLayout->addWidget(lblX);

        groupBox_2 = new QGroupBox(selectDiagonal);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(210, 675, 241, 51));
        groupBox_2->setFont(font);
        groupBox_2->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(8, 26, 41, 16));
        edtPoint1 = new QLineEdit(groupBox_2);
        edtPoint1->setObjectName(QStringLiteral("edtPoint1"));
        edtPoint1->setGeometry(QRect(49, 24, 61, 21));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(120, 26, 41, 16));
        edtPoint2 = new QLineEdit(groupBox_2);
        edtPoint2->setObjectName(QStringLiteral("edtPoint2"));
        edtPoint2->setGeometry(QRect(161, 24, 61, 21));
        lblErrorPoint = new QLabel(selectDiagonal);
        lblErrorPoint->setObjectName(QStringLiteral("lblErrorPoint"));
        lblErrorPoint->setGeometry(QRect(463, 701, 311, 20));
        QFont font1;
        font1.setPointSize(11);
        lblErrorPoint->setFont(font1);
        lblErrorPoint->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        frame = new QFrame(selectDiagonal);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 791, 31));
        QFont font2;
        font2.setFamily(QStringLiteral("Helvetica Neue"));
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setWeight(75);
        frame->setFont(font2);
        frame->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 127);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_26 = new QLabel(frame);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(780, 20, 201, 41));
        label_26->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_26->setScaledContents(true);
        lblTitle = new QLabel(frame);
        lblTitle->setObjectName(QStringLiteral("lblTitle"));
        lblTitle->setGeometry(QRect(10, 6, 761, 21));
        QFont font3;
        font3.setFamily(QStringLiteral("Calibri"));
        font3.setPointSize(14);
        font3.setBold(true);
        font3.setWeight(75);
        lblTitle->setFont(font3);
        lblTitle->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        lblTitle->setAlignment(Qt::AlignCenter);
        frame_2 = new QFrame(selectDiagonal);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(-10, 734, 801, 31));
        frame_2->setFont(font2);
        frame_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 127);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_27 = new QLabel(frame_2);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(780, 20, 201, 41));
        label_27->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_27->setScaledContents(true);

        retranslateUi(selectDiagonal);

        QMetaObject::connectSlotsByName(selectDiagonal);
    } // setupUi

    void retranslateUi(QDialog *selectDiagonal)
    {
        selectDiagonal->setWindowTitle(QApplication::translate("selectDiagonal", "Select diagonal points:", Q_NULLPTR));
        imagePoints->setText(QString());
        groupBox->setTitle(QApplication::translate("selectDiagonal", "Cursor's Position:", Q_NULLPTR));
        label_2->setText(QApplication::translate("selectDiagonal", "Y", Q_NULLPTR));
        lblY->setText(QString());
        label->setText(QApplication::translate("selectDiagonal", "X", Q_NULLPTR));
        lblX->setText(QString());
        groupBox_2->setTitle(QApplication::translate("selectDiagonal", "Points' Position:", Q_NULLPTR));
        label_3->setText(QApplication::translate("selectDiagonal", "Point 1", Q_NULLPTR));
        label_4->setText(QApplication::translate("selectDiagonal", "Point 2", Q_NULLPTR));
        lblErrorPoint->setText(QString());
        label_26->setText(QString());
        lblTitle->setText(QApplication::translate("selectDiagonal", "Select two diagonal and corner points to define region of interest (ROI): ", Q_NULLPTR));
        label_27->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class selectDiagonal: public Ui_selectDiagonal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTDIAGONAL_H
