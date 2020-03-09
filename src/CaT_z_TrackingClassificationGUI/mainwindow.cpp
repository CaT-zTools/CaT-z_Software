
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

MainWindow::
MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lblWarningROI->setVisible(false);

    nFramesBackModel = 300;
    medianKSize = 5;

    minRatRange = 5.0; 
    maxRatRange = 300.0;

    thresholdDiffFramesMov = 50;

    numbFramesCheckMov = 20;

    numberOfNextFrames = 3;

    percentageAreaRat = 0.75;
    areaMaxRat = 0;

    minimumAreaRat = 10;

    minimumAreaPixels = 300;

    maxSizeRat = 70;

    numberGMMSSSS = 500;

    taildiameter = 5;
    thresh_Radius = 0.3;

    windowAngleHeadNose = 9;

    thresholdRearing = 175.0;

    sizeTotal = 0;
    featureNumber = 11;
    numberOfClass = 8; 
    kFoldNum = 5;
    optimizeBool = true;
    memoryTimeConstant = 1000; 

    accuracyM = 0.0;

    obj_min_size_px = 100; 
	
	borderROIX = 60; 
    borderROIY= 30;

    maxAreaPossibleRat = 1500;

    ui->btnOpenROI->setEnabled(false);
    ui->btnTrackingSTART->setEnabled(false);
    ui->btnStartClassification->setEnabled(false);
    ui->lblTIRAR->setVisible(false);

    point1DiagonalVec.resize(2); point2DiagonalVec.resize(2);
    point1DiagonalVec[0] = 0; point1DiagonalVec[1] = 0; point2DiagonalVec[0] = 0; point2DiagonalVec[1] = 0;
    sigROI = false;

    ui->rBTrackingGMM->setEnabled(false);
    ui->rBTrackingMedian->setEnabled(false);
    ui->rBTrackingMedianDyn->setEnabled(false);

    ui->lblWarningTracking->setVisible(false);

    ui->lblSeeTracking->setVisible(false);
    ui->btnSeeTracking->setVisible(false);

    ui->pbProgressTrack->setVisible(false);
    ui->pbProgressClassification->setVisible(false);

    ui->btnStartNewClassification->setEnabled(false);
    ui->btnStartClassification->setEnabled(false);

    ui->btnStartTesting->setEnabled(false);

    ui->tabClassification->setCurrentIndex(0);
    ui->tabTrain->setEnabled(false);
    ui->tabClassify->setEnabled(false);
    ui->tabTesting->setEnabled(false);

    ui->gbParameters->setVisible(false);
    counterParameters = 0;

    ui->btnParameters->setVisible(true);

    ui->cbMultipleVideosForTraining->setToolTip("Training set for automatic classification composed by more than one RGB-D video");

	ui->cbEPM->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpenDir_clicked()
{

    if (ui->cbMultipleVideosForTraining->isChecked())
    {

        bool allOKMultiple = true;

        dirFile = QFileDialog::getExistingDirectory(this, tr("Select directory (main folder with multiple videos inside):"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (dirFile.isEmpty()){
            ui->edtFileVideo->setEnabled(true);

            ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
            ui->edtFileVideo->setText("Select directory!");

            allOKMultiple = false;
        }

        else
        {

            multipleDirectoryName.clear();
            multipleFolderName.clear();

            ui->edtFileVideo->setStyleSheet("QLineEdit {color: black;}");
            ui->edtFileVideo->setText(dirFile);

            QByteArray inBytes = dirFile.toUtf8();
            const char *pathConst = inBytes.constData();

            listFileNames = listFiles(pathConst);
            listFileSize = listFileNames.size();

            QStringList nameFilter;
            nameFilter << "*.csv";
            QDir Qdirect (dirFile);
            QFileInfoList list = Qdirect.entryInfoList(nameFilter, QDir::Files);

            Qdirect.setFilter(QDir::Dirs);
            QStringList folderNames = Qdirect.entryList(); 

            if (listFileSize== 0){
                ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                ui->edtFileVideo->setText("Empty folder!");
                allOKMultiple = false;
            }

            else if (list.isEmpty()) { 
                ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                ui->edtFileVideo->setText("Select folder with also .csv annotated files!");
                allOKMultiple = false;
            }

            else if (folderNames.size()-2 == 0)
            {
                ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                ui->edtFileVideo->setText("Select folder with subfolders also!");
                allOKMultiple = false;
            }

            else 
            {

                for (int fFolder = 2; fFolder < folderNames.size(); fFolder++)
                {
                    QString subFoldername = folderNames[fFolder];
                    string allnameDir = dirFile.toStdString() + "/" + subFoldername.toStdString();

                    QDir subDirec (QString::fromStdString(allnameDir));

                    QStringList nameFilter2;
                    nameFilter2 << "*.png";

                    QFileInfoList list2 = subDirec.entryInfoList(nameFilter2, QDir::Files);

                    QByteArray inBytes2 = QString::fromStdString(allnameDir).toUtf8();
                    const char *pathConst2 = inBytes2.constData();

                    vector<string> listFileNames2 = listFiles(pathConst2);
                    int listFileSize2 = listFileNames2.size();

                    if (list2.size() != listFileSize2)
                    {
                        ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                        ui->edtFileVideo->setText("Select folder with subfolders containing only .png files!");
                        allOKMultiple = false;
                    }

                    else if (list2.size() < nFramesBackModel*2)
                    {
                        ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                        ui->edtFileVideo->setText("Select folder with at least 300 .png depth files!");
                        allOKMultiple = false;
                    }

                    else 
                    {
                        multipleDirectoryName.push_back(allnameDir);
                        multipleFolderName.push_back(subFoldername.toStdString());
                        allOKMultiple = true;
                    }
                }

                if (allOKMultiple)
                {
                    ui->btnOpenROI->setEnabled(true);
                    ui->btnTrackingSTART->setEnabled(true);
                    ui->rBTrackingGMM->setEnabled(true);
                    ui->rBTrackingMedian->setEnabled(true);
                    ui->rBTrackingMedianDyn->setEnabled(true);
                }
            }




                // Select color and depth frames - vectors with position of frames in listFileNames object
                for (int i = 0; i<(listFileSize); i+=2)
                {
                    if (i>=listFileSize-1)
                    {
                        break;
                    }

                    else {
                        // Read two files - color and depth
                        string strName1 = listFileNames[i];
                        string strName2 = listFileNames[i+1];

                        // Split string into tokes -> separate frames from timestamps from name file (color/depth)
                        vector<string> v1 = splitString(strName1, '_');
                        vector<string> v2 = splitString(strName2, '_');

                        //First frame on the list
                        if (counterFramesTotal == 0){

                            // IF IT IS COLOR
                            if (v1[2]=="c.png"){
                                // Save color:
                                float timeStampC = stof(v1[1]);
                                timeStampsRGBInitial = timeStampC;

                                // Save depth:
                                float timeStampD = stof(v2[1]);
                                timeStampsDepthInitial = timeStampD;

                                locationRGBVec.push_back(i); locationDepthVec.push_back(i+1);
                                timestampsDepthVec.push_back(timeStampD);

                                counterFramesTotal++; counterFramesReal++;
                            }

                            // IF IT IS DEPTH
                            else {
                                // Save color:
                                float timeStampC = stof(v2[1]);
                                timeStampsRGBInitial = timeStampC;

                                // Save depth:
                                float timeStampD = stof(v1[1]);
                                timeStampsDepthInitial = timeStampD;

                                locationRGBVec.push_back(i+1); locationDepthVec.push_back(i);
                                timestampsDepthVec.push_back(timeStampD);

                                counterFramesTotal++; counterFramesReal++;
                            }
                        }

                        else { // Other frames:
                            // IF IT IS COLOR
                            if (v1[2]=="c.png"){
                                // Save color:
                                float timeStampC = stof(v1[1]);
                                float timeStampD = stof(v2[1]);

                                if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;} //Repeated timestamps/frames

                                //else if (timeStampD == timeStampsDepthInitial) {counterFramesTotal++;} //Repeated timestamps/frames

                                else {
                                    timeStampsRGBInitial = timeStampC;

                                    // Save depth:
                                    timeStampsDepthInitial = timeStampD;

                                    locationRGBVec.push_back(i); locationDepthVec.push_back(i+1);
                                    timestampsDepthVec.push_back(timeStampD);

                                    counterFramesReal++; counterFramesTotal++;
                                }
                            }

                            else {
                                // Save color:
                                float timeStampC = stof(v2[1]);
                                float timeStampD = stof(v1[1]);

                                if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;} //Repeated timestamps/frames

                                // else if (timeStampD == timeStampsDepthInitial) {counterFramesTotal++;} //Repeated timestamps/frames
                                else {
                                    timeStampsRGBInitial = timeStampC;

                                    // Save depth:
                                    timeStampsDepthInitial = timeStampD;

                                    locationRGBVec.push_back(i+1); locationDepthVec.push_back(i);
                                    timestampsDepthVec.push_back(timeStampD);

                                    counterFramesReal++;counterFramesTotal++;
                                }
                            }
                        }
                    }
                }


                if (locationDepthVec.size()<nFramesBackModel)
                {
                    ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                    ui->edtFileVideo->setText("Select folder with at least 300 valid depth files!");
                }

                else { // All ok! Desbloquear buttons!
                    ui->btnOpenROI->setEnabled(true);
                    ui->btnTrackingSTART->setEnabled(true);
                    ui->rBTrackingGMM->setEnabled(true);
                    ui->rBTrackingMedian->setEnabled(true);
                    ui->rBTrackingMedianDyn->setEnabled(true);
                }
            } */
        }

    }

    else 
    {

        int counterFramesTotal = 0;
        int counterFramesReal = 0;

        dirFile = QFileDialog::getExistingDirectory(this, tr("Select directory:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (dirFile.isEmpty()){
            ui->edtFileVideo->setEnabled(true);

            ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
            ui->edtFileVideo->setText("Select directory!");
        }

        else
        {
            ui->edtFileVideo->setStyleSheet("QLineEdit {color: black;}");
            ui->edtFileVideo->setText(dirFile);

            dirFileString = dirFile.toStdString();

            checkFilesDirectory(counterFramesTotal, counterFramesReal);
        }
    }
}


void MainWindow::checkFilesDirectory(int counterFramesTotal, int counterFramesReal)
{
    QByteArray inBytes = dirFile.toUtf8();
    const char *pathConst = inBytes.constData();

    listFileNames = listFiles(pathConst);
    listFileSize = listFileNames.size();

    QStringList nameFilter;
    nameFilter << "*.png";
    QDir Qdirect (dirFile);
    QFileInfoList list = Qdirect.entryInfoList( nameFilter, QDir::Files );

    if (listFileSize== 0){ 
        ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
        ui->edtFileVideo->setText("Empty folder!");
    }

    else if (list.isEmpty()) { 
        ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
        ui->edtFileVideo->setText("Select folder with .png files!");
    }

    else if (list.size() != listFileSize)
    {
        ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
        ui->edtFileVideo->setText("Select folder only with .png files!");
    }

    else if (list.size() < nFramesBackModel*2)
    {
        ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
        ui->edtFileVideo->setText("Select folder with at least 300 .png files!");
    }
    else { 

        list.clear();

        for (int i = 0; i<(listFileSize); i+=2)
        {
            if (i>=listFileSize-1)
            {
                break;
            }

            else {
                string strName1 = listFileNames[i];
                string strName2 = listFileNames[i+1];

                vector<string> v1 = splitString(strName1, '_');
                vector<string> v2 = splitString(strName2, '_');

                if (counterFramesTotal == 0){

                    if (v1[2]=="c.png"){
                        float timeStampC = stof(v1[1]);
                        timeStampsRGBInitial = timeStampC;

                        float timeStampD = stof(v2[1]);
                        timeStampsDepthInitial = timeStampD;

                        locationRGBVec.push_back(i); locationDepthVec.push_back(i+1);
                        timestampsDepthVec.push_back(timeStampD);

                        counterFramesTotal++; counterFramesReal++;
                    }

                    else {
                        float timeStampC = stof(v2[1]);
                        timeStampsRGBInitial = timeStampC;

                        float timeStampD = stof(v1[1]);
                        timeStampsDepthInitial = timeStampD;

                        locationRGBVec.push_back(i+1); locationDepthVec.push_back(i);
                        timestampsDepthVec.push_back(timeStampD);

                        counterFramesTotal++; counterFramesReal++;
                    }
                }

                else { 
					if (v1[2]=="c.png"){
                        float timeStampC = stof(v1[1]);
                        float timeStampD = stof(v2[1]);

                        if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;} 
						
						else {
                            timeStampsRGBInitial = timeStampC;

                            timeStampsDepthInitial = timeStampD;

                            locationRGBVec.push_back(i); locationDepthVec.push_back(i+1);
                            timestampsDepthVec.push_back(timeStampD);

                            counterFramesReal++; counterFramesTotal++;
                        }
                    }

                    else {
                        float timeStampC = stof(v2[1]);
                        float timeStampD = stof(v1[1]);

                        if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;} 
						
                        else {
                            timeStampsRGBInitial = timeStampC;

                            timeStampsDepthInitial = timeStampD;

                            locationRGBVec.push_back(i+1); locationDepthVec.push_back(i);
                            timestampsDepthVec.push_back(timeStampD);

                            counterFramesReal++;counterFramesTotal++;
                        }
                    }
                }
            }
        }


        if (locationDepthVec.size()<nFramesBackModel)
        {
            ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
            ui->edtFileVideo->setText("Select folder with at least 300 valid depth files!");
        }

        else {
            ui->btnOpenROI->setEnabled(true);
            ui->btnTrackingSTART->setEnabled(true);
            ui->rBTrackingGMM->setEnabled(true);
            ui->rBTrackingMedian->setEnabled(true);
            ui->rBTrackingMedianDyn->setEnabled(true);
        }
    }


}

vector<string> MainWindow::splitString(const string& s, const char& c) 
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
            if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}

vector<string> MainWindow::listFiles(const char* path){ 

    vector<string> listFileNames;
    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir(path) ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                listFileNames.push_back(string(entry->d_name));
        }
        closedir(pDIR);
    }

    return listFileNames;
}

Mat MainWindow::DepthToBeVisualized (Mat inputframe)
{

    Mat visualizInputframe = inputframe.clone();
    float medianVec = medianMat(inputframe);

    subtract(medianVec, inputframe, visualizInputframe);

    Mat visualizInputfram2e = visualizInputframe.clone();
    Mat visualizInputfram3e = visualizInputframe.clone();

    add(visualizInputfram2e, visualizInputfram3e, visualizInputframe);

    visualizInputframe.setTo(0, visualizInputframe >255);

    visualizInputframe.convertTo(visualizInputframe, CV_8U, 1, 0);
    return visualizInputframe;

}

float MainWindow::medianMat(Mat Input)
{
    Mat Input2 = Input.clone();
    Input2 = Input2.reshape(0,1);

    vector<float> vecFromMat;
    for (int i = 0; i<Input2.cols; i++)
    {
        vecFromMat.push_back(Input2.at<unsigned short>(i));
    }

    float medianVec = GetMedian(vecFromMat);

    return medianVec;
}

void MainWindow::on_btnOpenROI_clicked()
{

    string nameFileDF2;

    if (ui->cbMultipleVideosForTraining->isChecked())
    {

        string allnameDir = multipleDirectoryName[0];

        QByteArray inBytes2 = QString::fromStdString(allnameDir).toUtf8();
        const char *pathConst2 = inBytes2.constData();
        vector<string> listFileNames2 = listFiles(pathConst2);

        string strName1 = listFileNames2[0];
        string strName2 = listFileNames2[1];

        vector<string> v1 = splitString(strName1, '_');
        vector<string> v2 = splitString(strName2, '_');

        string strNameF1;

        if (v1[2]=="c.png"){
            strNameF1 = strName2;
        }
        else
        {
            strNameF1 = strName1;
        }

        nameFileDF2 = multipleDirectoryName[0] + "/" + strNameF1;
    }

    else
    {
        string strNameF1 = listFileNames[locationDepthVec[0]];
        nameFileDF2 = dirFileString + "/" + strNameF1;
    }

    Mat imageDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

    selectDiagonal *selD = new selectDiagonal(imageDepth);
    selD->setModal(true);
    selD->exec();

    point1DiagonalVec = selD->getPoint1(); 
    point2DiagonalVec = selD->getPoint2();

    if (point1DiagonalVec[0] == 0 && point1DiagonalVec[1] == 0 && point2DiagonalVec[0] == 0 && point2DiagonalVec[1] == 0) 
    {

        ui->lblWarningROI->setVisible(true);
        ui->lblWarningROI->setText("ROI with original size!");
        ui->lblWarningROI->setStyleSheet("QLabel {color: green;}");
        sigROI = false;
    }

    else
    {
        ui->lblWarningROI->setVisible(true);
        ui->lblWarningROI->setText("ROI selected!");
        ui->lblWarningROI->setStyleSheet("QLabel { color: green;}");
        sigROI = true;

        
        point1DiagonalVec[0] = round(point1DiagonalVec[0]/1.5); point1DiagonalVec[1] = round(point1DiagonalVec[1]/1.5);
        point2DiagonalVec[0] = round(point2DiagonalVec[0]/1.5); point2DiagonalVec[1] = round(point2DiagonalVec[1]/1.5);

        
        if (point1DiagonalVec[0] > point2DiagonalVec[0]){
            int a = point1DiagonalVec[0];
            point1DiagonalVec[0] = point2DiagonalVec[0];
            point2DiagonalVec[0] = a;
        }

        if (point1DiagonalVec[1] > point2DiagonalVec[1])
        {
            int b = point1DiagonalVec[1];
            point1DiagonalVec[1] = point2DiagonalVec[1];
            point2DiagonalVec[1] = b;
        }



    }
}

void MainWindow::on_btnTrackingSTART_clicked()
    if (ui->cbFullImageX->isChecked())
    {
        borderROIX = 0; 
    }
    else {borderROIX = ui->sbROIpixelsX->value();}

    if (ui->cbFullImageY->isChecked())
    {
        borderROIY = 0; 
    }
    else {borderROIY = ui->sbROIpixelsY->value();}

    minRatRange = ui->sbMinRange->value();
    maxRatRange = ui->sbMaxRange->value();

    nFramesBackModel = ui->sbFramesBackModel->value();

    maxAreaPossibleRat = ui->sbMaxPixelArea->value();

    if (point1DiagonalVec[0] == 0 && point1DiagonalVec[1] == 0 && point2DiagonalVec[0] == 0 && point2DiagonalVec[1] == 0)
    {
        sigROI = false;
    }

    else
    {
        ui->lblWarningROI->setVisible(false);
    }


    if (ui->cbMultipleVideosForTraining->isChecked())
    {
        for (int fFolder = 0; fFolder<multipleDirectoryName.size(); fFolder++)
        {
            int counterFramesTotal = 0;
            int counterFramesReal = 0;

            string allnameDir = multipleDirectoryName[fFolder];

            QByteArray inBytes = QString::fromStdString(allnameDir).toUtf8();

            const char *pathConst = inBytes.constData();

            listFileNames = listFiles(pathConst);
            listFileSize = listFileNames.size();

            for (int i = 0; i<(listFileSize); i+=2)
            {
                if (i>=listFileSize-1)
                {
                    break;
                }

                else {
                    string strName1 = listFileNames[i];
                    string strName2 = listFileNames[i+1];

                    vector<string> v1 = splitString(strName1, '_');
                    vector<string> v2 = splitString(strName2, '_');

                    if (counterFramesTotal == 0){

                        if (v1[2]=="c.png"){
                            float timeStampC = stof(v1[1]);
                            timeStampsRGBInitial = timeStampC;

                            float timeStampD = stof(v2[1]);
                            timeStampsDepthInitial = timeStampD;

                            locationRGBVec.push_back(i); locationDepthVec.push_back(i+1);
                            timestampsDepthVec.push_back(timeStampD);

                            counterFramesTotal++; counterFramesReal++;
                        }

                        else {
                            float timeStampC = stof(v2[1]);
                            timeStampsRGBInitial = timeStampC;

                            float timeStampD = stof(v1[1]);
                            timeStampsDepthInitial = timeStampD;

                            locationRGBVec.push_back(i+1); locationDepthVec.push_back(i);
                            timestampsDepthVec.push_back(timeStampD);

                            counterFramesTotal++; counterFramesReal++;
                        }
                    }

                    else { 
						if (v1[2]=="c.png"){
                            float timeStampC = stof(v1[1]);
                            float timeStampD = stof(v2[1]);

                            if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;}
                            else {
                                timeStampsRGBInitial = timeStampC;

                                timeStampsDepthInitial = timeStampD;

                                locationRGBVec.push_back(i); locationDepthVec.push_back(i+1);
                                timestampsDepthVec.push_back(timeStampD);

                                counterFramesReal++; counterFramesTotal++;
                            }
                        }

                        else {
                            float timeStampC = stof(v2[1]);
                            float timeStampD = stof(v1[1]);

                            if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;} 

                         
                            else {
                                timeStampsRGBInitial = timeStampC;

                                
                                timeStampsDepthInitial = timeStampD;

                                locationRGBVec.push_back(i+1); locationDepthVec.push_back(i);
                                timestampsDepthVec.push_back(timeStampD);

                                counterFramesReal++;counterFramesTotal++;
                            }
                        }
                    }
                }
            }

            mkdir((dirFile.toStdString() + "//" + multipleFolderName[fFolder] + "_trackedFrames").c_str());
            trackingAlgorithms(allnameDir, (dirFile.toStdString() + "//" + multipleFolderName[fFolder] + "_trackedFrames"));

            multipleLocationDepthVec.push_back(locationDepthVec);
            multipleTimestampsDepthVec.push_back(timestampsDepthVec);
            multipleIndexesCentroid.push_back(indexesCentroid);
            multipleIndexesBody.push_back(indexesBody);
            multipleCentroidVectorFinal.push_back(centroidVectorFinal);
            multipleBodyPartsVectorFinal.push_back(bodyPartsVectorFinal);
            multiplexCentroid.push_back(xCentroid); multipleyCentroid.push_back(yCentroid); multiplezCentroid.push_back(zCentroid);

            locationDepthVec.clear(); locationRGBVec.clear();
            timestampsDepthVec.clear();
            indexesCentroid.clear(); indexesBody.clear();
            centroidVectorFinal.clear();
            bodyPartsVectorFinal.clear();
            xCentroid.clear(); yCentroid.clear(); zCentroid.clear();
        }

    }

    else
    {
        mkdir((dirFileString + "//trackedFrames").c_str());

        centroidVectorFinal.clear();
        bodyPartsVectorFinal.clear();

        trackingAlgorithms(dirFileString, dirFileString + "//trackedFrames");
    }

    ui->btnOpenAnnotatedFile->setEnabled(true);
    sizeTotal = locationDepthVec.size();

    ui->tabTrain->setEnabled(true);
    ui->tabClassify->setEnabled(true);
    ui->tabTesting->setEnabled(true);
    ui->lblTIRAR->setVisible(false);

}

void MainWindow::trackingAlgorithms(string dirFileIndividual, string folderNameSaveTrackedFrames)
{
    if (ui->rBTrackingMedian->isChecked())
    {

        ui->lblSeeTracking->setVisible(false);
        ui->btnSeeTracking->setVisible(false);

        ui->lblWarningTracking->setVisible(false);
        ui->pbProgressTrack->setVisible(true);
        ui->lblTIRAR->setVisible(true);

        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->pbProgressTrack->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        realForegroundSubtracted.clear();


        Mat depthInitMedian = medianDepthBack(dirFileIndividual);

        medianBlur ( depthInitMedian, depthInitModel, medianKSize );

        for (int f = 0; f<locationDepthVec.size(); f++)
            
        {
            Mat frDepth;

            string strNameF1 = listFileNames[locationDepthVec[f]];

            string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
            frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

            if (sigROI){
                frDepth = frDepth(Rect(point1DiagonalVec[0] - borderROIX, point1DiagonalVec[1]- borderROIY, point2DiagonalVec[0]-point1DiagonalVec[0] + 2*borderROIX, point2DiagonalVec[1]-point1DiagonalVec[1] + 2*borderROIY));
            }


            if (ui->cbEPM->isChecked())
            {
                Mat maskBin = imread("imgBin_all.png", IMREAD_UNCHANGED);

                if (sigROI){
                    maskBin = maskBin(Rect(point1DiagonalVec[0] - borderROIX, point1DiagonalVec[1]- borderROIY, point2DiagonalVec[0]-point1DiagonalVec[0] + 2*borderROIX, point2DiagonalVec[1]-point1DiagonalVec[1] + 2*borderROIY));
                }

                frDepth.setTo(0, maskBin==0);
                depthInitModel.setTo(0, maskBin==0);
            }

            backgroundSubtractionStatic(depthInitModel, frDepth, f, folderNameSaveTrackedFrames);

            ui->lblTIRAR->setText("Segmentation...");

            ui->pbProgressTrack->setValue((f*100)/(locationDepthVec.size()));
            QCoreApplication::flush();
            QCoreApplication::processEvents();
        }

        ui->pbProgressTrack->setVisible(true);
        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->pbProgressTrack->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        indexesCentroid.clear();
        indexesBody.clear();

        for (int f = 0; f<locationDepthVec.size(); f++) 
            
        {
            string name = folderNameSaveTrackedFrames + "//" + to_string(f) + ".png";
            Mat finalForeground = imread(name, IMREAD_UNCHANGED);

            vector<float> centroidVectorFrame = centroidCalculation(finalForeground, f);
            centroidVectorFinal.push_back(centroidVectorFrame);


            ui->lblTIRAR->setText("Centroid calculation...");
            ui->pbProgressTrack->setValue((f*100)/(locationDepthVec.size()));
            QCoreApplication::flush();
            QCoreApplication::processEvents();

        }

        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->lblTIRAR->setText("Body parts' calculation...");
        ui->pbProgressTrack->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        for (int f = 0; f<locationDepthVec.size(); f++)    
        {
            string name = folderNameSaveTrackedFrames + "//" + to_string(f) + ".png";
            Mat finalForeground = imread(name, IMREAD_UNCHANGED);

            vector<float> bodyPartsVectorFrame = bodyPartsCalculation(finalForeground, centroidVectorFinal[f]);

            bodyPartsVectorFinal.push_back(bodyPartsVectorFrame);

            ui->pbProgressTrack->setValue((f*100)/(locationDepthVec.size()));
            QCoreApplication::flush();
            QCoreApplication::processEvents();
        }

        ui->pbProgressTrack->setVisible(false);

        ui->lblWarningTracking->setVisible(true);
        ui->lblWarningTracking->setText("End of Tracking!");
        ui->lblWarningTracking->setStyleSheet("QLabel {color: green;}");

        ui->lblSeeTracking->setVisible(true);
        ui->btnSeeTracking->setVisible(true);
        ui->lblTIRAR->setVisible(false);

    }

    else if (ui->rBTrackingMedianDyn->isChecked())
    {

        ui->lblSeeTracking->setVisible(false);
        ui->btnSeeTracking->setVisible(false);

        ui->lblWarningTracking->setVisible(false);
        ui->pbProgressTrack->setVisible(true);
        ui->lblTIRAR->setVisible(true);

        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->pbProgressTrack->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        realForegroundSubtracted.clear();

        nFramesBackModel = 100;
        Mat depthInitMedian = medianDepthBack(dirFileIndividual);

        medianBlur ( depthInitMedian, depthInitModel, 3 );

        for (int f = 0; f<locationDepthVec.size(); f++)
        {
            Mat frDepth;

            string strNameF1 = listFileNames[locationDepthVec[f]];

            string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
            frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

            if (sigROI){
                frDepth = frDepth(Rect(point1DiagonalVec[0] - borderROIX, point1DiagonalVec[1]- borderROIY, point2DiagonalVec[0]-point1DiagonalVec[0] + 2*borderROIX, point2DiagonalVec[1]-point1DiagonalVec[1] + 2*borderROIY));
            }

            backgroundSubtractionDYN(frDepth, f, folderNameSaveTrackedFrames);

            ui->lblTIRAR->setText("Segmentation...");
            ui->pbProgressTrack->setValue((f*100)/(locationDepthVec.size()));
            QCoreApplication::flush();
            QCoreApplication::processEvents();
        }

        
        ui->pbProgressTrack->setVisible(true);
        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->pbProgressTrack->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        
        for (int f = 0; f<locationDepthVec.size(); f++) 
        {
            string name = folderNameSaveTrackedFrames + "//" + to_string(f) + ".png";
            Mat finalForeground = imread(name, IMREAD_UNCHANGED);

            vector<float> centroidVectorFrame = centroidCalculation(finalForeground, f);
            centroidVectorFinal.push_back(centroidVectorFrame);

            ui->lblTIRAR->setText("Centroid calculation...");
        }

        for (int f = 0; f<locationDepthVec.size(); f++) 
        {
            string name = folderNameSaveTrackedFrames + "//" + to_string(f) + ".png";
            Mat finalForeground = imread(name, IMREAD_UNCHANGED);

            vector<float> bodyPartsVectorFrame = bodyPartsCalculation(finalForeground, centroidVectorFinal[f]);

            bodyPartsVectorFinal.push_back(bodyPartsVectorFrame);


            ui->lblTIRAR->setText("Body parts' calculation...");
            ui->pbProgressTrack->setValue((f*100)/(locationDepthVec.size()));
            QCoreApplication::flush();
            QCoreApplication::processEvents();
        }

        ui->pbProgressTrack->setVisible(false);

        ui->lblWarningTracking->setVisible(true);
        ui->lblWarningTracking->setText("End of Tracking!");
        ui->lblWarningTracking->setStyleSheet("QLabel {color: green;}");

        ui->lblSeeTracking->setVisible(true);
        ui->btnSeeTracking->setVisible(true);
        ui->lblTIRAR->setVisible(false);

    }

    else if (ui->rBTrackingGMM->isChecked()) 
    {
        ui->lblSeeTracking->setVisible(false);
        ui->btnSeeTracking->setVisible(false);

        ui->lblWarningTracking->setVisible(false);
        ui->pbProgressTrack->setVisible(true);
        ui->lblTIRAR->setVisible(true);

        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->pbProgressTrack->setValue(0);
        ui->lblTIRAR->setText("Segmentation...");
        QCoreApplication::flush();
        QCoreApplication::processEvents();


        realForegroundSubtracted.clear();

        depthInitModel = medianDepthBack(dirFileIndividual);

		Background_ForegroungModelling_GMMs(dirFileIndividual, folderNameSaveTrackedFrames);

        ui->pbProgressTrack->setVisible(true);
        ui->pbProgressTrack->setMinimum(0);
        ui->pbProgressTrack->setMaximum(100);
        ui->pbProgressTrack->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        for (int f = 0; f<locationDepthVec.size(); f++)
            
        {
            string name = folderNameSaveTrackedFrames + "//" + to_string(f) + ".png";
            Mat finalForeground = imread(name, IMREAD_UNCHANGED);

            vector<float> centroidVectorFrame = centroidCalculation(finalForeground, f);
            centroidVectorFinal.push_back(centroidVectorFrame);

            ui->lblTIRAR->setText("Centroid calculation...");
        }

        
        for (int f = 0; f<locationDepthVec.size(); f++) 
        {
            string name = folderNameSaveTrackedFrames + "//" + to_string(f) + ".png";
            Mat finalForeground = imread(name, IMREAD_UNCHANGED);

            vector<float> bodyPartsVectorFrame = bodyPartsCalculation(finalForeground, centroidVectorFinal[f]);

            bodyPartsVectorFinal.push_back(bodyPartsVectorFrame);


            ui->lblTIRAR->setText("Body parts' calculation...");
            ui->pbProgressTrack->setValue((f*100)/(locationDepthVec.size()));
            QCoreApplication::flush();
            QCoreApplication::processEvents();
        }

        ui->pbProgressTrack->setVisible(false);

        ui->lblWarningTracking->setVisible(true);
        ui->lblWarningTracking->setText("End of Tracking!");
        ui->lblWarningTracking->setStyleSheet("QLabel {color: green;}");

        ui->lblSeeTracking->setVisible(true);
        ui->btnSeeTracking->setVisible(true);

    }

    else
    {
        ui->lblSeeTracking->setVisible(false);
        ui->btnSeeTracking->setVisible(false);

        ui->lblTIRAR->setVisible(false);
        ui->lblWarningTracking->setVisible(true);
        ui->lblWarningTracking->setText("Please select the tracking technique!");
        ui->lblWarningTracking->setStyleSheet("QLabel {color: red;}");
    }

}

Mat MainWindow::medianDepthBack(string dirFileIndividual)
{
    int heightModel, widthModel;
    Mat frDepth;

    if (sigROI){

        heightModel = point2DiagonalVec[1]-point1DiagonalVec[1] + 2*borderROIY;
        widthModel = point2DiagonalVec[0]-point1DiagonalVec[0] + 2*borderROIX;;

    }

    else {

        string strNameF1 = listFileNames[locationDepthVec[0]];
        string nameFileDF2;
        if (ui->cbMultipleVideosForTraining->isChecked())
        {
            nameFileDF2 = dirFileIndividual + "/" + strNameF1;
        }
        else
        {
            nameFileDF2 = dirFileIndividual + "/" + strNameF1;
        }


        frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

        heightModel = frDepth.rows; widthModel = frDepth.cols;

    }

    vector<Mat> vecDepthFramesMedian;

    vector<int> indices(locationDepthVec.size());

    for (int i = 0; i < locationDepthVec.size(); i++ )
        indices[ i ] = i;

    indices = shuffleArray( indices, locationDepthVec.size() );

    for (int f = 0; f<nFramesBackModel; f++)
    {
        string strNameF1 = listFileNames[locationDepthVec[indices[f]]];
        string nameFileDF2 = dirFileIndividual + "/" + strNameF1;

        frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

        if (sigROI){
            frDepth = frDepth(Rect(point1DiagonalVec[0] - borderROIX, point1DiagonalVec[1]- borderROIY, point2DiagonalVec[0]-point1DiagonalVec[0] + 2*borderROIX, point2DiagonalVec[1]-point1DiagonalVec[1] + 2*borderROIY));
        }

        vecDepthFramesMedian.push_back(frDepth);
    }

    Mat frameDepthFinal =  Mat::zeros(heightModel,widthModel, CV_16UC1);

    vector<float> vectorMed;
    vectorMed.resize(nFramesBackModel);

    for (int i=0; i<heightModel; i++){
        for (int j = 0; j<widthModel; j++){
            for (int f = 0; f<nFramesBackModel; f++) 
            {
                frDepth = vecDepthFramesMedian[f];
                vectorMed[f] = (frDepth.at<unsigned short>(i, j));
            }


            float medV = GetMedian(vectorMed);
            frameDepthFinal.at<unsigned short>(i, j) = (unsigned short)medV;
        }
    }

    vecDepthFramesMedian.clear();
    return frameDepthFinal;

}

float MainWindow::GetMedian(vector<float> vectorM) {

    vector<float> dpSorted = vectorM;
    int len = dpSorted.size();

    nth_element( dpSorted.begin(), dpSorted.begin()+len/2, dpSorted.end() );

    float dMedian = dpSorted[len/2];

    return dMedian;
}

void MainWindow::Background_ForegroungModelling_GMMs (string dirFileIndividual, string folderNameSaveTrackedFrames)
{
    
    int height = depthInitModel.rows; int width = depthInitModel.cols;

    Mat bg_bw = depthInitModel.clone(); 
    Mat bg_bw_active = depthInitModel.clone(); 
    Mat fg = Mat::zeros(height,width, CV_16UC1);

    
    float C = 3;                                  
    int B = 3;                                    
    float D = 2.5;                                
    float alpha = 0.5;                           
    float thresh = 0.25;                          
    float sd_init = 2;                            

    vector<vector<vector<float>>> wV(height, vector<vector<float>>(width, vector<float>(3)));
    vector<vector<vector<float>>> meanV(height, vector<vector<float>>(width, vector<float>(3)));
    vector<vector<vector<float>>> sdV(height, vector<vector<float>>(width, vector<float>(3)));
    vector<vector<vector<float>>> u_diff(height, vector<vector<float>>(width, vector<float>(3)));

    float p = alpha/(1/C);                        
    vector<float> rank(3, 0);                      
    vector<int> rank_ind(3, 0);

    int match;

    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            for (int k = 0; k<C; k++)
            {
                meanV[i][j][k] = (float)(depthInitModel.at<unsigned short>(i, j)); 
                wV[i][j][k] = 1/(C);
                sdV[i][j][k] = sd_init;
                u_diff[i][j][k] = 0;
            }
        }
    }

    int fNew = 0;
    bool boolMovement = false;

    int f = 0;

    int counterSavingForeground = 0; 

    numberGMMSSSS = locationDepthVec.size();

    while (f<numberGMMSSSS)
    {
        Mat frDepth;

        if (f<numbFramesCheckMov) 
        {

            string strNameF1 = listFileNames[locationDepthVec[f]];
            string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
            frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

            if (sigROI){
                frDepth = frDepth(Rect(point1DiagonalVec[0] - borderROIX, point1DiagonalVec[1]- borderROIY, point2DiagonalVec[0]-point1DiagonalVec[0] + 2*borderROIX, point2DiagonalVec[1]-point1DiagonalVec[1] + 2*borderROIY));
            }

            for (int i = 0; i<height; i++){
                for (int j = 0; j<width; j++){
                    for (int m = 0; m<C; m++){
                        u_diff[i][j][m] = abs(frDepth.at<unsigned short>(i,j)-meanV[i][j][m]);
                    }
                }
            }

           for (int i = 0; i<height; i++){
                for (int j = 0; j<width; j++){

                    match = 0;

                    for (int k = 0; k<C; k++){

                        if (abs((u_diff[i][j][k]) <= (D*sdV[i][j][k]))) 
                        {
                            match = 1;

                            wV[i][j][k] = (1-alpha)* wV[i][j][k] + alpha;

                            p = alpha*(pdf_gaussian(frDepth.at<unsigned short>(i,j), meanV[i][j][k], sdV[i][j][k]));

                            meanV[i][j][k] = (1-p)*meanV[i][j][k] + p*(frDepth.at<unsigned short>(i,j));
                            sdV[i][j][k] = sqrt((1-p)*(pow((sdV[i][j][k]),2)) + p*(pow((frDepth.at<unsigned short>(i,j))-(meanV[i][j][k]),2)));

                        }

                        else
                        {
                            wV[i][j][k] = (1-alpha)*wV[i][j][k];
                        }
                    }

                   
                    float sumValueswV = 0;
                    for (int k = 0; k<C; k++)
                    {
                        sumValueswV = sumValueswV + wV[i][j][k];
                    }

                    for (int k = 0; k<C; k++)
                    {
                        wV[i][j][k] = wV[i][j][k]/ sumValueswV;
                    }

                    
                    bg_bw.at<unsigned short>(i,j) = (unsigned short)0;

                    for (int k = 0; k<C; k++)
                    {
                        bg_bw.at<unsigned short>(i,j)= bg_bw.at<unsigned short>(i,j)+ (meanV[i][j][k]*wV[i][j][k]);
                    }

                    if (match == 0)
                    {
                        float minV = wV[i][j][0];
                        int min_w_index = 0;

                        for (int k = 0; k<C; k++)
                        {
                            if (wV[i][j][k] < minV)
                            {
                                minV = wV[i][j][k];
                                min_w_index = k;
                            }
                        }

                        meanV[i][j][min_w_index] = (float)frDepth.at<unsigned short>(i,j);
                        sdV[i][j][min_w_index] = sd_init;
                    }

                    for (int k = 0; k<C; k++)
                    {
                        rank[k] = wV[i][j][k]/sdV[i][j][k];
                        rank_ind[k] = k;
                    }

                    for (int k = 1; k<C; k++)
                    {
                        for (int m = 0; m<(k-1); m++)
                        {
                            if (rank[k]>rank[m])
                            {
                                
                                float rank_temp = rank[m];
                                rank[m] = rank[k];
                                rank[k] = rank_temp;

                                int rank_ind_temp = rank_ind[m];
                                rank_ind[m] = rank_ind[k];
                                rank_ind[k] = rank_ind_temp;
                            }
                        }
                    }

                    match = 0;
                    int kT=0;

                    fg.at<unsigned short>(i,j) = (unsigned short)0;

                    while ((match == 0) && (kT<B))
                    {
                        if (wV[i][j][rank_ind[kT]] >= thresh) 
                        {
                            if (abs((u_diff[i][j][rank_ind[kT]]) <= (D*sdV[i][j][rank_ind[kT]]))) 
                            {
                                fg.at<unsigned short>(i,j) = (unsigned short)0;
                                match = 1;
                            }

                            else
                            {
                                fg.at<unsigned short>(i,j) = (frDepth.at<unsigned short>(i,j));
                            }
                        }
                        kT = kT+1;
                    }
                }
            }

            backgroundSubtractionStatic(bg_bw_active, frDepth, counterSavingForeground, folderNameSaveTrackedFrames);
            
            counterSavingForeground++;

            f++;
            ui->pbProgressTrack->setValue((f*100)/numberGMMSSSS);
            QCoreApplication::flush();
            QCoreApplication::processEvents();

        }

        else 
        {

            if ((f-1+numberOfNextFrames) >= numberGMMSSSS) 
            {
                for (int t = f; t<numberGMMSSSS; t++)
                {
                    string nameFileLast = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground-1) + ".png";
                    Mat lastMatForeground = imread(nameFileLast, IMREAD_UNCHANGED);

                    vector<int> compression_params;
                    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                    compression_params.push_back(9);

                    string nameFile = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground) + ".png";
                    imwrite(nameFile, lastMatForeground, compression_params);

                    counterSavingForeground++;

                    
                }

                f=numberGMMSSSS;
                ui->pbProgressTrack->setValue((f*100)/numberGMMSSSS);
                QCoreApplication::flush();
                QCoreApplication::processEvents();
            }

            else 
            {
                string strNameFP = listFileNames[locationDepthVec[f-1+numberOfNextFrames]];
                string nameFileDFP = dirFileIndividual + "/" + strNameFP;
                Mat frDepthNextframes = imread(nameFileDFP, IMREAD_UNCHANGED);

                string strNameF1 = listFileNames[locationDepthVec[f-1]];
                string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
                frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

                if (sigROI){
                    frDepth = frDepth(Rect(point1DiagonalVec[0], point1DiagonalVec[1], point2DiagonalVec[0]-point1DiagonalVec[0], point2DiagonalVec[1]-point1DiagonalVec[1]));
                    frDepthNextframes = frDepthNextframes(Rect(point1DiagonalVec[0], point1DiagonalVec[1], point2DiagonalVec[0]-point1DiagonalVec[0], point2DiagonalVec[1]-point1DiagonalVec[1]));
                }

                bool checkForegroundBool = checkMoveForeground(frDepthNextframes, frDepth);


                if (checkForegroundBool)
                {

                    string strNameF1 = listFileNames[locationDepthVec[f]];
                    string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
                    frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

                    if (sigROI){
                        frDepth = frDepth(Rect(point1DiagonalVec[0], point1DiagonalVec[1], point2DiagonalVec[0]-point1DiagonalVec[0], point2DiagonalVec[1]-point1DiagonalVec[1]));
                    }

                    for (int i = 0; i<height; i++){
                        for (int j = 0; j<width; j++){
                            for (int m = 0; m<C; m++){
                                u_diff[i][j][m] = abs(frDepth.at<unsigned short>(i,j)-meanV[i][j][m]);
                            }
                        }
                    }

                    for (int i = 0; i<height; i++){
                        for (int j = 0; j<width; j++){

                            match = 0;

                            for (int k = 0; k<C; k++){

                                if (abs((u_diff[i][j][k]) <= (D*sdV[i][j][k]))) // pixel matches component
                                {
                                    match = 1;

                                    wV[i][j][k] = (1-alpha)* wV[i][j][k] + alpha;

                                    p = alpha*(pdf_gaussian(frDepth.at<unsigned short>(i,j), meanV[i][j][k], sdV[i][j][k]));

                                    meanV[i][j][k] = (1-p)*meanV[i][j][k] + p*(frDepth.at<unsigned short>(i,j));
                                    sdV[i][j][k] = sqrt((1-p)*(pow((sdV[i][j][k]),2)) + p*(pow((frDepth.at<unsigned short>(i,j))-(meanV[i][j][k]),2)));

                                }

                                else 
                                {
                                    wV[i][j][k] = (1-alpha)*wV[i][j][k];
                                }
                            }

                            float sumValueswV = 0;
                            for (int k = 0; k<C; k++)
                            {
                                sumValueswV = sumValueswV + wV[i][j][k];
                            }

                            for (int k = 0; k<C; k++)
                            {
                                wV[i][j][k] = wV[i][j][k]/ sumValueswV;
                            }

                            bg_bw.at<unsigned short>(i,j) = (unsigned short)0;

                            for (int k = 0; k<C; k++)
                            {
                                bg_bw.at<unsigned short>(i,j)= bg_bw.at<unsigned short>(i,j)+ (meanV[i][j][k]*wV[i][j][k]);
                            }

                            if (match == 0)
                            {
                                float minV = wV[i][j][0];
                                int min_w_index = 0;

                                for (int k = 0; k<C; k++)
                                {
                                    if (wV[i][j][k] < minV)
                                    {
                                        minV = wV[i][j][k];
                                        min_w_index = k;
                                    }
                                }

                                meanV[i][j][min_w_index] = (float)frDepth.at<unsigned short>(i,j);
                                sdV[i][j][min_w_index] = sd_init;
                            }

                            for (int k = 0; k<C; k++)
                            {
                                rank[k] = wV[i][j][k]/sdV[i][j][k];
                                rank_ind[k] = k;
                            }

                            for (int k = 1; k<C; k++)
                            {
                                for (int m = 0; m<(k-1); m++)
                                {
                                    if (rank[k]>rank[m])
                                    {
                                        
                                        float rank_temp = rank[m];
                                        rank[m] = rank[k];
                                        rank[k] = rank_temp;

                                        int rank_ind_temp = rank_ind[m];
                                        rank_ind[m] = rank_ind[k];
                                        rank_ind[k] = rank_ind_temp;
                                    }
                                }
                            }

                            match = 0;
                            int kT=0;

                            fg.at<unsigned short>(i,j) = (unsigned short)0;

                            while ((match == 0) && (kT<B))
                            {
                                if (wV[i][j][rank_ind[kT]] >= thresh) 
                                {
                                    if (abs((u_diff[i][j][rank_ind[kT]]) <= (D*sdV[i][j][rank_ind[kT]]))) 
                                    {
                                        fg.at<unsigned short>(i,j) = (unsigned short)0;
                                        match = 1;
                                    }

                                    else
                                    {
                                        fg.at<unsigned short>(i,j) = (frDepth.at<unsigned short>(i,j));
                                    }
                                }
                                kT = kT+1;
                            }
                        }
                    }

                    bool boolNonRatArea = backgroundSubtractionGMMs(bg_bw, fg, counterSavingForeground, folderNameSaveTrackedFrames);

                    if (boolNonRatArea) 
                    {
                        string nameFileLast = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground-1) + ".png";
                        Mat lastMatForeground = imread(nameFileLast, IMREAD_UNCHANGED);

                        vector<int> compression_params;
                        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                        compression_params.push_back(9);

                        string nameFile = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground) + ".png";
                        imwrite(nameFile, lastMatForeground, compression_params);

                    }

                    else
                    {
                        bg_bw_active = bg_bw.clone();
                       
                    }


                    f++;
                    counterSavingForeground++;
                    ui->pbProgressTrack->setValue((f*100)/numberGMMSSSS);
                    QCoreApplication::flush();
                    QCoreApplication::processEvents();
                }

                else 
                {

                    for (int t = f; t<=(f-1 + numberOfNextFrames); t++) 
                    {
                        string nameFileLast = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground-1) + ".png";
                        Mat lastMatForeground = imread(nameFileLast, IMREAD_UNCHANGED);

                        vector<int> compression_params;
                        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                        compression_params.push_back(9);

                        string nameFile = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground) + ".png";
                        imwrite(nameFile, lastMatForeground, compression_params);

                        counterSavingForeground++;
                    }

                    fNew = (f + numberOfNextFrames);
                    boolMovement = false;

                    while (boolMovement == false)
                    {
                        if (fNew>=numberGMMSSSS) // end
                        {
                            f = fNew;
                            ui->pbProgressTrack->setValue(f);
                            break;
                        }

                        string strNameFP = listFileNames[locationDepthVec[fNew]];
                        string nameFileDFP = dirFileIndividual + "/" + strNameFP;
                        Mat frDepthNextframes = imread(nameFileDFP, IMREAD_UNCHANGED);

                        string strNameF1 = listFileNames[locationDepthVec[f-1]];
                        string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
                        frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

                        if (sigROI){
                            frDepth = frDepth(Rect(point1DiagonalVec[0], point1DiagonalVec[1], point2DiagonalVec[0]-point1DiagonalVec[0], point2DiagonalVec[1]-point1DiagonalVec[1]));
                            frDepthNextframes = frDepthNextframes(Rect(point1DiagonalVec[0], point1DiagonalVec[1], point2DiagonalVec[0]-point1DiagonalVec[0], point2DiagonalVec[1]-point1DiagonalVec[1]));
                        }

                        bool checkForegroundBool = checkMoveForeground(frDepthNextframes, frDepth);

                        if (checkForegroundBool) 
                        {

                            string strNameF1 = listFileNames[locationDepthVec[fNew]];
                            string nameFileDF2 = dirFileIndividual + "/" + strNameF1;
                            frDepth = imread(nameFileDF2, IMREAD_UNCHANGED);

                            if (sigROI){
                                frDepth = frDepth(Rect(point1DiagonalVec[0], point1DiagonalVec[1], point2DiagonalVec[0]-point1DiagonalVec[0], point2DiagonalVec[1]-point1DiagonalVec[1]));
                            }

                            for (int i = 0; i<height; i++){
                                for (int j = 0; j<width; j++){
                                    for (int m = 0; m<C; m++){
                                        u_diff[i][j][m] = abs(frDepth.at<unsigned short>(i,j)-meanV[i][j][m]);
                                    }
                                }
                            }

                            for (int i = 0; i<height; i++){
                                for (int j = 0; j<width; j++){

                                    match = 0;

                                    for (int k = 0; k<C; k++){

                                        if (abs((u_diff[i][j][k]) <= (D*sdV[i][j][k]))) 
                                        {
                                            match = 1;

                                            
                                            wV[i][j][k] = (1-alpha)* wV[i][j][k] + alpha;

                                            
                                            p = alpha*(pdf_gaussian(frDepth.at<unsigned short>(i,j), meanV[i][j][k], sdV[i][j][k]));

                                            meanV[i][j][k] = (1-p)*meanV[i][j][k] + p*(frDepth.at<unsigned short>(i,j));
                                            sdV[i][j][k] = sqrt((1-p)*(pow((sdV[i][j][k]),2)) + p*(pow((frDepth.at<unsigned short>(i,j))-(meanV[i][j][k]),2)));

                                        }

                                        else 
                                        {
                                            wV[i][j][k] = (1-alpha)*wV[i][j][k];
                                        }
                                    }

                                    float sumValueswV = 0;
                                    for (int k = 0; k<C; k++)
                                    {
                                        sumValueswV = sumValueswV + wV[i][j][k];
                                    }

                                    for (int k = 0; k<C; k++)
                                    {
                                        wV[i][j][k] = wV[i][j][k]/ sumValueswV;
                                    }

                                    bg_bw.at<unsigned short>(i,j) = (unsigned short)0;

                                    for (int k = 0; k<C; k++)
                                    {
                                        bg_bw.at<unsigned short>(i,j)= bg_bw.at<unsigned short>(i,j)+ (meanV[i][j][k]*wV[i][j][k]);
                                    }

                                    if (match == 0)
                                    {
                                        float minV = wV[i][j][0];
                                        int min_w_index = 0;

                                        for (int k = 0; k<C; k++)
                                        {
                                            if (wV[i][j][k] < minV)
                                            {
                                                minV = wV[i][j][k];
                                                min_w_index = k;
                                            }
                                        }

                                        meanV[i][j][min_w_index] = (float)frDepth.at<unsigned short>(i,j);
                                        sdV[i][j][min_w_index] = sd_init;
                                    }

                                    for (int k = 0; k<C; k++)
                                    {
                                        rank[k] = wV[i][j][k]/sdV[i][j][k];
                                        rank_ind[k] = k;
                                    }

                                    for (int k = 1; k<C; k++)
                                    {
                                        for (int m = 0; m<(k-1); m++)
                                        {
                                            if (rank[k]>rank[m])
                                            {
                                                
                                                float rank_temp = rank[m];
                                                rank[m] = rank[k];
                                                rank[k] = rank_temp;

                                                int rank_ind_temp = rank_ind[m];
                                                rank_ind[m] = rank_ind[k];
                                                rank_ind[k] = rank_ind_temp;
                                            }
                                        }
                                    }

                                    match = 0;
                                    int kT=0;

                                    
                                    fg.at<unsigned short>(i,j) = (unsigned short)0;

                                    while ((match == 0) && (kT<B))
                                    {
                                        if (wV[i][j][rank_ind[kT]] >= thresh)
                                        {
                                            if (abs((u_diff[i][j][rank_ind[kT]]) <= (D*sdV[i][j][rank_ind[kT]]))) 
                                            {
                                                fg.at<unsigned short>(i,j) = (unsigned short)0;
                                                match = 1;
                                            }

                                            else
                                            {
                                                fg.at<unsigned short>(i,j) = (frDepth.at<unsigned short>(i,j));
                                            }
                                        }
                                        kT = kT+1;
                                    }
                                }
                            }

                            backgroundSubtractionGMMs(bg_bw, fg, counterSavingForeground, folderNameSaveTrackedFrames);

                            bg_bw_active = bg_bw.clone();
                            
                            f = fNew+1;
                            counterSavingForeground++;

                            ui->pbProgressTrack->setValue((f*100)/numberGMMSSSS);
                            QCoreApplication::flush();
                            QCoreApplication::processEvents();

                            boolMovement = true;
                        }

                        else
                        {

                            string nameFileLast = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground-1) + ".png";
                            Mat lastMatForeground = imread(nameFileLast, IMREAD_UNCHANGED);

                            vector<int> compression_params;
                            compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                            compression_params.push_back(9);

                            string nameFile = folderNameSaveTrackedFrames + "//" + to_string(counterSavingForeground) + ".png";
                            imwrite(nameFile, lastMatForeground, compression_params);

                            counterSavingForeground++;

                        }

                        fNew = fNew+1;
                    }
                }
            }
        }
    }
}

bool MainWindow::checkMoveForeground(Mat frPreviousDepth, Mat frDepth)
{

    bool checkMove = false;

    Mat diffImage;
    absdiff(frPreviousDepth, frDepth, diffImage);

    Mat countValuesDistMat = diffImage.clone();
    countValuesDistMat.setTo(0, countValuesDistMat <=thresholdDiffFramesMov);

    int countDiff = countNonZero(countValuesDistMat);

    if (countDiff > percentageAreaRat*areaMaxRat) 
    {
        checkMove = true;
    }

    return checkMove;
}

float MainWindow::pdf_gaussian (float xV, float meanV, float stdV ) 
{
    float xGauss = ( 1 / ( stdV * sqrt(2*M_PI) ) ) * exp( -0.5 * pow( (xV-meanV)/stdV, 2.0 ) );
    return xGauss;
}

bool MainWindow::backgroundSubtractionGMMs(Mat depthBackModel, Mat depthForeground, int frameNumb, string folderNameSaveTrackedFrames) 
{

    bool boolNonRatArea = false;

    Mat maskForegroundFrame = cv::Mat::zeros(depthForeground.size(), depthForeground.type());
    Mat maskBackFrame = cv::Mat::zeros(depthForeground.size(), depthForeground.type());
    Mat subtractedDFrame = cv::Mat::zeros(depthForeground.size(), depthForeground.type());

    Mat binaryDTrack = depthForeground.clone();
    binaryDTrack.setTo(1, binaryDTrack != 0);

    const int connectivity_4 = 8;
    Mat labels, stats, centroids;

    binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);

    int nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    Mat areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 
    double minL, maxL;
    Point min_loc, max_loc;
    minMaxLoc(areaVector, &minL, &maxL, &min_loc, &max_loc); 

    

    if (maxL>minimumAreaRat)
    {

        binaryDTrack.setTo(0, labels != (max_loc.y+1));

        depthForeground.copyTo(maskForegroundFrame, binaryDTrack);

        depthBackModel.copyTo(maskBackFrame, binaryDTrack);

        absdiff(maskBackFrame, maskForegroundFrame, subtractedDFrame); 

        subtractedDFrame.setTo(0, subtractedDFrame <=minRatRange);
        subtractedDFrame.setTo(0, subtractedDFrame >=maxRatRange);

        subtractedDFrame.convertTo(subtractedDFrame,  CV_8U, 1,0);

        vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);

        string nameFile = folderNameSaveTrackedFrames + "//" + to_string(frameNumb) + ".png";
        imwrite(nameFile, subtractedDFrame, compression_params);
    

    }

    else
    {
        boolNonRatArea = true;
    }

    return boolNonRatArea;

}

void MainWindow::backgroundSubtractionStatic(Mat depthBackModel, Mat depthFrame, int frameNumb, string folderNameSaveTrackedFrames) 
{
    Mat subtractedDFrame = Mat::zeros(depthFrame.size(), depthFrame.type()); 
    Mat finalSegmentedFrame = Mat::zeros(depthFrame.size(), depthFrame.type()); 

    Mat depthFrameFilteredMedian = Mat::zeros(depthFrame.size(), depthFrame.type()); 

    medianBlur ( depthFrame, depthFrameFilteredMedian, 3 );

    subtract(depthBackModel, depthFrameFilteredMedian, subtractedDFrame); 

	Mat auxFrame = depthFrameFilteredMedian.clone();

    subtractedDFrame.setTo(0, subtractedDFrame <=minRatRange);
    subtractedDFrame.setTo(0, subtractedDFrame >=maxRatRange);

    subtractedDFrame.setTo(0.0, auxFrame == 0.0);

	Mat binaryDTrack = subtractedDFrame.clone();
    binaryDTrack.setTo(1, binaryDTrack != 0);

    const int connectivity_4 = 8;
    Mat labels, stats, centroids;

    binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);

    int nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    Mat areaVector = stats(cv::Rect(4,1,1,nLabels-1));

    areaVector.setTo(0, areaVector > minimumAreaPixels);
    areaVector.convertTo(areaVector, CV_8U, 1, 0);

    Mat locations;   
    findNonZero(areaVector, locations);

    for (int i = 0; i < locations.total(); i++ ) {

        pnt = locations.at<Point>(i); 
        binaryDTrack.setTo(0, labels == (pnt.y+1));

    }

    findNonZero(binaryDTrack, locations);
    vector<int> coordX, coordY;

    for (int i = 0; i < locations.total(); i++ ) {

        pnt = locations.at<Point>(i); 

        coordX.push_back(pnt.x);
        coordY.push_back(pnt.y);
    }

    double minX, minY, maxX, maxY;
    Point min_locX, max_locX, min_locY, max_locY;
    minMaxLoc(coordX, &minX, &maxX, &min_locX, &max_locX);

    minMaxLoc(coordY, &minY, &maxY, &min_locY, &max_locY);

    double amplitudeMaxX = maxX-minX;
    double amplitudeMaxY = maxY-minY;


    if (amplitudeMaxX>2*maxSizeRat || amplitudeMaxY>2*maxSizeRat) 
    {
        Mat destMat;

        Mat se = Mat::ones(3,1, binaryDTrack.type());
        erode( binaryDTrack, destMat, se );

        se = Mat::ones(1,3, binaryDTrack.type());
        erode( destMat, destMat, se );

        se = Mat::ones(3,3, binaryDTrack.type());
        dilate(destMat, destMat, se );

		nLabels = connectedComponentsWithStats(destMat, labels, stats, centroids, connectivity_4, CV_32S);
        areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 

        areaVector.setTo(0, areaVector > minimumAreaPixels);
        areaVector.convertTo(areaVector, CV_8U, 1, 0);

        findNonZero(areaVector, locations);

        for (int i = 0; i < locations.total(); i++ ) {

            pnt = locations.at<Point>(i); 
            destMat.setTo(0, labels == (pnt.y+1));

        }

        binaryDTrack = destMat.clone();
    }

    Mat binaryFinalTrack = removeReflections(binaryDTrack, subtractedDFrame);

    Mat bwImage_toFill=binaryFinalTrack.clone();

    findContours(bwImage_toFill, contoursStatic, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    drawContours(bwImage_toFill, contoursStatic, -1, Scalar(1), CV_FILLED);

    int nLabelsGMMs = connectedComponentsWithStats(bwImage_toFill, labels, stats, centroids, connectivity_4, CV_32S);
    Mat areaVectorGMMs = stats(cv::Rect(4,1,1,nLabelsGMMs-1)); 
    double minL, maxL;
    Point min_loc, max_loc;
    minMaxLoc(areaVectorGMMs, &minL, &maxL, &min_loc, &max_loc); 
    if (frameNumb == 0)
    {
        areaMaxRat = maxL;
    }

    subtractedDFrame.copyTo(finalSegmentedFrame, bwImage_toFill);

    binaryDTrack = finalSegmentedFrame.clone();
    binaryDTrack.setTo(1, finalSegmentedFrame != 0);

	binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);

    nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 

    areaVector.setTo(0, areaVector > minimumAreaPixels);
    areaVector.convertTo(areaVector, CV_8U, 1, 0);

    findNonZero(areaVector, locations);

    for (int i = 0; i < locations.total(); i++ ) {

        pnt = locations.at<Point>(i);
        finalSegmentedFrame.setTo(0, labels == (pnt.y+1));
    }


    Mat newF;
    finalSegmentedFrame.convertTo(newF,  CV_8U, 1,0);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    string nameFile = folderNameSaveTrackedFrames + "//" + to_string(frameNumb) + ".png";
    imwrite(nameFile, newF, compression_params);

    finalSegmentedFrame.release(); subtractedDFrame.release(); auxFrame.release(); binaryDTrack.release(); areaVector.release();

}

Mat MainWindow::removeReflections(Mat inputFrame, Mat depthValuesFrame)
{

    Mat outputFrame = inputFrame.clone(); 
    Point pnt, pntAgain, pntLocations, pntLocations2, pnt3;

    const int connectivity_4 = 8;
    Mat labels, stats, centroids;

    int nLabels = connectedComponentsWithStats(inputFrame, labels, stats, centroids, connectivity_4, CV_32S);

    if (nLabels > 2) 
    {

        for (int eachObject = 1; eachObject<nLabels; eachObject++) 
        {
            Point2d point1 = centroids.at<Point2d>(eachObject,0);

            if (point1.x < borderROIX || point1.x > (inputFrame.cols-borderROIX))
            {
                Mat pixelLabel = Mat::zeros(inputFrame.size(), inputFrame.type());
                pixelLabel.setTo(1, labels == eachObject);

                Mat locationsMask;
                findNonZero(pixelLabel, locationsMask);

                Mat outside1X = Mat::zeros(inputFrame.size(), inputFrame.type());
                Mat outside2X = Mat::zeros(inputFrame.size(), inputFrame.type());
                Mat outside1Y = Mat::zeros(inputFrame.size(), inputFrame.type());
                Mat outside2Y = Mat::zeros(inputFrame.size(), inputFrame.type());

                for (int i = 0; i < locationsMask.total(); i++ ) {

                    pnt = locationsMask.at<Point>(i); // access pixel coordinates

                    if (pnt.x<borderROIX)
                    {
                        outside1X.at<uchar>(pnt.y, pnt.x) = 1; // if all pixels are outside ROI - x coordinate
                    }

                    if (pnt.x>(inputFrame.cols-borderROIX))
                    {
                        outside2X.at<uchar>(pnt.y, pnt.x) = 1;
                    }

                    if (pnt.y<borderROIY)
                    {
                        outside1Y.at<uchar>(pnt.y, pnt.x) = 1;
                    }

                    if (pnt.y>(inputFrame.rows-borderROIY))
                    {
                        outside2Y.at<uchar>(pnt.y, pnt.x) = 1; // % if all pixels are outside ROI - y coordinate
                    }
                }

                Mat locationsOutside1X, locationsOutside2X, locationsOutside1Y, locationsOutside2Y;   // output, locations of non-zero pixels
                findNonZero(outside1X, locationsOutside1X);
                findNonZero(outside2X, locationsOutside2X);
                findNonZero(outside1Y, locationsOutside1Y);
                findNonZero(outside2Y, locationsOutside2Y);

                if (locationsOutside1X.total() == locationsMask.total()) // if all pixels are outside ROI - x coordinate
                {
                    outputFrame.setTo(0, labels==eachObject);
                }

                else if (locationsOutside2X.total() == locationsMask.total()) // if all pixels are outside ROI - x coordinate
                {
                    outputFrame.setTo(0, labels==eachObject);
                }

                else if (locationsOutside1Y.total() == locationsMask.total()) // if all pixels are outside ROI - y coordinate
                {
                    outputFrame.setTo(0, labels==eachObject);
                }

                else if (locationsOutside2Y.total() == locationsMask.total()) // if all pixels are outside ROI - y coordinate
                {
                    outputFrame.setTo(0, labels==eachObject);
                }

                else // do nothing
                {}*/

                int numberPixelsOutROI = 0;
                for (int i = 0; i < locationsMask.total(); i++ )
                {
                    pnt = locationsMask.at<Point>(i);

                    if (pnt.x<borderROIX) 
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else if (pnt.x>(inputFrame.cols-borderROIX))
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else if (pnt.y<borderROIY)
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else if (pnt.y>(inputFrame.rows-borderROIY))
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else
                    {}
                }

                if (numberPixelsOutROI == locationsMask.total())
                {
                    outputFrame.setTo(0, labels==eachObject);
                }

                else
                {}

            }

            else if (point1.y < borderROIY || point1.y > (inputFrame.rows-borderROIY))
            {
                Mat pixelLabel = Mat::zeros(inputFrame.size(), inputFrame.type());
                pixelLabel.setTo(1, labels == eachObject);

                Mat locationsMask;   
                findNonZero(pixelLabel, locationsMask);

                int numberPixelsOutROI = 0;
                for (int i = 0; i < locationsMask.total(); i++ )
                {
                    pnt = locationsMask.at<Point>(i); 

                    if (pnt.x<borderROIX)
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else if (pnt.x>(inputFrame.cols-borderROIX))
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else if (pnt.y<borderROIY)
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else if (pnt.y>(inputFrame.rows-borderROIY))
                    {numberPixelsOutROI = numberPixelsOutROI+1;}

                    else
                    {}
                }

                if (numberPixelsOutROI == locationsMask.total())
                {
                    outputFrame.setTo(0, labels==eachObject);
                }

                else
                {}
            }

            
            else
            {}
        }

        Mat labelsAgain, statsAgain, centroidsAgain;
        vector<int> numberPixelsOutside;

        int nLabelsAgain = connectedComponentsWithStats(outputFrame, labelsAgain, statsAgain, centroidsAgain, connectivity_4, CV_32S);

        if (nLabelsAgain > 2) 
        {
            Mat areaVector = statsAgain(cv::Rect(4,1,1,nLabelsAgain-1));
            areaVector.setTo(0, areaVector > minimumAreaPixels);
            areaVector.convertTo(areaVector, CV_8U, 1, 0);
            Mat locationsArea;   
            findNonZero(areaVector, locationsArea);
            Point pnt2;
            for (int i = 0; i < locationsArea.total(); i++ )
            {
                pnt2 = locationsArea.at<Point>(i); 
                outputFrame.setTo(0, labelsAgain == (pnt2.y+1));
            }

            for (int eachObject = 1; eachObject<nLabelsAgain; eachObject++) 
            {
                int numberPixelsOut = 0;

                Mat pixelLabelAgain = Mat::zeros(inputFrame.size(), inputFrame.type());
                pixelLabelAgain.setTo(1, labelsAgain == eachObject);

                Mat locationsMaskAgain;   
                findNonZero(pixelLabelAgain, locationsMaskAgain);

                for (int i = 0; i < locationsMaskAgain.total(); i++ )
                {
                    pntAgain = locationsMaskAgain.at<Point>(i); 

                    if (pntAgain.x<borderROIX)
                    {numberPixelsOut = numberPixelsOut+1;}

                    else if (pntAgain.x>(inputFrame.cols-borderROIX))
                    {numberPixelsOut = numberPixelsOut+1;}

                    else if (pntAgain.y<borderROIY)
                    {numberPixelsOut = numberPixelsOut+1;}

                    else if (pntAgain.y>(inputFrame.rows-borderROIY))
                    {numberPixelsOut = numberPixelsOut+1;}

                    else
                    {}
                }

                numberPixelsOutside.push_back(numberPixelsOut); 
            }

            double minX, maxX;
            Point min_locX, max_locX;

            minMaxLoc(numberPixelsOutside, &minX, &maxX, &min_locX, &max_locX);

            if (maxX==0)
            {}

            else
            {
                outputFrame.setTo(0, labelsAgain == (max_locX.x+1));
            }
        }



    }

    else 
    {}

    int countDiffArea = countNonZero(outputFrame);

    if (countDiffArea>maxAreaPossibleRat) 
    {
        
        Mat checkDepthValuesFrame = Mat::zeros(depthValuesFrame.size(), depthValuesFrame.type()); 
        depthValuesFrame.copyTo(checkDepthValuesFrame, outputFrame);

        Mat locationsMaskOutputFrame;   
        findNonZero(outputFrame, locationsMaskOutputFrame);

        vector<float> depthValuesVec;
        for (int i = 0; i < locationsMaskOutputFrame.total(); i++ )
        {
            pntLocations = locationsMaskOutputFrame.at<Point>(i); 
            float value = (float)checkDepthValuesFrame.at<unsigned short>(pntLocations.y, pntLocations.x);
            depthValuesVec.push_back(value); 
        }


        vector<float> dpSorted = depthValuesVec;
        sort (dpSorted.begin(), dpSorted.end());

        vector<float> median9(9);
        copy(dpSorted.end()-9, dpSorted.end(), median9.begin());
        float medianDepthValue = GetMedian(median9);

        if (medianDepthValue > thresholdRearing)
        {
            for (int i = 0; i < locationsMaskOutputFrame.total(); i++ )
            {
                pntLocations2 = locationsMaskOutputFrame.at<Point>(i);

                if (pntLocations2.x<borderROIX)
                {
                    float valueDepthOUTROI = (float)checkDepthValuesFrame.at<unsigned short>(pntLocations2.y, pntLocations2.x);

                    if (valueDepthOUTROI <= thresholdRearing)
                    {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}
                }

                else if (pntLocations2.x>(inputFrame.cols-borderROIX))
                {
                    float valueDepthOUTROI = (float)checkDepthValuesFrame.at<unsigned short>(pntLocations2.y, pntLocations2.x);

                    if (valueDepthOUTROI <= thresholdRearing) 
                    {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}
                }

                else if (pntLocations2.y<borderROIY)
                {
                    float valueDepthOUTROI = (float)checkDepthValuesFrame.at<unsigned short>(pntLocations2.y, pntLocations2.x);

                    if (valueDepthOUTROI <= thresholdRearing)
                    {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}
                }

                else if (pntLocations2.y>(inputFrame.rows-borderROIY))
                {
                    float valueDepthOUTROI = (float)checkDepthValuesFrame.at<unsigned short>(pntLocations2.y, pntLocations2.x);

                    if (valueDepthOUTROI <= thresholdRearing)
                    {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}
                }

                else {}
            }
        }

        else  
        {
            for (int i = 0; i < locationsMaskOutputFrame.total(); i++ )
            {
                pntLocations2 = locationsMaskOutputFrame.at<Point>(i); 

                if (pntLocations2.x<borderROIX)
                {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}

                else if (pntLocations2.x>(inputFrame.cols-borderROIX))
                {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}

                else if (pntLocations2.y<borderROIY)
                {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}

                else if (pntLocations2.y>(inputFrame.rows-borderROIY))
                {outputFrame.at<uchar>(pntLocations2.y, pntLocations2.x) = (uchar)0;}

                else {}
            }
        }

        Mat labels3, stats3, centroids3;
        int nLabels3 = connectedComponentsWithStats(outputFrame, labels3, stats3, centroids3, connectivity_4, CV_32S);

        if (nLabels3 > 2) 
        {
            if (nLabels3 > 3) 
            {
                Mat areaVector3 = stats3(cv::Rect(4,1,1,nLabels3-1));

                vector<int> vectorArea;
                for (int l = 0; l<nLabels3-1; l++)
                {
                    vectorArea.push_back(areaVector3.at<int>(l,0));
                }

                sort (vectorArea.begin(), vectorArea.end());
                areaVector3.setTo(0, areaVector3 >= vectorArea[vectorArea.size() - 2]);
                areaVector3.convertTo(areaVector3, CV_8U, 1, 0);

                Mat locations3;  
                findNonZero(areaVector3, locations3);

                Point pnt3;
                for (int i = 0; i < locations3.total(); i++ ) {

                    pnt3 = locations3.at<Point>(i);
                    outputFrame.setTo(0, labels3 == (pnt3.y+1));
                }
            }


            nLabels3 = connectedComponentsWithStats(outputFrame, labels3, stats3, centroids3, connectivity_4, CV_32S);

            Mat areaVector3 = stats3(cv::Rect(4,1,1,nLabels3-1)); 

            Point minCentroid1_coord, minCentroid2_coord;
            Point pnt4;
            Point2d p1, p2;
            Mat locations4;   

            if (areaVector3.at<int>(0,0)==1)
            {
                p1 = centroids3.at<Point2d>(1,0);
                minCentroid1_coord = p1;
            }

            else 
            {
                p1 = centroids3.at<Point2d>(1,0);
                Mat object1Mat = Mat::zeros( outputFrame.size(), CV_8UC1 );
                object1Mat.setTo(1, labels3 == 1);

                Mat dist_mask = Mat::ones( outputFrame.size(), CV_8UC1 );
                dist_mask.at<uchar>(p1.y, p1.x) = 0; 

                Mat dist_maskFinal = Mat::zeros( outputFrame.size(), CV_8UC1 );
                distanceTransform(dist_mask, dist_maskFinal, CV_DIST_L2, 3); 

                
                Mat dist_c = Mat::zeros( outputFrame.size(), CV_8UC1 );
                dist_maskFinal.copyTo(dist_c, object1Mat);


                dist_c.convertTo(dist_c, CV_8U, 1, 0);

                findNonZero(dist_c, locations4);


                vector<int> vecDistances;
                for (int i = 0; i < locations4.total(); i++ ) {

                    pnt4 = locations4.at<Point>(i); 
                    vecDistances.push_back(dist_c.at<uchar>(pnt4.y, pnt4.x));
                }

                double maxVal2, minCentroid1_dist;
                Point max_loc2, minPosition;
                minMaxLoc( vecDistances, &minCentroid1_dist, &maxVal2, &minPosition, &max_loc2 );

                minCentroid1_coord = locations4.at<Point>(minPosition.x);
            }

            if (areaVector3.at<int>(1,0)==1) 
            {
                p2 = centroids3.at<Point2d>(2,0); 
                minCentroid2_coord = p2;
            }

            else 
            {

                p2 = centroids3.at<Point2d>(2,0); 
               
                Mat object2Mat = Mat::zeros( outputFrame.size(), CV_8UC1 );
                object2Mat.setTo(1, labels3 == 2);

                Mat dist_mask2 = Mat::ones( outputFrame.size(), CV_8UC1 );
                dist_mask2.at<uchar>(p2.y, p2.x) = 0; 

                Mat dist_maskFinal2 = Mat::zeros( outputFrame.size(), CV_8UC1 );
                distanceTransform(dist_mask2, dist_maskFinal2, CV_DIST_L2, 3);

                Mat dist_c2 = Mat::zeros( outputFrame.size(), CV_8UC1 );
                dist_maskFinal2.copyTo(dist_c2, object2Mat); 

                dist_c2.convertTo(dist_c2, CV_8U, 1, 0);
                findNonZero(dist_c2, locations4);

                vector<int> vecDistances2;
                for (int i = 0; i < locations4.total(); i++ ) {

                    pnt4 = locations4.at<Point>(i);
                    vecDistances2.push_back(dist_c2.at<uchar>(pnt4.y, pnt4.x));

                }

                double minCentroid2_dist, maxVal2;
                Point minPosition2, max_loc2;
                minMaxLoc( vecDistances2, &minCentroid2_dist, &maxVal2, &minPosition2, &max_loc2 );

                minCentroid2_coord = locations4.at<Point>(minPosition2.x);
            }

            
            line(outputFrame, minCentroid1_coord, minCentroid2_coord, cv::Scalar(255, 255,255), 2);
            outputFrame.setTo(1, outputFrame!=0); 

        }

        else 
        {}
    }

    else 
    {}

    return outputFrame;
}

void MainWindow::backgroundSubtractionDYN(Mat depthFrame, int frameNumb, string folderNameSaveTrackedFrames) 
{
    
    Mat depthFrameFilteredMedian = Mat::zeros(depthFrame.size(), depthFrame.type()); 
    Mat subtractedDFrame = Mat::zeros(depthFrame.size(), depthFrame.type()); 
    Mat finalSegmentedFrame = Mat::zeros(depthFrame.size(), depthFrame.type());

    medianBlur ( depthFrame, depthFrameFilteredMedian, 3 );

    subtract(depthInitModel, depthFrameFilteredMedian, subtractedDFrame); 

    Mat auxFrame = depthFrame.clone();

    subtractedDFrame.setTo(0, subtractedDFrame <=minRatRange);
    subtractedDFrame.setTo(0, subtractedDFrame >=maxRatRange);

	subtractedDFrame.setTo(0.0, auxFrame == 0.0);

    Mat binaryDTrack = subtractedDFrame.clone();
    binaryDTrack.setTo(1, binaryDTrack != 0);

    const int connectivity_4 = 8;
    Mat labels, stats, centroids;

    binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);

    int nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    Mat areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 

    areaVector.setTo(0, areaVector > minimumAreaPixels);
    areaVector.convertTo(areaVector, CV_8U, 1, 0);

    Mat locations;
    findNonZero(areaVector, locations);

    Point pnt;

    for (int i = 0; i < locations.total(); i++ ) {

        pnt = locations.at<Point>(i);
        binaryDTrack.setTo(0, labels == (pnt.y+1));
    }

    
    findNonZero(binaryDTrack, locations);
    vector<int> coordX, coordY;

    for (int i = 0; i < locations.total(); i++ ) {

        pnt = locations.at<Point>(i); 

        coordX.push_back(pnt.x);
        coordY.push_back(pnt.y);
    }

    double minX, minY, maxX, maxY;
    Point min_locX, max_locX, min_locY, max_locY;
    minMaxLoc(coordX, &minX, &maxX, &min_locX, &max_locX);

    minMaxLoc(coordY, &minY, &maxY, &min_locY, &max_locY);

    double amplitudeMaxX = maxX-minX;
    double amplitudeMaxY = maxY-minY;


    if (amplitudeMaxX>2*maxSizeRat || amplitudeMaxY>2*maxSizeRat)
    {
        Mat destMat;

        Mat se = Mat::ones(3,1, binaryDTrack.type());
        erode( binaryDTrack, destMat, se );

        se = Mat::ones(1,3, binaryDTrack.type());
        erode( destMat, destMat, se );

        se = Mat::ones(3,3, binaryDTrack.type());
        dilate(destMat, destMat, se );

        nLabels = connectedComponentsWithStats(destMat, labels, stats, centroids, connectivity_4, CV_32S);
        areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 

        areaVector.setTo(0, areaVector > minimumAreaPixels);
        areaVector.convertTo(areaVector, CV_8U, 1, 0);

        findNonZero(areaVector, locations);

        for (int i = 0; i < locations.total(); i++ ) {

            pnt = locations.at<Point>(i); 
            destMat.setTo(0, labels == (pnt.y+1));

        }

        binaryDTrack = destMat.clone();
    }

    Mat binaryFinalTrack = removeReflections(binaryDTrack, subtractedDFrame);

    Mat bwImage_toFill=binaryFinalTrack.clone();

    findContours(bwImage_toFill, contoursStatic, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    drawContours(bwImage_toFill, contoursStatic, -1, Scalar(1), CV_FILLED);

    subtractedDFrame.copyTo(finalSegmentedFrame, bwImage_toFill);

    binaryDTrack = finalSegmentedFrame.clone();
    binaryDTrack.setTo(1, binaryDTrack != 0);

    binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);

    nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    areaVector = stats(cv::Rect(4,1,1,nLabels-1));

    areaVector.setTo(0, areaVector > minimumAreaPixels);
    areaVector.convertTo(areaVector, CV_8U, 1, 0);

    findNonZero(areaVector, locations);

    for (int i = 0; i < locations.total(); i++ ) {

        pnt = locations.at<Point>(i); 
        finalSegmentedFrame.setTo(0, labels == (pnt.y+1));
    }


    finalSegmentedFrame.convertTo(finalSegmentedFrame,  CV_8U, 1,0);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    string nameFile = folderNameSaveTrackedFrames + "/" + to_string(frameNumb) + ".png";
    imwrite(nameFile, finalSegmentedFrame, compression_params);


    absdiff(depthInitModel, depthFrameFilteredMedian, subtractedDFrame);
    auxFrame = depthFrame.clone();

    subtractedDFrame.setTo(0, subtractedDFrame <=minRatRange);
    subtractedDFrame.setTo(0, subtractedDFrame >=maxRatRange);

    subtractedDFrame.setTo(0.0, auxFrame == 0.0);

    binaryDTrack = subtractedDFrame.clone();
    binaryDTrack.setTo(1, binaryDTrack != 0);

    binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);
    nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 

    double minL, maxL;
    Point min_loc, max_loc;
    minMaxLoc(areaVector, &minL, &maxL, &min_loc, &max_loc); 
    binaryDTrack.setTo(0, labels != (max_loc.y+1));

    Mat temp_mask = Mat::zeros(depthFrame.size(), depthFrame.type());
    subtractedDFrame.copyTo(temp_mask, binaryDTrack);
	
    Mat newBACK = Mat::zeros(depthFrame.size(), depthFrame.type());
    subtract(subtractedDFrame, temp_mask, newBACK); 

    binaryDTrack = newBACK.clone();
    binaryDTrack.setTo(1, binaryDTrack != 0);

    binaryDTrack.convertTo(binaryDTrack, CV_8U, 1, 0);

    nLabels = connectedComponentsWithStats(binaryDTrack, labels, stats, centroids, connectivity_4, CV_32S);
    areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 
	
    for (int areaIt = 0; areaIt<areaVector.rows; areaIt++)
    {
        if (areaVector.at<unsigned short>(areaIt, 0) < obj_min_size_px)
        {
            binaryDTrack.setTo(0, labels == (areaIt+1));
        }

    }

    
    depthFrameFilteredMedian.copyTo(depthInitModel, binaryDTrack>0);

}

void MainWindow::on_btnSeeTracking_clicked()
{
    ofstream myfile2;
    myfile2.open ("fileOutputTrainValues.csv");

    if (myfile2) {
        myfile2 << "label real"  << ";" << "feature1 s/norm" << ";" << "feature1 c/norm" << ";" << "feature2 s/norm" << ";" << "feature2 c/norm"<< ";" << "feature3 s/norm"<< ";" << "feature3 c/norm"<< ";" << "feature4 s/norm"<< ";" << "feature4 c/norm"<< ";" << "feature5 s/norm"<< ";" << "feature5 c/norm"<< ";" << "feature6 s/norm"<< ";" << "feature6 c/norm"<< ";" << "feature7 s/norm" << ";" << "feature7 c/norm" << ";" << "feature8 s/norm"<< ";" << "feature8 c/norm" << ";" << "feature9 s/norm"<< ";" << "feature9 c/norm" << ";" << "feature10 s/norm"<< ";" << "feature10 c/norm"<< "\n";
        for (int i  = 0; i < trainLabels.size(); i++)
        {
            myfile2 << trainLabels[i] << ";";
            for (int col = 0; col < 10; col++)
            {
                myfile2 << trainMat2.at<float>(i,col)  << ";" << trainMat.at<float>(i,col) << ";";
            }
            myfile2 << "\n";
        }
    }

    myfile2.close();
}

void MainWindow::on_btnOpenDirectoryTracking_clicked()
{
    QString dirFileTracking = QFileDialog::getExistingDirectory(this, tr("Select directory to save tracking data:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dirFileTracking.isEmpty()){
        ui->edtSaveTrackingData->setEnabled(true);

        ui->edtSaveTrackingData->setStyleSheet("QLineEdit {color: red;}");
        ui->edtSaveTrackingData->setText("Select directory!");
    }

    else
    {
        ui->edtSaveTrackingData->setStyleSheet("QLineEdit {color: green;}");
        ui->edtSaveTrackingData->setText(dirFileTracking);

        if (ui->cbMultipleVideosForTraining->isChecked())
        {

            for (int fFolder = 0; fFolder<multipleDirectoryName.size(); fFolder++)
            {
                string allnameDir = multipleFolderName[fFolder];

                
                if (ui->cbBodyParts->isChecked()) 
                {
                    
                    string nameFile = dirFileTracking.toStdString() + "/" + allnameDir + "_centroidCoordResults.csv";
                    saveToCSVFileCentroid(nameFile, multipleCentroidVectorFinal[fFolder]);

                    
                    string nameFile2 = dirFileTracking.toStdString() + "/" + allnameDir + "_bodyPartsResults.csv";
                    saveToCSVFileBodyParts(nameFile2, multipleBodyPartsVectorFinal[fFolder]);
                }

                else 
                {
                    
                    string nameFile = dirFileTracking.toStdString() + "/" + "centroidCoordResults.csv";
                    saveToCSVFileCentroid(nameFile, centroidVectorFinal);

                }
            }
        }

        else 
        {
            

            if (ui->cbBodyParts->isChecked()) 
            {
                
                string nameFile = dirFileTracking.toStdString() + "/" + "centroidCoordResults.csv";
                saveToCSVFileCentroid(nameFile, centroidVectorFinal);

                string nameFile2 = dirFileTracking.toStdString() + "/" + "bodyPartsResults.csv";
                saveToCSVFileBodyParts(nameFile2, bodyPartsVectorFinal);

               

            }

            else 
            {
                string nameFile = dirFileTracking.toStdString() + "/" + "centroidCoordResults.csv";
                saveToCSVFileCentroid(nameFile, centroidVectorFinal);

            }
        }

    }
}

vector<float> MainWindow::centroidCalculation(Mat finalForeground, int f)
{

    vector<float> centroidVector;

    int countDiffArea = countNonZero(finalForeground);

    if (countDiffArea>maxAreaPossibleRat) 
    {
        centroidVector.push_back(-1); centroidVector.push_back(-1); centroidVector.push_back(-1);
    }

    else
    {
        Mat binaryDTrack = finalForeground.clone();
        binaryDTrack.setTo(1, binaryDTrack != 0); 
        Mat destMat = Mat::zeros( binaryDTrack.size(), binaryDTrack.type() );


       
        Mat se = Mat::ones(taildiameter,taildiameter, binaryDTrack.type());
        erode( binaryDTrack, destMat, se );

        const int connectivity_4 = 8;
        Mat labels, stats, centroids;
        destMat.convertTo(destMat, CV_8U, 1, 0);

        int nLabels = connectedComponentsWithStats(destMat, labels, stats, centroids, connectivity_4, CV_32S);
        Mat areaVector = stats(cv::Rect(4,1,1,nLabels-1)); 

        areaVector.setTo(0, areaVector > minimumAreaPixels);
        areaVector.convertTo(areaVector, CV_8U, 1, 0);

        Mat locations;   
        findNonZero(areaVector, locations);

        Point pnt;
        for (int i = 0; i < locations.total(); i++ ) {

            pnt = locations.at<Point>(i); 
            destMat.setTo(0, labels == (pnt.y+1));
        }

        
        destMat.convertTo(destMat, CV_8U, 1, 0);

        nLabels = connectedComponentsWithStats(destMat, labels, stats, centroids, connectivity_4, CV_32S);

        if (nLabels<2) 
        {

            destMat = binaryDTrack.clone(); 
            destMat.convertTo(destMat, CV_8U, 1, 0);

            nLabels = connectedComponentsWithStats(destMat, labels, stats, centroids, connectivity_4, CV_32S);

            
            if (centroids.rows <2)
            {
                centroidVector.push_back(-1); centroidVector.push_back(-1); centroidVector.push_back(-1);
            }

            else
            {
                Point2d point1 = centroids.at<Point2d>(1,0);
                xCentroid.push_back(float(point1.x));
                yCentroid.push_back(float(point1.y));
                zCentroid.push_back(float((finalForeground.at<uchar>(point1.y,point1.x))));

                centroidVector.push_back(float(point1.x)); centroidVector.push_back(float(point1.y)); centroidVector.push_back(float((finalForeground.at<uchar>(point1.y,point1.x)))); 

                indexesCentroid.push_back(f);
            }
        }

        else{

            Point2d point1 = centroids.at<Point2d>(1,0);
            xCentroid.push_back(float(point1.x));
            yCentroid.push_back(float(point1.y));
            zCentroid.push_back(float((finalForeground.at<uchar>(point1.y,point1.x))));

            centroidVector.push_back(float(point1.x)); centroidVector.push_back(float(point1.y)); centroidVector.push_back(float((finalForeground.at<uchar>(point1.y,point1.x)))); 

            indexesCentroid.push_back(f);
        }
    }

    return centroidVector;

}

vector<float> MainWindow::bodyPartsCalculation(Mat finalForeground, vector<float> centroidVectorFrame)
{
    bool flagEccent = 0;
    vector<float> bodyPartsVectorFrame; 
    vector<float> tailEndVec, tailBaseVec, headVec, noseVec;
    Mat binaryDTrack = finalForeground.clone();
    Point pntLocations;

    if (centroidVectorFrame[0] == -1)
    {
        flagEccent = 0;

        bodyPartsVectorFrame.clear();
        bodyPartsVectorFrame.resize(14);
        bodyPartsVectorFrame[0] = flagEccent;
        fill(bodyPartsVectorFrame.begin()+1, bodyPartsVectorFrame.end(), -1); 
    }

    else
    {
        binaryDTrack.setTo(1, binaryDTrack != 0);

        Mat rat_clone1 = binaryDTrack.clone();

        if ((rat_clone1.type() == CV_8UC1) && (rat_clone1.rows>0)) 
        {
            findContours( rat_clone1, contoursBodyParts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        }

        double largest_area = 0;
        int largest_contour_index = 0;

        for( int i = 0; i< contoursBodyParts.size(); i++ ) 
        {
            double a=contourArea(contoursBodyParts[i],false);
            if(a>largest_area){
                largest_area=a;
                largest_contour_index=i;
            }
        }

        Mat rat_c = Mat::zeros( binaryDTrack.size(), CV_8UC1 );

        if (largest_area==0) 
        {

        }

        else
        {
           
            Scalar colors = 255;

            for (size_t idx = 0; idx < contoursBodyParts.size(); idx++)
            {
                drawContours(rat_c, contoursBodyParts, largest_contour_index, colors); 
            }
        }

        rat_c.setTo(1, rat_c != 0);

       Mat bw = finalForeground.clone();
        threshold(bw, bw, 10, 255, CV_THRESH_BINARY);
        thinning(bw);

        Mat labels2, stats2, centroids2; const int connectivity_4 = 8;
        bw.convertTo(bw, CV_8U, 1, 0);
        int nLabels2 = connectedComponentsWithStats(bw, labels2, stats2, centroids2, connectivity_4, CV_32S);
        Mat areaVector2 = stats2(cv::Rect(4,1,1,nLabels2-1)); 
        double minL, maxL;
        Point min_loc, max_loc;
        minMaxLoc(areaVector2, &minL, &maxL, &min_loc, &max_loc); 

        bw.setTo(0, labels2 != (max_loc.y+1));


        Mat locations;  
        findNonZero(bw, locations);

        Point pnt;
        int count;

        vector<int> coordsEndpoints;
        vector<int> coordsBranchpoints;

        
        for (int i = 0; i < locations.total(); i++ ) {

            
            count = 0;

            pnt = locations.at<Point>(i); 

            uchar pix;
            

            int maxValueBorderY = min(pnt.y+1, bw.rows-1);
            int maxValueBorderX = min(pnt.x+1, bw.cols-1);

            int minValueBorderY = max(pnt.y-1, 0);
            int minValueBorderX = max(pnt.x-1, 0);

            for (int y_n = minValueBorderY; y_n <= maxValueBorderY; y_n++) {
                for (int x_n = minValueBorderX; x_n <= maxValueBorderX; x_n++) {

                    pix = bw.at<uchar>(y_n,x_n);

                    if (pix != 0)
                        count++;
                }
            }

            if (count == 2) {
                coordsEndpoints.push_back(pnt.x);
                coordsEndpoints.push_back(pnt.y);
            }
            else if (count >= 4) 
            {
                coordsBranchpoints.push_back(pnt.x);
                coordsBranchpoints.push_back(pnt.y);
            }
        }

        
        Mat rat_t = bw.clone();

        Mat dist_mask = Mat::ones( binaryDTrack.size(), CV_8UC1 );
        dist_mask.at<uchar>(centroidVectorFrame[1], centroidVectorFrame[0]) = 0; 

        Mat dist_maskFinal = Mat::zeros( binaryDTrack.size(), CV_8UC1 );
        distanceTransform(dist_mask, dist_maskFinal, CV_DIST_L2, 3); 

      
        Mat dist_c = Mat::zeros( binaryDTrack.size(), CV_8UC1 );
        dist_maskFinal.copyTo(dist_c, rat_c); 

        double minVal2, tail_end_dist;
        Point min_loc2, tailEndPoint;
        minMaxLoc( dist_c, &minVal2, &tail_end_dist, &min_loc2, &tailEndPoint );

    
        tailEndVec.push_back(tailEndPoint.x);
        tailEndVec.push_back(tailEndPoint.y);
        tailEndVec.push_back(float((finalForeground.at<uchar>(tailEndPoint.y,tailEndPoint.x)))); // z in mm

        
        RotatedRect minEllipse;
        minEllipse = fitEllipse(Mat(contoursBodyParts[largest_contour_index]));

        
        float a     = minEllipse.size.width  / 2;   
        float b     = minEllipse.size.height / 2;

        
        float eccentricityRatMask;

        if (a==0) {eccentricityRatMask = 0;}
        else if (b==0){eccentricityRatMask = 0;}
        else
        {
            if (a>b)
            {eccentricityRatMask = (sqrt((pow(a,2))-(pow(b,2))))/a;}
            else
            {eccentricityRatMask = (sqrt((pow(b,2))-(pow(a,2))))/b;}
        }


        Mat rat_clone2 = rat_t.clone();

        if ((rat_clone2.type() == CV_8UC1) && (rat_clone2.rows>0)) 
        {
            findContours( rat_clone2, contoursRatSkeleton, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); 
        }

        largest_area = 0;
        largest_contour_index = 0;

        for( int i = 0; i< contoursRatSkeleton.size(); i++ ) 
        {
            double a=contourArea(contoursRatSkeleton[i],false);
            if(a>largest_area){
                largest_area=a;
                largest_contour_index=i;
            }
        }

        Mat fig1 = Mat::zeros( binaryDTrack.size(), CV_8UC1 );
        Scalar colors = 255;

        for (size_t idx = 0; idx < contoursRatSkeleton.size(); idx++)
        {
            drawContours(fig1, contoursRatSkeleton, largest_contour_index, colors); 
        }

        if (contoursRatSkeleton[largest_contour_index].size() <=5) 
        {
            flagEccent = 0;
        }

        else
        {
            minEllipse = fitEllipse(Mat(contoursRatSkeleton[largest_contour_index]));

            

            a     = minEllipse.size.width  / 2;    
            b     = minEllipse.size.height / 2;

            
            float eccentricityRatSkeleton;

            if (a==0) {eccentricityRatSkeleton = 0;}
            else if (b==0){eccentricityRatSkeleton = 0;}
            else
            {
                if (a>b)
                {eccentricityRatSkeleton = (sqrt((pow(a,2))-(pow(b,2))))/a;}
                else
                {eccentricityRatSkeleton = (sqrt((pow(b,2))-(pow(a,2))))/b;}
            }


            
            if (eccentricityRatMask > 0.7) 
            {

                Point nosePoint;

                Mat checkDepthValuesFrame = finalForeground.clone();

                Mat locationsMaskOutputFrame;   
                findNonZero(finalForeground, locationsMaskOutputFrame);

                vector<float> depthValuesVec;
                for (int i = 0; i < locationsMaskOutputFrame.total(); i++ )
                {
                    pntLocations = locationsMaskOutputFrame.at<Point>(i); 
                    float value = (float)checkDepthValuesFrame.at<uchar>(pntLocations.y, pntLocations.x);
                    depthValuesVec.push_back(value);
                }


                vector<float> dpSorted = depthValuesVec;
                sort (dpSorted.begin(), dpSorted.end());

                vector<float> median9(49);
                copy(dpSorted.end()-49, dpSorted.end(), median9.begin());
                float medianDepthValue = GetMedian(median9);

                if (medianDepthValue > thresholdRearing) 
                {
                  
                    double minVal3, noseValue3;
                    Point min_loc3;
                    minMaxLoc( median9, &minVal3, &noseValue3, &min_loc3, &nosePoint );

                    checkDepthValuesFrame.setTo(0, finalForeground!=noseValue3);
                    findNonZero(checkDepthValuesFrame, locationsMaskOutputFrame);

                    pntLocations = locationsMaskOutputFrame.at<Point>(0); 

                    
                    noseVec.push_back(pntLocations.x);
                    noseVec.push_back(pntLocations.y);
                    noseVec.push_back(float((finalForeground.at<uchar>(pntLocations.y,pntLocations.x)))); 
                }

                else
                {
                    Mat Z = Mat::ones( binaryDTrack.size(), CV_8UC1 );
                    Z.at<uchar>(tailEndPoint.y, tailEndPoint.x) = 0; 

                    Mat rostral_mask = Z.clone();
                    distanceTransform(Z, Z, CV_DIST_L2, 3);

                    rostral_mask.setTo(0, Z<=tail_end_dist);
                    rostral_mask.setTo(1, Z>tail_end_dist);

                    Mat dist_tail_multi = dist_c.clone();

                    dist_tail_multi.setTo(0, rostral_mask==0);

                    double minVal3, maxVal3;
                    Point min_loc3;
                    minMaxLoc( dist_tail_multi, &minVal3, &maxVal3, &min_loc3, &nosePoint );


                    vector<float> neighboorDepthValues;
                    Mat maskNeigh = Mat::zeros( dist_c.size(), CV_8UC1 );

                    for (int lineN = -3; lineN <= 3; lineN ++)
                    {
                        for (int colN = -3; colN <= 3; colN ++)
                        {
                            if ((nosePoint.y+lineN>=dist_c.rows) || (nosePoint.y+lineN<0) || (nosePoint.x+colN>=dist_c.cols) || (nosePoint.x+colN<0)) 
                            {}

                            else 
                            {
                                neighboorDepthValues.push_back(float((finalForeground.at<uchar>(nosePoint.y+lineN,nosePoint.x+colN))));
                                maskNeigh.at<uchar>(nosePoint.y+lineN,nosePoint.x+colN) = (finalForeground.at<uchar>(nosePoint.y+lineN,nosePoint.x+colN));
                            }

                        }
                    }

                    double minVal4, noseValue4;
                    Point min_loc4, max_loc4;
                    minMaxLoc( neighboorDepthValues, &minVal4, &noseValue4, &min_loc4, &max_loc4 );


                    if (noseValue4 == 0)
                    {
                        flagEccent = 0;

                    }

                    else
                    {
                        maskNeigh.setTo(0, maskNeigh<noseValue4);
                        findNonZero(maskNeigh, locationsMaskOutputFrame);

                        pntLocations = locationsMaskOutputFrame.at<Point>(0); 

                        nosePoint.x = pntLocations.x; nosePoint.y = pntLocations.y;
                        noseVec.push_back(nosePoint.x);
                        noseVec.push_back(nosePoint.y);
                        noseVec.push_back(float((finalForeground.at<uchar>(nosePoint.y,nosePoint.x)))); 
                    }

                }

                if (flagEccent == 0)
                {} 

                else
                {
                    if ((coordsEndpoints.size()/2) <=1) 
                    {
                        flagEccent = 0;
                    }

                    else
                    {

                        vector <double> distancesToNose;
                        for (int t = 0; t<(coordsEndpoints.size()); t+=2)
                        {
                            double dist_nose = distanceBetweenTwoPoints((double)noseVec[0],  (double)noseVec[1],  (double)coordsEndpoints[t],  (double)coordsEndpoints[t+1]);
                            distancesToNose.push_back(dist_nose);
                        }

                        double minVal4, maxVal4;
                        Point headPoint, max_loc4;
                        minMaxLoc( distancesToNose, &minVal4, &maxVal4, &headPoint, &max_loc4 );

                        headVec.push_back(coordsEndpoints[(headPoint.x)*2]);
                        headVec.push_back(coordsEndpoints[(headPoint.x)*2+1]);
                        headVec.push_back(float((finalForeground.at<uchar>(headVec[1],headVec[0])))); // z in mm

                        flagEccent = 1;
                    }
                }
            }

            else 
            {
                if (eccentricityRatSkeleton > 0.8) 

                {
                    
                    flagEccent = 0;
                }

                else
                {

                    if ((coordsEndpoints.size()/2) <=1) 
                    {
                        flagEccent = 0;
                    }

                    else
                    {
                        
                        vector <double> distancesToTail;
                        for (int t = 0; t<(coordsEndpoints.size()); t+=2)
                        {
                            double dist_tail = distanceBetweenTwoPoints((double)tailEndPoint.x,  (double)tailEndPoint.y,  (double)coordsEndpoints[t],  (double)coordsEndpoints[t+1]);
                            distancesToTail.push_back(dist_tail);
                        }

                        double minVal5, maxVal5;
                        Point minloc5, headPoint2;
                        minMaxLoc( distancesToTail, &minVal5, &maxVal5, &minloc5, &headPoint2 );

                        headVec.push_back(coordsEndpoints[(headPoint2.x)*2]);
                        headVec.push_back(coordsEndpoints[(headPoint2.x)*2+1]);
                        headVec.push_back(float((finalForeground.at<uchar>(headVec[1],headVec[0]))));


                        Mat checkDepthValuesFrame = finalForeground.clone();

                        Mat locationsMaskOutputFrame;  
                        findNonZero(finalForeground, locationsMaskOutputFrame);

                        vector<float> depthValuesVec;
                        for (int i = 0; i < locationsMaskOutputFrame.total(); i++ )
                        {
                            pntLocations = locationsMaskOutputFrame.at<Point>(i); 
                            float value = (float)checkDepthValuesFrame.at<uchar>(pntLocations.y, pntLocations.x);
                            depthValuesVec.push_back(value); 
                        }


                        vector<float> dpSorted = depthValuesVec;
                        sort (dpSorted.begin(), dpSorted.end());

                        vector<float> median9(49);
                        copy(dpSorted.end()-49, dpSorted.end(), median9.begin());
                        float medianDepthValue = GetMedian(median9);

                        if (medianDepthValue > thresholdRearing) 
                        {
                            
                            double minVal3, noseValue3;
                            Point min_loc3, nosePoint;
                            minMaxLoc( median9, &minVal3, &noseValue3, &min_loc3, &nosePoint );

                            checkDepthValuesFrame.setTo(0, finalForeground!=noseValue3);
                            findNonZero(checkDepthValuesFrame, locationsMaskOutputFrame);

                            pntLocations = locationsMaskOutputFrame.at<Point>(0); 

                            
                            noseVec.push_back(pntLocations.x);
                            noseVec.push_back(pntLocations.y);
                            noseVec.push_back(float((finalForeground.at<uchar>(pntLocations.y,pntLocations.x))));

                            flagEccent = 1;
                        }

                        else
                        {

                            Mat Z = Mat::ones( binaryDTrack.size(), CV_8UC1 ); 
                            Z.at<uchar>(headVec[1], headVec[0]) = 0; 

                            distanceTransform(Z, Z, CV_DIST_L2, 3); 

                           
                            Mat dist_HeadMat = Mat::zeros( binaryDTrack.size(), CV_8UC1 );
                            Z.copyTo(dist_HeadMat, rat_c);

                            double minVal6, maxVal6;
                            Point min_loc6, max_loc6;
                            minMaxLoc( dist_HeadMat, &minVal6, &maxVal6, &min_loc6, &max_loc6 );

                            dist_HeadMat.setTo(maxVal6, dist_HeadMat==0); 

                            Point nosePoint2;
                            minMaxLoc( dist_HeadMat, &minVal6, &maxVal6, &nosePoint2, &max_loc6 );

                            
                            vector<float> neighboorDepthValues;
                            Mat maskNeigh = Mat::zeros( dist_c.size(), CV_8UC1 );

                            for (int lineN = -3; lineN <= 3; lineN ++)
                            {
                                for (int colN = -3; colN <= 3; colN ++)
                                {
                                    if ((nosePoint2.y+lineN>=dist_c.rows) || (nosePoint2.y+lineN<0) || (nosePoint2.x+colN>=dist_c.cols) || (nosePoint2.x+colN<0)) 
                                    {}

                                    else 
                                    {
                                        neighboorDepthValues.push_back(float((finalForeground.at<uchar>(nosePoint2.y+lineN,nosePoint2.x+colN))));
                                        maskNeigh.at<uchar>(nosePoint2.y+lineN,nosePoint2.x+colN) = (finalForeground.at<uchar>(nosePoint2.y+lineN,nosePoint2.x+colN));
                                    }

                                }
                            }

                            double minVal4, noseValue4;
                            Point min_loc4, max_loc4;
                            minMaxLoc( neighboorDepthValues, &minVal4, &noseValue4, &min_loc4, &max_loc4 );

                            if (noseValue4 == 0)
                            {
                                flagEccent = 0;
                            }

                            else
                            {
                                maskNeigh.setTo(0, maskNeigh<noseValue4);
                                findNonZero(maskNeigh, locationsMaskOutputFrame);

                                pntLocations = locationsMaskOutputFrame.at<Point>(0); 

                                nosePoint2.x = pntLocations.x; nosePoint2.y = pntLocations.y;
                                
                                noseVec.push_back(nosePoint2.x);
                                noseVec.push_back(nosePoint2.y);
                                noseVec.push_back(float(finalForeground.at<uchar>(nosePoint2.y,nosePoint2.x)));

                                flagEccent = 1;}
                        }
                    }
                }
            }
        }


        if (flagEccent == 0) 
        {
            
            bodyPartsVectorFrame.clear();
            bodyPartsVectorFrame.resize(14);
            bodyPartsVectorFrame[0] = flagEccent;
            fill(bodyPartsVectorFrame.begin()+1, bodyPartsVectorFrame.end(), -1);

        }

        else
        {
            Mat Z = Mat::ones( binaryDTrack.size(), CV_8UC1 );
            Z.at<uchar>(tailEndPoint.y, tailEndPoint.x) = 0; 

            Mat rostral_mask = Z.clone();
            distanceTransform(Z, Z, CV_DIST_L2, 3);

            rostral_mask.setTo(1, Z<=tail_end_dist);
            rostral_mask.setTo(0, Z>tail_end_dist);

            Mat bwdist_negative_rat_mask = Mat::zeros( binaryDTrack.size(), CV_8UC1 );
            distanceTransform(binaryDTrack, bwdist_negative_rat_mask, CV_DIST_L2, 3); 

            Mat midline_contour_dist = Mat::zeros( binaryDTrack.size(), CV_8UC1 );

            bwdist_negative_rat_mask.copyTo(midline_contour_dist, rat_t);
            midline_contour_dist.setTo(0, rostral_mask==0);

            double minVal7, max_d;
            Point min_loc7, max_loc7;
            minMaxLoc( midline_contour_dist, &minVal7, &max_d, &min_loc7, &max_loc7 );

            float ref_radius = (float)thresh_Radius * (float)max_d;

            Mat diff_matrix = midline_contour_dist.clone();
            diff_matrix.convertTo(diff_matrix, CV_32FC1, 1, 0);

            absdiff(diff_matrix, ref_radius, diff_matrix);

            diff_matrix.setTo(0, midline_contour_dist==0);

            double minVal8, maxVal8;
            Point min_loc8, max_loc8;
            minMaxLoc( diff_matrix, &minVal8, &maxVal8, &min_loc8, &max_loc8 );
            diff_matrix.setTo(maxVal8, diff_matrix==0);

            Point tail_basePoint;
            minMaxLoc( diff_matrix, &minVal8, &maxVal8, &tail_basePoint, &max_loc8 );

            tailBaseVec.push_back(tail_basePoint.x);
            tailBaseVec.push_back(tail_basePoint.y);
            tailBaseVec.push_back(float(finalForeground.at<uchar>(tail_basePoint.y,tail_basePoint.x))); 

            double ang_deg = atan2 (noseVec[1] - headVec[1],noseVec[0] - headVec[0]) * 180 / PI;

            bodyPartsVectorFrame.push_back(flagEccent);

            bodyPartsVectorFrame.push_back(tailEndVec[0]); bodyPartsVectorFrame.push_back(tailEndVec[1]); bodyPartsVectorFrame.push_back(tailEndVec[2]);
            bodyPartsVectorFrame.push_back(noseVec[0]); bodyPartsVectorFrame.push_back(noseVec[1]); bodyPartsVectorFrame.push_back(noseVec[2]);
            bodyPartsVectorFrame.push_back(headVec[0]); bodyPartsVectorFrame.push_back(headVec[1]); bodyPartsVectorFrame.push_back(headVec[2]);
            bodyPartsVectorFrame.push_back(tailBaseVec[0]); bodyPartsVectorFrame.push_back(tailBaseVec[1]); bodyPartsVectorFrame.push_back(tailBaseVec[2]);
            bodyPartsVectorFrame.push_back(ang_deg);
        }
    }

    return bodyPartsVectorFrame;
}

void MainWindow::thinningIteration( Mat& im, int iter)
{
    Mat marker =  Mat::zeros(im.size(), CV_8UC1);

    for (int i = 1; i < im.rows-1; i++)
    {
        for (int j = 1; j < im.cols-1; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);
            uchar p9 = im.at<uchar>(i-1, j-1);

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                    (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                    (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                    (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

void MainWindow::thinning( Mat& im)
{
    im /= 255;

    Mat prev =  Mat::zeros(im.size(), CV_8UC1);
    Mat diff;

    do {
        thinningIteration(im, 0);
        thinningIteration(im, 1);
        absdiff(im, prev, diff);
        im.copyTo(prev);
    }
    while ( countNonZero(diff) > 0);

    im *= 255;
}

void MainWindow::on_btnOpenAnnotatedFile_clicked()
{

    if (!ui->cbMultipleVideosForTraining->isChecked()) 
    {
        QString fileName1_singlevideo = QFileDialog::getOpenFileName(this, tr("Select annotation file with following name : 'dataBehav_' "), " ", tr("CSV files (*.csv)"));
        string str2 ("dataBehav");

        if (fileName1_singlevideo.isEmpty()){
            ui->edtAnnotatedFile->setEnabled(true);

            ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: red;}");
            ui->edtAnnotatedFile->setText("Select file!");
        }
        else if ((fileName1_singlevideo.toStdString()).find(str2) == string::npos) 
        {
            ui->edtAnnotatedFile->setEnabled(true);

            ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: red;}");
            ui->edtAnnotatedFile->setText("Select a correct file!");

        }
        else
        {
            ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: black;}");
            ui->edtAnnotatedFile->setText(fileName1_singlevideo);

            
            ifstream fileResume_singlevideo (fileName1_singlevideo.toStdString());

            contentsFileResume.clear();
            labelsObject.clear();

            string value;
            int count = 0;
            
            while ( fileResume_singlevideo.good() )
            {
                getline ( fileResume_singlevideo, value, '\n' );
                if (value.empty()){
                    count++;
                }
                else{
                    if (count == 0 ) 
                    {
                        nameBehaviorsLabel = value;
                        count++;
                    }

                    else if (count == 1) 
                    {
                        count++;
                    }

                    else
                    {
                        contentsFileResume.push_back(value);
                        count++;
                    }
                }
            }

            fileResume_singlevideo.close();

            for (int cont=0; cont<locationDepthVec.size(); cont++)
            {
                int locationOfDepth = locationDepthVec[cont];
                vector<string> vF1 = splitString(contentsFileResume[floor(locationOfDepth/2)], ';');
                labelsObject.push_back(stoi(vF1[1]));
            }

            ui->btnStartClassification->setEnabled(true);
        }
    }

    else
    {
        QStringList fileNames1_multiple = QFileDialog::getOpenFileNames(this,tr("Select annotation file with following name : 'dataBehav_' "), " ", tr("CSV files (*.csv)"));
        if( fileNames1_multiple.isEmpty() )
        {
            ui->edtAnnotatedFile->setEnabled(true);

            ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: red;}");
            ui->edtAnnotatedFile->setText("Select files!");
        }

        else
        {
            for (int i =0;i<fileNames1_multiple.count();i++)
            {
                QString nameFiles = (fileNames1_multiple.at(i));
                string str2 ("dataBehav");

                if ((nameFiles.toStdString()).find(str2) == string::npos)
                {
                    ui->edtAnnotatedFile->setEnabled(true);

                    ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: red;}");
                    ui->edtAnnotatedFile->setText("Select a file with the correct name!");
                    break;
                }
                else
                {
                    if (fileNames1_multiple.count() != multipleFolderName.size()) 
                    {
                        ui->edtAnnotatedFile->setEnabled(true);
                        ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: red;}");
                        ui->edtAnnotatedFile->setText("Select the correct number of files!");
                        break;
                    }

                    else
                    {

                        
                        ifstream fileResume_multiple (nameFiles.toStdString());

                        contentsFileResume.clear();
                        labelsObject.clear();

                        string value;
                        int count = 0;
                        
                        while ( fileResume_multiple.good() )
                        {
                            getline ( fileResume_multiple, value, '\n' );
                            if (value.empty()){
                                count++;
                            }
                            else{
                                if (count == 0 )
                                {
                                    nameBehaviorsLabel = value;
                                    count++;
                                }

                                else if (count == 1)
                                {
                                    count++;
                                }

                                else
                                {
                                    contentsFileResume.push_back(value);
                                    count++;
                                }
                            }
                        }

                        fileResume_multiple.close();

                        vector <int> locationD = multipleLocationDepthVec[i];
                        for (int cont=0; cont<locationD.size(); cont++)
                        {
                            int locationOfDepth = locationD[cont];
                            vector<string> vF1 = splitString(contentsFileResume[floor(locationOfDepth/2)], ';');
                            labelsObject.push_back(stoi(vF1[1]));
                        }

                        if (locationD.size()!= labelsObject.size())
                        {
                            ui->edtAnnotatedFile->setEnabled(true);
                            ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: red;}");
                            ui->edtAnnotatedFile->setText("Select annotated files by the correct order!");
                            break;
                        }

                        else
                        {
                            if (i == fileNames1_multiple.count()-1)
                            {
                                ui->edtAnnotatedFile->setStyleSheet("QLineEdit {color: black;}");
                                ui->edtAnnotatedFile->setText(nameFiles);

                                ui->btnStartClassification->setEnabled(true);
                            }

                            multipleLabelsObject.push_back(labelsObject);
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::on_btnStartClassification_clicked()
{

    numberOfClass = ui->sbnumberOfClasses->value();

    ui->tabClassify->setEnabled(false);
    ui->tabTesting->setEnabled(false);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    if (ui->cbMultipleVideosForTraining->isChecked()) 
    {

         int lastAppend_index = 0;

        int sizeTotal_multiple = 0;
        for (int fFolder = 0; fFolder<multipleDirectoryName.size(); fFolder++)
        {
            sizeTotal_multiple = sizeTotal_multiple + multipleLocationDepthVec[fFolder].size();
        }

        featureMatObject_Multiple = Mat::zeros(sizeTotal_multiple,featureNumber*2,CV_32FC1);

        for (int fFolder = 0; fFolder<multipleDirectoryName.size(); fFolder++)
        {

            locationDepthVec.clear(); locationRGBVec.clear();
            timestampsDepthVec.clear();
            indexesCentroid.clear(); indexesBody.clear();
            centroidVectorFinal.clear();
            bodyPartsVectorFinal.clear();
            xCentroid.clear(); yCentroid.clear(); zCentroid.clear();
            labelsObject.clear();

            locationDepthVec = multipleLocationDepthVec[fFolder];
            timestampsDepthVec = multipleTimestampsDepthVec[fFolder];
            indexesCentroid = multipleIndexesCentroid[fFolder];
            indexesBody = multipleIndexesBody[fFolder];

            centroidVectorFinal = multipleCentroidVectorFinal[fFolder];
            bodyPartsVectorFinal = multipleBodyPartsVectorFinal[fFolder];

            xCentroid = multiplexCentroid[fFolder];
            yCentroid = multipleyCentroid[fFolder];
            zCentroid = multiplezCentroid[fFolder];

            labelsObject = multipleLabelsObject[fFolder];

            string folderTrackedFrames = (dirFile.toStdString() + "//" + multipleFolderName[fFolder] + "_trackedFrames").c_str();

           
            CreateFeatureObject(folderTrackedFrames);

            QCoreApplication::flush();
            QCoreApplication::processEvents();

			vector<Mat>().swap(realForegroundSubtracted); vector<vector<float>>().swap(bodyPartsVectorFinal); vector<vector<float>>().swap(centroidVectorFinal);

            featureAugmentation(false);
            labelsObject_totalMultiple.insert(labelsObject_totalMultiple.end(), labelsObject.begin(), labelsObject.end());

            «featureMatObject22Features.copyTo(featureMatObject_Multiple.colRange(0,featureNumber*2).rowRange(lastAppend_index, lastAppend_index + featureMatObject22Features.rows));

            lastAppend_index = lastAppend_index + featureMatObject22Features.rows;

        }

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        featureNormalizationZ();

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        trainMat = Mat::zeros(lastAppend_index,featureNumber*2,CV_32FC1);
        featureMatObject_Multiple.rowRange(0, lastAppend_index).copyTo(trainMat);

        trainLabels = labelsObject_totalMultiple;

        QCoreApplication::flush();
        QCoreApplication::processEvents();
    }

    else 
    {
        string folderTrackedFrames = dirFileString + "//trackedFrames";
        CreateFeatureObject(folderTrackedFrames);

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        
        vector<Mat>().swap(realForegroundSubtracted); vector<vector<float>>().swap(bodyPartsVectorFinal); vector<vector<float>>().swap(centroidVectorFinal);

       
        featureAugmentation(false);

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        

        QCoreApplication::flush();
        QCoreApplication::processEvents();

        trainMat = featureMatObject22Features.clone();
        trainLabels = labelsObject;

        QCoreApplication::flush();
        QCoreApplication::processEvents();
    }

    
    SVMtrain();
}

void MainWindow::CreateFeatureObject(string folderTrackedFrames)
{

    sizeTotal = locationDepthVec.size();

    vector<float> NEWxVectorFrame = gaussianFilteringCorrectedCentroid(xCentroid, indexesCentroid);
    vector<float> NEWyVectorFrame = gaussianFilteringCorrectedCentroid(yCentroid, indexesCentroid);
    vector<float> NEWzVectorFrame = gaussianFilteringCorrectedCentroid(zCentroid, indexesCentroid);

    for (int it = 0; it<indexesCentroid.size(); it++)
    {
        centroidVectorFinal[indexesCentroid[it]][0] = NEWxVectorFrame[it];
        centroidVectorFinal[indexesCentroid[it]][1] = NEWyVectorFrame[it];
        centroidVectorFinal[indexesCentroid[it]][2] = NEWzVectorFrame[it];
    }

    for (int itBody = 0; itBody<locationDepthVec.size(); itBody++)
    {
        if (bodyPartsVectorFinal[itBody][0] == 0)
        {}
        else
        {
            indexesBody.push_back(itBody);
        }
    }

    «for (int itIndex = 1; itIndex<5; itIndex++) «
    {
        vector<float> bodyToCorrect;

        for (int allFrames = 0; allFrames<indexesBody.size(); allFrames++)
        {
            bodyToCorrect.push_back(bodyPartsVectorFinal[indexesBody[allFrames]][itIndex*3]); «
        }

        vector<float> NEWBodyVector = gaussianFilteringCorrectedCentroid(bodyToCorrect, indexesBody); 

        
        for (int it = 0; it<indexesBody.size(); it++)
        {
            bodyPartsVectorFinal[indexesBody[it]][itIndex*3] = NEWBodyVector[it];
        }
    }

    
    featureMatObject = Mat::zeros(sizeTotal,featureNumber,CV_32FC1);

    vector<float> vectorVelocities;
    vector<float> vectorAngleHeadNose;
    vectorAngleHeadNose.resize(sizeTotal);

    Point pntLocations;

    
    for (int f=0; f<locationDepthVec.size();f++)
    {

        if (centroidVectorFinal[f][0] == -1) 
        {
            if (f == 0) 
            {

            }
            else
            {
                featureMatObject.at<float>(f,0) = featureMatObject.at<float>(f-1,0);
                featureMatObject.at<float>(f,1) = featureMatObject.at<float>(f-1,1);
                featureMatObject.at<float>(f,2) = featureMatObject.at<float>(f-1,2);
                featureMatObject.at<float>(f,3) = featureMatObject.at<float>(f-1,3);
                featureMatObject.at<float>(f,4) = featureMatObject.at<float>(f-1,4);
                featureMatObject.at<float>(f,5) = featureMatObject.at<float>(f-1,5);
                featureMatObject.at<float>(f,6) = featureMatObject.at<float>(f-1,6);
                featureMatObject.at<float>(f,7) = featureMatObject.at<float>(f-1,7);
                featureMatObject.at<float>(f,8) = featureMatObject.at<float>(f-1,8);
                featureMatObject.at<float>(f,9) = featureMatObject.at<float>(f-1,9);
            }
        }

        else
        {
            string name = (folderTrackedFrames + "//").c_str() + to_string(f) + ".png";
            Mat frameTracked = imread(name, IMREAD_UNCHANGED);


            
            Mat locationsMaskOutputFrame;
            findNonZero(frameTracked, locationsMaskOutputFrame);

            vector<float> depthValuesVec;
            
            for (int i = 0; i < locationsMaskOutputFrame.total(); i++ )
            {
                pntLocations = locationsMaskOutputFrame.at<Point>(i); 
                float value = (float)frameTracked.at<uchar>(pntLocations.y, pntLocations.x);
                depthValuesVec.push_back(value);
            }

            vector<float> dpSorted = depthValuesVec;
            sort (dpSorted.begin(), dpSorted.end());

            vector<float> median9(9);
            copy(dpSorted.end()-9, dpSorted.end(), median9.begin());
            float amplitudeD = GetMedian(median9);
            featureMatObject.at<float>(f,0) = amplitudeD;


            int countDiff = countNonZero(frameTracked);
            featureMatObject.at<float>(f,1) = (float)countDiff;

			Point2d point1;
            point1.x = centroidVectorFinal[f][0]; point1.y = centroidVectorFinal[f][1];

            Mat contourOutput = frameTracked.clone();
            Mat binaryDTrack = contourOutput.clone();
            binaryDTrack.setTo(1, binaryDTrack != 0); 

            
            Mat se = Mat::ones(taildiameter,taildiameter, contourOutput.type());
            erode( frameTracked, contourOutput, se );

            if ((contourOutput.type() == CV_8UC1) && (contourOutput.rows>0)){
                findContours( contourOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);} 

            double largest_area = 0;
            int largest_contour_index = 0;

            for( int i = 0; i< contours.size(); i++ ) 
            {
                double a=contourArea(contours[i],false);
                if(a>largest_area){
                    largest_area=a;
                    largest_contour_index=i;
                }
            }

            double maxDistContour = 0;

            if (largest_area==0) 
            {
                featureMatObject.at<float>(f,2) = (float)maxDistContour;
            }

            else
            {
                
                vector<Point> contourFinal = contours[largest_contour_index];
                vector<double> distancesContour;

                for (int cF = 0; cF <contourFinal.size(); cF++)
                {
                    Point point2 = contourFinal[cF];
                    double dist = distanceBetweenTwoPoints((double)point1.x,  (double)point1.y,  (double)point2.x,  (double)point2.y);
                    distancesContour.push_back(dist);
                }

                maxDistContour = *max_element(distancesContour.begin(),distancesContour.end());

                featureMatObject.at<float>(f,2) = (float)maxDistContour;
            }

            if (largest_area==0)
            {
                float circuL = 0;
                featureMatObject.at<float>(f,3) = circuL;
            }

            else
            {
                float circuL = (float)((float)countDiff/(pow((float)maxDistContour,2)));
                featureMatObject.at<float>(f,3) = circuL;
            }


            if (f == 0)
            {
                float speedOfCentroid = 0.0;
                initialCentroid = point1;

                vectorVelocities.push_back(speedOfCentroid);
            }
            else
            {
                double distTwoCentroids = distanceBetweenTwoPoints((double)point1.x,  (double)point1.y,  (double)initialCentroid.x,  (double)initialCentroid.y);
                vectorVelocities.push_back((float)distTwoCentroids);
                initialCentroid = point1;
            }

            RotatedRect minEllipse;

            if (contours[largest_contour_index].size() <=5)
            {
                if (f == 0)
                {
                    featureMatObject.at<float>(f,5) = 0.0;
                }
                else
                {
                    featureMatObject.at<float>(f,5) = featureMatObject.at<float>(f-1,5);
                }
            }
            else
            {
                minEllipse = fitEllipse(Mat(contours[largest_contour_index]));
 
                float a     = minEllipse.size.width  / 2;    
                
                float eccentricityElip;

                if (a==0)
                {
                    eccentricityElip = 0;
                }

                else if (b==0)
                {
                    eccentricityElip = 0;
                }

                else
                {
                    if (a>b)
                    {
                        eccentricityElip = (sqrt((pow(a,2))-(pow(b,2))))/a;
                    }
                    else
                    {
                        eccentricityElip = (sqrt((pow(b,2))-(pow(a,2))))/b;
                    }

                }

                featureMatObject.at<float>(f,5) = eccentricityElip;
            }

            if (ui->cbEPM->isChecked())
            {

                vector<int> EPM_diagonal { 172, 3,  228, 426}; 

               
                Mat dist_mask_closedArms = Mat::ones( frameTracked.size(), CV_8UC1 );

                for (int rowsEPM = 0; rowsEPM < (EPM_diagonal[2] - EPM_diagonal[0] +1); rowsEPM++)
                {
                    for(int colsEPM = 0; colsEPM < (EPM_diagonal[3] - EPM_diagonal[1] +1); colsEPM++)
                    {
                        dist_mask_closedArms.at<uchar>(EPM_diagonal[0]+rowsEPM, EPM_diagonal[1]+colsEPM) = 0; 
                    }
                }

                Mat dist_maskFinal = Mat::zeros( frameTracked.size(), CV_8U );
                distanceTransform(dist_mask_closedArms, dist_maskFinal, CV_DIST_L2, 3);

                float minDist = (dist_maskFinal.at<float>((int)(round(centroidVectorFinal[f][1])), (int)(round(centroidVectorFinal[f][0]))));
                featureMatObject.at<float>(f,6) = minDist;

            }

            else
            {
                vector <float> difValues;

                difValues.push_back(((float)point1.x-borderROIX));
                difValues.push_back(abs(((float)frameTracked.cols-borderROIX) - (float)point1.x));
                difValues.push_back((float)point1.y-borderROIY);
                difValues.push_back(abs(((float)frameTracked.rows-borderROIY) - (float)point1.y));

                double minDist, maxDist;
                Point min_locD, max_locD;
                minMaxLoc(difValues, &minDist, &maxDist, &min_locD, &max_locD); // Calculus of min distance

                if (minDist <0.0)
                {
                    minDist = 0.0;
                }

                featureMatObject.at<float>(f,6) = (float) minDist;
            }


            featureMatObject.at<float>(f,7) = (float) bodyPartsVectorFinal[f][0];


            if (bodyPartsVectorFinal[f][0] == 0) 
            {
                if (f == 0)
                {
                    featureMatObject.at<float>(f,8) = 0.0;
                }
                else
                {
                    featureMatObject.at<float>(f,8) = featureMatObject.at<float>(f-1,8);
                }
            }

            else
            {
                featureMatObject.at<float>(f,8) = (float) bodyPartsVectorFinal[f][6];
            }


            if (bodyPartsVectorFinal[f][0] == 0) 
            {
                if (f == 0) 
                {
                    vectorAngleHeadNose[f]=0.0;
                }
                else
                {
                    vectorAngleHeadNose[f] = vectorAngleHeadNose[f-1];
                }
            }

            else 
            {
                float x1, x2, y1, y2, z1, z2;
                
                x1 = bodyPartsVectorFinal[f][7] - centroidVectorFinal[f][0];
                y1 = bodyPartsVectorFinal[f][8] - centroidVectorFinal[f][1];
                z1 = bodyPartsVectorFinal[f][9] - centroidVectorFinal[f][2];
               
                x2 = bodyPartsVectorFinal[f][4] - bodyPartsVectorFinal[f][7];
                y2 = bodyPartsVectorFinal[f][5] - bodyPartsVectorFinal[f][8];
                z2 = bodyPartsVectorFinal[f][6] - bodyPartsVectorFinal[f][9];

                double dot = x1*x2 + y1*y2 + z1*z2;
                double lenSq1 = x1*x1 + y1*y1 + z1*z1;
                double lenSq2 = x2*x2 + y2*y2 + z2*z2;

                if (lenSq1==0)
                {
                    if (f == 0) 
                    {
                        vectorAngleHeadNose[f]=0.0;
                    }
                    else
                    {
                        vectorAngleHeadNose[f] = vectorAngleHeadNose[f-1];
                    }

                }

                else if (lenSq2 ==0)
                {
                    if (f == 0) 
                    {
                        vectorAngleHeadNose[f]=0.0;
                    }
                    else
                    {
                        vectorAngleHeadNose[f] = vectorAngleHeadNose[f-1];
                    }
                }
                else
                {
                    float angleHeadNose = (float)acos(dot/sqrt(lenSq1 * lenSq2));
                    angleHeadNose = angleHeadNose * 180.0 / PI;
                    vectorAngleHeadNose[f] = angleHeadNose;
                }
            }
        }


    }

    vector<float> diffValuesTime;
    diffValuesTime.resize(timestampsDepthVec.size());

    adjacent_difference(timestampsDepthVec.begin(), timestampsDepthVec.end(), diffValuesTime.begin());

    for (int i=1; i<vectorVelocities.size(); i++) 
    {
        float velocityPoint =  vectorVelocities[i]/diffValuesTime[i];

        if (velocityPoint>0.99)
        {
            vectorVelocities[i] = vectorVelocities[i-1];
        }

        else
        {
            vectorVelocities[i] = velocityPoint;
        }
    }


    int sigmaVelocity1 = 150;
    int sigmaVelocity2 = 450;

    vector <float> correctedVelocities1 = gaussianFilteringCorrectedVelocity(vectorVelocities, sigmaVelocity1);
    vector <float> correctedVelocities2 = gaussianFilteringCorrectedVelocity(vectorVelocities, sigmaVelocity2);

    for (int fT=0; fT<correctedVelocities1.size(); fT++)
    {
        featureMatObject.at<float>(fT,4) = correctedVelocities1[fT]; 
        featureMatObject.at<float>(fT,10) = correctedVelocities2[fT];
    }


    vector<float> diffAngle;
    diffAngle.resize(vectorAngleHeadNose.size());

    
    adjacent_difference(vectorAngleHeadNose.begin(), vectorAngleHeadNose.end(), diffAngle.begin());

    for (int i=0; i<vectorAngleHeadNose.size(); i++)
    {
        if (i == 0)
        {
            vectorAngleHeadNose[i] = 0;
        }

        else
        {
            float vectorPoint =  diffAngle[i]/diffValuesTime[i];

            if (vectorPoint>1.50) 
            {
                vectorAngleHeadNose[i] = vectorAngleHeadNose[i-1];
            }

            else if(vectorPoint<-1.50)
            {
                vectorAngleHeadNose[i] = vectorAngleHeadNose[i-1];
            }
            else
            {
                vectorAngleHeadNose[i] = vectorPoint;
            }
        }
    }


    vector <float> correctedVectorAngle = gaussianFilteringCorrectedVelocity(vectorAngleHeadNose, sigmaVelocity1);

    for (int fT=0; fT<correctedVectorAngle.size(); fT++)
    {
        featureMatObject.at<float>(fT,9) = correctedVectorAngle[fT];
    }

}

    vector <float> correctedVelocities;

    // Parameters: polynomial degree & number of samples
    int n = 5;
    int N = 20; //number of samples per batch -> interpolation batch

    for (int batchID = 0; batchID<velocitiesVec.size(); batchID+=N)
    {

        // Variables: x and y
        vector<float> y (N); copy (velocitiesVec.begin(), velocitiesVec.begin()+N, y.begin() );
        vector<float> x (N); copy (timestampsDepthVec.begin(), timestampsDepthVec.begin()+N, x.begin() );
        //vector<float> x = timestampsDepthVec;

        vector <float> X(2*n+1);                        //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
        for (int i=0;i<2*n+1;i++)
        {
            X[i]=0;
            for (int j=0;j<N;j++)
                X[i]=X[i]+pow(x[j],i);         //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
        }

        vector< vector<float> > B(n+1, vector<float>(n+2));
        vector<float> a (n+1);            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients

        for (int i=0;i<=n;i++)
            for (int j=0;j<=n;j++)
                B[i][j]=X[i+j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix


        vector <float> Y(n+1);                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
        for (int i=0;i<n+1;i++)
        {
            Y[i]=0;
            for (int j=0;j<N;j++)
                Y[i]=Y[i]+(float)pow(x[j],i)*y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
        }


        for (int i=0;i<=n;i++)
            B[i][n+1]=Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
        n=n+1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations


        for (int i=0;i<n;i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
            for (int k=i+1;k<n;k++)
                if (B[i][i]<B[k][i])
                    for (int j=0;j<=n;j++)
                    {
                        float temp=B[i][j];
                        B[i][j]=B[k][j];
                        B[k][j]=temp;
                    }

        for (int i=0;i<n-1;i++)            //loop to perform the gauss elimination
            for (int k=i+1;k<n;k++)
            {
                float t=B[k][i]/B[i][i];
                for (int j=0;j<=n;j++)
                    B[k][j]=B[k][j]-t*B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
            }

        for (int i=n-1;i>=0;i--)                //back-substitution
        {                        //x is an array whose values correspond to the values of x,y,z..
            a[i]=B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
            for (int j=0;j<n;j++)
                if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                    a[i]=a[i]-B[i][j]*a[j];
            a[i]=a[i]/B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
        }

        // Fill a new vector with the corrected values for each timestamps
        for (int timeS = 0; timeS<N; timeS++)
        {
            float valueTime = timestampsDepthVec[timeS+batchID];
            float newValue = 0;

            for (int id=0; id<=n-1; id++)
            {
                newValue = newValue + pow(valueTime, id)*a[id];
            }
            correctedVelocities.push_back(newValue);
        }
    }

    return correctedVelocities;

} */

vector <float> MainWindow::movingAverageFilteringVelocity(vector<float> velocitiesVec){ 

    Mat src = Mat( velocitiesVec ).reshape( 0, 1 );

    
    Point anchor = Point( -1, -1 );
    double delta = 0;
    int ddepth = -1;
    int kernel_size = 7;

    
    Mat kernel = Mat::ones( 1, kernel_size, CV_32F )/ (float)(kernel_size);

    Mat dst;

    
    filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_ISOLATED );

    
    vector<float> vecCorrected;
    for (int i =0; i<dst.cols; i++)
    {
        vecCorrected.push_back(dst.at<float>(0,i));
    }

    return vecCorrected;

}


vector <float> MainWindow::gaussianFilteringCorrectedCentroid(vector<float> coordinateCentroid, vector<int> indexesAll){

    struct Sum
    {
        Sum(): sum{0} { }
        void operator()(float n) { sum += n; }
        float sum;
    };

    
    int sigmaK = 150; 


    vector<float> correctTimeStampsCentroid;

    for (int it = 0; it<indexesAll.size(); it++)
    {
        correctTimeStampsCentroid.push_back(timestampsDepthVec[indexesAll[it]]);
    }


    int sizeTemp = 4;

    float tStart = correctTimeStampsCentroid[0];
    float tStop = correctTimeStampsCentroid[correctTimeStampsCentroid.size()-1];

    vector <float> t_pre, t_pos;
    for (int t = 0; t<sizeTemp; t++)
    {
        t_pre.push_back(((-sizeTemp+t)*sigmaK) + tStart);
        t_pos.push_back(((t+1)*sigmaK) + tStop);
    }

    vector <float> t_Temp(correctTimeStampsCentroid.size() + 2*sizeTemp, 0);
    copy(t_pre.begin(), t_pre.end(), t_Temp.begin());
    copy(correctTimeStampsCentroid.begin(), correctTimeStampsCentroid.end(), t_Temp.begin() + sizeTemp);
    copy(t_pos.begin(), t_pos.end(), t_Temp.end()-sizeTemp);


    float vStart = coordinateCentroid[0];
    float vStop = coordinateCentroid[coordinateCentroid.size()-1];

    vector <float> v_pre(sizeTemp, vStart);
    vector <float> v_pos (sizeTemp, vStop);


    vector <float> v_Temp(coordinateCentroid.size() + 2*sizeTemp, 0);
    copy(v_pre.begin(), v_pre.end(), v_Temp.begin());
    copy(coordinateCentroid.begin(), coordinateCentroid.end(), v_Temp.begin() + sizeTemp);
    copy(v_pos.begin(), v_pos.end(), v_Temp.end()-sizeTemp);


    vector <float> vecCorrected;

    for (int it = sizeTemp; it<correctTimeStampsCentroid.size() + sizeTemp; it++)
    {
        vector<float> timeSDif(t_Temp.size(), 1);

        for (int it0 = 0; it0<timeSDif.size();it0++)
        {
            timeSDif[it0] = t_Temp[it0] - t_Temp[it];
        }

        vector<float> w = timeSDif;

        for (int it2 = 0; it2<w.size();it2++)
        {
            w[it2] = (float)exp((-w[it2]*w[it2])/(2*sigmaK*sigmaK))/(sqrt(2*PI)*sigmaK);
        }

        Sum s = for_each(w.begin(), w.end(), Sum());

        for (int it3 = 0; it3<w.size();it3++)
        {
            w[it3] = (float)(w[it3]/(s.sum));
        }

        float product = 0;
        
        for (int i = 0; i < v_Temp.size(); i++)
        {
            product = product + v_Temp[i] * w[i];
        }

        vecCorrected.push_back(product);
    }

    return vecCorrected;

}


vector <float> MainWindow::gaussianFilteringCorrectedVelocity(vector<float> velocitiesVec, int sigmaK){ 

    struct Sum
    {
        Sum(): sum{0} { }
        void operator()(float n) { sum += n; }
        float sum;
    };

    int sizeTemp = 4;

    float tStart = timestampsDepthVec[0];
    float tStop = timestampsDepthVec[timestampsDepthVec.size()-1];

    vector <float> t_pre, t_pos;
    for (int t = 0; t<sizeTemp; t++)
    {
        t_pre.push_back(((-sizeTemp+t)*sigmaK) + tStart);
        t_pos.push_back(((t+1)*sigmaK) + tStop);
    }

    vector <float> t_Temp(timestampsDepthVec.size() + 2*sizeTemp, 0);
    copy(t_pre.begin(), t_pre.end(), t_Temp.begin());
    copy(timestampsDepthVec.begin(), timestampsDepthVec.end(), t_Temp.begin() + sizeTemp);
    copy(t_pos.begin(), t_pos.end(), t_Temp.end()-sizeTemp);


    float vStart = velocitiesVec[0];
    float vStop = velocitiesVec[velocitiesVec.size()-1];

    vector <float> v_pre(sizeTemp, vStart);
    vector <float> v_pos (sizeTemp, vStop);

    vector <float> v_Temp(velocitiesVec.size() + 2*sizeTemp, 0);
    copy(v_pre.begin(), v_pre.end(), v_Temp.begin());
    copy(velocitiesVec.begin(), velocitiesVec.end(), v_Temp.begin() + sizeTemp);
    copy(v_pos.begin(), v_pos.end(), v_Temp.end()-sizeTemp);


    vector <float> vecCorrected;

    for (int it = sizeTemp; it<timestampsDepthVec.size() + sizeTemp; it++)
    {
        vector<float> timeSDif(t_Temp.size(), 1);

        for (int it0 = 0; it0<timeSDif.size();it0++)
        {
            timeSDif[it0] = t_Temp[it0] - t_Temp[it];
        }

        vector<float> w = timeSDif;

        for (int it2 = 0; it2<w.size();it2++)
        {
            w[it2] = (float)exp((-w[it2]*w[it2])/(2*sigmaK*sigmaK))/(sqrt(2*PI)*sigmaK);
        }

        Sum s = for_each(w.begin(), w.end(), Sum());

        for (int it3 = 0; it3<w.size();it3++)
        {
            w[it3] = (float)(w[it3]/(s.sum));
        }

        float product = 0;
        for (int i = 0; i < v_Temp.size(); i++)
        {
            product = product + v_Temp[i] * w[i];
        }

        vecCorrected.push_back(product);
    }

    return vecCorrected;

}

void MainWindow::featureAugmentation(bool classifyBoolean) {


    int iDFrameMemory = 0;

    for (int i =0; i<timestampsDepthVec.size(); i++){

        if (timestampsDepthVec[i] >= memoryTimeConstant){
            iDFrameMemory = i;
            break;
        }
    }

    featureMatObject22Features = Mat::zeros(featureMatObject.rows - iDFrameMemory, featureNumber*2, CV_32FC1);

    featureMatObject(Rect(0,iDFrameMemory,featureNumber, featureMatObject.rows - iDFrameMemory)).copyTo(featureMatObject22Features.colRange(0,featureNumber).rowRange(0, featureMatObject.rows - iDFrameMemory));
    featureMatObject(Rect(0,0,featureNumber, featureMatObject.rows - iDFrameMemory)).copyTo(featureMatObject22Features.colRange(featureNumber, featureNumber*2).rowRange(0, featureMatObject.rows - iDFrameMemory));

    if (classifyBoolean)
    {}
    else
    {
       
        labelsObject.erase (labelsObject.begin(),labelsObject.begin()+iDFrameMemory);
    }

}

void MainWindow::featureNormalizationZ(){ 

    for (int feat=0; feat<featureNumber*2; feat++)
    {
        Mat auxMat = Mat::zeros(featureMatObject22Features.rows,1,CV_32FC1); 

        featureMatObject22Features.col(feat).copyTo(auxMat.col(0));

        Scalar meanV; Scalar stdV;
        meanStdDev(auxMat, meanV, stdV);

        if (stdV[0] == 0)
        {

        }
        else
        {
            auxMat = auxMat-meanV[0];
            auxMat = auxMat/stdV[0];
        }

        auxMat.col(0).copyTo(featureMatObject22Features.col(feat));
    }

}

double MainWindow::distanceBetweenTwoPoints(double x1, double y1, double x2, double y2){

    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector <int> MainWindow::crossValidationKfold_TestTrainMat()
{
    
    vector<int> indices = kfold(trainMat.rows, kFoldNum);

    return indices;
}

vector<int> MainWindow::shuffleArray(vector<int> array,int size)
{
    srand(time(NULL));

    int n = size;
    while (n > 1)
    {
        
        int k = rand()%n;

        n--;

        int temp = array[n];
        array[n] = array[k];
        array[k] = temp;
    }

    return array;
}

vector<int> MainWindow::kfold( int size, int k )
{
    vector<int> indices(size);

    for (int i = 0; i < size; i++ )
        indices[ i ] = i%k;

    indices = shuffleArray( indices, size );

    return indices;
}

void MainWindow::percentageSelection_TestTrainMat()
{
    

    vector <int> indexTrainTest = random90Train10Test(featureMatObject22Features.rows);

    int mycountTest = count (indexTrainTest.begin(), indexTrainTest.end(), 0); 
    int mycountTrain = featureMatObject22Features.rows - mycountTest;

    trainMat = Mat::zeros(mycountTrain,featureNumber*2,CV_32FC1);
    testMat = Mat::zeros(mycountTest,featureNumber*2,CV_32FC1);


    int counterTest1 = 0; int counterTrain1 = 0;
    for (int sizeIndices = 0; sizeIndices<indexTrainTest.size(); sizeIndices++)
    {
        if (indexTrainTest[sizeIndices] == 0) 
        {
            featureMatObject22Features.row(sizeIndices).copyTo(testMat.row(counterTest1));

            testLabels.push_back(labelsObject[sizeIndices]);
            counterTest1++;

            testFramesVec.push_back(sizeIndices);
        }
        else
        {
            featureMatObject22Features.row(sizeIndices).copyTo(trainMat.row(counterTrain1));

            trainLabels.push_back(labelsObject[sizeIndices]);
            counterTrain1++;

            trainFramesVec.push_back(sizeIndices);
        }
    }

}

vector<int> MainWindow::random90Train10Test(int sizeV)
{
    
    vector<int> indices(sizeV);

    for (int i = 0; i < round(sizeV*0.9); i++ )
        indices[ i ] = 1;

    
    srand(time(NULL));

    int n = sizeV;
    while (n > 1)
    {
        
        int k = rand()%n;

        
        n--;

        int temp = indices[n];
        indices[n] = indices[k];
        indices[k] = temp;
    }

    return indices;

}

void MainWindow::SVMtrain(){

    kFoldNum = ui->sbKValue->value();

    if (ui->cbOptimize->isChecked())
    {
        optimizeBool = true;

    }
    else
    {
        optimizeBool = false;

    }

    if (optimizeBool)
    {

        ui->pbProgressClassification->setVisible(true);
        ui->pbProgressClassification->setMinimum(0);
        ui->pbProgressClassification->setMaximum(100);
        ui->pbProgressClassification->setValue(0);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        
        vector<double> gridC { 0.001, 0.01, 0.1, 1, 2, 5, 10, 20 };

        vector<double> gridGamma {0.0001, 0.001, 0.01, 0.1, 0.5, 1.0, 2, 4, 6 };

        Mat performanceGrid = Mat::zeros(gridC.size(), gridGamma.size(), CV_32FC1);

        
        ui->pbProgressClassification->setVisible(true);
        int countProgress = 0;

        for (int paramC=0; paramC<gridC.size(); paramC++)
        {
            for (int paramG=0; paramG<gridGamma.size(); paramG++)
            {
                vector <float> meanAccuracyKFold; 

                indicesKFOLD = crossValidationKfold_TestTrainMat();

                
                for (int kF = 0; kF < kFoldNum; kF++)
                {
                    int mycountTestOpt = count (indicesKFOLD.begin(), indicesKFOLD.end(), kF); 
                    int mycountTrainOpt = trainMat.rows - mycountTestOpt;

                    Mat trainOptimizeMat = Mat::zeros(mycountTrainOpt,featureNumber,CV_32FC1);
                    Mat testOptimizeMat = Mat::zeros(mycountTestOpt,featureNumber,CV_32FC1);

                    vector<int> trainOptimizeLabels; vector<int> testOptimizeLabels;

                    int counterTest1 = 0; int counterTrain1 = 0;
                    for (int sizeIndices = 0; sizeIndices<indicesKFOLD.size(); sizeIndices++)
                    {
                        if (indicesKFOLD[sizeIndices] == kF)
                        {
                            trainMat.row(sizeIndices).copyTo(testOptimizeMat.row(counterTest1));

                            testOptimizeLabels.push_back(trainLabels[sizeIndices]);
                            counterTest1++;
                        }
                        else
                        {
                            trainMat.row(sizeIndices).copyTo(trainOptimizeMat.row(counterTrain1));
                            trainOptimizeLabels.push_back(trainLabels[sizeIndices]);
                            counterTrain1++;
                        }
                    }

                    Mat testOptimizeResponse;

                    Ptr<SVM> svm = SVM::create();

                    
                    svm->setGamma(gridGamma[paramG]);
                    svm->setC(gridC[paramC]);
                    svm->setTermCriteria( TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1e6, 1e-3 ) );


                    
                    svm->setKernel(SVM::RBF);

                    
                    svm->setType(SVM::C_SVC);

                    
                    Ptr<TrainData> tdOptimize = TrainData::create(trainOptimizeMat, ROW_SAMPLE, trainOptimizeLabels);
                    svm->train(tdOptimize);

                    svm->predict(testOptimizeMat, testOptimizeResponse);

                    
                    int counterTrue = 0;
                    for(int i=0;i<testOptimizeResponse.rows;i++)
                    {
                        if(testOptimizeResponse.at<float>(i,0) == testOptimizeLabels[i]){
                            counterTrue++;
                        }
                    }
                    float accuracy = (float)((float)counterTrue/(float)testOptimizeResponse.rows)*100;
                    meanAccuracyKFold.push_back(accuracy);
                }


                Scalar meanVKFold; Scalar stdVVKFold;
                meanStdDev(meanAccuracyKFold, meanVKFold, stdVVKFold);

                performanceGrid.at<float>(paramC, paramG) = meanVKFold[0];

                countProgress = countProgress+1;

                
                ui->pbProgressClassification->setValue((countProgress*100)/(gridC.size()*gridGamma.size()));
                QCoreApplication::flush();
                QCoreApplication::processEvents();
            }
        } 


        ui->pbProgressClassification->setVisible(false);
        QCoreApplication::flush();
        QCoreApplication::processEvents();

        
        double minL, maxL;
        Point min_loc, max_loc;

        minMaxLoc(performanceGrid, &minL, &maxL, &min_loc, &max_loc); 

        bestC = gridC[max_loc.y];
        
        bestG = gridGamma[max_loc.x];
        Ptr<SVM> svm = SVM::create();

       
        svm->setGamma(bestG);
        svm->setC(bestC);
        svm->setTermCriteria( TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1e6, 1e-3 ) );

        svm->setKernel(SVM::RBF);

        svm->setType(SVM::C_SVC);

        Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
        svm->train(td);

        QString dirFileTrainedModel = QFileDialog::getExistingDirectory(this, tr("Select directory to save trained model:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (dirFileTrainedModel.isEmpty()){

            svm->save("modelTrained.yml");
        }

        else
        {
            string nameModelTrainedFile = dirFileTrainedModel.toStdString() + "/modelTrained.yml";
            svm->save(nameModelTrainedFile);
        }

        ui->lblClassificationStart->setVisible(true);
        ui->lblClassificationStart->setText("Machine learning model trained and saved!");
        ui->lblClassificationStart->setStyleSheet("QLabel {color: green;}");
        QCoreApplication::flush();
        QCoreApplication::processEvents();

    }

    else 
    {
        Ptr<SVM> svm = SVM::create();

        svm->setC(1.0); 
        svm->setGamma(0.1);

        
        svm->setKernel(SVM::RBF);

        svm->setType(SVM::C_SVC);

        Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
        svm->train(td);

        QString dirFileTrainedModel = QFileDialog::getExistingDirectory(this, tr("Select directory to save trained model:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (dirFileTrainedModel.isEmpty()){

            
            svm->save("modelTrained.yml");
        }

        else
        {
            
            string nameModelTrainedFile = dirFileTrainedModel.toStdString() + "/modelTrained.yml";
            svm->save(nameModelTrainedFile);
        }

        ui->lblClassificationStart->setVisible(true);
        ui->lblClassificationStart->setText("Machine learning model trained and saved!");
        ui->lblClassificationStart->setStyleSheet("QLabel {color: green;}");
        QCoreApplication::flush();
        QCoreApplication::processEvents();
    }
}

void MainWindow::SVMevaluate(){

    
    int counterTrue = 0;
    for(int i=0;i<testResponse.rows;i++)
    {
        if(testResponse.at<float>(i,0) == testLabels[i]){
            counterTrue++;
        }

    }

    int totalnumberOfObservations = testResponse.rows;

    accuracyM = (float)((float)counterTrue/(float)totalnumberOfObservations)*100;

    cout.precision(4);
    confusionMatrix = Mat::zeros(numberOfClass,numberOfClass,CV_32FC1);

    for (int featRow=0; featRow<numberOfClass; featRow++)
    {
        int counterTotalRow = 0;

        for(int j=0;j<testLabels.size();j++)
        {
            if(testLabels[j] == featRow){
                counterTotalRow++;
            }
        }

        for (int featCol=0; featCol<numberOfClass; featCol++)
        {
            int counterTrue2 = 0;
            for(int i=0;i<testResponse.rows;i++)
            {
                if(testResponse.at<float>(i,0)==featCol && testLabels[i] == featRow){
                    counterTrue2++;
                }
            }
            if (counterTotalRow == 0)
            {
                confusionMatrix.at<float>(featRow,featCol) = ((float)0.0);
            }
            else
            {
                confusionMatrix.at<float>(featRow,featCol) = ((float)((((float)counterTrue2)/((float)counterTotalRow))*100));
            }
        }
    }
    
	float TP, FP, FN;

    for (int classPositive=0; classPositive<numberOfClass+1; classPositive++) 
    {
        TP = 0; FP = 0; FN = 0;

        for(int i=0;i<testResponse.rows;i++)
        {
            if(testResponse.at<float>(i,0)==classPositive && testLabels[i] == classPositive){
                TP = TP+1;
            }
            else if (testResponse.at<float>(i,0)==classPositive && testLabels[i] != classPositive){
                FP = FP +1;
            }
            else if (testResponse.at<float>(i,0)!=classPositive && testLabels[i] == classPositive){
                FN = FN +1;
            }
        }

        float precision, recall;
        if (((TP + FP) == 0) && ((TP + FP) == 0))
            F1ScoreVec.push_back(0.0);
        else
        {
            if ((TP + FN) == 0)
            {
                recall = 0.0;
            }
            else if (((TP + FN) == 0))
            {
                precision = 0.0;
            }
            else
            {
                precision = (float)(TP/(TP + FP));
                recall = (float)(TP/(TP + FN));
            }

            if ((precision + recall)==0)
                F1ScoreVec.push_back(0.0);
            else
            {
                F1ScoreVec.push_back(2*((precision*recall)/(precision + recall)));
            }
        }
    }

    
    ui->pbProgressClassification->setVisible(false);

    
    QString dirFileResultsTesting = QFileDialog::getExistingDirectory(this, tr("Select directory to save testing results:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    string nameDirectoryResultsTesting;

    if (dirFileResultsTesting.isEmpty()){

        
        nameDirectoryResultsTesting = "";
    }

    else
    {
        
        nameDirectoryResultsTesting = dirFileResultsTesting.toStdString() + "/";
    }

    
    string nameFiletestingResults = nameDirectoryResultsTesting + "outputPerformanceTrainedModel.csv";

    ofstream myfile;
    myfile.open (nameFiletestingResults);

    if (myfile) {
        myfile << "accuracy" << ";" << accuracyM << "\n";
        myfile << "\n";

        
        for (int line=0; line<confusionMatrix.rows; line++)
        {
            for (int col=0; col<confusionMatrix.cols; col++)
            {
                myfile << confusionMatrix.at<float>(line,col) << ";";
            }
            myfile << "\n";
        }

        myfile << "\n";

        for (int vI = 0; vI<F1ScoreVec.size(); vI ++)
        {
            myfile << F1ScoreVec[vI] << "\n";

        }
    }

    myfile.close();

    
    string nameFileTestingEthogram = nameDirectoryResultsTesting + "ethogramTestingResults.csv";

    ofstream myfileEthogram1;
    myfileEthogram1.open (nameFileTestingEthogram);

    if (myfileEthogram1) {

        myfileEthogram1 << "GroundTruth labels" << ";" << "Predicted labels" << "\n";

        for(int r=0;r<testResponse.rows;r++)
        {
            int responseValue = testResponse.at<float>(r,0);
            int labelValue = testLabels[r];
            myfileEthogram1 << to_string(labelValue) << ";" << to_string(responseValue) << "\n";
        }
    }
    myfileEthogram1.close();

}

void MainWindow::saveToCSVFileCentroid(string nameFile, vector<vector<float>> coordVector)
{
    ofstream myfile;
    myfile.open (nameFile);

    if (myfile) {
        myfile << "x [pix.]"  << ";" << "y [pix.]"  << ";" << "z [mm]" << "\n";
        for (int i  = 0; i < coordVector.size(); i++)
        {
            for (int col = 0; col < 3; col++)
            {
                myfile << coordVector[i][col]  << ";";
            }
            myfile << "\n";
        }
    }

    else
    {
        ui->edtSaveTrackingData->setStyleSheet("QLabel { color: red;}");
        ui->edtSaveTrackingData->setText("Error: file not saved!");
    }


    myfile.close();
}

void MainWindow::saveToCSVFileBodyParts(string nameFile, vector<vector<float>> bodyVector)
{
    ofstream myfile;
    myfile.open (nameFile);

    if (myfile) {
        myfile << "Flag" << ";" << "Tail end"  << ";" << ";" <<  ";" << "Nose" <<  ";" << ";" << ";" << "Head" <<  ";" << ";" << ";" << "Tail base" <<  ";" << ";" << ";" << "Head direction" << "\n";
        myfile << "0/1" << ";" << "x [pix.]"  << ";" << "y [pix.]"  << ";" << "z [mm]" << ";" << "x [pix.]"  << ";" << "y [pix.]"  << ";" << "z [mm]" << ";" << "x [pix.]"  << ";" << "y [pix.]"  << ";" << "z [mm]" << ";" << "x [pix.]"  << ";" << "y [pix.]"  << ";" << "z [mm]" << ";" << "degrees" << "\n";
        for (int i  = 0; i < bodyVector.size(); i++)
        {
            for (int col = 0; col < 14; col++)
            {
                myfile << bodyVector[i][col]  << ";";
            }
            myfile << "\n";
        }

        ui->edtSaveTrackingData->setText("All files saved!");
        ui->edtSaveTrackingData->setStyleSheet("QLabel { color: green;}");

    }

    else
    {
        ui->edtSaveTrackingData->setText("Error: file not saved!");
        ui->edtSaveTrackingData->setStyleSheet("QLabel { color: red;}");
    }


    myfile.close();
}

void MainWindow::saveToCSVFileFEATURES(string nameFile, Mat FeatureMatToSave, vector<int> LabelsVecToSave)
{
    ofstream myfile;
    myfile.open (nameFile);

    if (myfile) {
        myfile << "Labels" << ";" << "F1"  << ";" << "F2"  << ";" << "F3"  << ";" << "F4"  << ";" << "F5"  << ";" << "F6"  << ";" << "F7"  << ";" << "F8"  << ";" << "F9"  << ";" << "F10"  << ";" << "F11" << "\n";
        for (int i  = 0; i < LabelsVecToSave.size(); i++)
        {
            myfile << LabelsVecToSave[i]  << ";";

            for (int col = 0; col < FeatureMatToSave.cols; col++)
            {
                myfile << FeatureMatToSave.at<float>(i,col)  << ";";
            }
            myfile << "\n";
        }

        cout << "DONE" << endl;
    }

    else
    {
        cout << "NOT DONE" << endl;
    }


    myfile.close();
}

void MainWindow::on_btnStartNewClassification_clicked()
{
    ui->tabTrain->setEnabled(false);
    ui->tabTesting->setEnabled(false);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    string folderTrackedFrames = dirFileString + "//trackedFrames";
    CreateFeatureObject(folderTrackedFrames);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    vector<Mat>().swap(realForegroundSubtracted); vector<vector<float>>().swap(bodyPartsVectorFinal); vector<vector<float>>().swap(centroidVectorFinal);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    featureAugmentation(true);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    featureNormalizationZ();

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    testMat = featureMatObject22Features.clone();
    
    QCoreApplication::flush();
    QCoreApplication::processEvents();

    svmNew->predict(testMat, testResponse);

    QString dirFileResultsClassify = QFileDialog::getExistingDirectory(this, tr("Select directory to save classification results:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    string nameDirectoryResultsClassify;

    if (dirFileResultsClassify.isEmpty()){

        
        nameDirectoryResultsClassify = "";
    }

    else
    {
        
        nameDirectoryResultsClassify = dirFileResultsClassify.toStdString() + "/";
    }
    string nameFileClassifyEthogram = nameDirectoryResultsClassify + "ethogramClassificationResults.csv";

    ofstream myfileEthogram1;
    myfileEthogram1.open (nameFileClassifyEthogram);

    if (myfileEthogram1) {

        myfileEthogram1 << "Predicted labels" << "\n";

        for(int r=0;r<testResponse.rows;r++)
        {
            int responseValue = testResponse.at<float>(r,0);
            myfileEthogram1 << to_string(responseValue) << "\n";
        }
    }
    myfileEthogram1.close();

    ui->lblClassificationStart_2->setVisible(true);
    ui->lblClassificationStart_2->setText("Classification completed!");
    ui->lblClassificationStart_2->setStyleSheet("QLabel {color: green;}");
    QCoreApplication::flush();
    QCoreApplication::processEvents();

}

void MainWindow::on_btnStartTesting_clicked()
{
    
    ui->tabTrain->setEnabled(false);
    ui->tabClassify->setEnabled(false);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    
    string folderTrackedFrames = dirFileString + "//trackedFrames";
    CreateFeatureObject(folderTrackedFrames);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    
    vector<Mat>().swap(realForegroundSubtracted); vector<vector<float>>().swap(bodyPartsVectorFinal); vector<vector<float>>().swap(centroidVectorFinal);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    
    featureAugmentation(false);

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    featureNormalizationZ();

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    testMat = featureMatObject22Features.clone();
    testLabels = labelsObject;

    QCoreApplication::flush();
    QCoreApplication::processEvents();

    svmNew->predict(testMat, testResponse);

    SVMevaluate();

    ui->lblClassificationTesting->setVisible(true);
    ui->lblClassificationTesting->setText("Testing method implemented!");
    ui->lblClassificationTesting->setStyleSheet("QLabel {color: green;}");
    QCoreApplication::flush();
    QCoreApplication::processEvents();


}

void MainWindow::on_btnOpenAnnotatedFileTesting_clicked()
{
    QString fileName1 = QFileDialog::getOpenFileName(this, tr("Select annotation file with following name : 'dataBehav_' "), " ", tr("CSV files (*.csv)"));
    string str2 ("dataBehav");

    if (fileName1.isEmpty()){
        ui->edtAnnotatedFileTesting->setEnabled(true);

        ui->edtAnnotatedFileTesting->setStyleSheet("QLineEdit {color: red;}");
        ui->edtAnnotatedFileTesting->setText("Select file!");
    }
    else if ((fileName1.toStdString()).find(str2) == string::npos)
    {
        ui->edtAnnotatedFileTesting->setEnabled(true);

        ui->edtAnnotatedFileTesting->setStyleSheet("QLineEdit {color: red;}");
        ui->edtAnnotatedFileTesting->setText("Select a correct file!");

    }
    else
    {
        ui->edtAnnotatedFileTesting->setStyleSheet("QLineEdit {color: black;}");
        ui->edtAnnotatedFileTesting->setText(fileName1);

        
        ifstream fileResume (fileName1.toStdString());

        string value;
        int count = 0;
        
        while ( fileResume.good() )
        {
            getline ( fileResume, value, '\n' );
            if (value.empty()){
                count++;
            }
            else{
                if (count == 0 ) 
                {
                    nameBehaviorsLabel = value;
                    count++;
                }

                else if (count == 1) 
                {
                    count++;
                }

                else
                {
                    contentsFileResume.push_back(value);
                    count++;
                }
            }
        }

        fileResume.close();

        for (int cont=0; cont<locationDepthVec.size(); cont++)
        {
            int locationOfDepth = locationDepthVec[cont];
            vector<string> vF1 = splitString(contentsFileResume[floor(locationOfDepth/2)], ';');
            labelsObject.push_back(stoi(vF1[1]));
        }

        ui->btnStartTesting->setEnabled(true);

    }

}

void MainWindow::on_btnOpenTrainedModel_2_clicked()
{
    QString fileName1 = QFileDialog::getOpenFileName(this, tr("Select trained model file with following name : 'modelTrained' "), " ", tr("Model files (*.yml)"));

    if (fileName1.isEmpty()){
        ui->edtTrainedModel_2->setEnabled(true);

        ui->edtTrainedModel_2->setStyleSheet("QLineEdit {color: red;}");
        ui->edtTrainedModel_2->setText("Select file!");
    }
    else
    {
        ui->edtTrainedModel_2->setStyleSheet("QLineEdit {color: black;}");
        ui->edtTrainedModel_2->setText(fileName1);

        
        svmNew = SVM::load<SVM>(fileName1.toStdString());
    }
}

void MainWindow::on_btnOpenTrainedModel_3_clicked()
{
    QString fileName1 = QFileDialog::getOpenFileName(this, tr("Select trained model file with following name : 'modelTrained' "), " ", tr("Model files (*.yml)"));

    if (fileName1.isEmpty()){
        ui->edtTrainedModel_3->setEnabled(true);

        ui->edtTrainedModel_3->setStyleSheet("QLineEdit {color: red;}");
        ui->edtTrainedModel_3->setText("Select file!");
    }
    else
    {
        ui->edtTrainedModel_3->setStyleSheet("QLineEdit {color: black;}");
        ui->edtTrainedModel_3->setText(fileName1);

        
        svmNew = SVM::load<SVM>(fileName1.toStdString());
    }

    ui->btnStartNewClassification->setEnabled(true);
}

void MainWindow::on_btnParameters_clicked()
{

    if (counterParameters == 0)
    {
        ui->gbParameters->setVisible(true);
        counterParameters = 1;
    }

    else
    {
        counterParameters = 0;
        ui->gbParameters->setVisible(false);
    }
}

