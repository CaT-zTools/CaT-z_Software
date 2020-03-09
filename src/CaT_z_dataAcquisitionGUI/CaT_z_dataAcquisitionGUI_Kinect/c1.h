
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

#ifndef C1_H
#define C1_H

#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QReadWriteLock>
#include <QString>
#include <QLineEdit>
#include <iostream>

#include <sstream>
#include <fstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "kinectmodule.h"
#include "r.h"

using namespace std;
using namespace cv;

class C1 : public QThread
{
    Q_OBJECT

public:
    C1(int id, R *r, QObject *parent=0):
        QThread(parent),
        abort(false)
    {
        this->r = r;
        this->id = id;

    }
    ~C1() {

        stopConsume();
        wait();
    }

    void stopConsume() {
        QMutexLocker locker2(&abortMutex);
        abort = true;
    }

    void setC1(string nameW, vector<int> vecParam, string folderName){

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
            r->usedBytes1->acquire();
            Mat depth_f = r->buffer1[i % BufferSize];

            //
            r->usedBytes3->acquire();
            Mat timeStampDepth = r->buffer3[i % BufferSize];

            if (depth_f.cols == 0){
            }

            else {

                double timeDValue = timeStampDepth.at<double>(0,0);

                if (i == 0) {
                    r->timeDepthRef = timeDValue;
                }

                char result[100];
                unsigned long int fnum = (timeDValue - (r->timeDepthRef))* (pow(10.0, -4.0));

                sprintf( result, "%09u_%09u_d.png", i,  fnum);

                string name = folderName_ + "\\" +string(result);
                imwrite(name, depth_f, vecParam_); 
                i++;


            }
            r->freeBytes1->release();
            r->freeBytes3->release();

            QMutexLocker locker2(&abortMutex);
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
};

#endif // C1_H
