
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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>

#include "QDebug"
#include <QCoreApplication>
#include <QDir>
#include <QObject>
#include <QTimer>


#include "BinFileFrameHandler.h"
#include "kinectdata.h"
#include "stdafx.h"

using namespace facegram;
using namespace facegram::cam;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {

    auto glambda = [](auto a, auto &&b) { return a < b; };
    ui->setupUi(this);

    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint & Qt::FramelessWindowHint);
    setFixedWidth(605);
    setFixedHeight(435);
    setWindowIcon(QIcon("Resources/RatLogo.ico"));

    timerAcq = 0;
    counter = 0;

    ui->btnFileLocat2->setEnabled(true);
    ui->lblWarningStart->setVisible(false);
    ui->lblWarningSave->setVisible(false);
    ui->btnSaveToPNG->setEnabled(false);

    if (!this->cameraHandler.isCameraConnected()) {
        ui->imageColorMap->setText("Connect camera");
        ui->imageDepthMap->setText("Connect camera");
        ui->btnPreview->setEnabled(false);
    } else {
        ui->imageColorMap->setText("Camera connected");
        ui->imageDepthMap->setText("Camera connected");
        ui->btnPreview->setText("Preview");
        ui->btnPreview->setEnabled(true);
    }

    
}

MainWindow::~MainWindow() {

    if (timerAcq != 0) {
        killTimer(timerAcq);
    }

    delete ui;
}

void MainWindow::on_btnPreview_clicked() {
    counter++;

    ui->imageColorMap->setText("");
    ui->imageDepthMap->setText("");

    if (ui->btnPreview->text() == "Connect camera") {
        ui->btnPreview->setText("STOP");
        timerAcq = startTimer(20);
    }

    else if (ui->btnPreview->text() == "Preview") {
        ui->btnPreview->setText("STOP");
        timerAcq = startTimer(20);
    }

    else {
        ui->btnPreview->setText("Preview");

        killTimer(timerAcq);
        timerAcq = 0;
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    facegram::cam::PreviewColorFrameHandler colorHandler(ui->imageColorMap);
    facegram::cam::PreviewDepthFrameHandler depthHandler(ui->imageDepthMap);
    cameraHandler.captureFrames(1, &colorHandler, &depthHandler, &colorHandler);
}

void MainWindow::on_btnFileLocat2_clicked() {

    killTimer(timerAcq);
    timerAcq = 0;

    ui->imageColorMap->clear();
    ui->imageDepthMap->clear();

    ui->btnPreview->setText("Preview");

    dirTAB1 = QFileDialog::getExistingDirectory(this, tr("Select directory:"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QPalette palet;
    if (dirTAB1.isEmpty()) {
        ui->edtNameFile->setEnabled(true);

        ui->edtNameFile->setStyleSheet("QLineEdit { color:rgb(170, 0, 0) }");
        ui->edtNameFile->setText("Select directory");

    } else {
        ui->edtNameFile->setStyleSheet("QLineEdit { color:rgb(130, 130, 130) }");
        ui->edtNameFile->setText(dirTAB1);
    }
}

void MainWindow::on_btnStart_clicked() {
    if (timerAcq != 0) {
        killTimer(timerAcq);
        timerAcq = 0;
        ui->btnPreview->setText("Preview");
    }

    ui->imageColorMap->clear();
    ui->imageDepthMap->clear();

    QCoreApplication::flush();

    QDate dateTAB1 = QDate::currentDate();
    QString dateStringTAB1 = dateTAB1.toString("dd.MM.yyyy");

    QTime timeTAB1 = QTime::currentTime();
    QString timeStringTAB1 = timeTAB1.toString();

    timeStringTAB1.replace(QString(":"), QString("."));

    if (counter == 0)
    {
        ui->lblWarningStart->setVisible(true);
        ui->lblWarningStart->setText("First connect camera");
        ui->lblWarningStart->setStyleSheet("QPushButton { color:rgb(170, 0, 0);}");

        ui->btnSaveToPNG->setEnabled(false);
    }

    else {

		ui->btnSaveToPNG->setEnabled(false);
        ui->btnPreview->setEnabled(false);
        ui->sbHours->setEnabled(false);
        ui->sbMinutes->setEnabled(false);
        ui->sbSeconds->setEnabled(false);
        ui->btnFileLocat2->setEnabled(false);

        nameFileTAB1 = dateStringTAB1 + "_" + timeStringTAB1;

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        int numberOfHours = ui->sbHours->value();
        int numberOfMinutes = ui->sbMinutes->value();
        int numberOfSeconds = ui->sbSeconds->value();

        if (numberOfHours == 0 && numberOfMinutes == 0 && numberOfSeconds == 0) // do not set timer -> click to Stop
        {

        }

        else 
        {
            int nFramesTotal = 30 * (numberOfHours * 3600 + numberOfMinutes * 60 + numberOfSeconds);

            ui->btnStart->setText("START");
            ui->btnStart->setStyleSheet("QPushButton { background-color: rgb(121, 121, 121); border-radius:10px; color: rgb(84, 84, 84);}");
            ui->lblWarningStart->setVisible(true);
            ui->lblWarningStart->setText("Please wait...");
            ui->lblWarningStart->setStyleSheet("QLabel { color: rgb(255, 255, 255);}");

            QCoreApplication::flush();
            QCoreApplication::processEvents();

            {
               
                string nameFileRGB = (dirTAB1 + "/" + nameFileTAB1 + ".avi").toStdString();
                string nameFileRGB_notReg = (dirTAB1 + "/" + nameFileTAB1 + "_notReg.avi").toStdString();
                string nameFileDepth = (dirTAB1 + "/" + nameFileTAB1 + ".BIN").toStdString();

                vector<int> compression_params;
                compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                compression_params.push_back(9);

                ColorFrameHandler colorHandler(nameFileRGB, 1280, 720, compression_params); 
                ColorNotRegFrameHandler colorNotRegHandler(nameFileRGB_notReg, 1920, 1080, compression_params);
				
				BinFileFrameHandler depthHandler(nameFileDepth, 1280, 720, nFramesTotal, compression_params);
                timestamps = cameraHandler.captureFrames(nFramesTotal, &colorHandler, &depthHandler, &colorNotRegHandler);
            }
        }

        QApplication::beep();

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        
        string nameFileTAB2 = nameFileTAB1.toStdString(); //without ".avi"
        string dirTAB2 = dirTAB1.toStdString();

        ofstream fileTColor = ofstream((dirTAB1.toStdString() + "/" + nameFileTAB1.toStdString() + "_TimeColor" + ".BIN"), ios::out | ios::binary);
        ofstream fileTDepth = ofstream((dirTAB1.toStdString() + "/" + nameFileTAB1.toStdString() + "_TimeDepth" + ".BIN"), ios::out | ios::binary);

        char *buf;

        if (fileTColor.is_open()) {
            for (int f = 0; f < timestamps.first.size(); f++) {

                fileTColor.write((char *)&timestamps.first[f], sizeof(double));
            }
            fileTColor.flush();
            fileTColor.close();
        }

        else {
            cout << "Unable to open file";
        };

        if (fileTDepth.is_open()) {
            for (int f = 0; f < timestamps.second.size(); f++) {

                fileTDepth.write((char *)&timestamps.second[f], sizeof(double));
            }
            fileTDepth.flush();
            fileTDepth.close();
        }

        
        ui->btnStart->setText("START");
        ui->btnStart->setStyleSheet("QPushButton { background-color: rgb(0, 68, 100); border-radius:10px; color: rgb(255, 255, 255);}");
        ui->lblWarningStart->setVisible(true);
        ui->lblWarningStart->setText("Data saved");
        ui->lblWarningStart->setStyleSheet("QLabel { color: rgb(255, 255, 255);}");
        
        ui->btnSaveToPNG->setEnabled(true);

        ui->btnPreview->setEnabled(true);
        ui->sbHours->setEnabled(true);
        ui->sbMinutes->setEnabled(true);
        ui->sbSeconds->setEnabled(true);
        ui->btnFileLocat2->setEnabled(true);
        QCoreApplication::flush();
        QCoreApplication::processEvents();
    }
}

void MainWindow::on_btnSaveToPNG_clicked() {

    
    ui->lblWarningStart->setVisible(false);
	ui->btnPreview->setEnabled(false);
    ui->sbHours->setEnabled(false);
    ui->sbMinutes->setEnabled(false);
    ui->sbSeconds->setEnabled(false);
    ui->btnFileLocat2->setEnabled(false);

    ui->btnStart->setEnabled(false);
    ui->lblWarningSave->setVisible(true);
    ui->lblWarningSave->setText("Please wait...");
    ui->lblWarningSave->setStyleSheet("QLabel { color: rgb(255, 255, 255);}");

    ui->btnSaveToPNG->setText("SAVE");
    ui->btnSaveToPNG->setStyleSheet("QPushButton { background-color: rgb(121, 121, 121); border-radius:10px; color: rgb(84, 84, 84);}");
    ui->btnSaveToPNG->setEnabled(false);
    QCoreApplication::flush();
    QCoreApplication::processEvents();

    char result[100];

    string folderNamePNG = "Frames_PNG_" + nameFileTAB1.toStdString();

    QString folderCreateCommand = QString::fromStdString(dirTAB1.toStdString() + "/" + folderNamePNG);
    QDir().mkdir(folderCreateCommand);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    fstream file2(dirTAB1.toStdString() + "/" + nameFileTAB1.toStdString() + ".BIN", ios::in | ios::binary);
    file2.seekg(0);

    int nframes;
    int h;
    int w;
    unsigned short value1;
    file2.read((char *)&nframes, sizeof(int));
    file2.read((char *)&h, sizeof(int)); //rows
    file2.read((char *)&w, sizeof(int)); //cols

    double previousTimeStamp = 0;
    int counterFramesDepthName = 0;

    for (int f = 0; f < nframes; f++) {
        Mat Dimage = Mat::zeros(h, w, CV_16UC1);
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {

                file2.read((char *)&value1, sizeof(unsigned short));
                Dimage.at<unsigned short>(r, c) = value1;
            }
        }

        if (f == 0) {
            unsigned long int fnum = timestamps.second[f];
            sprintf(result, "%09u_%09u_d.png", counterFramesDepthName, fnum);

            string nameDepth = dirTAB1.toStdString() + "/" + folderNamePNG + "/" + string(result);
            imwrite(nameDepth, Dimage, compression_params);

            previousTimeStamp = timestamps.second[f];
            counterFramesDepthName++;
        }

        else {

            if (previousTimeStamp == timestamps.second[f]) // do nothing if timestamps are the same
            {
            }

            else { 
				unsigned long int fnum = timestamps.second[f];
                sprintf(result, "%09u_%09u_d.png", counterFramesDepthName, fnum);

                string nameDepth = dirTAB1.toStdString() + "/" + folderNamePNG + "/" + string(result);
                imwrite(nameDepth, Dimage, compression_params);

                previousTimeStamp = timestamps.second[f];
                counterFramesDepthName++;
            }
        }
    }

    file2.close();

    string nameFileRGB2 = dirTAB1.toStdString() + "/" + nameFileTAB1.toStdString() + ".avi";

    VideoCapture cap2(nameFileRGB2); 

    if (!cap2.isOpened()) 
    {
        ui->lblWarningSave->setVisible(true);
        ui->lblWarningSave->setText("Error in AVI file");
        ui->lblWarningSave->setStyleSheet("QLabel { color:rgb(170, 0, 0);}");

    } else {

        ui->lblWarningSave->setVisible(false);

        previousTimeStamp = 0;
        int counterFramesRGBName = 0;

        for (int fFrames = 0; fFrames < nframes; fFrames++) {

            Mat frame2;

            bool bSuccess = cap2.read(frame2); 

            if (!bSuccess) 
            {
                ui->lblWarningSave->setVisible(true);
                ui->lblWarningSave->setText("Error in AVI file");
                ui->lblWarningSave->setStyleSheet("QLabel { color:rgb(170, 0, 0);}");
                break;
            }

            if (fFrames == 0) {
                
                unsigned long int fnum = timestamps.second[fFrames];
                sprintf(result, "%09u_%09u_c.png", counterFramesRGBName, fnum);

                string nameRGB = dirTAB1.toStdString() + "/" + folderNamePNG + "/" + string(result);
                imwrite(nameRGB, frame2, compression_params);

				previousTimeStamp = timestamps.second[fFrames];
                counterFramesRGBName++;
            }

            else {

                if (previousTimeStamp == timestamps.second[fFrames]) {
                } 
				else{

                    unsigned long int fnum = timestamps.second[fFrames];
                    sprintf(result, "%09u_%09u_c.png", counterFramesRGBName, fnum);

                    string nameRGB = dirTAB1.toStdString() + "/" + folderNamePNG + "/" + string(result);
                    imwrite(nameRGB, frame2, compression_params);

                    previousTimeStamp = timestamps.second[fFrames];
                    counterFramesRGBName++;
                }
            }
        }
    }

    if (cap2.isOpened()) {
        cap2.release();
    }

    ui->btnPreview->setEnabled(true);
    ui->sbHours->setEnabled(true);
    ui->sbMinutes->setEnabled(true);
    ui->sbSeconds->setEnabled(true);
    ui->btnFileLocat2->setEnabled(true);
    ui->btnSaveToPNG->setEnabled(true);

	ui->btnSaveToPNG->setText("SAVE");
    ui->btnSaveToPNG->setStyleSheet("QPushButton { background-color: rgb(121, 121, 121); border-radius:10px; color: rgb(84, 84, 84);}");
    ui->btnStart->setEnabled(true);
    ui->lblWarningSave->setVisible(true);
    ui->lblWarningSave->setText("All images saved.");
    ui->lblWarningSave->setStyleSheet("QLabel { color: rgb(255, 255, 255);}");
    QCoreApplication::flush();
    QCoreApplication::processEvents();
}

vector<float> MainWindow::setTime(vector<INT64> TimeVec) {
    vector<float> a;
    INT64 timeRef;

    timeRef = TimeVec[0];

    float numC;

    for (int i = 0; i < TimeVec.size(); i++) {
        TimeVec[i] = (TimeVec[i] - timeRef);
        numC = TimeVec[i] * (pow(10.0, -7.0));
        a.push_back(numC);
    }
    return a;
}
