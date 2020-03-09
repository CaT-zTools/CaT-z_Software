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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_15;
    QFrame *frame;
    QLabel *label_26;
    QFrame *frame_3;
    QLabel *label_28;
    QLabel *lblVideo2_6;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *lblDepth;
    QLabel *lblRGB;
    QLabel *label_5;
    QComboBox *cbVisualization;
    QLabel *label;
    QLineEdit *edtTimeStampFrame_2;
    QLineEdit *edtTimeStampFrame;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *lblMinFrames;
    QTableWidget *tEthogram;
    QLineEdit *edtBehaviorEvent;
    QPushButton *btnOpenAnnotationTool;
    QLabel *lblMaxFrames;
    QLineEdit *edtFrameNumber;
    QLabel *label_4;
    QLineEdit *edtElapsedTime;
    QScrollBar *sbFrames;
    QPushButton *btnFastF;
    QPushButton *btnPlay;
    QPushButton *btnStop;
    QLabel *label_7;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *btnOpenDir;
    QLineEdit *edtFileVideo;
    QLabel *lblVideo2;
    QPushButton *btnOpenFileResume;
    QLabel *lblVideo2_3;
    QLineEdit *edtFileResume;
    QPushButton *btnSave;
    QLabel *lblVideo2_2;
    QLineEdit *edtSaveFile;
    QFrame *line_2;
    QFrame *frame_9;
    QLabel *label_12;
    QFrame *frame_2;
    QLabel *label_27;
    QLabel *label_14;
    QLabel *lblBehavioralEthogram;
    QLabel *label_19;
    QLabel *label_20;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1043, 742);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(1085, 802, 61, 21));
        label_15->setStyleSheet(QStringLiteral("color: rgb(140, 140, 140);"));
        label_15->setFrameShape(QFrame::NoFrame);
        label_15->setAlignment(Qt::AlignCenter);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 0, 1151, 31));
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
        label_26->setGeometry(QRect(778, -4, 201, 41));
        label_26->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_26->setScaledContents(true);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(0, 0, 1151, 41));
        frame_3->setFont(font);
        frame_3->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 127);"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label_28 = new QLabel(frame_3);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(778, -4, 201, 41));
        label_28->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_28->setScaledContents(true);
        lblVideo2_6 = new QLabel(frame_3);
        lblVideo2_6->setObjectName(QStringLiteral("lblVideo2_6"));
        lblVideo2_6->setGeometry(QRect(10, 0, 501, 31));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        lblVideo2_6->setFont(font1);
        lblVideo2_6->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        lblVideo2_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2_6->setWordWrap(true);
        label_17 = new QLabel(frame);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(830, 723, 121, 21));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        label_17->setFont(font2);
        label_17->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_17->setFrameShape(QFrame::NoFrame);
        label_17->setAlignment(Qt::AlignCenter);
        label_18 = new QLabel(frame);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(953, 724, 111, 20));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        label_18->setFont(font3);
        label_18->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_18->setFrameShape(QFrame::NoFrame);
        label_18->setAlignment(Qt::AlignCenter);
        lblDepth = new QLabel(centralWidget);
        lblDepth->setObjectName(QStringLiteral("lblDepth"));
        lblDepth->setGeometry(QRect(524, 128, 512, 424));
        lblDepth->setFrameShape(QFrame::StyledPanel);
        lblRGB = new QLabel(centralWidget);
        lblRGB->setObjectName(QStringLiteral("lblRGB"));
        lblRGB->setGeometry(QRect(6, 128, 512, 424));
        lblRGB->setFrameShape(QFrame::StyledPanel);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(727, 107, 81, 20));
        QFont font4;
        font4.setPointSize(9);
        font4.setBold(true);
        font4.setWeight(75);
        label_5->setFont(font4);
        label_5->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        cbVisualization = new QComboBox(centralWidget);
        cbVisualization->setObjectName(QStringLiteral("cbVisualization"));
        cbVisualization->setGeometry(QRect(835, 107, 201, 19));
        QFont font5;
        font5.setBold(true);
        font5.setWeight(75);
        cbVisualization->setFont(font5);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 109, 71, 16));
        label->setFont(font4);
        label->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        edtTimeStampFrame_2 = new QLineEdit(centralWidget);
        edtTimeStampFrame_2->setObjectName(QStringLiteral("edtTimeStampFrame_2"));
        edtTimeStampFrame_2->setEnabled(false);
        edtTimeStampFrame_2->setGeometry(QRect(954, 557, 81, 16));
        QFont font6;
        font6.setFamily(QStringLiteral("Calibri"));
        font6.setPointSize(8);
        edtTimeStampFrame_2->setFont(font6);
        edtTimeStampFrame_2->setStyleSheet(QStringLiteral(""));
        edtTimeStampFrame_2->setInputMethodHints(Qt::ImhDigitsOnly);
        edtTimeStampFrame_2->setAlignment(Qt::AlignCenter);
        edtTimeStampFrame = new QLineEdit(centralWidget);
        edtTimeStampFrame->setObjectName(QStringLiteral("edtTimeStampFrame"));
        edtTimeStampFrame->setEnabled(false);
        edtTimeStampFrame->setGeometry(QRect(118, 557, 81, 16));
        edtTimeStampFrame->setFont(font6);
        edtTimeStampFrame->setFocusPolicy(Qt::StrongFocus);
        edtTimeStampFrame->setStyleSheet(QStringLiteral(""));
        edtTimeStampFrame->setInputMethodHints(Qt::ImhDigitsOnly);
        edtTimeStampFrame->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(851, 554, 101, 21));
        QFont font7;
        font7.setPointSize(8);
        font7.setBold(true);
        font7.setWeight(75);
        label_9->setFont(font7);
        label_9->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 554, 101, 21));
        label_8->setFont(font7);
        label_8->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 585, 51, 16));
        QFont font8;
        font8.setPointSize(10);
        font8.setBold(true);
        font8.setWeight(75);
        label_2->setFont(font8);
        label_2->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(465, 565, 115, 41));
        label_3->setFrameShape(QFrame::StyledPanel);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(588, 586, 51, 16));
        label_6->setFont(font8);
        label_6->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        lblMinFrames = new QLabel(centralWidget);
        lblMinFrames->setObjectName(QStringLiteral("lblMinFrames"));
        lblMinFrames->setGeometry(QRect(9, 596, 51, 16));
        lblMinFrames->setFont(font8);
        lblMinFrames->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        tEthogram = new QTableWidget(centralWidget);
        tEthogram->setObjectName(QStringLiteral("tEthogram"));
        tEthogram->setGeometry(QRect(7, 666, 1031, 31));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tEthogram->sizePolicy().hasHeightForWidth());
        tEthogram->setSizePolicy(sizePolicy);
        QFont font9;
        font9.setPointSize(4);
        tEthogram->setFont(font9);
        tEthogram->setFocusPolicy(Qt::NoFocus);
        tEthogram->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tEthogram->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tEthogram->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tEthogram->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tEthogram->setSelectionMode(QAbstractItemView::NoSelection);
        tEthogram->setShowGrid(false);
        tEthogram->horizontalHeader()->setVisible(false);
        tEthogram->verticalHeader()->setVisible(false);
        edtBehaviorEvent = new QLineEdit(centralWidget);
        edtBehaviorEvent->setObjectName(QStringLiteral("edtBehaviorEvent"));
        edtBehaviorEvent->setGeometry(QRect(641, 584, 151, 21));
        QFont font10;
        font10.setPointSize(11);
        edtBehaviorEvent->setFont(font10);
        edtBehaviorEvent->setStyleSheet(QStringLiteral(""));
        edtBehaviorEvent->setFrame(true);
        btnOpenAnnotationTool = new QPushButton(centralWidget);
        btnOpenAnnotationTool->setObjectName(QStringLiteral("btnOpenAnnotationTool"));
        btnOpenAnnotationTool->setGeometry(QRect(800, 584, 41, 21));
        btnOpenAnnotationTool->setFont(font5);
        btnOpenAnnotationTool->setStyleSheet(QLatin1String("background-color: rgb(0, 85, 127);\n"
"color: rgb(255, 255, 255);\n"
" border-radius:10px;"));
        lblMaxFrames = new QLabel(centralWidget);
        lblMaxFrames->setObjectName(QStringLiteral("lblMaxFrames"));
        lblMaxFrames->setGeometry(QRect(933, 597, 101, 16));
        lblMaxFrames->setFont(font8);
        lblMaxFrames->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        edtFrameNumber = new QLineEdit(centralWidget);
        edtFrameNumber->setObjectName(QStringLiteral("edtFrameNumber"));
        edtFrameNumber->setEnabled(false);
        edtFrameNumber->setGeometry(QRect(159, 584, 81, 21));
        QFont font11;
        font11.setFamily(QStringLiteral("Calibri"));
        font11.setPointSize(11);
        font11.setBold(false);
        font11.setWeight(50);
        edtFrameNumber->setFont(font11);
        edtFrameNumber->setStyleSheet(QStringLiteral(""));
        edtFrameNumber->setInputMethodHints(Qt::ImhDigitsOnly);
        edtFrameNumber->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(250, 585, 111, 16));
        label_4->setFont(font8);
        label_4->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        edtElapsedTime = new QLineEdit(centralWidget);
        edtElapsedTime->setObjectName(QStringLiteral("edtElapsedTime"));
        edtElapsedTime->setEnabled(false);
        edtElapsedTime->setGeometry(QRect(364, 584, 91, 21));
        edtElapsedTime->setFont(font11);
        edtElapsedTime->setStyleSheet(QStringLiteral(""));
        edtElapsedTime->setInputMethodHints(Qt::ImhDigitsOnly);
        edtElapsedTime->setAlignment(Qt::AlignCenter);
        sbFrames = new QScrollBar(centralWidget);
        sbFrames->setObjectName(QStringLiteral("sbFrames"));
        sbFrames->setGeometry(QRect(10, 617, 1031, 20));
        sbFrames->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 127);"));
        sbFrames->setOrientation(Qt::Horizontal);
        btnFastF = new QPushButton(centralWidget);
        btnFastF->setObjectName(QStringLiteral("btnFastF"));
        btnFastF->setGeometry(QRect(545, 571, 31, 31));
        btnPlay = new QPushButton(centralWidget);
        btnPlay->setObjectName(QStringLiteral("btnPlay"));
        btnPlay->setGeometry(QRect(507, 571, 31, 31));
        btnStop = new QPushButton(centralWidget);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        btnStop->setGeometry(QRect(469, 571, 31, 31));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 40, 211, 16));
        label_7->setFont(font4);
        label_7->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(357, 40, 191, 16));
        label_10->setFont(font4);
        label_10->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(663, 43, 171, 16));
        label_11->setFont(font4);
        label_11->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        btnOpenDir = new QPushButton(centralWidget);
        btnOpenDir->setObjectName(QStringLiteral("btnOpenDir"));
        btnOpenDir->setGeometry(QRect(302, 63, 27, 25));
        QFont font12;
        font12.setFamily(QStringLiteral("Calibri"));
        font12.setPointSize(13);
        font12.setBold(true);
        font12.setWeight(75);
        btnOpenDir->setFont(font12);
        btnOpenDir->setStyleSheet(QLatin1String("background-color:rgb(220, 220, 220);  border-radius:10px;\n"
"color: rgb(84, 84, 84);"));
        edtFileVideo = new QLineEdit(centralWidget);
        edtFileVideo->setObjectName(QStringLiteral("edtFileVideo"));
        edtFileVideo->setEnabled(false);
        edtFileVideo->setGeometry(QRect(113, 66, 185, 20));
        QFont font13;
        font13.setFamily(QStringLiteral("Calibri"));
        edtFileVideo->setFont(font13);
        edtFileVideo->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        edtFileVideo->setInputMethodHints(Qt::ImhDigitsOnly);
        edtFileVideo->setAlignment(Qt::AlignCenter);
        lblVideo2 = new QLabel(centralWidget);
        lblVideo2->setObjectName(QStringLiteral("lblVideo2"));
        lblVideo2->setGeometry(QRect(10, 64, 101, 21));
        lblVideo2->setFont(font4);
        lblVideo2->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        lblVideo2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2->setWordWrap(true);
        btnOpenFileResume = new QPushButton(centralWidget);
        btnOpenFileResume->setObjectName(QStringLiteral("btnOpenFileResume"));
        btnOpenFileResume->setGeometry(QRect(610, 63, 27, 25));
        btnOpenFileResume->setFont(font12);
        btnOpenFileResume->setStyleSheet(QLatin1String("background-color:rgb(220, 220, 220);  border-radius:10px;\n"
"color: rgb(84, 84, 84);"));
        lblVideo2_3 = new QLabel(centralWidget);
        lblVideo2_3->setObjectName(QStringLiteral("lblVideo2_3"));
        lblVideo2_3->setGeometry(QRect(356, 65, 91, 21));
        lblVideo2_3->setFont(font4);
        lblVideo2_3->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        lblVideo2_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2_3->setWordWrap(true);
        edtFileResume = new QLineEdit(centralWidget);
        edtFileResume->setObjectName(QStringLiteral("edtFileResume"));
        edtFileResume->setEnabled(false);
        edtFileResume->setGeometry(QRect(422, 66, 185, 20));
        edtFileResume->setFont(font13);
        edtFileResume->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        edtFileResume->setInputMethodHints(Qt::ImhDigitsOnly);
        edtFileResume->setAlignment(Qt::AlignCenter);
        btnSave = new QPushButton(centralWidget);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setGeometry(QRect(1000, 63, 27, 25));
        QFont font14;
        font14.setPointSize(13);
        font14.setBold(true);
        font14.setWeight(75);
        btnSave->setFont(font14);
        btnSave->setStyleSheet(QLatin1String("background-color:rgb(220, 220, 220);  border-radius:10px;\n"
"color: rgb(84, 84, 84);"));
        lblVideo2_2 = new QLabel(centralWidget);
        lblVideo2_2->setObjectName(QStringLiteral("lblVideo2_2"));
        lblVideo2_2->setGeometry(QRect(664, 65, 171, 21));
        lblVideo2_2->setFont(font4);
        lblVideo2_2->setStyleSheet(QStringLiteral("color: rgb(83, 83, 83);"));
        lblVideo2_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblVideo2_2->setWordWrap(true);
        edtSaveFile = new QLineEdit(centralWidget);
        edtSaveFile->setObjectName(QStringLiteral("edtSaveFile"));
        edtSaveFile->setEnabled(false);
        edtSaveFile->setGeometry(QRect(812, 67, 185, 21));
        edtSaveFile->setFont(font13);
        edtSaveFile->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        edtSaveFile->setInputMethodHints(Qt::ImhDigitsOnly);
        edtSaveFile->setAlignment(Qt::AlignCenter);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 102, 1200, 2));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy1);
        line_2->setBaseSize(QSize(0, 0));
        line_2->setStyleSheet(QStringLiteral("background:rgb(230, 230, 230)"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        frame_9 = new QFrame(centralWidget);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setGeometry(QRect(9, 641, 151, 21));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_9->sizePolicy().hasHeightForWidth());
        frame_9->setSizePolicy(sizePolicy2);
        frame_9->setMinimumSize(QSize(0, 0));
        frame_9->setBaseSize(QSize(0, 0));
        QFont font15;
        font15.setPointSize(11);
        font15.setBold(false);
        font15.setUnderline(false);
        font15.setWeight(50);
        frame_9->setFont(font15);
        frame_9->setStyleSheet(QLatin1String("background-color: rgb(220, 236, 255);\n"
""));
        frame_9->setFrameShape(QFrame::Panel);
        frame_9->setFrameShadow(QFrame::Plain);
        frame_9->setLineWidth(0);
        frame_9->setMidLineWidth(0);
        label_12 = new QLabel(frame_9);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 1, 131, 17));
        QFont font16;
        font16.setFamily(QStringLiteral("Helvetica Neue"));
        font16.setPointSize(9);
        font16.setBold(true);
        font16.setWeight(75);
        label_12->setFont(font16);
        label_12->setStyleSheet(QStringLiteral("color: rgb(85, 85, 85);"));
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(-24, 722, 1111, 31));
        frame_2->setFont(font);
        frame_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 85, 127);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_27 = new QLabel(frame_2);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(780, 20, 201, 41));
        label_27->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/Face3dYN/Face3dYN/Documents/Work/Face3Dyn/ap_face3dyn-11.jpg")));
        label_27->setScaledContents(true);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(929, 702, 111, 20));
        QFont font17;
        font17.setPointSize(9);
        font17.setBold(false);
        font17.setWeight(50);
        label_14->setFont(font17);
        label_14->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_14->setFrameShape(QFrame::NoFrame);
        label_14->setAlignment(Qt::AlignCenter);
        lblBehavioralEthogram = new QLabel(centralWidget);
        lblBehavioralEthogram->setObjectName(QStringLiteral("lblBehavioralEthogram"));
        lblBehavioralEthogram->setGeometry(QRect(171, 645, 291, 16));
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(842, 700, 81, 21));
        QFont font18;
        font18.setPointSize(12);
        font18.setBold(false);
        font18.setWeight(50);
        label_19->setFont(font18);
        label_19->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_19->setFrameShape(QFrame::NoFrame);
        label_19->setAlignment(Qt::AlignCenter);
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(764, 699, 81, 21));
        label_20->setFont(font14);
        label_20->setStyleSheet(QStringLiteral("color: rgb(0, 85, 127);"));
        label_20->setFrameShape(QFrame::NoFrame);
        label_20->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CaT-z : RGB-D DATA VISUALIZATION AND ANNOTATION GUI", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "AG. 2017", Q_NULLPTR));
        label_26->setText(QString());
        label_28->setText(QString());
        lblVideo2_6->setText(QApplication::translate("MainWindow", "RGB-D DATA VISUALIZATION AND ANNOTATION GUI", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "RAT3DPack", Q_NULLPTR));
        label_18->setText(QApplication::translate("MainWindow", " by AG & PA. 2017", Q_NULLPTR));
        lblDepth->setText(QString());
        lblRGB->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "Depth frame", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "RGB frame", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Timestamp (ms):", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Timestamp (ms):", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Frame: ", Q_NULLPTR));
        label_3->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "State:", Q_NULLPTR));
        lblMinFrames->setText(QApplication::translate("MainWindow", "MIN= 0", Q_NULLPTR));
        btnOpenAnnotationTool->setText(QApplication::translate("MainWindow", "OPEN", Q_NULLPTR));
        lblMaxFrames->setText(QApplication::translate("MainWindow", "MAX = ", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Elapsed time (s):", Q_NULLPTR));
        btnFastF->setText(QString());
        btnPlay->setText(QString());
        btnStop->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "Files' Selection - New annotation", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Resume previous annotation", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "Save annotation work", Q_NULLPTR));
        btnOpenDir->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        lblVideo2->setText(QApplication::translate("MainWindow", "Select directory:", Q_NULLPTR));
        btnOpenFileResume->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        lblVideo2_3->setText(QApplication::translate("MainWindow", "Select file:", Q_NULLPTR));
        btnSave->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        lblVideo2_2->setText(QApplication::translate("MainWindow", "Save data to directory:", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Behavioral ethogram:", Q_NULLPTR));
        label_27->setText(QString());
        label_14->setText(QApplication::translate("MainWindow", " by AG & PA. 2017", Q_NULLPTR));
        lblBehavioralEthogram->setText(QString());
        label_19->setText(QApplication::translate("MainWindow", "software", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "CaT - z", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
