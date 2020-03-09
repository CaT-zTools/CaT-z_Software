
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

#ifndef KINECTMODULE_H
#define KINECTMODULE_H

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Kinect.h>
#include <stdio.h>     
#include <time.h>       
#include <math.h>
#include <bitset>
#include <vector>
#include <cmath>
#include <limits>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class kinectModule
{
public:
    kinectModule();
    ~kinectModule();

    int NUMFRAMES;
    vector<Mat> depthVec;

    vector<Mat> frameDepth; vector<Mat> frameRGB; 

    vector<INT64>time_color; 
    vector<INT64>time_depth;

    Mat timeDepthStamps;
    Mat timeColorStamps;

    vector<Mat> recordFrame;

    int boolReader;

    int color_width, color_height, depth_width, depth_height;

    IKinectSensor* pSensor;
    IMultiSourceFrameReader* reader;   
    ICoordinateMapper* mapper;

    CameraSpacePoint *depth2xyz; 
    ColorSpacePoint *depth2rgb; 

    float* xyzarray;

    bool cameraInit();
    void cameraRelease();

    vector<Mat> acquireOneFrame();
    void acquireSingleFrame();
    void cleanVectors();

    vector<int> getSizeColor();
    vector<int> getSizeDepth();

    Mat registrationDRGB(Mat);

    int frameC;
};

#endif // KINECTMODULE_H
