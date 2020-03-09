
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

#include "BinFileFrameHandler.h"
#include <fstream>

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Kinect.h>

#include <stdio.h>      /* printf */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

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


using namespace facegram::cam;
using namespace cv;
using namespace std;

BinFileFrameHandler::BinFileFrameHandler(std::string filename, const unsigned int width, const unsigned int height, const unsigned int frames, std::vector<int> compression_params)
    : filename(filename), width(width), height(height), _compression_params(compression_params) 
{
	file.open(filename, std::ios::binary | std::ios::out);
	file.write((char*)&frames, sizeof(int));
	file.write((char*)&height, sizeof(int));
    file.write((char *)&width, sizeof(int));
}


BinFileFrameHandler::~BinFileFrameHandler()
{	
	file.close();
}

void BinFileFrameHandler::handleFrame(cv::Mat mat, int frameNumber) {
	
	// Crop image: 2/3 width
    //int centralPixel_width = round(mat.cols / 2);
    //int widthROI = round((float)(mat.cols / 2) );
    
	//int centralPixel_height = round(mat.rows / 2);
    //int heightROI = round((float)(mat.rows * 2) / 3);

	//Mat matCropped = mat(Range(0, mat.rows), Range(centralPixel - round((widthROI / 2)), centralPixel + round((widthROI / 2)))).clone();
    //Mat matCropped = mat(Range(centralPixel_height - round((heightROI / 2)), centralPixel_height + round((heightROI / 2))), Range(centralPixel_width - round((widthROI / 2)), centralPixel_width + round((widthROI / 2)))).clone();
	
	// PNG files
	//std::string finalName = filename + "_" + std::to_string(frameNumber) + ".png";

    // Save to .png file:
    //imwrite(finalName, matCropped, _compression_params);

	// BIN file
	//file.write(matCropped.ptr<char>(0), (matCropped.dataend - matCropped.datastart));
    file.write(mat.ptr<char>(0), (mat.dataend - mat.datastart));


}
