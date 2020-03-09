
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
#include <QStandardItemModel>
#include <QDockWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QSignalMapper>
#include <QMessageBox>
#include <QToolTip>

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
#include <filesystem>
#include <dirent.h>
#include <numeric>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "selectbehaviorlabels.h"

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

private slots:
    void on_btnOpenDir_clicked();

    void valueChanged( int );
    void FrameTextChanged( const QString & );


    void on_btnSave_clicked();

    void btnTableDock_clicked();

    void btnTableDockDelete_clicked();

    void CellEditTextDockChanged(int RowNum);

    void CellComboBoxClicked (int RowNum);

    Mat DepthToBeVisualized (Mat);

    float medianMat(Mat);
    float GetMedian(vector<float>);

    void on_cbVisualization_currentIndexChanged(int index);

    void on_btnPlay_clicked();

    void setTimerSingleFrame();

    void on_btnStop_clicked();

    void on_btnFastF_clicked();

    void on_btnOpenAnnotationTool_clicked();

    void on_btnOpenFileResume_clicked();

private:
    Ui::MainWindow *ui;

    QString dirFile; string dirFileString;

    float timeStampsDepthInitial;
    float timeStampsRGBInitial;

    vector<float> timeStampsRGBVec;

    string nameFolderSave;
    bool fileNotSaved;

    bool countClick;

    bool YesItOpennedDir;

    vector<string> splitString(const string&, const char&);

    vector<string> listFiles(const char*);

    vector<string> listFileNames; int listFileSize;

    vector<int> behaviorVec;
    QStringList listNamesBehav; vector<QColor> colorBehavior; vector<QString> colorStringBehavior;

    QDockWidget *dock;
    QTableWidget *tablewidget; int countNewWindow;
    QPushButton* btnTableDock; QPushButton* btnTableDockDelete;

    bool correctItemBool;

    QSignalMapper EditTextSignalMapper; 
    int countNumb;

    QSignalMapper ComboBoxSignalMapper;

    QStringList listVisualizationCombo;

    QTimer *timerPlayVideo; int activePlayVideo;
    int frameNumberPlay; int velocityVideo;

    vector<string> contentsFileResume;
    string dirFileResumeString;

signals:
    void timeout();


};

#endif // MAINWINDOW_H
