#ifndef RESPONSEWIDGET_H
#define RESPONSEWIDGET_H

#include <QWidget>
#include <QVector>

class QCustomPlot;
class QLineEdit;
class QCPGraph;
class QCPCurve;
class QCPItemTracer;
class MainWindow;

class ResponseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResponseWidget(MainWindow *main, int mainWindowItem, QString &label, QString &xAxis, QString &yAxis, bool verticalLayout = true, bool scalesSame = true, QWidget *parent = 0);
    ~ResponseWidget();

    int getItem();
    void setItem(int);
    void setData(QVector<double> &dataE, QVector<double> &dataW, QVector<double> &time, int numSteps, double dt);
    void setData(QVector<double> &dataE, QVector<double> &xE, QVector<double> &dataW, QVector<double> &xW, int numSteps);

signals:

public slots:
    void itemEditChanged(void);

private:
    QCustomPlot *thePlot1;
    QCustomPlot *thePlot2;

    QLineEdit *theItemEdit;
    int theItem; // floor or story #
    int mainWindowItem; // tag used in call back

    MainWindow *main;

    QCPGraph *graph;
    QCPItemTracer *groupTracer;
    QCPCurve *curve1, *curve2;

    bool scalesSame;

};

#endif // NODERESPONSEWIDGET_H
