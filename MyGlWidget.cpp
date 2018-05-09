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

#include "MyGlWidget.h"
#include "MainWindow.h"

#include <QtGui/QMouseEvent>
#include <QDebug>
#include <Matrix.h>
#include <Vector.h>
#include <QOpenGLShaderProgram>
//#include <GL/gl.h>



MyGlWidget::MyGlWidget(int loadType, QWidget *parent)
    : QOpenGLWidget(parent), selectMode(0), loading(loadType)
{
    setMouseTracking(true);

    timer.setInterval(200);
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(mouseSingleClickEvent()));
    doubleClicked = 0;

    numPoint = 0;
    maxNumPoint = 16;
    pointIDs = new int[maxNumPoint];
    pointVertices = new GLfloat[maxNumPoint*3];
    pointColors = new GLfloat[maxNumPoint*3];

    numLine = 0;
    maxNumLine = 16;
    lineIDs = new int[maxNumLine];
    lineVertices = new GLfloat[maxNumLine*2*3];
    lineColors = new GLfloat[maxNumLine*2*3];

    thePointVertices = new vertexStruct[maxNumPoint];
    theLineVertices = new vertexStruct[maxNumLine*2]; // 2 points for every line

}

MyGlWidget::~MyGlWidget()
{

}

void MyGlWidget::setModel(MainWindow *theM)
{
    theModel = theM;
}


// extern void glPointSize(GLfloat size);
void
MyGlWidget::drawBuffers()
{
 //glClearColor(0, 1, 1, 0);
    // program->setUniformValue("mvpMatrix", orthoProjectionMatrix);
    //program->bind();
    program->bind();
   // gl.enable(gl.POINT_SPRITE);
    //    gl.enable(gl.VERTEX_PROGRAM_POINT_SIZE);
    glLineWidth(4.0);

   // glPointSize(10.0);


    /*
    glGenBuffers(1, &pointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
    glBufferData(GL_ARRAY_BUFFER, numPoint*sizeof(vertexStruct), thePointVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertexStruct), (void *)offsetof(vertexStruct, position));
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertexStruct), (void *)offsetof(vertexStruct, color));

   // glDrawArrays(GL_POINTS, 0, numPoint);
    //glDrawBuffers(1,pointBuffer);

    glGenBuffers(1, &lineBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
    glBufferData(GL_ARRAY_BUFFER, numLine*2*sizeof(vertexStruct), theLineVertices, GL_DYNAMIC_DRAW);

   // return;

    // draw the shapes
  //  glEnableClientState(GL_COLOR_ARRAY);
  //  glEnableClientState(GL_VERTEX_ARRAY);

   */


    program->setUniformValue(mvpMatrix, orthoProjectionMatrix);

    if (numPoint > 0) {

        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,0, pointVertices);
        glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, pointColors);

        glEnableVertexAttribArray(vPosition);
        glEnableVertexAttribArray(vColor);

        glDrawArrays(GL_POINTS, 0, numPoint);

        glDisableVertexAttribArray(vPosition);
        glDisableVertexAttribArray(vColor);
    }


    if (numLine > 0) {

        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,0, lineVertices);
        glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, lineColors);

        glEnableVertexAttribArray(vPosition);
        glEnableVertexAttribArray(vColor);

        glDrawArrays(GL_LINES, 0, 2*numLine);

        glDisableVertexAttribArray(vPosition);
        glDisableVertexAttribArray(vColor);
    }



    program->release();
/*
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0, 0, 0, 0);
    program->bind();
    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
   // matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    program->setUniformValue(mvpMatrix, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f, 0.,
        -0.5f, -0.5f, 0.,
        0.5f, -0.5f, 0.
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);

    program->release();
*/
}

void
MyGlWidget::drawPoint(int tag, float x1, float y1, int numPixels, float r, float g, float b)
{

    numPoint++;
    if (numPoint > maxNumPoint) {

        GLfloat *oldPointColors = pointColors;
        GLfloat *oldPointVertices = pointVertices;
        int *oldPointIDs = pointIDs;

        int newPointSize = (maxNumPoint+32)*3;
        pointVertices = new GLfloat[newPointSize];

        pointColors = new GLfloat[newPointSize];
        pointIDs = new int[(maxNumPoint+32)];

        for (int i=0; i<maxNumPoint*3; i++) {
            pointVertices[i] = oldPointVertices[i];
            pointColors[i] = oldPointColors[i];
        }
        for (int i=0; i<maxNumPoint; i++) {
            pointIDs[i] = oldPointIDs[i];
        }

        if (oldPointVertices != 0)
            delete [] oldPointVertices;

        if (oldPointColors != 0)
            delete [] oldPointColors;
        if (oldPointIDs != 0)
            delete [] oldPointIDs;

        /*
        vertexStruct *oldVertexPoints = thePointVertices;
        thePointVertices = new vertexStruct[maxNumPoint+32];
        for (int i=0; i<maxNumPoint; i++) {
            thePointVertices[i].position[0] = oldVertexPoints[i].position[0];
            thePointVertices[i].position[1] = oldVertexPoints[i].position[1];
            thePointVertices[i].position[2] = oldVertexPoints[i].position[2];
            thePointVertices[i].color[0]= oldVertexPoints[i].color[0];
            thePointVertices[i].color[1]= oldVertexPoints[i].color[1];
            thePointVertices[i].color[2]= oldVertexPoints[i].color[2];
        }
        if (oldVertexPoints != 0)
            delete [] oldVertexPoints;
        */
        maxNumPoint += 32;
    }

    pointIDs[numPoint-1] = tag;

    GLfloat *locInVertices = &pointVertices[(numPoint-1)*3];
    GLfloat *locInColors = &pointColors[(numPoint-1)*3];


    // add location and value for point to pointVertices and pointColors
    locInVertices[0] = x1;
    locInVertices[1] = y1;
    locInVertices[2] = 0.;
    locInColors[0] = r;
    locInColors[1]=g;
    locInColors[2]=b;

    /*
    thePointVertices[numPoint-1].position[0] = x1;
    thePointVertices[numPoint-1].position[1] = y1;
    thePointVertices[numPoint-1].position[2] = 0;
    thePointVertices[numPoint-1].color[0] = r;
    thePointVertices[numPoint-1].color[1] = g;
    thePointVertices[numPoint-1].color[2] = b;
    */

    return;

}

void MyGlWidget::drawText(int tag, float x1, float y1, char *text, float r, float g, float b)
{
  //  glPushMatrix();
  //  glColor3f(r, g, b);
    //renderText(x1, y1, 0, text);
  //  glPopMatrix();
}

void
MyGlWidget::drawLine(int tag, float x1, float y1, float x2, float y2, float thick, float r, float g, float b)
{

    numLine++;
    if (numLine > maxNumLine) {
        GLfloat *oldLineColors = lineColors;
        GLfloat *oldLineVertices = lineVertices;
        int *oldLineIDs = lineIDs;

        int newLineSize = (maxNumLine+32)*2*3;
        lineVertices = new GLfloat[newLineSize];
        lineColors = new GLfloat[newLineSize];
        lineIDs = new int[(maxNumLine+32)];

        for (int i=0; i<2*maxNumLine*3; i++) {
            lineVertices[i] = oldLineVertices[i];
            lineColors[i] = oldLineColors[i];
        }
        for (int i=0; i<maxNumLine; i++) {
            lineIDs[i] = oldLineIDs[i];
        }
        if (oldLineVertices != 0)
            delete [] oldLineVertices;
        if (oldLineColors != 0)
            delete [] oldLineColors;
        if (oldLineIDs != 0)
            delete [] oldLineIDs;
        maxNumLine += 32;
    }

    GLfloat *locInVertices = &lineVertices[2*(numLine-1)*3];
    GLfloat *locInColors = &lineColors[2*(numLine-1)*3];

    locInVertices[0] = x1;
    locInVertices[1] = y1;
    locInVertices[2] = 0.;
    locInVertices[3] = x2;
    locInVertices[4] = y2;
    locInVertices[5] = 0.;

    locInColors[0] = r;
    locInColors[1] = g;
    locInColors[2] = b;
    locInColors[3] = r;
    locInColors[4] = g;
    locInColors[5] = b;
}

void MyGlWidget::reset() {
    numPoint = 0;
    numLine = 0;
}



static const char *vertexShaderSource1 =
        "attribute vec3 vPosition;\n"
        "attribute vec3 vColor;\n"
        "varying   vec3 fColor;\n"
        "uniform   mat4 mvpMatrix;\n"
        "void main() {\n"
        "fColor = vColor;\n"
        "gl_PointSize = 10.0;"
        "   gl_Position = mvpMatrix * vec4(vPosition, 1.0);\n"
        "}\n";

static const char *fragmentShaderSource1 =
        "varying vec3 fColor;\n"
        "void main() {\n"
        "   gl_FragColor = vec4(fColor, 1.0);\n"
        "}\n";

static const char *vertexShaderSource =
    "attribute highp vec4 vPosition;\n"
    "attribute lowp vec4 vColor;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 mvpMatrix;\n"
    "void main() {\n"
    "   col = vColor;\n"
    "   gl_PointSize = 10.0;\n"
    "   gl_Position = mvpMatrix * vPosition;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void MyGlWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(0x8642);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(0x8642);
   // glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
   // glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1, 1, 1, 0);

    vPosition = 0;
    vColor = 1;
    mvpMatrix = 2;


    /*
     QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
     vshader->compileSourceCode(vertexShaderSource);
      QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
      fshader->compileSourceCode(fragmentShaderSource);
      program = new QOpenGLShaderProgram;
      program->addShader(vshader);
      program->addShader(fshader);
      program->bindAttributeLocation("vertexPosition", vPosition);
      program->bindAttributeLocation("vertexColor", vColor);
      mvpMatrix = program->uniformLocation("mvpMatrix");
      program->link();
      program->bind();
      */

    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    program->link();

    vPosition = program->attributeLocation("vPosition");
    vColor = program->attributeLocation("vColor");
    mvpMatrix = program->uniformLocation("mvpMatrix");
}

void MyGlWidget::resizeGL(int width, int height) {
    w = width;
    h=height;
    glViewport(0, 0, w, h);

    if (theModel != 0) {
        float heightB = theModel->getBuildingHeight();
        float maxDisp = theModel->getMaxDisp();
        float bounH = heightB/20;
        if (maxDisp == 0)
            maxDisp = 10.0;

        float bounW = maxDisp*1.1;

        orthoProjectionMatrix.setToIdentity();
        orthoProjectionMatrix.ortho(-bounW, bounW, -bounH, bounH+heightB, -bounW, bounW);
    }
    return;
}

void MyGlWidget::update() {

    if (theModel != 0) {
        float heightB = theModel->getBuildingHeight();
        float maxDisp = theModel->getMaxDisp();
        float bounH = heightB/20;
        if (maxDisp == 0)
            maxDisp = 10.0;
        float bounW = 1.1*maxDisp;
        orthoProjectionMatrix.setToIdentity();
        orthoProjectionMatrix.ortho(-bounW, bounW, -bounH, bounH+heightB, -bounW, bounW);
    }
    this->QOpenGLWidget::update();
}

void MyGlWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    if (theModel != 0)
        theModel->draw(this, loading);

    //this->drawBuffers();

}

void MyGlWidget::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}

void MyGlWidget::mousePressEvent(QMouseEvent *event)
{
    // Save mouse press position
    mousePressPosition = event->localPos();

    if(event->buttons() & Qt::LeftButton) {
        clickedLeft = true;
    } else {
        clickedLeft = false;
    }
    if(doubleClicked){
        doubleClicked = 1;
    } else{
        timer.start();
    }
    QWidget::mousePressEvent(event);
}

void MyGlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouseReleasePosition = event->localPos();

    //
    // need to determine world coords represented by mouse position in view world
    //

    static Vector in(4);
    float xPos = mousePressPosition.x();
    float yPos = h - mousePressPosition.y() -1;

    in[0]=2*xPos/(1.0*w) - 1.0;
    in[1]=2*yPos/(1.0*h) - 1.0;
    in[2]=0.0;
    in[3]=1.0;
    static Vector pressCrd(4);
    QMatrix4x4 invert = orthoProjectionMatrix.inverted();
    static Matrix A(4,4);
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            A(i,j)=invert(i,j);

    pressCrd.addMatrixVector(0.,A,in,1.0);

    xPos = mouseReleasePosition.x();
    yPos = h- mouseReleasePosition.y() -1;

    in[0]=2*xPos/(1.0*w) - 1.0;
    in[1]=2*yPos/(1.0*h) - 1.0;
    in[2]=0.0;
    in[3]=1.0;
    static Vector releaseCrd(4);

    releaseCrd.addMatrixVector(0.,A,in,1.0);

    // given press and release coordinated, inform ManWindow
    theModel->setSelectionBoundary(pressCrd(1),releaseCrd(1));
}

void MyGlWidget::mouseMoveEvent(QMouseEvent *event)
{
    // not used yet
}

void MyGlWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // not used yet
    timer.stop();
    doubleClicked = 0; // this is to discard another press event coming

}

void MyGlWidget::mouseSingleClickEvent(void) {
    // not used yet
}

