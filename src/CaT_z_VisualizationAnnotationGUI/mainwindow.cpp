
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon("RatLogo.ico");
    setWindowIcon(icon);

    ui->sbFrames->setEnabled(false);
     ui->edtFrameNumber->setEnabled(false);
    ui->edtTimeStampFrame->setEnabled(false);

    connect( ui->sbFrames, SIGNAL( valueChanged( int )),
            this, SLOT( valueChanged( int ) ));
    connect( ui->edtFrameNumber, SIGNAL( textChanged( const QString & )),
            this, SLOT( FrameTextChanged( const QString & ) ));

    listNamesBehav=(QStringList()<<"Undefined"<<"Standstill"<<"Walking"<<"Moving Exploration"<< "Local Exploration" << "Supported Rearing"<<"Unsupported Rearing"<<"Grooming");

    colorBehavior = {Qt::black, Qt::darkYellow, Qt::darkRed, Qt::darkMagenta, Qt::darkGreen, Qt::darkCyan, Qt::darkBlue, Qt::darkGray, Qt::yellow, Qt::red, Qt::magenta, Qt::green, Qt::cyan, Qt::blue, Qt::gray};
    colorStringBehavior = {"black", "darkYellow", "darkRed", "darkMagenta", "darkGreen", "darkCyan", "darkBlue", "darkGray", "yellow", "red", "magenta", "green", "cyan", "blue", "gray"};

    ui->tEthogram->setVisible(false);

    ui->btnSave->setEnabled(false);
    ui->edtBehaviorEvent->setEnabled(false);
    ui->edtElapsedTime->setEnabled(false);

    countNewWindow = 0;
    YesItOpennedDir = false;

    btnTableDock = new QPushButton();
    btnTableDock->setText("New Entry");

    btnTableDock->setStyleSheet("background-color: rgb(0, 85, 127);color: rgb(255, 255, 255);border-radius:10px;");

    connect(btnTableDock, SIGNAL(clicked()), this, SLOT(btnTableDock_clicked()));

    btnTableDockDelete = new QPushButton();
    btnTableDockDelete->setText("Delete Last Entry");
    btnTableDockDelete->setEnabled(false);
    btnTableDockDelete->setStyleSheet("background-color: rgb(0, 85, 127);color: rgb(255, 255, 255);border-radius:10px;");

    connect(btnTableDockDelete, SIGNAL(clicked()), this, SLOT(btnTableDockDelete_clicked()));

    ui->btnOpenAnnotationTool->setEnabled(false);

    countNumb = 0;

    connect(&EditTextSignalMapper, SIGNAL(mapped(int)), this, SLOT(CellEditTextDockChanged(int)));
    connect(&ComboBoxSignalMapper, SIGNAL(mapped(int)), this, SLOT(CellComboBoxClicked(int)));

    listVisualizationCombo=(QStringList()<<"Gray Scale - default contrast" << "Gray Scale - increased contrast" << "Depth Gradient");
    ui->cbVisualization->addItems(listVisualizationCombo);

    ui->cbVisualization->setEnabled(false);

    nameFolderSave = "";

    fileNotSaved = true;

    ui->lblBehavioralEthogram->setVisible(false);

    QRect *rect = new QRect(0,0,29,29);
    QRegion* region = new QRegion(*rect,QRegion::Ellipse);
    ui->btnPlay->setMask(*region);
    QPixmap pixmap("playBTN.png");
    QIcon ButtonIcon(pixmap);
    ui->btnPlay->setIcon(ButtonIcon);
    ui->btnPlay->setIconSize(pixmap.rect().size());

    countClick = false;

    ui->btnFastF->setMask(*region);
    QPixmap pixmap2("fastFBTN.png");
    QIcon ButtonIcon2(pixmap2);
    ui->btnFastF->setIcon(ButtonIcon2);
    ui->btnFastF->setIconSize(pixmap.rect().size());

    ui->btnStop->setMask(*region);
    QPixmap pixmap3("stopBTN.png");
    QIcon ButtonIcon3(pixmap3);
    ui->btnStop->setIcon(ButtonIcon3);
    ui->btnStop->setIconSize(pixmap.rect().size());

    ui->btnFastF->setEnabled(false);
    ui->btnPlay->setEnabled(false);
    ui->btnStop->setEnabled(false);

    // Timer for videoPlayer
    timerPlayVideo = new QTimer(this);
    activePlayVideo = 0;

    connect(timerPlayVideo, SIGNAL(timeout()), this, SLOT(setTimerSingleFrame()));
    frameNumberPlay = 0;
    velocityVideo = 0;
    ui->btnFastF->setToolTip("Click to Fast-forward: 2x or 4x");
    ui->btnPlay->setToolTip("Play");
    ui->btnStop->setToolTip("Stop");

    correctItemBool = true;

}

MainWindow::~MainWindow()
{

    if (YesItOpennedDir){

        if (activePlayVideo==1)
        {
            timerPlayVideo->stop();
        }

        if (fileNotSaved)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Save annotation files", "Do you want to save the annotation file?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                on_btnSave_clicked();
                delete ui;
            }

            else
            {
                delete ui;
            }
        }

        else
        { delete ui;}
    }

    else
    {
        delete ui;
    }
}

void MainWindow::on_btnOpenDir_clicked() 
{

    ui->edtFileResume->setStyleSheet("QLineEdit {color: black;}");
    ui->edtFileResume->setText("");

    int counterFramesTotal = 0;
    int counterFramesReal = 0;

    if (YesItOpennedDir)
    {
          dock->close();
    }

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
        else {

            list.clear();

            SelectBehaviorLabels *selB = new SelectBehaviorLabels();
            selB->setModal(true);
            selB->exec();

            listNamesBehav = selB->getNewListOfBehaviorNames();

            vector<string> v1Folder = splitString(dirFileString, '/');
            nameFolderSave = v1Folder.back();

            string strName1 = listFileNames[0];
            string strName2 = listFileNames[0+1];

            vector<string> v1Test = splitString(strName1, '_');
            vector<string> v2Test = splitString(strName2, '_');

            if (v1Test.size() != 3){ 
                ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                ui->edtFileVideo->setText("Select folder with valid .png files!");
            }
            else if (v2Test.size() != 3){ 
                ui->edtFileVideo->setStyleSheet("QLineEdit {color: red;}");
                ui->edtFileVideo->setText("Select folder with valid .png files!");
            }

            else {

                timeStampsRGBVec.resize(listFileSize/2); int countTime = 0;

                
                for (int i = 0; i<(listFileSize); i+=2){

                    if (i>=listFileSize-1)
                    {
                        listFileNames.erase(listFileNames.end() - 1);
                        listFileSize = listFileNames.size();
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

                                timeStampsRGBVec[countTime] = timeStampC;

                                float timeStampD = stof(v2[1]);
                                timeStampsDepthInitial = timeStampD;

                                counterFramesTotal++; counterFramesReal++; countTime++;
                            }

                            
                            else {
                                
                                float timeStampC = stof(v2[1]);
                                timeStampsRGBInitial = timeStampC;

                                timeStampsRGBVec[countTime] = timeStampC;

                                float timeStampD = stof(v1[1]);
                                timeStampsDepthInitial = timeStampD;

                                counterFramesTotal++; counterFramesReal++; countTime++;
                            }
                        }

                        else { 
							if (v1[2]=="c.png"){
                                float timeStampC = stof(v1[1]);
                                float timeStampD = stof(v2[1]);

                                timeStampsRGBVec[countTime] = timeStampC;
                                countTime++;

                                if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;} 

                                else if (timeStampD == timeStampsDepthInitial) {counterFramesTotal++;}

                                else {
                                    timeStampsRGBInitial = timeStampC;

                                    timeStampsDepthInitial = timeStampD;

                                    counterFramesReal++; counterFramesTotal++;
                                }
                            }

                            else {
                                
                                float timeStampC = stof(v2[1]);
                                float timeStampD = stof(v1[1]);

                                timeStampsRGBVec[countTime] = timeStampC;
                                countTime++;

                                if (timeStampC == timeStampsRGBInitial){counterFramesTotal++;}
                                else if (timeStampD == timeStampsDepthInitial) {counterFramesTotal++;} 
                                else {
                                    timeStampsRGBInitial = timeStampC;

                                    timeStampsDepthInitial = timeStampD;

                                    counterFramesReal++;counterFramesTotal++;
                                }
                            }
                        }
                    }
                }

                YesItOpennedDir = true;

                if (timeStampsRGBVec[0]>1000)
                {
                    adjacent_difference (timeStampsRGBVec.begin(), timeStampsRGBVec.end(), timeStampsRGBVec.begin());
                    timeStampsRGBVec[0] = 0;
                    partial_sum (timeStampsRGBVec.begin(), timeStampsRGBVec.end(), timeStampsRGBVec.begin());

                }

                ui->cbVisualization->setEnabled(true);

				ui->sbFrames->setRange(0, (listFileSize/2)-1);
                ui->sbFrames->setEnabled(true);
                ui->sbFrames->setValue(0);
                ui->edtFrameNumber->setEnabled(true);

                QValidator *validator = new QIntValidator(0, (listFileSize/2)-1, this);
                ui->edtFrameNumber->setValidator(validator);

                QString stringF = "MAX = " + QString::number((listFileSize/2)-1);
                ui->lblMaxFrames->setText(stringF);

                behaviorVec.resize((listFileSize/2));
                fill(behaviorVec.begin(),behaviorVec.end(),0); 
				
				string strNameF1 = listFileNames[0];
                string strNameF2 = listFileNames[1];

                
				vector<string> vF1 = splitString(strNameF1, '_');
                vector<string> vF2 = splitString(strNameF2, '_');

                Mat imageRGBF1; Mat imageDepthF1; float timeStampC; float timeStampD;
                
				if (vF1[2]=="c.png"){

                    timeStampC = stof(vF1[1]);
                    timeStampD = stof(vF2[1]);

                    string nameFileCF1 = dirFileString + "/" + strNameF1;
                    imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

                    string nameFileDF2 = dirFileString + "/" + strNameF2;
                    imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);
                }
                else {

                    timeStampC = stof(vF2[1]);
                    timeStampD = stof(vF1[1]);

                    string nameFileCF1 = dirFileString + "/" + strNameF2;
                    imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

                    string nameFileDF2 = dirFileString + "/" + strNameF1;
                    imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);

                }

                Mat clone2 = imageRGBF1.clone();
                cvtColor(clone2, clone2, CV_BGR2RGB);
                QImage imageRGB (clone2.data, clone2.cols, clone2.rows, QImage::Format_RGB888);
                ui->lblRGB->setPixmap(QPixmap::fromImage(imageRGB));
                ui->lblRGB->repaint();

                Mat viz2 = DepthToBeVisualized (imageDepthF1);

                QImage imageD(viz2.data, viz2.cols, viz2.rows, viz2.step, QImage::Format_Indexed8);
                ui->lblDepth->setPixmap(QPixmap::fromImage(imageD));
                ui->lblDepth->repaint();

                ui->edtFrameNumber->setText(QString::number(0));
                ui->edtTimeStampFrame->setText(QString::number(timeStampC));
                ui->edtTimeStampFrame_2->setText(QString::number(timeStampD));
                ui->edtBehaviorEvent->setText(listNamesBehav[0]);
                ui->edtBehaviorEvent->setStyleSheet("color:" + colorStringBehavior[0]);
                ui->edtElapsedTime->setText(QString::number(timeStampsRGBVec[0]*pow(10,-3)));

                ui->tEthogram->setVisible(false);
                ui->tEthogram->setRowCount(1);
                ui->tEthogram->setColumnCount((listFileSize/2));

                for (int j = 0; j<(listFileSize/2); j++){
                    ui->tEthogram->setItem(0,j,new QTableWidgetItem(""));
                    float a = (ui->tEthogram->width());
                    float b = ((listFileSize/2));
                    int c = ceil(a/b);
                    ui->tEthogram->setColumnWidth(j, c);

                }

                ui->tEthogram->setVisible(true);
                ui->btnSave->setEnabled(true);

                ui->lblBehavioralEthogram->setVisible(true);

                ui->btnStop->setEnabled(false);
                ui->btnFastF->setEnabled(false);
                ui->btnPlay->setEnabled(true);

                countNewWindow++;

                ui->btnOpenAnnotationTool->setEnabled(true);

                dock = new QDockWidget(tr("Behaviors' Annotation"), this);

                dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
                tablewidget = new QTableWidget(dock);

                tablewidget->insertRow(0);
                tablewidget->insertRow(1);
                tablewidget->insertColumn(0);
                tablewidget->insertColumn(1);
                tablewidget->insertColumn(2);

                tablewidget->setHorizontalHeaderLabels(QStringList() << "Frame" << "Behavior" << "Comments");
                tablewidget->verticalHeader()->setVisible(false);

                tablewidget->setCellWidget(0,0,btnTableDock); btnTableDock->setToolTip("Click to add a new behavior");
                tablewidget->setCellWidget(0,1, btnTableDockDelete); btnTableDockDelete->setToolTip("Click to remove the last behavior");
                tablewidget->setItem(0, 2, new QTableWidgetItem(""));
                tablewidget->item(0, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                tablewidget->setItem(1, 0, new QTableWidgetItem(""));
                tablewidget->item(1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

                tablewidget->horizontalHeader()->setStretchLastSection(true);
                tablewidget->resizeColumnsToContents();

                tablewidget->setSpan(1,0,1,3);
                tablewidget->setShowGrid(false);

                dock->setWidget(tablewidget);
                addDockWidget(Qt::RightDockWidgetArea, dock);
                dock->show();

            }
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

    if (ui->cbVisualization->currentIndex() == 0) 
    {

        float medianVec = medianMat(inputframe);

        subtract(medianVec, inputframe, visualizInputframe);

        visualizInputframe.setTo(0, visualizInputframe >255);

        visualizInputframe.convertTo(visualizInputframe, CV_8U, 1, 0);

    }

    else if (ui->cbVisualization->currentIndex() == 1) 
    {
        float medianVec = medianMat(inputframe);

        subtract(medianVec, inputframe, visualizInputframe);

        Mat visualizInputfram2e = visualizInputframe.clone();
        Mat visualizInputfram3e = visualizInputframe.clone();

        add(visualizInputfram2e, visualizInputfram3e, visualizInputframe);

        visualizInputframe.setTo(0, visualizInputframe >255);

        visualizInputframe.convertTo(visualizInputframe, CV_8U, 1, 0);
    }

    else { 

        
        for (int i = 0; i<inputframe.rows; i++)
        {
            for (int j = 0; j<inputframe.cols; j++)
            {
                unsigned short value1 = inputframe.at<unsigned short>(i,j);
                visualizInputframe.at<unsigned short>(i, j) = ((5*(unsigned short)value1)%255);
            }
        }
        visualizInputframe.convertTo(visualizInputframe, CV_8U, 1, 0);
    }

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

float MainWindow::GetMedian(vector<float> vectorM) {

    vector<float> dpSorted = vectorM;
    int len = dpSorted.size();

    nth_element( dpSorted.begin(), dpSorted.begin()+len/2, dpSorted.end() );

    float dMedian = dpSorted[len/2];

    return dMedian;
}

void MainWindow::FrameTextChanged( const QString &text )
{
    ui->sbFrames->setValue( (ui->edtFrameNumber->text()).toInt());
}

void MainWindow::valueChanged( int value )
{

    frameNumberPlay = value;

    string strNameF1 = listFileNames[value*2];
    string strNameF2 = listFileNames[value*2 +1];

    vector<string> vF1 = splitString(strNameF1, '_');
    vector<string> vF2 = splitString(strNameF2, '_');

    Mat imageRGBF1; Mat imageDepthF1; float timeStampC; float timeStampD;

    if (vF1[2]=="c.png"){

        timeStampC = stof(vF1[1]);
        timeStampD = stof(vF2[1]);

        string nameFileCF1 = dirFileString + "/" + strNameF1;
        imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

        string nameFileDF2 = dirFileString + "/" + strNameF2;
        imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);
    }
    else {

        timeStampC = stof(vF2[1]);
        timeStampD = stof(vF1[1]);

        string nameFileCF1 = dirFileString + "/" + strNameF2;
        imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

        string nameFileDF2 = dirFileString + "/" + strNameF1;
        imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);

    }

    Mat clone2 = imageRGBF1.clone();
    cvtColor(clone2, clone2, CV_BGR2RGB);
    QImage imageRGB (clone2.data, clone2.cols, clone2.rows, clone2.step, QImage::Format_RGB888);
    ui->lblRGB->setPixmap(QPixmap::fromImage(imageRGB));
    ui->lblRGB->repaint();

    Mat viz2 = DepthToBeVisualized (imageDepthF1);

    QImage imageD(viz2.data, viz2.cols, viz2.rows, viz2.step, QImage::Format_Indexed8);
    ui->lblDepth->setPixmap(QPixmap::fromImage(imageD));
    ui->lblDepth->repaint();

    ui->edtFrameNumber->setText(QString::number(value));
    ui->edtTimeStampFrame->setText(QString::number(timeStampC));
    ui->edtTimeStampFrame_2->setText(QString::number(timeStampD));

    ui->edtElapsedTime->setText(QString::number(timeStampsRGBVec[value]*pow(10,-3)));

    ui->edtBehaviorEvent->setText(listNamesBehav[behaviorVec[value]]);
    ui->edtBehaviorEvent->setStyleSheet("color:" + colorStringBehavior[behaviorVec[value]]);

    ui->tEthogram->horizontalScrollBar()->setValue(value);


}

void MainWindow::btnTableDock_clicked(){ 

    if (tablewidget->rowCount()==2){ 
        tablewidget->insertRow( tablewidget->rowCount()-2 );

        QLineEdit *edit = new QLineEdit(tablewidget);
        edit->setValidator(new QIntValidator(0, (listFileNames.size()/2)-1, this));
        tablewidget->setCellWidget(tablewidget->rowCount()-3, 0, edit);

        connect(edit, SIGNAL(editingFinished()), &EditTextSignalMapper, SLOT(map()));
        EditTextSignalMapper.setMapping(edit, tablewidget->rowCount()-3);

        QComboBox* combo = new QComboBox();
        combo->addItems(listNamesBehav);
        for (int i = 0; i<listNamesBehav.size(); i++){
            combo->setItemData( i, QColor( colorBehavior[i] ), Qt::ForegroundRole );
        }

        tablewidget->setCellWidget(tablewidget->rowCount()-3,1,combo);

        connect(combo, SIGNAL(currentIndexChanged(int)), &ComboBoxSignalMapper, SLOT(map()));
        ComboBoxSignalMapper.setMapping(combo, tablewidget->rowCount()-3);

        QLineEdit *edit2 = new QLineEdit(tablewidget);
        edit2->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_? .-]*"), this ));
        tablewidget->setCellWidget(tablewidget->rowCount()-3, 2, edit2);

        tablewidget->setItem(tablewidget->rowCount()-1,0, new QTableWidgetItem(""));
        tablewidget->item(tablewidget->rowCount()-1,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        btnTableDockDelete->setEnabled(true);

    }

    else { 

        QLineEdit *editText = qobject_cast<QLineEdit*>(tablewidget->cellWidget(tablewidget->rowCount()-3,0));
        QString valueEdit = editText->text();

        if (valueEdit.isEmpty() ){}

        else {

            tablewidget->insertRow( tablewidget->rowCount()-2 );

            QLineEdit *edit = new QLineEdit(tablewidget);
            edit->setValidator(new QIntValidator(0, (listFileNames.size()/2)-1, this));
            tablewidget->setCellWidget(tablewidget->rowCount()-3, 0, edit);

            connect(edit, SIGNAL(editingFinished()), &EditTextSignalMapper, SLOT(map()));
            EditTextSignalMapper.setMapping(edit, tablewidget->rowCount()-3);

            QComboBox* combo = new QComboBox();
            combo->addItems(listNamesBehav);
            for (int i = 0; i<listNamesBehav.size(); i++){
                combo->setItemData( i, QColor( colorBehavior[i] ), Qt::ForegroundRole );
            }

            tablewidget->setCellWidget(tablewidget->rowCount()-3,1,combo);

            connect(combo, SIGNAL(currentIndexChanged(int)), &ComboBoxSignalMapper, SLOT(map()));
            ComboBoxSignalMapper.setMapping(combo, tablewidget->rowCount()-3);

            QLineEdit *edit2 = new QLineEdit(tablewidget);
            edit2->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_? .-]*"), this ));
            tablewidget->setCellWidget(tablewidget->rowCount()-3, 2, edit2);

            tablewidget->setItem(tablewidget->rowCount()-1,0, new QTableWidgetItem(""));
            tablewidget->item(tablewidget->rowCount()-1,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        }
    }

}

void MainWindow::btnTableDockDelete_clicked(){ 

    if (tablewidget->rowCount()==2){

    }

    else if (tablewidget->rowCount()==3)
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete the last classification entry", "Are you sure you want to delete the last classification entry?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            tablewidget->removeRow(tablewidget->rowCount()-3 );
            btnTableDockDelete->setEnabled(false);

        }
        else 
        {}
    }

    else {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete the last classification entry", "Are you sure you want to delete the last classification entry?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            tablewidget->removeRow(tablewidget->rowCount()-3 );
        }
        else
        {}

    }

}

void MainWindow::CellEditTextDockChanged(int RowNum)
{

    if (RowNum == 0) 
    {

        if (tablewidget->rowCount() >3)
        {
            QLineEdit *editTextNext = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum+1,0));
            QString valueEditNext = editTextNext->text();

            QLineEdit *editTextCurrent = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum,0));
            QString valueEditCurrent = editTextCurrent->text();

            if ((valueEditNext.toInt()) <= (valueEditCurrent.toInt())) 
            {
                tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("Insert a bigger frame number or modify the previous ones!"));
                tablewidget->item(tablewidget->rowCount()-1, 0)->setForeground(Qt::red);
                btnTableDock->setEnabled(false);
                correctItemBool = false;
            }

            else {
                tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("")); 
                tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                btnTableDock->setEnabled(true);
                correctItemBool = true;
                CellComboBoxClicked(RowNum);

            }
        }

        else
        {CellComboBoxClicked(RowNum);}
    }

    else
    {
        QLineEdit *editTextPrevious = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum-1,0));
        QString valueEditPrevious = editTextPrevious->text();

        QLineEdit *editTextCurrent = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum,0));
        QString valueEditCurrent = editTextCurrent->text();

        if ((valueEditPrevious.toInt()) >= (valueEditCurrent.toInt())) 
        {
            tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("Insert a bigger frame number or modify the previous ones!"));
            tablewidget->item(tablewidget->rowCount()-1, 0)->setForeground(Qt::red);
            btnTableDock->setEnabled(false);
            correctItemBool = false;
        }

        else {
            tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("")); 
            tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            correctItemBool = true;
            CellComboBoxClicked(RowNum);
            btnTableDock->setEnabled(true);
        }

    }
}

void MainWindow::CellComboBoxClicked (int RowNum) 
{
    QLineEdit *editText = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum,0));
    QString valueEdit = editText->text();

    QComboBox *myCB2 = qobject_cast<QComboBox*>(tablewidget->cellWidget(RowNum,1));

    vector <int> FramesV; vector <int> BehaviorV;

    if (valueEdit.isEmpty())
    {
        QLineEdit *editTextFrame = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum,0));
        QString valueEditFrame = ui->edtFrameNumber->text();

        editTextFrame->setText(valueEditFrame);
        CellEditTextDockChanged(RowNum);

    }

    else
    {

        if (RowNum == 0) 
        {
            if (tablewidget->rowCount() >3) 
            {
                QLineEdit *editTextNext = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum+1,0));
                QString valueEditNext = editTextNext->text();

                QLineEdit *editTextCurrent = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum,0));
                QString valueEditCurrent = editTextCurrent->text();

                if ((valueEditNext.toInt()) <= (valueEditCurrent.toInt())) 
                {
                    tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("Insert a bigger frame number or modify the previous ones!"));
                    tablewidget->item(tablewidget->rowCount()-1, 0)->setForeground(Qt::red);
                    tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    btnTableDock->setEnabled(false);
                    correctItemBool = false;
                }

                else {
                    tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("")); 
                    correctItemBool = true;
                    tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    fill(behaviorVec.begin(),behaviorVec.end(),0);

                    for (int colorEtho = 0; colorEtho<(ui->tEthogram->columnCount()); colorEtho++){
                        ui->tEthogram->item(0,colorEtho)->setBackgroundColor(colorBehavior[0]);
                    }

                    btnTableDock->setEnabled(true);

                    for (int rT = 0; rT<tablewidget->rowCount()-2; rT++)
                    {
                        QLineEdit *editText = qobject_cast<QLineEdit*>(tablewidget->cellWidget(rT,0));
                        QString valueEdit = editText->text();

                        QComboBox *myCB2 = qobject_cast<QComboBox*>(tablewidget->cellWidget(rT,1));
                        int valueCB2 = myCB2->currentIndex();

                        FramesV.push_back(valueEdit.toInt());
                        BehaviorV.push_back(valueCB2);
                    }

                    for (int s = 0; s<FramesV.size(); s++)
                    {

                        if (s==0){ 
                            behaviorVec[FramesV[s]] = BehaviorV[s];

                            ui->tEthogram->item(0,FramesV[s])->setBackgroundColor(colorBehavior[BehaviorV[s]]);
                        }

                        else
                        {
                            fill(behaviorVec.begin()+FramesV[s-1],behaviorVec.begin()+FramesV[s],BehaviorV[s-1]);

                            for (int colorT = FramesV[s-1]; colorT<FramesV[s]+1; colorT++){
                                ui->tEthogram->item(0,colorT)->setBackgroundColor(colorBehavior[behaviorVec[colorT]]);
                            }
                        }
                    }

                    fill(behaviorVec.begin() + FramesV.back(),behaviorVec.end(),BehaviorV.back());

                    for (int colorT2 = FramesV.back(); colorT2<behaviorVec.size(); colorT2++){
                        ui->tEthogram->item(0,colorT2)->setBackgroundColor(colorBehavior[behaviorVec[colorT2]]);
                    };
                }
            }

            else
            {
                tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("")); 
                correctItemBool = true;
                tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                fill(behaviorVec.begin(),behaviorVec.end(),0);

                for (int colorEtho = 0; colorEtho<(ui->tEthogram->columnCount()); colorEtho++){
                    ui->tEthogram->item(0,colorEtho)->setBackgroundColor(colorBehavior[0]);
                }

                for (int rT = 0; rT<tablewidget->rowCount()-2; rT++)
                {
                    QLineEdit *editText = qobject_cast<QLineEdit*>(tablewidget->cellWidget(rT,0));
                    QString valueEdit = editText->text();

                    QComboBox *myCB2 = qobject_cast<QComboBox*>(tablewidget->cellWidget(rT,1));
                    int valueCB2 = myCB2->currentIndex();

                    FramesV.push_back(valueEdit.toInt());
                    BehaviorV.push_back(valueCB2);
                }

                for (int s = 0; s<FramesV.size(); s++)
                {
                    if (s==0){ 
                        behaviorVec[FramesV[s]] = BehaviorV[s];
                        ui->tEthogram->item(0,FramesV[s])->setBackgroundColor(colorBehavior[BehaviorV[s]]);

                        for (int itcolorT = 0; itcolorT<FramesV[s]; itcolorT++){
                            ui->tEthogram->item(0,itcolorT)->setBackgroundColor(colorBehavior[behaviorVec[s]]);
                        }
                    }

                    else
                    {
                        fill(behaviorVec.begin()+FramesV[s-1],behaviorVec.begin()+FramesV[s],BehaviorV[s-1]); 

                        for (int colorT = FramesV[s-1]; colorT<FramesV[s]+1; colorT++){
                            ui->tEthogram->item(0,colorT)->setBackgroundColor(colorBehavior[behaviorVec[colorT]]);
                        }
                    }
                }

                fill(behaviorVec.begin() + FramesV.back(),behaviorVec.end(),BehaviorV.back()); 

                for (int colorT2 = FramesV.back(); colorT2<behaviorVec.size(); colorT2++){
                    ui->tEthogram->item(0,colorT2)->setBackgroundColor(colorBehavior[behaviorVec[colorT2]]);
                }
            }
        }

        else
        {
            QLineEdit *editTextPrevious = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum-1,0));
            QString valueEditPrevious = editTextPrevious->text();

            QLineEdit *editTextCurrent = qobject_cast<QLineEdit*>(tablewidget->cellWidget(RowNum,0));
            QString valueEditCurrent = editTextCurrent->text();

            if ((valueEditPrevious.toInt()) >= (valueEditCurrent.toInt()))
            {
                tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("Insert a bigger frame number! Unsorted values are not valid"));
                tablewidget->item(tablewidget->rowCount()-1, 0)->setForeground(Qt::red);
                tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                btnTableDock->setEnabled(false);
                correctItemBool = false;
            }

            else {

                tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem("")); 
                correctItemBool = true;
                tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                fill(behaviorVec.begin(),behaviorVec.end(),0);

                for (int colorEtho = 0; colorEtho<(ui->tEthogram->columnCount()); colorEtho++){
                    ui->tEthogram->item(0,colorEtho)->setBackgroundColor(colorBehavior[0]);
                }

                btnTableDock->setEnabled(true);

                for (int rT = 0; rT<tablewidget->rowCount()-2; rT++)
                {
                    QLineEdit *editText = qobject_cast<QLineEdit*>(tablewidget->cellWidget(rT,0));
                    QString valueEdit = editText->text();

                    QComboBox *myCB2 = qobject_cast<QComboBox*>(tablewidget->cellWidget(rT,1));
                    int valueCB2 = myCB2->currentIndex();

                    FramesV.push_back(valueEdit.toInt());
                    BehaviorV.push_back(valueCB2);
                }

                for (int s = 0; s<FramesV.size(); s++)
                {

                    if (s==0){ 
                        behaviorVec[FramesV[s]] = BehaviorV[s];

                        ui->tEthogram->item(0,FramesV[s])->setBackgroundColor(colorBehavior[BehaviorV[s]]);
                    }

                    else
                    {
                        fill(behaviorVec.begin()+FramesV[s-1],behaviorVec.begin()+FramesV[s],BehaviorV[s-1]); 

                        for (int colorT = FramesV[s-1]; colorT<FramesV[s]+1; colorT++){
                            ui->tEthogram->item(0,colorT)->setBackgroundColor(colorBehavior[behaviorVec[colorT]]);
                        }
                    }
                }

                fill(behaviorVec.begin() + FramesV.back(),behaviorVec.end(),BehaviorV.back()); 

                for (int colorT2 = FramesV.back(); colorT2<behaviorVec.size(); colorT2++){
                    ui->tEthogram->item(0,colorT2)->setBackgroundColor(colorBehavior[behaviorVec[colorT2]]);
                }

            }
        }
    }
}

void MainWindow::on_btnSave_clicked() 
{
    if (correctItemBool == false)
    {
        ui->edtSaveFile->setStyleSheet("QLineEdit {color: red;}");
        ui->edtSaveFile->setText("Correct items in the docktable and save!");
    }
    else
    {
        ui->edtSaveFile->setText(" ");

        QString dirSave = QFileDialog::getExistingDirectory(this, tr("Select directory:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (dirSave.isEmpty()){
            ui->edtSaveFile->setStyleSheet("QLineEdit {color: red;}");
            ui->edtSaveFile->setText("Select directory!");
        }

        else
        {
            ui->edtSaveFile->setStyleSheet("QLineEdit {color: green;}");
            ui->edtSaveFile->setText(dirSave);

            string nameDirFiletoSave = "BehavioralData_" + nameFolderSave;

             QString folderCreateCommand = QString::fromStdString(dirSave.toStdString() + "/" + nameDirFiletoSave);

            QDir().mkdir(folderCreateCommand);

            ofstream fileBehaviorSaveFinal, fileBehaviorSaveNotFinishedJob;

            string filenameFinalClassification = dirSave.toStdString() + "/" + nameDirFiletoSave + "/dataBehav_" + nameFolderSave + ".csv";

            string filenameNotFinishedClassification = dirSave.toStdString() + "/" + nameDirFiletoSave + "/behavDataInProgress_" + nameFolderSave + ".csv";

            fileBehaviorSaveFinal.open (filenameFinalClassification);

            if (fileBehaviorSaveFinal.is_open())
            {
                for (int b = 0; b<listNamesBehav.size(); b++){

                    fileBehaviorSaveFinal << b;
                    fileBehaviorSaveFinal << ";";

                    QString nameBehav = listNamesBehav[b];
                    fileBehaviorSaveFinal << nameBehav.toStdString();

                    fileBehaviorSaveFinal << ";";
                }

                fileBehaviorSaveFinal << "\n";

                fileBehaviorSaveFinal << "Frames; Behavior \n";

                for (int b = 0; b<behaviorVec.size(); b++){

                    fileBehaviorSaveFinal << b;
                    fileBehaviorSaveFinal << ";";
                    fileBehaviorSaveFinal << behaviorVec[b];
                    fileBehaviorSaveFinal << "\n";
                }

                fileBehaviorSaveFinal.close();

                fileBehaviorSaveNotFinishedJob.open (filenameNotFinishedClassification);

                if (fileBehaviorSaveNotFinishedJob.is_open())
                {
                    fileBehaviorSaveNotFinishedJob << dirFileString;
                    fileBehaviorSaveNotFinishedJob << "\n";

                    for (int b = 0; b<listNamesBehav.size(); b++){

                        fileBehaviorSaveNotFinishedJob << b;
                        fileBehaviorSaveNotFinishedJob << ";";

                        QString nameBehav = listNamesBehav[b];
                        fileBehaviorSaveNotFinishedJob << nameBehav.toStdString();

                        fileBehaviorSaveNotFinishedJob << ";";
                    }

                    fileBehaviorSaveNotFinishedJob << "\n";

                    for (int rT = 0; rT<tablewidget->rowCount()-2; rT++)
                    {
                        QLineEdit *editText = qobject_cast<QLineEdit*>(tablewidget->cellWidget(rT,0));
                        QString valueEdit = editText->text();

                        QComboBox *myCB2 = qobject_cast<QComboBox*>(tablewidget->cellWidget(rT,1));
                        int valueCB2 = myCB2->currentIndex();

                        fileBehaviorSaveNotFinishedJob << valueEdit.toInt();
                        fileBehaviorSaveNotFinishedJob << ";";
                        fileBehaviorSaveNotFinishedJob << valueCB2;

                        QLineEdit *editText2 = qobject_cast<QLineEdit*>(tablewidget->cellWidget(rT,2));
                        QString valueEdit2 = editText2->text();

                        if (valueEdit2.isEmpty())
                        {}
                        else {
                            fileBehaviorSaveNotFinishedJob << ";";
                            fileBehaviorSaveNotFinishedJob << valueEdit2.toStdString();
                        }


                        fileBehaviorSaveNotFinishedJob << "\n";
                    }

                    fileBehaviorSaveNotFinishedJob.close();                   

                    fileNotSaved = false;
                }

                else
                {
                    ui->edtSaveFile->setStyleSheet("QLineEdit {color: red;}");
                    ui->edtSaveFile->setText("ERROR! TRY AGAIN!");

                    fileNotSaved = true;
                }


            }

            else
            {
                ui->edtSaveFile->setStyleSheet("QLineEdit {color: red;}");
                ui->edtSaveFile->setText("ERROR! TRY AGAIN!");

                fileNotSaved = true;

            }
        }
    }
}

void MainWindow::on_cbVisualization_currentIndexChanged(int index) 
{

    QString sValue = ui->edtFrameNumber->text();

    if (sValue.isEmpty())
    {}
    else
    {
        int value = sValue.toInt();

        string strNameF1 = listFileNames[value*2];
        string strNameF2 = listFileNames[value*2 +1];

        vector<string> vF1 = splitString(strNameF1, '_');

        Mat imageDepthF1;

        if (vF1[2]=="c.png"){

            string nameFileDF2 = dirFileString + "/" + strNameF2;
            imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);
        }
        else {

            string nameFileDF2 = dirFileString + "/" + strNameF1;
            imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);

        }

        Mat viz2 = DepthToBeVisualized (imageDepthF1);

        QImage imageD(viz2.data, viz2.cols, viz2.rows, viz2.step, QImage::Format_Indexed8);
        ui->lblDepth->setPixmap(QPixmap::fromImage(imageD));
        ui->lblDepth->repaint();
    }



}

void MainWindow::on_btnPlay_clicked()
{
    if (countClick == false)
    {
        QPixmap pixmap("pauseBTN.png");
        QIcon ButtonIcon(pixmap);
        ui->btnPlay->setIcon(ButtonIcon);
        ui->btnPlay->setIconSize(pixmap.rect().size());

        countClick = true;

        ui->btnStop->setEnabled(true);
        ui->btnFastF->setEnabled(true);

        ui->btnOpenDir->setEnabled(false);
        ui->btnOpenFileResume->setEnabled(false);
        ui->edtFrameNumber->setEnabled(false);
        ui->sbFrames->setEnabled(false);
        ui->tEthogram->setEnabled(false);
        ui->btnPlay->setToolTip("Pause");

        QCoreApplication::processEvents();
        QCoreApplication::flush();

        timerPlayVideo->start(30);
        activePlayVideo = 1;

    }
    else
    {
        timerPlayVideo->stop(); activePlayVideo = 0;

        QPixmap pixmap("playBTN.png");
        QIcon ButtonIcon(pixmap);
        ui->btnPlay->setIcon(ButtonIcon);
        ui->btnPlay->setIconSize(pixmap.rect().size());

        countClick = false;

        ui->btnFastF->setEnabled(false);
        ui->btnOpenDir->setEnabled(true);
        ui->btnOpenFileResume->setEnabled(true);
        ui->edtFrameNumber->setEnabled(true);
        ui->sbFrames->setEnabled(true);
        ui->tEthogram->setEnabled(true);
        ui->btnPlay->setToolTip("Play");

        QCoreApplication::processEvents();
        QCoreApplication::flush();
    }

}

void MainWindow::setTimerSingleFrame()
{

    if (frameNumberPlay>=(listFileSize/2)) 
    {
        on_btnStop_clicked();
    }

    else
    {
        string strNameF1 = listFileNames[frameNumberPlay*2];
        string strNameF2 = listFileNames[frameNumberPlay*2 +1];

        vector<string> vF1 = splitString(strNameF1, '_');
        vector<string> vF2 = splitString(strNameF2, '_');

        Mat imageRGBF1; Mat imageDepthF1; float timeStampC; float timeStampD;

        if (vF1[2]=="c.png"){

            timeStampC = stof(vF1[1]);
            timeStampD = stof(vF2[1]);

            string nameFileCF1 = dirFileString + "/" + strNameF1;
            imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

            string nameFileDF2 = dirFileString + "/" + strNameF2;
            imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);
        }
        else {

            timeStampC = stof(vF2[1]);
            timeStampD = stof(vF1[1]);

            string nameFileCF1 = dirFileString + "/" + strNameF2;
            imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

            string nameFileDF2 = dirFileString + "/" + strNameF1;
            imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);

        }

        Mat clone2 = imageRGBF1.clone();
        cvtColor(clone2, clone2, CV_BGR2RGB);
        QImage imageRGB (clone2.data, clone2.cols, clone2.rows, clone2.step, QImage::Format_RGB888);
        ui->lblRGB->setPixmap(QPixmap::fromImage(imageRGB));
        ui->lblRGB->repaint();

        Mat viz2 = DepthToBeVisualized (imageDepthF1);

        QImage imageD(viz2.data, viz2.cols, viz2.rows, viz2.step, QImage::Format_Indexed8);
        ui->lblDepth->setPixmap(QPixmap::fromImage(imageD));
        ui->lblDepth->repaint();

        ui->edtFrameNumber->setText( QString::number(frameNumberPlay));

        QCoreApplication::processEvents();
        QCoreApplication::flush();

        frameNumberPlay++; }
}

void MainWindow::on_btnStop_clicked()
{

    if (activePlayVideo==1)
    {
        timerPlayVideo->stop(); activePlayVideo = 0;
        frameNumberPlay = (ui->edtFrameNumber->text()).toInt();

        QPixmap pixmap("playBTN.png");
        QIcon ButtonIcon(pixmap);
        ui->btnPlay->setIcon(ButtonIcon);
        ui->btnPlay->setIconSize(pixmap.rect().size());

        countClick = false;

        ui->btnFastF->setEnabled(false);
        ui->btnOpenDir->setEnabled(true);
        ui->btnOpenFileResume->setEnabled(true);
        ui->edtFrameNumber->setEnabled(true);
        ui->sbFrames->setEnabled(true);
        ui->tEthogram->setEnabled(true);

        QCoreApplication::processEvents();
        QCoreApplication::flush();
    }

    else
    {
        frameNumberPlay = (ui->edtFrameNumber->text()).toInt();
    }


}

void MainWindow::on_btnFastF_clicked()
{

    if (activePlayVideo==1)
    {
        timerPlayVideo->stop();

        if (velocityVideo == 0)
        {
            velocityVideo = 1;
            QToolTip::showText(ui->btnFastF->mapToGlobal(QPoint()), "Video Velocity 2x");
            QCoreApplication::processEvents();
            QCoreApplication::flush();

            timerPlayVideo->start(15);
            activePlayVideo = 1;


        }

        else if (velocityVideo == 1)
        {
            velocityVideo = 2;
            QToolTip::showText(ui->btnFastF->mapToGlobal(QPoint()), "Video Velocity 4x");
            QCoreApplication::processEvents();
            QCoreApplication::flush();

            timerPlayVideo->start(8);
            activePlayVideo = 1;

        }

        else
        {
            velocityVideo = 0;
            QToolTip::showText(ui->btnFastF->mapToGlobal(QPoint()), "Original Video Velocity");
            QCoreApplication::processEvents();
            QCoreApplication::flush();


            timerPlayVideo->start(30);
            activePlayVideo = 1;

        }
    }

}

void MainWindow::on_btnOpenAnnotationTool_clicked()
{
    if (dock->isHidden())
    {
        dock->show();
    }

}

void MainWindow::on_btnOpenFileResume_clicked()
{
    ui->edtFileVideo->setStyleSheet("QLineEdit {color: black;}");
    ui->edtFileVideo->setText("");

    if (YesItOpennedDir)
    {
        dock->close();
    }

    string str2 ("behavDataInProgress");

    QString fileNameResumeFile = QFileDialog::getOpenFileName(this,
        tr("Select annotation file with following name : '_behavDataInProgress_' "), " ", tr("CSV files (*.csv)"));

    string fileNameResumeFileString = fileNameResumeFile.toStdString();

    if (fileNameResumeFile.isEmpty()){
        ui->edtFileResume->setEnabled(true);

        ui->edtFileResume->setStyleSheet("QLineEdit {color: red;}");
        ui->edtFileResume->setText("Select a file!");
    }

    else if (fileNameResumeFileString.find(str2) == string::npos) 
    {
        ui->edtFileResume->setEnabled(true);

        ui->edtFileResume->setStyleSheet("QLineEdit {color: red;}");
        ui->edtFileResume->setText("Select a correct file!");

    }
    else
    {

        contentsFileResume.clear();

        ui->edtFileResume->setStyleSheet("QLineEdit {color: black;}");
        ui->edtFileResume->setText(fileNameResumeFile);

        YesItOpennedDir = true;

        ifstream fileResume ( fileNameResumeFileString );

        string value;
        while ( fileResume.good() )
        {
            getline ( fileResume, value, '\n' );
            if (value.empty()){}
            else{
                contentsFileResume.push_back(value);}
        }

        vector<string> nameDirectory = splitString(contentsFileResume[0], ';');
        dirFileResumeString = nameDirectory[0];
        dirFileString = dirFileResumeString;

        vector<string> namesBehaviors = splitString(contentsFileResume[1], ';');
        listNamesBehav.clear();

        for (int names = 1; names <namesBehaviors.size(); names+=2)
        {
            listNamesBehav << QString::fromStdString(namesBehaviors[names]);
        }

        vector<string> v1Folder = splitString(dirFileResumeString, '/');
        nameFolderSave = v1Folder.back(); 

        QByteArray inBytes = QString::fromStdString(dirFileResumeString).toUtf8();
        const char *pathConst = inBytes.constData();

        listFileNames = listFiles(pathConst);
        listFileSize = listFileNames.size();

        if (listFileSize == 0)
        {
            ui->edtFileResume->setEnabled(true);

            ui->edtFileResume->setStyleSheet("QLineEdit {color: red;}");
            ui->edtFileResume->setText("Select a correct file!");
        }
        else
        {
            ui->edtFileResume->setEnabled(false);
            ui->edtFileResume->setStyleSheet("QLineEdit {color: black;}");
            ui->edtFileResume->setText(fileNameResumeFile);

            timeStampsRGBVec.resize(listFileSize/2); int countTime = 0;

            for (int i = 0; i<(listFileSize); i+=2){

                if (i>=listFileSize-1)
                {
                    listFileNames.erase(listFileNames.end() - 1);
                    listFileSize = listFileNames.size();
                    break;
                }

                else {
                    string strName1 = listFileNames[i];
                    string strName2 = listFileNames[i+1];

                    vector<string> v1 = splitString(strName1, '_');
                    vector<string> v2 = splitString(strName2, '_');

                    if (v1[2]=="c.png"){
                        float timeStampC = stof(v1[1]);
                        timeStampsRGBVec[countTime] = timeStampC;
                        countTime++;
                    }

                    else {
                        float timeStampC = stof(v2[1]);

                        timeStampsRGBVec[countTime] = timeStampC;
                        countTime++;
                    }
                }
            }


            if (timeStampsRGBVec[0]>1000)
            {
                adjacent_difference (timeStampsRGBVec.begin(), timeStampsRGBVec.end(), timeStampsRGBVec.begin());
                timeStampsRGBVec[0] = 0;
                partial_sum (timeStampsRGBVec.begin(), timeStampsRGBVec.end(), timeStampsRGBVec.begin());

            }


            ui->cbVisualization->setEnabled(true);

            ui->sbFrames->setRange(0, (listFileSize/2)-1);
            ui->sbFrames->setEnabled(true);
            ui->sbFrames->setValue(0);
            ui->edtFrameNumber->setEnabled(true);

            QValidator *validator = new QIntValidator(0, (listFileSize/2)-1, this);
            ui->edtFrameNumber->setValidator(validator);

            QString stringF = "MAX = " + QString::number((listFileSize/2)-1);
            ui->lblMaxFrames->setText(stringF);

            behaviorVec.resize((listFileSize/2));
            fill(behaviorVec.begin(),behaviorVec.end(),0); 

            vector<string> lastVectString = splitString(contentsFileResume.back(), ';');

            if (stoi(lastVectString[0]) == 0)
            {
               lastVectString = splitString(contentsFileResume[contentsFileResume.size()-2], ';');
            }
            string lastframeRead = lastVectString[0];

            string strNameF1 = listFileNames[stoi(lastframeRead)*2];
            string strNameF2 = listFileNames[stoi(lastframeRead)*2 +1];

            vector<string> vF1 = splitString(strNameF1, '_');
            vector<string> vF2 = splitString(strNameF2, '_');

            Mat imageRGBF1; Mat imageDepthF1; float timeStampC; float timeStampD;
            if (vF1[2]=="c.png"){

                timeStampC = stof(vF1[1]);
                timeStampD = stof(vF2[1]);

                string nameFileCF1 = dirFileResumeString + "/" + strNameF1;
                imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

                string nameFileDF2 = dirFileResumeString + "/" + strNameF2;
                imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);
            }
            else {

                timeStampC = stof(vF2[1]);
                timeStampD = stof(vF1[1]);

                string nameFileCF1 = dirFileResumeString + "/" + strNameF2;
                imageRGBF1 = imread(nameFileCF1, IMREAD_UNCHANGED);

                string nameFileDF2 = dirFileResumeString + "/" + strNameF1;
                imageDepthF1 = imread(nameFileDF2, IMREAD_UNCHANGED);

            }

            Mat clone2 = imageRGBF1.clone();
            cvtColor(clone2, clone2, CV_BGR2RGB);
            QImage imageRGB (clone2.data, clone2.cols, clone2.rows, QImage::Format_RGB888);
            ui->lblRGB->setPixmap(QPixmap::fromImage(imageRGB));
            ui->lblRGB->repaint();

            Mat viz2 = DepthToBeVisualized (imageDepthF1);

            QImage imageD(viz2.data, viz2.cols, viz2.rows, viz2.step, QImage::Format_Indexed8);
            ui->lblDepth->setPixmap(QPixmap::fromImage(imageD));
            ui->lblDepth->repaint();

            ui->edtFrameNumber->setText(QString::fromStdString(lastframeRead));
            ui->edtTimeStampFrame->setText(QString::number(timeStampC));
            ui->edtTimeStampFrame_2->setText(QString::number(timeStampD));
            ui->edtBehaviorEvent->setText(listNamesBehav[stoi(lastVectString[1])]);
            ui->edtBehaviorEvent->setStyleSheet("color:" + colorStringBehavior[stoi(lastVectString[1])]);

            ui->tEthogram->setVisible(false);
            ui->tEthogram->setRowCount(1);
            ui->tEthogram->setColumnCount((listFileSize/2));

            for (int j = 0; j<(listFileSize/2); j++){
                ui->tEthogram->setItem(0,j,new QTableWidgetItem(""));
                float a = (ui->tEthogram->width());
                float b = ((listFileSize/2));
                int c = ceil(a/b);
                ui->tEthogram->setColumnWidth(j, c);

            }

            ui->tEthogram->setVisible(true);
            ui->btnSave->setEnabled(true);

            ui->lblBehavioralEthogram->setVisible(true);

            ui->btnStop->setEnabled(false);
            ui->btnFastF->setEnabled(false);
            ui->btnPlay->setEnabled(true);

            fileNotSaved = true;

            ui->btnOpenAnnotationTool->setEnabled(true);

            dock = new QDockWidget(tr("Behaviors' Annotation"), this);

            dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
            tablewidget = new QTableWidget(dock);

            tablewidget->insertColumn(0);
            tablewidget->insertColumn(1);
            tablewidget->insertColumn(2);

            for (int cont=1; cont<contentsFileResume.size()-1; cont++)
            {
                vector<string> vF1 = splitString(contentsFileResume[cont+1], ';');

                tablewidget->insertRow( cont-1 );

                QLineEdit *edit = new QLineEdit(tablewidget);
                edit->setValidator(new QIntValidator(0, (listFileNames.size()/2)-1, this));
                tablewidget->setCellWidget(tablewidget->rowCount()-1, 0, edit);

                edit->setText(QString::fromStdString(vF1[0]));

                connect(edit, SIGNAL(editingFinished()), &EditTextSignalMapper, SLOT(map()));
                EditTextSignalMapper.setMapping(edit, tablewidget->rowCount()-1);

                QComboBox* combo = new QComboBox();
                combo->addItems(listNamesBehav);
                for (int i = 0; i<listNamesBehav.size(); i++){
                    combo->setItemData( i, QColor( colorBehavior[i] ), Qt::ForegroundRole );
                }

                tablewidget->setCellWidget(tablewidget->rowCount()-1,1,combo);
                combo->setCurrentIndex(atoi( vF1[1].c_str() ));

                connect(combo, SIGNAL(currentIndexChanged(int)), &ComboBoxSignalMapper, SLOT(map()));
                ComboBoxSignalMapper.setMapping(combo, tablewidget->rowCount()-1);

                if (vF1.size()<3)
                {
                    QLineEdit *edit2 = new QLineEdit(tablewidget);
                    edit2->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_? .-]*"), this ));
                    tablewidget->setCellWidget(tablewidget->rowCount()-1, 2, edit2);
                }

                else{
                    QLineEdit *edit2 = new QLineEdit(tablewidget);
                    edit2->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_? .-]*"), this ));
                    tablewidget->setCellWidget(tablewidget->rowCount()-1, 2, edit2);

                    edit2->setText(QString::fromStdString(vF1[2]));
                }

            }

            tablewidget->setHorizontalHeaderLabels(QStringList() << "Frame" << "Behavior" << "Comments");
            tablewidget->verticalHeader()->setVisible(false);

            tablewidget->insertRow(tablewidget->rowCount());
            tablewidget->insertRow(tablewidget->rowCount());

            tablewidget->setCellWidget(tablewidget->rowCount()-2,0,btnTableDock);
            tablewidget->setCellWidget(tablewidget->rowCount()-2,1, btnTableDockDelete);
            btnTableDockDelete->setEnabled(true);
            tablewidget->setItem(0, 2, new QTableWidgetItem(""));
            tablewidget->item(0, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            tablewidget->setItem(tablewidget->rowCount()-1, 0, new QTableWidgetItem(""));
            tablewidget->item(tablewidget->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            tablewidget->horizontalHeader()->setStretchLastSection(true);
            tablewidget->resizeColumnsToContents();

            tablewidget->setSpan(tablewidget->rowCount()-1,0,1,3);
            tablewidget->setShowGrid(false);

            dock->setWidget(tablewidget);
            addDockWidget(Qt::RightDockWidgetArea, dock);
            dock->show();

            CellComboBoxClicked (0);
            CellComboBoxClicked (tablewidget->rowCount()-3);

        }
    }
}


