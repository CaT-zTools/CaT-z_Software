
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


#ifndef SELECTBEHAVIORLABELS_H
#define SELECTBEHAVIORLABELS_H

#include <QDialog>

namespace Ui {
class SelectBehaviorLabels;
}

class SelectBehaviorLabels : public QDialog
{
    Q_OBJECT

public:
    explicit SelectBehaviorLabels(QWidget *parent = 0);
    ~SelectBehaviorLabels();

    QStringList getNewListOfBehaviorNames();

private slots:
    void on_btnOK_clicked();

    void on_btnFinish_clicked();

private:
    Ui::SelectBehaviorLabels *ui;

    QStringList listNamesBehavDefault;

};

#endif // SELECTBEHAVIORLABELS_H
