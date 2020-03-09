
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


#ifndef SELECTDIAGONAL_H
#define SELECTDIAGONAL_H

#include <QDialog>
#include <mainwindow.h>
#include <QtCore>
#include <QPainter>
#include <limits>
#include <QThread>
#include <QString>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

namespace Ui {
class selectDiagonal;
}

class selectDiagonal : public QDialog
{
    Q_OBJECT

public:
    explicit selectDiagonal(Mat frameRGB, QWidget *parent = 0);
    ~selectDiagonal();

    vector<int> getPoint1();
    vector<int> getPoint2();


private:
    Ui::selectDiagonal *ui;

    
    vector<int> CoordVector;

    QPixmap pix1;
    Mat img;
    int count;

    QPoint point1, point2;


    int imageHeight, imageWidth;

private slots:
    void Mouse_current_pos();
    void Mouse_Pressed();


};

#endif // SELECTDIAGONAL_H
