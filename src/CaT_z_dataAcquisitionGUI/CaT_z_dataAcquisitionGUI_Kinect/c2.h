
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


#ifndef C2_H
#define C2_H

#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QReadWriteLock>
#include <QString>
#include <QLineEdit>
#include <iostream>
#include "r.h"
#include "kinectmodule.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class C2 : public QThread
{
    Q_OBJECT

public:
    C2(int id, R *r, kinectModule *kinectSensor, QObject *parent=0):
        QThread(parent),
        abort(false)
    {
        this->r = r;
        this->id = id;

        kinectSensor2_ = kinectSensor;
    }
    ~C2() {

        stopConsume();
        wait();
    }

    void stopConsume() {
        QMutexLocker locker2(&abortMutex);
        abort = true;
    }

    void setC2(string nameW, vector<int> vecParam, string folderName){
        counterframes = 0;
        nameW_ = nameW;
        vecParam_ = vecParam;
        folderName_ = folderName;
    }

protected:
    R *r;
    int id;

    void run()
    {
        i = 0;
        while(true) {
            r->usedBytes2->acquire();
            Mat RGB_f = r->buffer2[i % BufferSize];

            r->usedBytes4->acquire();
            Mat timeStampColor = r->buffer4[i % BufferSize];

            if (RGB_f.cols == 0){
                 }

            else {


                double timeCValue = timeStampColor.at<double>(0,0);

                if (i == 0) {
                    r->timeColorRef = timeCValue;
                }

                char result[100];
                unsigned long int fnum = (timeCValue - (r->timeColorRef))* (pow(10.0, -4.0));

                sprintf( result, "%09u_%09u_c.png", i,  fnum);

                string name = folderName_ + "\\" +string(result);

                imwrite(name, RGB_f, vecParam_);

                i++;

            }
            r->freeBytes2->release();
            r->freeBytes4->release();

            QMutexLocker locker(&abortMutex);
            if(abort) {
                break;
            }

        }
    }

private:
    QMutex abortMutex;
    bool abort;
    int i;

    int counterframes;
    string nameW_;
    vector<int> vecParam_;
    string folderName_;

    kinectModule *kinectSensor2_;

};

#endif // C2_H
