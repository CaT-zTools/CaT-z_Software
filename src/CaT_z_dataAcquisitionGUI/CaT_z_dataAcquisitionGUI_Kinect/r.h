#ifndef R_H
#define R_H

#include <QDebug>
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QSemaphore>
#include <QQueue>

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

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


const int DataSize = 1000; 
const int BufferSize = 2; 

class R {

public:

    R() {

        
        freeBytes1 = new QSemaphore(BufferSize);
        freeBytes2 = new QSemaphore(BufferSize);

        //
        freeBytes3 = new QSemaphore(BufferSize);
        freeBytes4 = new QSemaphore(BufferSize);

        usedBytes1 = new QSemaphore(0);
        usedBytes2 = new QSemaphore(0);

        //
        usedBytes3 = new QSemaphore(0);
        usedBytes4 = new QSemaphore(0);

        buffer1.resize(BufferSize);
        buffer2.resize(BufferSize);

        buffer3.resize(BufferSize);
        buffer4.resize(BufferSize);

        timeDepthRef = 0;
        timeColorRef = 0;


    }

    QSemaphore *freeBytes1; 
    QSemaphore *usedBytes1;

    QSemaphore *freeBytes2;
    QSemaphore *usedBytes2;

    QSemaphore *freeBytes3;
    QSemaphore *usedBytes3;

    QSemaphore *freeBytes4;
    QSemaphore *usedBytes4;

    vector<Mat> buffer1; 
    vector<Mat> buffer2;

    vector<Mat> buffer3;
    vector<Mat> buffer4;

    double timeDepthRef;
    double timeColorRef;

};
#endif // R_H
