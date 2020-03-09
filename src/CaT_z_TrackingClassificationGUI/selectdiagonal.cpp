
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

#include "selectdiagonal.h"
#include "ui_selectdiagonal.h"
#include "imagepointsevent.h"

selectDiagonal::selectDiagonal(Mat frameD, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectDiagonal)
{
    ui->setupUi(this);

    QIcon icon("RatLogo.ico");
    setWindowIcon(icon);

    count = 0;

    connect(ui->imagePoints, SIGNAL (Mouse_Pos()), this, SLOT(Mouse_current_pos()));
    connect(ui->imagePoints, SIGNAL (Mouse_Pressed()), this, SLOT(Mouse_Pressed()));


    // Depth:
    // Visualization
    Mat viz2 = frameD.clone();
    for (int i = 0; i<viz2.rows; i++)
    {
        for (int j = 0; j<viz2.cols; j++)
        {
            unsigned short value1 = viz2.at<unsigned short>(i,j);
            viz2.at<unsigned short>(i, j) = (5*value1)%255;
        }
    }
    viz2.convertTo(viz2, CV_8U, 1, 0);

    cv::resize(viz2, viz2, Size(), 1.5, 1.5); 
    QImage imageD(viz2.data, viz2.cols, viz2.rows, viz2.step, QImage::Format_Indexed8);
    pix1 = QPixmap::fromImage(imageD);
    ui->imagePoints->setPixmap(pix1);
    ui->imagePoints->repaint();

    point1 = QPoint(); point2 = QPoint(); 
    ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");
    ui->edtPoint2->setText(" (" + QString::number(point2.x()) + "," + QString::number(point2.y()) + ") ");

    ui->edtPoint1->setEnabled(false); ui->edtPoint2->setEnabled(false);

    ui->lblErrorPoint->setVisible(false);

    imageWidth = 767; imageHeight = 635;

}

selectDiagonal::~selectDiagonal()
{
    QApplication::restoreOverrideCursor();
    QCoreApplication::processEvents();
    QCoreApplication::flush();

    count = 0;

    delete ui;
}

void selectDiagonal::Mouse_current_pos()
{

    QApplication::restoreOverrideCursor();
    QCoreApplication::processEvents();
    QCoreApplication::flush();

    ui->lblX->setText(QString ("%1").arg(ui->imagePoints->x));
    ui->lblY->setText(QString ("%1").arg(ui->imagePoints->y));

}

void selectDiagonal::Mouse_Pressed()
{
    if (count == 0)
    {
        point1 = QPoint(ui->imagePoints->x, ui->imagePoints->y);

        ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");

        count = 1;

        QPixmap pix3 = pix1;

        QPainter painter(&pix3);
        QPen pointpen(Qt::magenta);
        pointpen.setWidth(8);

        painter.setPen(pointpen);
        painter.drawPoint(point1);
        painter.end ();

        ui->imagePoints->setPixmap(pix3);
        ui->imagePoints->repaint();
    }

    else if (count == 1) 
    {


        if (((point1.y())<(imageHeight/2)) && ((point1.x())<(imageWidth/2))) 
        {
            if (((ui->imagePoints->x)<(imageHeight/2)) && ((ui->imagePoints->y)<(imageWidth/2))) 
            {
                 ui->lblErrorPoint->setVisible(true);
                 ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                 ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->x)>(imageHeight/2)) && ((ui->imagePoints->y)<(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->x)<(imageHeight/2)) && ((ui->imagePoints->y)>(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else {

                ui->lblErrorPoint->setVisible(false);

                point2 = QPoint(ui->imagePoints->x, ui->imagePoints->y);

                ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");
                ui->edtPoint2->setText(" (" + QString::number(point2.x()) + "," + QString::number(point2.y()) + ") ");

                count = 2;

                QPixmap pix3 = pix1;

                QPainter painter(&pix3);
                QPen pointpen(Qt::magenta);
                pointpen.setWidth(8);

                painter.setPen(pointpen);
                painter.drawPoint(point2);
                painter.drawPoint(point1);
                painter.end ();

                ui->imagePoints->setPixmap(pix3);
                ui->imagePoints->repaint();
            }
        }

        else if (((point1.y())>(imageHeight/2)) && ((point1.x())<(imageWidth/2))) 
        {
            if (((ui->imagePoints->y)<(imageHeight/2)) && ((ui->imagePoints->x)<(imageWidth/2))) 
            {
                 ui->lblErrorPoint->setVisible(true);
                 ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                 ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->y)>(imageHeight/2)) && ((ui->imagePoints->x)<(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->y)>(imageHeight/2)) && ((ui->imagePoints->x)>(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else {

                ui->lblErrorPoint->setVisible(false);

                point2 = QPoint(ui->imagePoints->x, ui->imagePoints->y);

                ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");
                ui->edtPoint2->setText(" (" + QString::number(point2.x()) + "," + QString::number(point2.y()) + ") ");

                count = 2;

                QPixmap pix3 = pix1;

                QPainter painter(&pix3);
                QPen pointpen(Qt::magenta);
                pointpen.setWidth(8);

                painter.setPen(pointpen);
                painter.drawPoint(point2);
                painter.drawPoint(point1);
                painter.end ();

                ui->imagePoints->setPixmap(pix3);
                ui->imagePoints->repaint();
            }
        }

        else if (((point1.y())<(imageHeight/2)) && ((point1.x())>(imageWidth/2))) 
        {
            if (((ui->imagePoints->y)<(imageHeight/2)) && ((ui->imagePoints->x)<(imageWidth/2))) 
            {
                 ui->lblErrorPoint->setVisible(true);
                 ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                 ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->y)<(imageHeight/2)) && ((ui->imagePoints->x)>(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->y)>(imageHeight/2)) && ((ui->imagePoints->x)>(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else {

                ui->lblErrorPoint->setVisible(false);

                point2 = QPoint(ui->imagePoints->x, ui->imagePoints->y);

                ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");
                ui->edtPoint2->setText(" (" + QString::number(point2.x()) + "," + QString::number(point2.y()) + ") ");

                count = 2;

                QPixmap pix3 = pix1;

                QPainter painter(&pix3);
                QPen pointpen(Qt::magenta);
                pointpen.setWidth(8);

                painter.setPen(pointpen);
                painter.drawPoint(point2);
                painter.drawPoint(point1);
                painter.end ();

                ui->imagePoints->setPixmap(pix3);
                ui->imagePoints->repaint();
            }
        }

        else 
        {
            if (((ui->imagePoints->y)<(imageHeight/2)) && ((ui->imagePoints->x)>(imageWidth/2))) 
            {
                 ui->lblErrorPoint->setVisible(true);
                 ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                 ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->y)>(imageHeight/2)) && ((ui->imagePoints->x)<(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else if ( ((ui->imagePoints->y)>(imageHeight/2)) && ((ui->imagePoints->x)>(imageWidth/2)) ) 
            {
                ui->lblErrorPoint->setVisible(true);
                ui->lblErrorPoint->setStyleSheet("QLabel {color: red;}");
                ui->lblErrorPoint->setText("Select a valid diagonal point!");
            }

            else {

                ui->lblErrorPoint->setVisible(false);

                point2 = QPoint(ui->imagePoints->x, ui->imagePoints->y);

                ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");
                ui->edtPoint2->setText(" (" + QString::number(point2.x()) + "," + QString::number(point2.y()) + ") ");

                count = 2;

                QPixmap pix3 = pix1;

                QPainter painter(&pix3);
                QPen pointpen(Qt::magenta);
                pointpen.setWidth(8);

                painter.setPen(pointpen);
                painter.drawPoint(point2);
                painter.drawPoint(point1);
                painter.end ();

                ui->imagePoints->setPixmap(pix3);
                ui->imagePoints->repaint();
            }
        }

    }

    else if (count == 2) 
    {
        point1.setX(ui->imagePoints->x); point1.setY(ui->imagePoints->y);
        point2 = QPoint();

        ui->edtPoint1->setText(" (" + QString::number(point1.x()) + "," + QString::number(point1.y()) + ") ");
        ui->edtPoint2->setText(" (" + QString::number(point2.x()) + "," + QString::number(point2.y()) + ") ");

        QPixmap pix3 = pix1;

        QPainter painter(&pix3);
        QPen pointpen(Qt::magenta);
        pointpen.setWidth(8);

        painter.setPen(pointpen);
        painter.drawPoint(point1);
        painter.end ();

        ui->imagePoints->setPixmap(pix3);
        ui->imagePoints->repaint();

        count = 1;

    }

}

vector<int> selectDiagonal::getPoint1()
{
    vector <int> point1Vector;

    point1Vector.push_back(point1.x());  point1Vector.push_back(point1.y());

    return point1Vector;
}

vector<int> selectDiagonal::getPoint2()
{
    vector <int> point2Vector;

    point2Vector.push_back(point2.x());  point2Vector.push_back(point2.y());

    return point2Vector;

}
