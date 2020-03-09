
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

#include "imagepointsevent.h"

imagePointsEvent::imagePointsEvent(QWidget *parent) :
    QLabel(parent)
{
}

void imagePointsEvent::mouseMoveEvent( QMouseEvent *ev)
{

    this->setCursor(Qt::CrossCursor);
    this->x = ev->x();
    this->y = ev->y();

    emit Mouse_Pos(ev->x(), ev->y());
}


void imagePointsEvent::mousePressEvent( QMouseEvent *ev)
{
    this->setCursor(Qt::SizeAllCursor);

	if (ev->button() == Qt::LeftButton) {
		emit Lft_Mouse_Pressed(ev->x(), ev->y());
	}
	else if (ev->button() == Qt::RightButton) {
		emit Rgt_Mouse_Pressed(ev->x(), ev->y());
	}
}

void imagePointsEvent::mouseReleaseEvent(QMouseEvent *ev) {
	this->setCursor(Qt::SizeAllCursor);
	
	if (ev->button() == Qt::LeftButton) {
		emit Lft_Mouse_Released(ev->x(), ev->y());
	}
	else if (ev->button() == Qt::RightButton) {
		emit Rgt_Mouse_Released(ev->x(), ev->y());
	}
}

void imagePointsEvent::wheelEvent(QWheelEvent *ev) {
	if (ev->angleDelta().y() < 0) {
		emit Bwd_Scroll(ev->x(), ev->y());
	}
	else {
		emit Fwd_Scroll(ev->x(), ev->y());
	}
}
