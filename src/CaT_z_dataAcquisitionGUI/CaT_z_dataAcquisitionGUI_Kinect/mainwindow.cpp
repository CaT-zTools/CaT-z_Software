
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

    counterCamera = 0;
    timerCamera = new QTimer(this);

    ui->btnSaveReal->setEnabled(false);
    ui->btnOpenDir->setEnabled(false);

    QIcon icon("RatLogo.ico");
    setWindowIcon(icon);

}

MainWindow::~MainWindow()
{
    if (check1){
        kinectSensorOBJ.cameraRelease();
    }

    if (activeTimer == 1)
    {
        timerCamera->stop();
        delete timerCamera;
        destroyAllWindows();
    }

    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    counterCamera++;

    if (ui->btnConnect->text() == "ON" && counterCamera==1){

        check1 = kinectSensorOBJ.cameraInit();

        if (check1)
        {
            ui->btnConnect->setText("OFF");

            namedWindow("Color Map", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);
            namedWindow("Depth Map", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);

            connect(timerCamera, SIGNAL(timeout()), this, SLOT(setTimerSingleFrame()));
            timerCamera->start(15);
            activeTimer = 1;
        }

        else
        {}
    }

    else if (ui->btnConnect->text() == "ON"){

        ui->btnConnect->setText("OFF");

        namedWindow("Color Map", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);
        namedWindow("Depth Map", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO);

        connect(timerCamera, SIGNAL(timeout()), this, SLOT(setTimerSingleFrame()));
        timerCamera->start(15);
        activeTimer = 1;
    }

    else
    {
        ui->btnConnect->setText("ON");

        timerCamera->stop(); activeTimer = 0;
        destroyAllWindows();

        ui->btnOpenDir->setEnabled(true);
    }
}

void MainWindow::setTimerSingleFrame()
{
    kinectSensorOBJ.acquireSingleFrame();
}

void MainWindow::on_btnSaveReal_clicked()
{
    if (ui->btnSaveReal->text() == "START"){

        ui->btnSaveReal->setText("STOP");
        QCoreApplication::processEvents();
        QCoreApplication::flush();

        string nameWindowD = "Real Time";

        time_t now;
        char the_date[80]; the_date[0] = '\0'; now = time(NULL);

        if (now != -1)
        {
            strftime(the_date, 80, "%Y_%m_%d_%H_%M_%S", gmtime(&now));
        }

        folderName = "Frames_" + string(the_date);
        QString folderCreateCommand = QString::fromStdString(dirFileString + "/" + folderName);

        QDir().mkdir(folderCreateCommand);
        
		vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);


        initProducer();
        initConsumer(nameWindowD, compression_params, folderCreateCommand.toStdString());
        begin2 = clock();
        startAll();
    }

    else { 

        end2 = clock();
        double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;

        stopProducer();
        stopConsumers();

        ui->btnSaveReal->setText("START");
        destroyAllWindows();
        QCoreApplication::processEvents();
        QCoreApplication::flush();

        kinectSensorOBJ.cleanVectors();

        cout << elapsed_secs2 << endl;

    }

}

void MainWindow::initProducer(){

    p = new P(&r1, &kinectSensorOBJ);

}

void MainWindow::initConsumer(string nameW, vector<int> vecParam, string folderName){


    c1 = new C1(1, &r1);
    c1->setC1(nameW, vecParam, folderName);

    c2 = new C2(2, &r1, &kinectSensorOBJ);
    c2->setC2(nameW, vecParam, folderName);

}

void MainWindow::startAll(){

    p->start();

    c1->start();
    c2->start();
}

void MainWindow::stopProducer() {
    p->stopProduce();
}

void MainWindow::stopConsumers() {
    c1->stopConsume();
    c2->stopConsume();
}

void MainWindow::on_btnOpenDir_clicked()
{
    dirFile = QFileDialog::getExistingDirectory(this, tr("Select directory:"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dirFile.isEmpty()){
        ui->edtFileVideo->setEnabled(true);

        QPalette palet;
        palet.setColor( ui->edtFileVideo->foregroundRole(), Qt::red );
        ui->edtFileVideo->setPalette(palet);
        ui->edtFileVideo->setText("Select directory!");
    }

    else
    {
        QPalette palet1;
        palet1.setColor( ui->edtFileVideo->foregroundRole(), Qt::black );
        ui->edtFileVideo->setPalette(palet1);
        ui->edtFileVideo->setText(dirFile);

        dirFileString = dirFile.toStdString();

        ui->btnSaveReal->setEnabled(true);

    }
}
