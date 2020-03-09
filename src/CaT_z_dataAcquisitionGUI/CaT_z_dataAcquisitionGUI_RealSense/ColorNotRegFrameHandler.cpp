
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

#include "ColorNotRegFrameHandler.h"

#include <QCoreApplication>
#include <QMainWindow>
#include <QApplication>

#include <sstream>
using namespace facegram::cam;
using namespace cv;

ColorNotRegFrameHandler::ColorNotRegFrameHandler(std::string outputFile, unsigned int width, unsigned int height, std::vector<int> compression_params)
    : outputFile(outputFile),
      width(width),
      height(height),
      _compression_params(compression_params)//,
       
{}

ColorNotRegFrameHandler::~ColorNotRegFrameHandler() {
    if (vwNotReg.isOpened()) {
        vwNotReg.release();
    }
}

void ColorNotRegFrameHandler::handleFrame(Mat colorFrame, int frameNumber) {
    if (vwNotReg.isOpened()) {

        vwNotReg.write(colorFrame);

        frameCount++;
    } else {
        throw std::runtime_error("File location is not writable");
    }
}
