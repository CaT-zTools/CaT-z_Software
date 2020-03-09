
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

#pragma once
#include <librealsense2\rs.hpp>
#include <opencv2\core.hpp>
#include "CameraHandler.h"
#include "FrameHandler.h"

namespace facegram {
	namespace cam {
		class RealSenseCameraHandler: CameraHandler
		{
		private:
			rs2::pipeline pipe;
			rs2::pipeline_profile profile;
			bool isConnected;
			float depth_scale;
			float get_depth_scale(rs2::device dev); 

		public:
            std::pair<std::vector<double>, std::vector<double>> captureFrames(const unsigned int frames, FrameHandler *handlerColor, FrameHandler *handlerDepth, FrameHandler *handlerRGBNotReg);
			bool isCameraConnected();
			RealSenseCameraHandler();
			~RealSenseCameraHandler();
		};

	}
}

