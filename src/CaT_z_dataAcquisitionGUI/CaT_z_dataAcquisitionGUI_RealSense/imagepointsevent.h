
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

#ifndef IMAGEPOINTSEVENT_H
#define IMAGEPOINTSEVENT_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#include <mainwindow.h>

class imagePointsEvent : public QLabel
{
    Q_OBJECT
public:
    //imagePointsEvent();

    explicit imagePointsEvent (QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *ev);

    void mousePressEvent (QMouseEvent *ev);

	void mouseReleaseEvent(QMouseEvent *ev);

	void wheelEvent(QWheelEvent *ev);

    int x,y;

signals:
    void Lft_Mouse_Pressed(int x, int y);
	void Rgt_Mouse_Pressed(int x, int y);
	void Lft_Mouse_Released(int x, int y);
	void Rgt_Mouse_Released(int x, int y);
	void Fwd_Scroll(int, int);
	void Bwd_Scroll(int, int);

    void Mouse_Pos(int x, int y);


public slots:

};

#endif // IMAGEPOINTSEVENT_H
