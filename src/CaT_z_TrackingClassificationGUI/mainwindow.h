
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
#include <iomanip>
#include <Windows.h>

#include <stdio.h>      
#include <time.h>       

#include <math.h>
#include <bitset>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <filesystem>

#ifdef _WIN32
#include <direct.h>
#endif

#include <dirent.h>


#include "selectdiagonal.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml.hpp>

#define PI 3.14159265


using namespace std;
using namespace cv;
using namespace cv::ml;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vector <int> xROI;
    vector <int> yROI;

private slots:
    void on_btnOpenDir_clicked();

    void on_btnOpenAnnotatedFile_clicked();

    void on_btnOpenROI_clicked();

    void on_btnTrackingSTART_clicked();

    void on_btnSeeTracking_clicked();

    void on_btnStartClassification_clicked();

    void on_btnOpenDirectoryTracking_clicked();

    void on_btnStartNewClassification_clicked();

    void on_btnStartTesting_clicked();

    void on_btnOpenAnnotatedFileTesting_clicked();

    void on_btnOpenTrainedModel_2_clicked();

    void on_btnParameters_clicked();

    void on_btnOpenTrainedModel_3_clicked();


private:
    Ui::MainWindow *ui;

    int nFramesBackModel;

    QString dirFile; string dirFileString;
    vector<string> multipleDirectoryName;
    vector<string> multipleFolderName;

    bool sigROI;
    vector <int> point1DiagonalVec; vector <int> point2DiagonalVec;
    int borderROIX, borderROIY;

    vector<string> listFileNames; int listFileSize;

    vector<int> locationDepthVec; vector<int> locationRGBVec;
    
    vector<vector<int>> multipleLocationDepthVec;

    float timeStampsDepthInitial; float timeStampsRGBInitial;
    vector<float> timestampsDepthVec;
    
    vector<vector<float>> multipleTimestampsDepthVec;


    Mat depthInitModel;

    vector<Mat> realForegroundSubtracted; vector<Mat> activeBackground;

    int areaMaxRat; 
    float minRatRange;
    float maxRatRange;
    int obj_min_size_px;
    int medianKSize;

    vector<vector<Point>> contoursStatic;

    float percentageAreaRat;
    int taildiameter;

    float thresholdDiffFramesMov;

    int numbFramesCheckMov; int numberOfNextFrames;

    int minimumAreaRat;
    int maxSizeRat; 
    int minimumAreaPixels;

    int numberGMMSSSS; 

    Point pnt;

    int counterParameters;

    vector<vector<float>> centroidVectorFinal;
    vector<vector<float>> bodyPartsVectorFinal;
    vector<vector<Point>> contoursBodyParts;
    vector<vector<Point>> contoursRatSkeleton;
    double thresh_Radius;
    vector<vector<vector<float>>> multipleCentroidVectorFinal, multipleBodyPartsVectorFinal;

    vector<float> xCentroid;
    vector<float> yCentroid;
    vector<float> zCentroid;
    vector<int> indexesCentroid;
    vector<int> indexesBody;
    vector<vector<int>> multipleIndexesCentroid, multipleIndexesBody;
    vector<vector<float>> multiplexCentroid, multipleyCentroid, multiplezCentroid;


    int windowAngleHeadNose;
    int maxAreaPossibleRat;

    float thresholdRearing;

    
    int sizeTotal;
    Mat featureMatObject, featureMatObject_Multiple;
    Mat featureMatObject22Features;
    int featureNumber;
    int numberOfClass;
    Point2d initialCentroid;
    vector<vector<Point>> contours;
    int memoryTimeConstant;

    double bestC, bestG;

    int kFoldNum;
    vector <int>  indicesKFOLD;

    vector<string> contentsFileResume;
    string nameBehaviorsLabel;

    vector<int> labelsObject;
    
    vector<vector<int>> multipleLabelsObject;

    vector<int> labelsObject_totalMultiple;

    Mat trainMat; Mat testMat, trainMat2;
    vector<int> testFramesVec, trainFramesVec;

    vector<int> trainLabels; vector<int> testLabels;
    Mat testResponse;

    bool optimizeBool;

    Ptr<SVM> svmNew;

    
    float accuracyM;
    Mat confusionMatrix;
    vector <float> F1ScoreVec;


    void checkFilesDirectory(int, int);

    Mat DepthToBeVisualized (Mat);
    float medianMat(Mat);
    vector<string> splitString(const string&, const char&);
    vector<string> listFiles(const char*);

    void trackingAlgorithms(string, string);
    void Background_ForegroungModelling_GMMs_old ();
    void Background_ForegroungModelling_GMMs(string, string);
    bool checkMoveForeground(Mat, Mat);
    bool backgroundSubtractionGMMs(Mat, Mat, int, string);
    void backgroundSubtractionStatic(Mat, Mat, int, string);
    void backgroundSubtractionDYN(Mat, int, string);
    float pdf_gaussian (float, float, float);
    Mat medianDepthBack(string);
    float GetMedian(vector<float>);
    double distanceBetweenTwoPoints(double, double, double, double);

    Mat removeReflections(Mat, Mat);


    vector<float> centroidCalculation(Mat, int);
    vector<float> bodyPartsCalculation(Mat, vector<float>);
    vector <float> gaussianFilteringCorrectedCentroid(vector<float>, vector<int>);
    void thinningIteration(Mat&, int);
    void thinning(Mat&);
    void saveToCSVFileCentroid(string, vector<vector<float>>);
    void saveToCSVFileBodyParts(string, vector<vector<float>>);

    void saveToCSVFileFEATURES(string, Mat, vector<int>);


    void CreateFeatureObject(string);
    void featureAugmentation(bool);
    void featureNormalizationZ();
    void SVMtrain();
    void SVMevaluate();
    vector<int> crossValidationKfold_TestTrainMat();
    vector<int> shuffleArray(vector<int>,int);
    vector<int> kfold(int, int);
    void percentageSelection_TestTrainMat();
    vector<int> random90Train10Test(int);

    vector <float> movingAverageFilteringVelocity(vector<float>);
    vector <float> gaussianFilteringCorrectedVelocity(vector<float>, int);


};

#endif // MAINWINDOW_H
