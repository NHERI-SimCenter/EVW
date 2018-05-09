#include "ResponseWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <MainWindow.h>

#include <qcustomplot.h>

extern QLineEdit *
createTextEntry(QString text,
                QVBoxLayout *theLayout,
                int minL=100,
                int maxL=100,
        QString *unitText =0);



ResponseWidget::ResponseWidget(MainWindow *mainWindow,
                               int mainItem,
                               QString &label,
                               QString &xLabel,
                               QString &yLabel,
                               QWidget *parent)
    : QWidget(parent), theItem(0), mainWindowItem(mainItem), main(mainWindow)
{
    // create a main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    theItemEdit = createTextEntry(label, mainLayout);
    theItemEdit->setValidator(new QIntValidator);
    connect(theItemEdit, SIGNAL(editingFinished()), this, SLOT(itemEditChanged()));

    thePlot1=new QCustomPlot();
    thePlot1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QRect rec = QApplication::desktop()->screenGeometry();

    int height = 0.2*rec.height();
    int width = 0.5*rec.width();

    thePlot1->setMinimumWidth(width);
    thePlot1->setMinimumHeight(height);
    mainLayout->addWidget(thePlot1);

    thePlot1->xAxis->setLabel(xLabel);
    thePlot1->yAxis->setLabel(yLabel);

    thePlot2=new QCustomPlot();
    thePlot2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    thePlot2->setMinimumWidth(width);
    thePlot2->setMinimumHeight(height);
    mainLayout->addWidget(thePlot2);

    thePlot2->xAxis->setLabel(xLabel);
    thePlot2->yAxis->setLabel(yLabel);

    this->setLayout(mainLayout);
}

ResponseWidget::~ResponseWidget()
{

}

int
ResponseWidget::getItem() {
    return theItem;
}

void
ResponseWidget::setItem(int newItem) {
    theItem = newItem;
    theItemEdit->setText(QString::number(newItem));
}

void
ResponseWidget::itemEditChanged() {
    int oldItem = theItem;
    QString textItems =  theItemEdit->text();
    theItem = textItems.toInt();
    if (oldItem == theItem)
        return;

    if (theItem <= 0 || theItem > main->getNumFloors()) {
        theItem=oldItem;
        theItemEdit->setText(QString::number(theItem));
    } else {
        main->setResponse(theItem, mainWindowItem);
    }
}

void
ResponseWidget::setData(QVector<double> &data1, QVector<double> &data2, QVector<double> &time, int numSteps, double dt) {


    double minValue = 0;
    double maxValue = 0;
    for (int i=0; i<numSteps; i++) {
        double value = data1.at(i);
        if (value < minValue)
            minValue = value;
        if (value > maxValue)
            maxValue = value;
    }

    for (int i=0; i<numSteps; i++) {
        double value = data2.at(i);
        if (value < minValue)
            minValue = value;
        if (value > maxValue)
            maxValue = value;
    }

    thePlot1->clearGraphs();
    graph = thePlot1->addGraph();

    thePlot1->graph(0)->setData(time, data1);
    thePlot1->xAxis->setRange(0, numSteps*dt);
    thePlot1->yAxis->setRange(minValue, maxValue);
    thePlot1->axisRect()->setMargins(QMargins(0,0,0,0));
    thePlot1->replot();

    thePlot2->clearGraphs();
    graph = thePlot2->addGraph();

    thePlot2->graph(0)->setData(time, data2);
    thePlot2->xAxis->setRange(0, numSteps*dt);
    thePlot2->yAxis->setRange(minValue, maxValue);
    //thePlot->axisRect()->setAutoMargins(QCP::msNone);
    thePlot2->axisRect()->setMargins(QMargins(0,0,0,0));
    thePlot2->replot();
}

void
ResponseWidget::setData(QVector<double> &data, QVector<double> &x, int numSteps) {

    thePlot1->clearGraphs();
    //thePlot->clearItems();
   thePlot1->clearPlottables();
   curve = new QCPCurve(thePlot1->xAxis, thePlot1->yAxis);

   curve->setData(x,data);

    //thePlot->graph(0)->setData(x, data, true);

    double minValue = 0;
    double maxValue = 0;
    double xMinValue = 0;
    double xMaxValue = 0;
    for (int i=0; i<numSteps; i++) {
        double value = data.at(i);
        double xValue = x.at(i);
        if (value < minValue)
            minValue = value;
        if (value > maxValue)
            maxValue = value;
        if (xValue < xMinValue)
            xMinValue = xValue;
        if (xValue > xMaxValue)
            xMaxValue = xValue;
    }

   //
   thePlot1->yAxis->setRange(minValue, maxValue);
   thePlot1->xAxis->setRange(xMinValue, xMaxValue);

    //thePlot->axisRect()->setAutoMargins(QCP::msNone);
   // thePlot->axisRect()->setMargins(QMargins(0,0,0,0));
    thePlot1->replot();
}
