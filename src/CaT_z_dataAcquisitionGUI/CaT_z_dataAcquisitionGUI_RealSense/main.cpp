
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

#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>

#include <QStyle>
#include <QDesktopWidget>
#include <QMessageBox>

#include "stdafx.h"
using namespace rs2;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	time_t now = time(0);

    tm *ltm = localtime(&now);
	
	if ((1900 + ltm->tm_year > 2021) || (1 + ltm->tm_mon > 9)) // October (1 + 9)
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "CaT-z software out of date! Please contact the authors: pauloaguiar@ineb.up.pt");
        messageBox.setFixedSize(500, 200);

        return 0;

    } else {

        MainWindow w;
        w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
        w.setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                w.size(),
                qApp->desktop()->availableGeometry()));

        w.show();

        return a.exec();
    }	
}