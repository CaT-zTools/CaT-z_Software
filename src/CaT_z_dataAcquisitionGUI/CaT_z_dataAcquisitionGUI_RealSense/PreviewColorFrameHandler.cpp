
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

#include "PreviewFrameHandler.h"
#include <opencv2\imgproc.hpp>

namespace facegram {
	namespace cam {
		PreviewColorFrameHandler::PreviewColorFrameHandler(QLabel *imageMap) : imageMap(imageMap){
			
		}


		PreviewColorFrameHandler::~PreviewColorFrameHandler()
		{
		}

		void PreviewColorFrameHandler::handleFrame(cv::Mat mat, int i)
		{
			cv::Mat resized;
            cv::cvtColor(mat, mat, CV_BGR2RGB);

			cv::resize(mat, resized, cv::Size(), 0.106, 0.106);

            QImage img(resized.data, resized.cols, resized.rows, resized.step, QImage::Format_RGB888);
			imageMap->setPixmap(QPixmap::fromImage(img));
			imageMap->repaint();
		}

	}
}

