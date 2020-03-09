
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

#ifndef P_H
#define P_H

#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QLabel>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "r.h"
#include "kinectmodule.h"

using namespace cv;
using namespace std;

class P : public QThread
{
    Q_OBJECT
public:
    explicit P(R *r, kinectModule *kinectSensor,QObject *parent = 0) :
        QThread(parent),
        abort(false)
    {
        this->r = r;

        kinectSensor_ = kinectSensor;

    }

    ~P() {
        stopProduce();
        wait();
    }

    void stopProduce() {

        QMutexLocker locker2(&abortMutex);
        abort = true;

    }

protected:
    R *r;

    void run()
    {
        i=0;

        while(true){

            r->freeBytes1->acquire();
            r->freeBytes2->acquire();

            //
            r->freeBytes3->acquire();
            r->freeBytes4->acquire();

            vector<Mat> frameRGBD = kinectSensor_->acquireOneFrame();

            r->buffer1[i % BufferSize] = frameRGBD[0];
            r->buffer2[i % BufferSize] = frameRGBD[1];

            //
            r->buffer3[i % BufferSize] = frameRGBD[2];
            r->buffer4[i % BufferSize] = frameRGBD[3];

            i++;

            r->usedBytes1->release();
            r->usedBytes2->release();

            //
            r->usedBytes3->release();
            r->usedBytes4->release();

            QMutexLocker locker2(&abortMutex);
            if(abort) {
                break;
            }
        }
    }

private:
    QMutex abortMutex;
    bool abort;

    kinectModule *kinectSensor_;
    clock_t begin2, end2;
    
    int i;

};


#endif // P_H
