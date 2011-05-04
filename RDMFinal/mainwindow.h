/****************************************************************************
** RDM - Reconhecimento Digital de Matriculas
** Copyright (C) 2011  Duarte Duarte, Miguel Mendes
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** We are not to be held liable for any illegal use of this software.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTimer>



class QLabel;
class QGroupBox;
class QTableWidget;
class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSpinBox;
class DBViewer;
class QFormLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    // TODO move this to private (move createConnect to this class)
    QString dbUser;
    QString dbPass;
    QString dbPort;
    QString dbServer;
    QString dbType;
    QString dbName;
    QLabel *dbStatus;

private slots:
    void about();
    void help();
    void takeScreenshot();
    void importImage();
    void importCam();
    void importVideo();
    void start();
    void startChoose();
    void stop();
    void refresh();
    void setSettings();
    void writeSettings();
    void setLoadDirectory(); // settings win
    void setPlateTemplatePath(); // settings win
    void showTime();
    void dbViewer();
    void refreshDBSettings();
    void getPlateText();

    void camTimeout(); // cam related

private:
    DBViewer *dbViewerWin;

    void createActions();
    void createToolBars();
    void createGroupBoxes();
    void createLabels();
    void createTables();
    void createList();
    void createStatusBar();
    void createLayouts();
    void readSettings();
    void OCR();

    void lookupDB(QString plateText); // TODO replace by RegisterPlate class

    void clock();

    QString plate;

    QAction *takeScreenshotAction;
    QAction *importImageAction;
    QAction *importCamAction;
    QAction *importVideoAction;
    QAction *startAction;
    QAction *stopAction;
    QAction *refreshAction;
    QAction *aboutAction;
    QAction *settingsAction;
    QAction *exitAction;
    QAction *newCarAction;
    QAction *dbViewerAction;
    QAction *helpAction;

    QLabel *camLabel; // Pixmap
    QLabel *plateLabel; // Pixmap
    QLabel *statusLabel;
    QLabel *clockLabel;

    QGroupBox *camGroupBox;
    QGroupBox *plateGroupBox;
    QGroupBox *listGroupBox;
    QGroupBox *infoGroupBox;

    QTableWidget *plateTable;
    QTableWidget *listTable;

    QListWidget *infoList;

    QHBoxLayout *layoutCam;
    QVBoxLayout *layoutPlate;
    QHBoxLayout *layoutList;
    QVBoxLayout *layoutInfo;

    QVBoxLayout *layoutMainRight;
    QVBoxLayout *layoutMainLeft;
    QHBoxLayout *layoutMain;

    /* settings */
    QWidget *settingsWindow;
    QLabel *loadDirLabel;
    QLabel *plateTemplatePathLabel; // plate template (template match algorithm)
    QVBoxLayout *layoutSettings;
    QString loadPath;
    QString platePath;
    QLineEdit *userLineEdit;
    QLineEdit *passLineEdit;
    QSpinBox *portSpinBox;
    QLineEdit *serverLineEdit;
    QComboBox *typeComboBox;
    QLineEdit *nameLineEdit;
    
    /* CV:, TODO: move to new file/class */
    cv::Mat 	mImage2;
    cv::Mat     tmpImg;
    IplImage    *sub;

    /* CV CAM */
    cv::Mat mImageCam;
    bool mCameraRunning;
    cv::VideoCapture mCap;
    QTimer mCameraTimer;
    QTime *mTime;
    double mFPS;

};

#endif // MAINWINDOW_H
