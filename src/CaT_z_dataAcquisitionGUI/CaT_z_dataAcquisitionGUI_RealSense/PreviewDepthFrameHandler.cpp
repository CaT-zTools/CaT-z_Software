
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

#include "PreviewDepthFrameHandler.h"
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>

namespace facegram {
	namespace cam {
		PreviewDepthFrameHandler::PreviewDepthFrameHandler(QLabel *imageMap) : imageMap(imageMap) {

		}


		PreviewDepthFrameHandler::~PreviewDepthFrameHandler()
		{
		}

		void PreviewDepthFrameHandler::handleFrame(cv::Mat mat, int i)
		{

			cv::Mat resized;
			cv::resize(mat, resized, cv::Size(), 0.16, 0.16);
            resized.convertTo(resized, CV_8U, 255.0f / 2000.0f, 0.0f);
            QImage img(resized.data, resized.cols, resized.rows, resized.step, QImage::Format_Indexed8);
			imageMap->setPixmap(QPixmap::fromImage(img));
			imageMap->repaint();
		}

	}
}

