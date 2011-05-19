#define _CRT_SECURE_NO_DEPRECATE //warning C4996: 'strcpy' was declared deprecated
#include "GLWindow.h"
#include <qapplication.h>
#include <qtimer.h>
#include <string>

using namespace std;

VideoManControl videoMan;

GLWindow::GLWindow(int timerInterval, QWidget *parent, const char *name) :
            QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    if(timerInterval == 0)
        m_timer = 0;
    else
    {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        m_timer->start(timerInterval);
    }
}

void GLWindow::timeOutSlot()
{
    updateGL();
}

void GLWindow::openCamera()
{
    inputIdentification *deviceList;
    int numDevices;
    videoMan.getAvailableDevices(&deviceList, numDevices); //list all the available devices

    int inputID = -1;
    int d = 0;
    while (d < numDevices && inputID == -1)
    {
        VideoManInputFormat format;
        inputIdentification device = deviceList[d];
        format.showDlg = true;
        format.clock = true;
        format.dropFrames = true;
        if ((inputID = videoMan.addVideoInput(device, &format)) != -1)
        {
            videoMan.showPropertyPage(inputID);
        }
        ++d;
    }    
    videoMan.freeAvailableDevicesList(&deviceList, numDevices);
    if (inputID == -1)
        QMessageBox::critical(this, tr("Error"), tr("Camera not found"));
}


void GLWindow::initializeGL()
{
}
    
void GLWindow::resizeGL(int width, int height)
{
    videoMan.changeScreenSize(0, 0, width, height);
}
    
void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
        videoMan.getFrame(0);
        videoMan.updateTexture(0);
        videoMan.releaseFrame(0);
        videoMan.renderFrame(0);
        videoMan.drawInputBorder(0, 1, 0.6f, 0.6f, 0.6f);
}