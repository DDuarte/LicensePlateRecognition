#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>

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

    // TODO move this to private (move createConnect (DB) to this class)
    QString dbUser;
    QString dbPass;
    QString dbPort;
    QString dbServer;
    QString dbType;
    QString dbName;
    QLabel *dbStatus;

private slots:
    void startChoose();
    void refresh();
    void stop();
    void takeScreenshot();
    void dbViewer();
    void importImage();
    void importImageDir();
    void importVideo();
    void importCam();
    void setSettings();
    void writeSettings();
    void about();
    void help();

    void setLoadDirectory(); // settings win
    void setPlateTemplatePath(); // settings win
    void showTime();
    
    void refreshDBSettings();

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
    void showWarningsDialog(int warnings);
    void enableDisplay();
    void disableDisplay();

    void startImage();
    void startImageDir(QString dir);
    
    void clock();

    QString plate;
    QString fileName;
    QString dirFileName;

    QAction *startAction;
    QAction *refreshAction;
    QAction *stopAction;
    QAction *takeScreenshotAction;
    QAction *dbViewerAction;
    QAction *importImageAction;
    QAction *importImageDirAction;
    QAction *importVideoAction;
    QAction *importCamAction;
    QAction *settingsAction;
    QAction *aboutAction;
    QAction *helpAction;
    QAction *exitAction;

    QLabel *camLabel; // Pixmap
    QLabel *plateLabel; // Pixmap
    QLabel *statusLabel;
    QLabel *clockLabel;

    QGroupBox *camGroupBox;
    QGroupBox *plateGroupBox;
    QGroupBox *listGroupBox;
    QGroupBox *infoGroupBox;

    QTableWidget *plateTable;
    QTableWidget *extraPlateTable;
    QTableWidget *listTable;

    QListWidget *infoList;

    QHBoxLayout *layoutCam;
    QVBoxLayout *layoutPlate;
    QHBoxLayout *layoutInfoPlate;
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

    /* CV CAM */
    bool mCameraRunning;
    cv::VideoCapture mCap;

private: // static
    static const QString StyleSheet();

};

#endif // MAINWINDOW_H
