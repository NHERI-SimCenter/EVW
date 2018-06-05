#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS 
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, 
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: fmckenna

#include <QMainWindow>
#include <math.h>
#include <map>
#include <QString>

class QLineEdit;
class QComboBox;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QStackedWidget;
class QCustomPlot;
class MyGlWidget;
class QSlider;
class QCheckBox;


class MyGlWidget;
class Vector;
class EarthquakeRecord;
class QCPGraph;
class QCPItemText;
class QCPItemTracer;
class EarthquakeRecord;
class QNetworkAccessManager;
class QNetworkReply;
class QFrame;

class SimpleSpreadsheetWidget;
class ResponseWidget;

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void draw(MyGlWidget *, int loading);
    void doAnalysis();
    float getBuildingHeight() {return buildingH;};
    float getMaxDisp(){return maxDisp;};
    int getNumFloors(){return numFloors;};
    void setResponse(int floor, int responseItem);
    float setSelectionBoundary(float y1, float y2);


    friend class PropertiesWidget;

private slots:
    // main edits
    void on_EarthquakeButtonPressed(void);
    void on_WindButtonPressed(void);

    void on_PeriodSelectionChanged(const QString &arg1);
    void on_includePDeltaChanged(int);
    void on_inFloors_editingFinished();
    void on_inWeight_editingFinished();
    void on_inHeight_editingFinished();
    void on_inK_editingFinished();
    void on_inDamping_editingFinished();
    void on_inGravity_editingFinished();
    void on_shapeChange(int);

    // for selected floor edits
    void on_inFloorWeight_editingFinished();

    // for selected story edits
    void on_inStoryHeight_editingFinished();
    void on_inStoryK_editingFinished();
    void on_inStoryFy_editingFinished();
    void on_inStoryB_editingFinished();
    void on_dragCoefficient_editingFinished();
    void on_windGustSpeed_editingFinished();
    void on_Seed_editingFinished();
    void on_expCatagory_indexChanged();


    // for earthquake motion combo box
    void on_inEarthquakeMotionSelectionChanged(const QString &arg1);
    void on_addMotion_clicked();
    void on_scaleFactor_editingFinished();

     // for stop and start buttons
    void on_stopButton_clicked();
    void on_runButton_clicked();
    void on_exitButton_clicked();

    // for time slider
    void on_slider_valueChanged(int value);
    void on_slider_sliderPressed();
    void on_slider_sliderReleased();

    // for table editing
    void on_theSpreadsheet_cellChanged(int row, int column);
    void on_theSpreadsheet_cellClicked(int row, int column);

    void replyFinished(QNetworkReply*);

    void on_pushButton_2_clicked();

    void resetFile();
    void open();
    bool save();
    bool saveAs();

    void about();
    void submitFeedback();
    void version();
    void copyright();

    void viewNodeResponse();
    void viewStoryResponse();

private:
    void doEarthquakeAnalysis(void);
    void doWindAnalysis(void);

    void updatePeriod();
    void setBasicModel(int numFloors, double buildingW, double buildingH, double storyK, double zeta, double grav, double width, double depth);
    void setData(int numSteps, double dT, Vector *data);
    void reset(void);

    // methods to create some of the main layouts
    void createHeaderBox(); 
    void createFooterBox(); 
    void createInputPanel();
    void createOutputPanel();
    void createActions();

    QFrame *eqMotionFrame;
    QFrame *harmonicMotionFrame;

    // the main layouts created
    QHBoxLayout *mainLayout;
    QVBoxLayout *largeLayout;
    QHBoxLayout *headerLayout;
    QHBoxLayout *footerLayout;
    QVBoxLayout *outputLayout;
    QVBoxLayout *inputLayout;

    // methods for loading and saving files given filename
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void loadFile(const QString &fileName);

    QComboBox *periodComboBox;
   // QComboBox *motionType;
   // QComboBox *inputMotionType;
    QComboBox *eqMotion;
    QPushButton *addMotion;
    QLineEdit *scaleFactorEQ;

    // global properties inputs when nothing slected
    QLineEdit *inFloors;
    QLineEdit *inWeight;
    QLineEdit *inHeight;
    QLineEdit *inK;
    QLineEdit *inDamping;
    QCheckBox *pDeltaBox;

    QLineEdit *dragCoefficient;

    // specific inputs when many selected
    QLineEdit *inFloorWeight;
    QLineEdit *inStoryK;
    QLineEdit *inStoryFy;
    QLineEdit *inStoryB;
    QLineEdit *inStoryHeight;

    QLineEdit *windGustSpeed;
    QLineEdit *seed;

    QLineEdit *squareHeight;
    QLineEdit *squareWidth;
    QLineEdit *rectangularHeight;
    QLineEdit *rectangularWidth;
    QLineEdit *rectangularDepth;
    QLineEdit *circularHeight;
    QLineEdit *circularDiameter;
    QStackedWidget *shapesWidget;
    QComboBox *shapeSelectionBox;

    QComboBox *expCatagory;
    QLineEdit *inScaleFactor;

    // buttons for running, stoppping & exiting
    QPushButton *runButton;
    QPushButton *stopButton;
    QPushButton *exitButton;

    // input frames displayed depending on selection
    QFrame *floorMassFrame;
    QFrame *storyPropertiesFrame;
    QFrame *spreadSheetFrame;

    SimpleSpreadsheetWidget *theSpreadsheet;

    // output panel widgets
    QCustomPlot *earthquakePlot;
    QCPItemText *earthquakeText;

    QSlider *slider;

    MyGlWidget *myEarthquakeResult;
    MyGlWidget *myWindResponseResult;

    QComboBox *periods;

    QLabel *currentTime;
    QLabel *currentDisp;
    QLabel *currentPeriod;
    QLabel *maxEarthquakeDispLabel;
    QLabel *maxWindDispLabel;

    QStringList headings;
    QList<int> dataTypes;
    

    Ui::MainWindow *ui;

    //
    // following are the model properties
    //

    int    numFloors;
    double buildingW;
    double buildingH;
    double storyK;
    double dampingRatio;
    double buildingWidth;
    double buildingDepth;

    double *weights;
    double *k;
    double *fy;
    double *b;

    double *floorHeights;
    double *storyHeights;
    double *dampRatios;

    double g;

    // properties related to currently selected ground motion
   // int motionTypeValue;

    double dt;
    int numSteps;
    double *gMotion;
    Vector *motionData;
    double scaleFactor;

    int numStepEarthquake;
    double dtEarthquakeMotion;
    Vector *eqData;

    Vector *eigValues;

    bool includePDelta;
    bool needAnalysis;
    bool  analysisFailed;


    // data stored from earthquake response
    double **dispResponsesEarthquake;
    double **storyForceResponsesEarthquake;
    double **storyDriftResponsesEarthquake;
    double **floorForcesEarthquake;

    // data stored from wind response
    double **dispResponsesWind;
    double **storyForceResponsesWind;
    double **storyDriftResponsesWind;
    double **floorForcesWind;

    // data for response widgets
    QVector<double> time;
    QVector<double> excitationValues;
    QVector<double> nodeResponseValuesEarthquake;
    QVector<double> storyForceValuesEarthquake;
    QVector<double> storyDriftValuesEarthquake;
    QVector<double> nodeResponseValuesWind;
    QVector<double> storyForceValuesWind;
    QVector<double> storyDriftValuesWind;
    QVector<double> floorForceValuesWind;
    QVector<double> floorForceValueEarthquake;


    // the response widgets to display the results
    ResponseWidget *theNodeResponse;
    ResponseWidget *theForceTimeResponse;
    ResponseWidget *theForceDispResponse;
    ResponseWidget *theAppliedForcesResponse;

    double maxDisp;
    int    currentStep;
    bool   stopRun;

    bool   movingSlider;
    int    fMinSelected, fMaxSelected;
    int    sMinSelected, sMaxSelected;
    int    floorSelected, storySelected;

    bool updatingPropertiesTable;


    QCPGraph *graph;
    QCPItemTracer *groupTracer;

    // map to hold earthquake records
    std::map <QString, EarthquakeRecord *> records;
    EarthquakeRecord *theCurrentRecord;

    QNetworkAccessManager *manager;

    QString currentFile;
};

#endif // MAINWINDOW_H
