
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QApplication>
#include <QObject>
#include <QSound>
#include <QDir>
#include <QStringList>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QtWidgets/QApplication>
#include <QTime>

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Kinect.h>

#include <stdio.h>      /* printf */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#include <math.h>
#include <bitset>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <direct.h>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

const int INF = 2147483647;

#include "kinectdata.h"

#undef foreach
#include "RealSenseCameraHandler.h"
#include "PreviewFrameHandler.h"
#include "PreviewDepthFrameHandler.h"
#include "ColorFrameHandler.h"
#include "ColorNotRegFrameHandler.h"

using namespace std;
using namespace cv;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

		int o() {

	};


public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:

    void on_btnPreview_clicked();

    void on_btnStart_clicked();

    void on_btnFileLocat2_clicked();

	 void on_btnSaveToPNG_clicked();

private:
    Ui::MainWindow *ui;

    std::pair<std::vector<double>, std::vector<double>> timestamps;

    int counter; 
    
    int timerAcq;
    
	facegram::cam::RealSenseCameraHandler cameraHandler;
    
	vector<Mat> depthVector;
    vector<float> timeVecColor;
    vector<float> timeVecDepth;
    vector<int> S_Color;
    vector<int> S_Depth;
    QString nameFileTAB1;
    
	QString dirTAB1;
   

 protected:
    void timerEvent(QTimerEvent *event);
    vector<float> setTime(vector<INT64> TimeVec);

};

#endif // MAINWINDOW_H
