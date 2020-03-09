
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
#include <QObject>
#include <QDir>
#include <QtCore>
#include <QFileDialog>

#include <iostream>
#include <fstream>
#include <Windows.h>

#include <stdio.h>      
#include <time.h>       

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

#include "kinectmodule.h"
#include "r.h"
#include "p.h"
#include "c1.h"
#include "c2.h"

using namespace std;
using namespace cv;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initConsumer(string, vector<int>, string);
    void initProducer();
    vector<float> setTimeVector(vector<INT64>);

    void startAll();
    void stopProducer();
    void stopConsumers();

private slots:
    void on_btnConnect_clicked();

    void on_btnSaveReal_clicked();

    void setTimerSingleFrame();

    void on_btnOpenDir_clicked();

private:
    Ui::MainWindow *ui;

    kinectModule kinectSensorOBJ;

    bool check1;

    CameraSpacePoint *depth2xyz; 
    ColorSpacePoint *depth2rgb;

    R r1;
    P *p;
    C1 *c1;
    C2 *c2;

    clock_t begin2, end2;
    int counterCamera; int activeTimer;
    QTimer *timerCamera;

    vector <double> TimesProd;

    string folderName; QString dirFile; string dirFileString;


};

#endif // MAINWINDOW_H
