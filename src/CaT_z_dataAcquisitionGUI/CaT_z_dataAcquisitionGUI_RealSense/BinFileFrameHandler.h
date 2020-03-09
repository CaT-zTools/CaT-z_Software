
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
#include "FrameHandler.h"
#include <fstream>
#include <opencv2\core.hpp>

namespace facegram {
	namespace cam {
		class BinFileFrameHandler : public facegram::cam::FrameHandler
		{
		private:
			std::ofstream file;
			std::string filename;
			const unsigned int height;
			const unsigned int width;
			std::vector<cv::Mat> buffer;
            std::vector<int> _compression_params;

		public:

			BinFileFrameHandler(std::string filename, const unsigned int height, const unsigned int width, const unsigned int frames, std::vector<int> compression_params);
			~BinFileFrameHandler();

			// Inherited via FrameHandler
			virtual void handleFrame(cv::Mat mat, int frameNumber) override;
		};

	}
}

