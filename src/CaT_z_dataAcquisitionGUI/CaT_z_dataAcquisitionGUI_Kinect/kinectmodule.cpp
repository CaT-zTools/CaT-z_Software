
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

#include "kinectmodule.h"

kinectModule::kinectModule()
{
    color_width = 1920; 
    color_height = 1080;

    depth_width = 512;
    depth_height = 424;

    NUMFRAMES = 0;

    boolReader = 0;

    depth2xyz = new CameraSpacePoint [512*424];
    depth2rgb = new ColorSpacePoint [512*424];


    //
    recordFrame.resize(4);

    timeDepthStamps = Mat::zeros(1, 1, CV_64F);
    timeColorStamps = Mat::zeros(1, 1, CV_64F);

    frameC = 0;

}

kinectModule::~kinectModule() {

    
}

bool kinectModule::cameraInit() {

    pSensor = NULL;
    reader = NULL;   
    mapper = NULL;

    bool check1 = false;

    HRESULT hR = S_OK;

    
    hR = GetDefaultKinectSensor(&pSensor);


    while (!pSensor || FAILED(hR)) {

        hR = GetDefaultKinectSensor(&pSensor);
    }

    if (pSensor) {

        hR = pSensor->Open();

        if (SUCCEEDED(hR))
        {
            pSensor->get_CoordinateMapper(&mapper);

            pSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color, &reader);
            boolReader = 1;

            check1 = true;
        }

        else {
            check1 = false;
        }
    }
    return check1;
}

void kinectModule::cameraRelease() {

    if(boolReader == 1){
        reader->Release();
        pSensor->Close();
        pSensor->Release();

        cout << frameC << endl;
    }
}

vector<Mat> kinectModule::acquireOneFrame() {

	bool TrueFrame = false;
    int color_width = 1920;
    int color_height = 1080;
    unsigned int color_bufferSize = color_width * color_height * 4 * sizeof(unsigned char);

    Mat color_bufferMat(color_height, color_width, CV_8UC4);

    unsigned int depth_bufferSize = depth_width * depth_height * sizeof(unsigned short);

    Mat depth_bufferMat(depth_height, depth_width, CV_16UC1);

    HRESULT hR3 = S_OK; 
    HRESULT hR2 = S_OK; 

    IMultiSourceFrame *frame = NULL;
    hR3 = reader->AcquireLatestFrame(&frame);

    if (SUCCEEDED(hR3)) {

        IDepthFrame* depthframe = NULL;
        IDepthFrameReference* frameref1 = NULL;
        frame->get_DepthFrameReference(&frameref1);
        hR2 = frameref1->AcquireFrame(&depthframe);
        if (frameref1) frameref1->Release();


        IColorFrame* colorframe;
        IColorFrameReference* frameref2 = NULL;
        frame->get_ColorFrameReference(&frameref2);
        hR3 = frameref2->AcquireFrame(&colorframe);
        if (frameref2) frameref2->Release();

        if (SUCCEEDED(hR2)) {

            INT64 nTimeDepth = 0;
            depthframe->get_RelativeTime(&nTimeDepth);

            hR2 = depthframe->AccessUnderlyingBuffer(&depth_bufferSize, reinterpret_cast<UINT16**>(&depth_bufferMat.data));

            if (SUCCEEDED(hR2)) {

                mapper->MapDepthFrameToColorSpace(depth_width*depth_height, reinterpret_cast<UINT16*>(depth_bufferMat.data),depth_width*depth_height, depth2rgb);

                recordFrame[0] = depth_bufferMat.clone();
                timeDepthStamps.at<double>(0,0) = nTimeDepth;
                recordFrame[2] = timeDepthStamps;

            }
            depthframe->Release();
        }

        if (SUCCEEDED(hR3)) {

            INT64 nTimeColor = 0;
            colorframe->get_RelativeTime(&nTimeColor);
            hR3 = colorframe->CopyConvertedFrameDataToArray(color_bufferSize, reinterpret_cast<BYTE*>(color_bufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);

            if (SUCCEEDED(hR3)) {

                Mat regCOLOR = registrationDRGB(color_bufferMat.clone());
                recordFrame[1] = regCOLOR;

                timeColorStamps.at<double>(0,0) = nTimeColor;
                recordFrame[3] = timeColorStamps;

             }

            colorframe->Release();
        }
    }


    return recordFrame;

}

void kinectModule::acquireSingleFrame() {

    int color_width = 1920;
    int color_height = 1080;
    unsigned int color_bufferSize = color_width * color_height * 4 * sizeof(unsigned char); 
    Mat color_bufferMat(color_height, color_width, CV_8UC4);
    Mat colorFrameREG = Mat(depth_height, depth_width, CV_8UC3);

    unsigned int depth_bufferSize = depth_width * depth_height * sizeof(unsigned short);

    Mat depth_bufferMat(depth_height, depth_width, CV_16UC1);
    Mat depthMat(depth_height, depth_width, CV_16UC1);


    HRESULT hR3 = S_OK; 
    HRESULT hR2 = S_OK; 

    IMultiSourceFrame *frame = NULL;
    hR3 = reader->AcquireLatestFrame(&frame);

    if (SUCCEEDED(hR3)) {

        IDepthFrame* depthframe = NULL;
        IDepthFrameReference* frameref1 = NULL;
        frame->get_DepthFrameReference(&frameref1);
        hR2 = frameref1->AcquireFrame(&depthframe);
        if (frameref1) frameref1->Release();


        IColorFrame* colorframe;
        IColorFrameReference* frameref2 = NULL;
        frame->get_ColorFrameReference(&frameref2);
        hR3 = frameref2->AcquireFrame(&colorframe);
        if (frameref2) frameref2->Release();

        if (SUCCEEDED(hR2)) {

            INT64 nTimeDepth = 0;
            depthframe->get_RelativeTime(&nTimeDepth);

            hR2 = depthframe->AccessUnderlyingBuffer(&depth_bufferSize, reinterpret_cast<UINT16**>(&depth_bufferMat.data));

            if (SUCCEEDED(hR2)) {

                mapper->MapDepthFrameToColorSpace(depth_width*depth_height, reinterpret_cast<UINT16*>(depth_bufferMat.data),depth_width*depth_height, depth2rgb);

                depthMat = depth_bufferMat.clone();
            }
            depthframe->Release();
        }

        if (SUCCEEDED(hR3)) {

            INT64 nTimeColor = 0;
            colorframe->get_RelativeTime(&nTimeColor);
            hR3 = colorframe->CopyConvertedFrameDataToArray(color_bufferSize, reinterpret_cast<BYTE*>(color_bufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);

            if (SUCCEEDED(hR3)) {

                colorFrameREG = registrationDRGB(color_bufferMat.clone());
            }

            colorframe->Release();
        }



        imshow("Color Map", colorFrameREG);

        Mat viz1 = depthMat.clone();
        for (int i = 0; i<depthMat.rows; i++)
        {
            for (int j = 0; j<depthMat.cols; j++)
            {
                unsigned short value1 = depthMat.at<unsigned short>(i,j);
                viz1.at<unsigned short>(i, j) = (unsigned short)((5*(unsigned short)value1)%255);
            }
        }
        viz1.convertTo(viz1, CV_8U, 1, 0);

        imshow("Depth Map", viz1);

    }
}


vector<int> kinectModule::getSizeColor() {

    vector<int> SizeColor;

    SizeColor.push_back(color_height);
    SizeColor.push_back(color_width);

    return SizeColor;
}

vector<int> kinectModule::getSizeDepth() {

    vector<int> SizeDepth;

    SizeDepth.push_back(depth_height);
    SizeDepth.push_back(depth_width);

    return SizeDepth;
}

Mat kinectModule::registrationDRGB(Mat rgbMat){ 

	Mat rgbarray = Mat(depth_height, depth_width, CV_8UC3);

    int i = 0; 
    for (int y1 = 0; y1< depth_height; y1++){
        for (int x1 = 0; x1< depth_width; x1++){
            ColorSpacePoint p = depth2rgb[i];

            
            if (p.X < 0 || p.Y < 0 || p.X > color_width || p.Y > color_height) {
                rgbarray.at<Vec3b>(y1, x1).val[0] = 0;
                rgbarray.at<Vec3b>(y1, x1).val[1]= 0;
                rgbarray.at<Vec3b>(y1, x1).val[2] = 0;
            }

            else {
                

                rgbarray.at<Vec3b>(y1, x1).val[0] = rgbMat.at<Vec4b>((int)p.Y, (int)p.X).val[0];
                rgbarray.at<Vec3b>(y1, x1).val[1] = rgbMat.at<Vec4b>((int)p.Y, (int)p.X).val[1];
                rgbarray.at<Vec3b>(y1, x1).val[2] = rgbMat.at<Vec4b>((int)p.Y, (int)p.X).val[2];
            }
            i++;
        }
    }
    return rgbarray;
}

void kinectModule::cleanVectors() {

    recordFrame.clear();
    recordFrame.resize(4);

    timeDepthStamps = Mat::zeros(1, 1, CV_64F);
    timeColorStamps = Mat::zeros(1, 1, CV_64F);

}
