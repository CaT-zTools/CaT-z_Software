
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


#include "selectbehaviorlabels.h"
#include "ui_selectbehaviorlabels.h"

SelectBehaviorLabels::SelectBehaviorLabels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectBehaviorLabels)
{
    ui->setupUi(this);

    ui->tbTableEvents->setVisible(false);

    ui->btnFinish->setVisible(false);

    listNamesBehavDefault = (QStringList()<<"Undefined"<<"Resting"<<"Walking"<<"Moving Exploration"<< "Local Exploration" << "Supported Rearing"<<"Unsupported Rearing"<<"Grooming"); 

}

SelectBehaviorLabels::~SelectBehaviorLabels()
{
    delete ui;
}

void SelectBehaviorLabels::on_btnOK_clicked()
{

    int numberOfRows = ui->sbNumberOfEvents->value() +1; 

    //
    ui->tbTableEvents->setVisible(true);
    ui->tbTableEvents->setStyleSheet("QTableWidget {background-color: rgb(220, 236, 255);}");
    ui->tbTableEvents->setRowCount(numberOfRows);
    ui->tbTableEvents->setColumnCount(2);

    ui->tbTableEvents->setHorizontalHeaderLabels(QStringList() << "id #" << "Event name (editable)");
    ui->tbTableEvents->verticalHeader()->setVisible(false);
    ui->tbTableEvents->setShowGrid(false);

    for (int i = 0; i<numberOfRows; i++)
    {
        if (i > listNamesBehavDefault.size()-1)
        {
            ui->tbTableEvents->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            ui->tbTableEvents->item(i, 0)->setFlags(Qt::ItemIsEditable);

            ui->tbTableEvents->setItem(i, 1, new QTableWidgetItem(""));
        }
        else
        {
            ui->tbTableEvents->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            ui->tbTableEvents->item(i, 0)->setFlags(Qt::ItemIsEditable);

            if (i == 0)
            {
                ui->tbTableEvents->setItem(i, 1, new QTableWidgetItem(listNamesBehavDefault[i]));
                ui->tbTableEvents->item(i, 1)->setFlags(Qt::ItemIsEditable);
            }

            else
            {
                ui->tbTableEvents->setItem(i, 1, new QTableWidgetItem(listNamesBehavDefault[i]));
            }

        }
    }

    ui->tbTableEvents->horizontalHeader()->setStretchLastSection(true);
    ui->tbTableEvents->resizeColumnsToContents();

    ui->btnFinish->setVisible(true);

}

void SelectBehaviorLabels::on_btnFinish_clicked()
{

    listNamesBehavDefault.clear();

    int numberOfRows = ui->sbNumberOfEvents->value() +1; 
    for (int i = 0; i<numberOfRows; i++)
    {
        listNamesBehavDefault << ui->tbTableEvents->item(i, 1)->text();
    }

}

QStringList SelectBehaviorLabels::getNewListOfBehaviorNames()
{

    QStringList listNamesBehavNew;

    listNamesBehavNew = listNamesBehavDefault;
    return listNamesBehavNew;

}
