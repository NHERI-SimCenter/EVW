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
                               bool verticalLayout,
                               QWidget *parent)
    : QWidget(parent), theItem(0), mainWindowItem(mainItem), main(mainWindow)
{
    // create a main layout
    QVBoxLayout *mainLayout= new QVBoxLayout();

    QLayout *graphsLayout;
    if (verticalLayout == true)
        graphsLayout = new QVBoxLayout();
    else
        graphsLayout = new QHBoxLayout();

    theItemEdit = createTextEntry(label, mainLayout);
    theItemEdit->setValidator(new QIntValidator);
    connect(theItemEdit, SIGNAL(editingFinished()), this, SLOT(itemEditChanged()));

    thePlot1=new QCustomPlot();
    thePlot1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QRect rec = QApplication::desktop()->screenGeometry();

    int height;
    int width;

    if (verticalLayout == true) {
        height = 0.2*rec.height();
        width = 0.5*rec.width();
    } else {
        height = 0.25*rec.height();
        width = 0.25*rec.width();

    }

    thePlot1->setMinimumWidth(width);
    thePlot1->setMinimumHeight(height);
  //  mainLayout->addWidget(thePlot1);
     graphsLayout->addWidget(thePlot1);

    thePlot1->xAxis->setLabel(xLabel);
    thePlot1->yAxis->setLabel(yLabel);

    thePlot2=new QCustomPlot();
    thePlot2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    thePlot2->setMinimumWidth(width);
    thePlot2->setMinimumHeight(height);
//    mainLayout->addWidget(thePlot2);
    graphsLayout->addWidget(thePlot2);

    thePlot2->xAxis->setLabel(xLabel);
    thePlot2->yAxis->setLabel(yLabel);

    mainLayout->addLayout(graphsLayout);

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


    thePlot2->clearGraphs();
    graph = thePlot2->addGraph();

    thePlot2->graph(0)->setData(time, data2);
    thePlot2->xAxis->setRange(0, numSteps*dt);
    thePlot2->yAxis->setRange(minValue, maxValue);
    //thePlot->axisRect()->setAutoMargins(QCP::msNone);
    thePlot2->axisRect()->setMargins(QMargins(0,0,0,0));


    QCPItemText *textLabel1 = new QCPItemText(thePlot1);
    textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
    textLabel1->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel1->position->setCoords(1, 0); // place position at center/top of axis rect
    textLabel1->setText("Earthquake");
    textLabel1->setFont(QFont(font().family(), 16)); // make font a bit larger

    QCPItemText *textLabel2 = new QCPItemText(thePlot2);
    textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
    textLabel2->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel2->position->setCoords(1, 0); // place position at center/top of axis rect
    textLabel2->setText("Wind");
    textLabel2->setFont(QFont(font().family(), 16)); // make font a bit larger

     thePlot1->replot();
     thePlot2->replot();
}

void
ResponseWidget::setData(QVector<double> &data1, QVector<double> &x1,QVector<double> &data2, QVector<double> &x2, int numSteps) {

   thePlot1->clearGraphs();
    //thePlot->clearItems();
   thePlot1->clearPlottables();
   curve1 = new QCPCurve(thePlot1->xAxis, thePlot1->yAxis);

   curve1->setData(x1,data1);

   thePlot2->clearGraphs();
    //thePlot->clearItems();
   thePlot2->clearPlottables();
   curve2 = new QCPCurve(thePlot2->xAxis, thePlot2->yAxis);

   curve2->setData(x2,data2);

   QCPItemText *textLabel1 = new QCPItemText(thePlot1);
   textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
   textLabel1->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel1->position->setCoords(1, 0); // place position at center/top of axis rect
   textLabel1->setText("Earthquake");
   textLabel1->setFont(QFont(font().family(), 16)); // make font a bit larger

   QCPItemText *textLabel2 = new QCPItemText(thePlot2);
   textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
   textLabel2->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel2->position->setCoords(1, 0); // place position at center/top of axis rect
   textLabel2->setText("Wind");
   textLabel2->setFont(QFont(font().family(), 16)); // make font a bit larger


   thePlot1->addGraph();
   thePlot1->graph(0)->setName("Earthquake");
   //thePlot2->plotLayout()->addElement(0, 0, new QCPTextElement(thePlot2, "Wind"));

    //thePlot->graph(0)->setData(x, data, true);

    double minValue = 0;
    double maxValue = 0;
    double xMinValue = 0;
    double xMaxValue = 0;

    for (int i=0; i<numSteps; i++) {
        double value = data1.at(i);
        double xValue = x1.at(i);
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

    thePlot2->yAxis->setRange(minValue, maxValue);
    thePlot2->xAxis->setRange(xMinValue, xMaxValue);

     //thePlot->axisRect()->setAutoMargins(QCP::msNone);
    // thePlot->axisRect()->setMargins(QMargins(0,0,0,0));
     thePlot2->replot();
}
