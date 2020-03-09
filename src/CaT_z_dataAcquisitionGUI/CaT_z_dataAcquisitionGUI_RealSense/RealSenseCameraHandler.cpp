
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

#include "RealSenseCameraHandler.h"
#include "FrameHandler.h"
#include <algorithm>
#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <string>

using namespace facegram::cam;

std::pair<std::vector<double>, std::vector<double>> facegram::cam::RealSenseCameraHandler::captureFrames(const unsigned int frames, FrameHandler *handlerColor, FrameHandler *handlerDepth, FrameHandler *handlerRGBNotReg) {
    std::vector<double> colorTimestamps(frames);
    std::vector<double> depthTimestamps(frames);

    rs2::align align(RS2_STREAM_DEPTH);

    if (frames == 1) { 

        for (unsigned int i = 0; i < frames; i++) {
            try {
                rs2::frameset framesCaptured = pipe.wait_for_frames();
                rs2::frameset alignedFrames = align.process(framesCaptured); 

                auto colorFrame = alignedFrames.get_color_frame();
                auto depthFrame = alignedFrames.get_depth_frame();

                auto colorFrame_NotRegistered = framesCaptured.get_color_frame();

                colorTimestamps[i] = colorFrame.get_timestamp();
                depthTimestamps[i] = depthFrame.get_timestamp();

                cv::Mat depth16(depthFrame.get_height(), depthFrame.get_width(), CV_16U, (void *)depthFrame.get_data());
                cv::Mat color8(colorFrame.get_height(), colorFrame.get_width(), CV_8UC3, (void *)colorFrame.get_data());

                cv::Mat color8_notRegistered(colorFrame_NotRegistered.get_height(), colorFrame_NotRegistered.get_width(), CV_8UC3, (void *)colorFrame_NotRegistered.get_data());

                
                handlerColor->handleFrame(color8_notRegistered, i);
                handlerDepth->handleFrame(depth16, i);

            } catch (rs2::error &e) {
                std::cout << "error";
                break;
            }
        }
    }

    else {

        for (unsigned int i = 0; i < frames; i++) {
            try {
                rs2::frameset framesCaptured = pipe.wait_for_frames();
                rs2::frameset alignedFrames = align.process(framesCaptured); 
				
                auto colorFrame = alignedFrames.get_color_frame();
                auto depthFrame = alignedFrames.get_depth_frame();

                colorTimestamps[i] = colorFrame.get_timestamp();
                depthTimestamps[i] = depthFrame.get_timestamp();

                cv::Mat depth16(depthFrame.get_height(), depthFrame.get_width(), CV_16U, (void *)depthFrame.get_data());
                cv::Mat color8(colorFrame.get_height(), colorFrame.get_width(), CV_8UC3, (void *)colorFrame.get_data());

                handlerColor->handleFrame(color8, i);
                handlerDepth->handleFrame(depth16, i);
                
            } catch (rs2::error &e) {
                std::cout << "error";
                break;
            }
        }
    }
    
	double colorTimeRef = colorTimestamps.at(0);
    double depthTimeRef = depthTimestamps.at(0);
    std::transform(colorTimestamps.begin(), colorTimestamps.end(), colorTimestamps.begin(), std::bind2nd(std::minus<double>(), colorTimeRef));
    std::transform(depthTimestamps.begin(), depthTimestamps.end(), depthTimestamps.begin(), std::bind2nd(std::minus<double>(), depthTimeRef));

    return std::pair<std::vector<double>, std::vector<double>>(colorTimestamps, depthTimestamps);
}

RealSenseCameraHandler::RealSenseCameraHandler()
    : pipe() {
    try {
        rs2::config cfg;
        cfg.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30); 
        cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);   
        this->profile = pipe.start(cfg);
        this->depth_scale = get_depth_scale(profile.get_device());
        this->isConnected = true;
    } catch (std::exception e) {
        this->isConnected = false;
    }
}

RealSenseCameraHandler::~RealSenseCameraHandler() {
}

float RealSenseCameraHandler::get_depth_scale(rs2::device dev) {
    for (rs2::sensor &sensor : dev.query_sensors()) {
        if (rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>()) {
            return dpt.get_depth_scale();
        }
    }
    throw std::runtime_error("Device does not have a depth sensor");
}

bool RealSenseCameraHandler::isCameraConnected() {
    return isConnected;
}