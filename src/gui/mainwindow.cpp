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

#include <QtGui>
#include <QtSql>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfoList>
#include <QSizePolicy>

#include "mainwindow.h"
#include "dbviewer.h"
#include "../connection.h"
#include "../cv/FindPlate.h"
#include "../licenseplatestring.h"

#include "opencv2/imgproc/imgproc.hpp" // still needed for outdated cam processing
                                       // it will be removed soon

MainWindow::MainWindow()
{    
    QTime t;
    t.start();
    QWidget *centralW;
    centralW = new QWidget;
    setCentralWidget(centralW);

    createGroupBoxes();
    createLabels(); // Pixmaps
    createActions();
    createToolBars();
    createTables();
    createList();
    createStatusBar();
    createLayouts();
    clock();

    readSettings();

    dbViewerWin = 0;
    mCameraRunning = false;
    loadDirLabel = new QLabel(loadPath);

    setWindowIcon(QIcon(":/images/icon.png"));
    setWindowTitle(tr("Reconhecimento Digital de Matrículas - Versão 0.6"));
    setStyleSheet(StyleSheet());

    infoList->addItem((tr("Início: %1 ms").arg(t.elapsed())));
}

const QString MainWindow::StyleSheet()
{
    return "MainWindow"
           "{"
               "background-color:"
                   "qlineargradient(spread:pad, x1:0.468055, y1:1, x2:0.482826, y2:0, stop:0 rgba(98, 206, 255, 255),"
                    "stop:1 rgba(255, 255, 255, 255));"
           "}";
}

void MainWindow::createActions()
{
    startAction = new QAction(tr("Iniciar"), this);
    startAction->setIcon(QIcon(":/images/start.png"));
    startAction->setShortcut(QKeySequence::New); // Ctrl + N
    startAction->setStatusTip(tr("Inicia a captura de vídeo ou imagem"));
    connect(startAction, SIGNAL(triggered()), this, SLOT(startChoose()));

    refreshAction = new QAction(tr("Actualizar"), this);
    refreshAction->setIcon(QIcon(":/images/refresh.png"));
    refreshAction->setShortcut(QKeySequence::Refresh); // F5
    refreshAction->setStatusTip(tr("Actualiza a captura de vídeo ou imagem"));
    refreshAction->setDisabled(true);
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refresh()));

    stopAction = new QAction(tr("Parar"), this);
    stopAction->setIcon(QIcon(":/images/stop.png"));
    stopAction->setShortcut(QKeySequence::Delete); // Delete
    stopAction->setStatusTip(tr("Para a captura de vídeo ou imagem"));
    stopAction->setDisabled(true);
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

    takeScreenshotAction = new QAction(tr("Capturar ecrã"), this);
    takeScreenshotAction->setIcon(QIcon(":images/screenshot.png"));
    takeScreenshotAction->setShortcut(QKeySequence::Save); // Ctrl + S
    takeScreenshotAction->setStatusTip(tr("Captura e guarda uma imagem da câmara"));
    takeScreenshotAction->setDisabled(true);
    connect(takeScreenshotAction, SIGNAL(triggered()), this, SLOT(takeScreenshot()));

    dbViewerAction = new QAction(tr("Base de dados"), this);
    dbViewerAction->setIcon(QIcon(":/images/db.png"));
    dbViewerAction->setShortcut(QKeySequence::New); // Ctrl + N
    dbViewerAction->setStatusTip(tr("Mostra/adiciona/elimina os automóveis registados"));
    connect(dbViewerAction, SIGNAL(triggered()), this, SLOT(dbViewer()));

    importImageAction = new QAction(tr("Imagem"), this);
    importImageAction->setIcon(QIcon(":images/image.png"));
    importImageAction->setShortcut(QKeySequence::Open); // Ctrl + O
    importImageAction->setStatusTip(tr("Carrega uma imagem para ser analisada"));
    connect(importImageAction, SIGNAL(triggered()), this, SLOT(importImage()));

    importImageDirAction = new QAction(tr("Directório"), this);
    importImageDirAction->setIcon(QIcon(":images/loadimagedir.png"));
    importImageDirAction->setShortcut(QKeySequence::Forward); // ?
    importImageDirAction->setStatusTip(tr("Analisa todas as imagens de uma dada pasta"));
    connect(importImageDirAction, SIGNAL(triggered()), this, SLOT(importImageDir()));

    importVideoAction = new QAction(tr("Vídeo"), this);
    importVideoAction->setIcon(QIcon(":images/video.png"));
    importVideoAction->setShortcut(QKeySequence::Undo); // Ctrl + Z
    importVideoAction->setStatusTip(tr("Carrega um vídeo para ser analisado"));
    connect(importVideoAction, SIGNAL(triggered()), this, SLOT(importVideo()));

    importCamAction = new QAction(tr("Câmara"), this);
    importCamAction->setIcon(QIcon(":images/webcam.png"));
    importCamAction->setShortcut(QKeySequence::Back); // Backspace
    importCamAction->setStatusTip(tr("Faz com que uma câmara de vídeo seja a fonte de vídeo"));
    connect(importCamAction, SIGNAL(triggered()), this, SLOT(importCam()));

    settingsAction = new QAction(tr("Opções"), this);
    settingsAction->setIcon(QIcon(":/images/settings.png"));
    settingsAction->setShortcut(QKeySequence::Preferences); // Ctrl + ?
    settingsAction->setStatusTip(tr("Configurações"));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(setSettings()));

    helpAction = new QAction(tr("Ajuda"), this);
    helpAction->setIcon(QIcon(":/images/help.png"));
    helpAction->setShortcut(QKeySequence::HelpContents); // F1
    helpAction->setStatusTip(tr("Mostra a ajuda e procedimentos"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

    aboutAction = new QAction(tr("Sobre nós..."), this);
    aboutAction->setIcon(QIcon(":/images/about.png"));
    aboutAction->setShortcut(QKeySequence::WhatsThis); // Shift + F1
    aboutAction->setStatusTip(tr("Mostra informações sobre quem nós somos"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    exitAction = new QAction(tr("Sair"), this);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    exitAction->setShortcut(QKeySequence::Quit); // Ctrl + Esc
    exitAction->setStatusTip(tr("Termina o programa"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Not yet implemeted stuff
    helpAction->setDisabled(true);
    importCamAction->setDisabled(true);
    importVideoAction->setDisabled(true);
}

void MainWindow::createToolBars() 
{
    QToolBar *toolBar;
    toolBar = addToolBar(tr("&Geral"));
    toolBar->addAction(startAction);
    toolBar->addAction(refreshAction);
    toolBar->addAction(stopAction);
    toolBar->addSeparator();
    toolBar->addAction(takeScreenshotAction);
    toolBar->addSeparator();
    toolBar->addAction(dbViewerAction);
    toolBar->addSeparator();
    toolBar->addAction(importImageAction);
    toolBar->addAction(importImageDirAction);
    toolBar->addAction(importVideoAction);
    toolBar->addAction(importCamAction);
    toolBar->addSeparator();
    toolBar->addAction(settingsAction);
    toolBar->addSeparator();
    toolBar->addAction(aboutAction);
    toolBar->addAction(helpAction);
    toolBar->addSeparator();
    toolBar->addAction(exitAction);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::createGroupBoxes()
{
    camGroupBox = new QGroupBox(tr("Vídeo/imagem ao vivo"), this);
    camGroupBox->setMinimumSize(384, 288);
    camGroupBox->setMaximumSize(768, 576);
    camGroupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    plateGroupBox = new QGroupBox(tr("Último veículo detectado"), this);
    plateGroupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    listGroupBox = new QGroupBox(tr("Lista de veículos"), this);
    infoGroupBox = new QGroupBox(QObject::tr("Informações técnicas"), this);
}

void MainWindow::createLabels()
{
    camLabel = new QLabel("pixmap", camGroupBox);
    camLabel->setMinimumSize(camGroupBox->minimumSize());
    camLabel->setMaximumSize(camGroupBox->maximumSize());
    camLabel->setAlignment(Qt::AlignCenter);
    camLabel->setScaledContents(true);

    plateLabel = new QLabel(tr("Placa não encontrada"), plateGroupBox);
    plateLabel->setMaximumSize(460, 115);
    plateLabel->setScaledContents(true);

    clockLabel = new QLabel(infoGroupBox);

    if (!camLabel->pixmap() || !plateLabel->pixmap()) {
            plateGroupBox->hide();
            camGroupBox->hide();
        } // Fixme: find me a better place to live
}

void MainWindow::createTables()
{
    plateTable = new QTableWidget(6, 1); // 6 rows x 1 column
    plateTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    plateTable->setSortingEnabled(false);
    plateTable->setShowGrid(false);
    plateTable->setGridStyle(Qt::SolidLine);
    QStringList plateRows;
    QStringList plateColumns;
    plateRows << tr("Matrícula") << tr("Cor") << tr("Hora") << tr("Data") << tr("Avisos") << tr("N.º de passagens");
    plateColumns << tr("Automóvel");
    plateTable->setVerticalHeaderLabels(plateRows);
    plateTable->setHorizontalHeaderLabels(plateColumns);
    plateTable->resizeColumnsToContents();

    extraPlateTable = new QTableWidget(6, 1); // 6 rows x 1 column ??
    extraPlateTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    extraPlateTable->setSortingEnabled(false);
    extraPlateTable->setShowGrid(false);
    extraPlateTable->setGridStyle(Qt::SolidLine);
    QStringList extraPlateRows;
    QStringList extraPlateColumns;
    extraPlateRows << tr("Proprietário") << tr("Cor") << tr("Ano de compra") << tr("Marca") << tr("Modelo") << tr("Registo");
    extraPlateColumns << tr("Informações");
    extraPlateTable->setVerticalHeaderLabels(extraPlateRows);
    extraPlateTable->setHorizontalHeaderLabels(extraPlateColumns);
    extraPlateTable->hide();

    listTable = new QTableWidget(0, 5, listGroupBox); // 5 column - empty table
    listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listTable->setSortingEnabled(false);
    listTable->setShowGrid(true);
    listTable->setGridStyle(Qt::SolidLine);
    QStringList listColumns;
    listColumns << tr("Matrícula") << tr("Cor") << tr("Hora") << tr("Data") << tr("Avisos");
    listTable->setHorizontalHeaderLabels(listColumns);
    listTable->resizeColumnsToContents();
    listTable->setAlternatingRowColors(true);
    listTable->setAutoScrollMargin(5);
}

void MainWindow::createList()
{
    infoList = new QListWidget(infoGroupBox);
    infoList->addItem(tr("* Informação variada *"));
}

void MainWindow::createStatusBar()
{
    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);
}

void MainWindow::createLayouts()
{
    layoutCam = new QHBoxLayout(camGroupBox);
    layoutCam->addWidget(camLabel);

    layoutInfoPlate = new QHBoxLayout;
    layoutInfoPlate->addStretch();
    layoutInfoPlate->addWidget(plateTable);
    layoutInfoPlate->addWidget(extraPlateTable);
    layoutInfoPlate->addStretch();

    layoutPlate = new QVBoxLayout(plateGroupBox);
    layoutPlate->addWidget(plateLabel);
    layoutPlate->setAlignment(Qt::AlignHCenter);
    layoutPlate->addLayout(layoutInfoPlate);

    layoutList = new QHBoxLayout(listGroupBox);
    layoutList->addWidget(listTable);

    layoutInfo = new QVBoxLayout(infoGroupBox);
    layoutInfo->addWidget(infoList);
    layoutInfo->addWidget(clockLabel);

    layoutMainRight = new QVBoxLayout;
    layoutMainRight->addWidget(plateGroupBox);
    layoutMainRight->addWidget(infoGroupBox);

    layoutMainLeft = new QVBoxLayout;
    layoutMainLeft->addWidget(camGroupBox);
    layoutMainLeft->addWidget(listGroupBox);

    layoutMain = new QHBoxLayout(centralWidget());
    layoutMain->addLayout(layoutMainLeft);
    layoutMain->addLayout(layoutMainRight);
}

void MainWindow::importImage()
{
    stop();
    fileName = QFileDialog::getOpenFileName(camLabel, tr("Carregar"), loadPath,
        tr("Imagens (*.png *.jpg *.gif *.tiff)"));
    if (fileName.isEmpty())
        return;
    startImage();
}

void MainWindow::importImageDir()
{
    stop();

    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this,
        tr("Pasta para carregar imagens"), loadDirLabel->text(), options);
    if (!directory.isEmpty()) // -> QString.isEmpty()
        startImageDir(directory);
}

void MainWindow::importCam()
{
    // TODO Implement this
}

void MainWindow::importVideo()
{
    // TODO Implement this
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Sobre nós"),
                       tr("<h2><i>RDM</i> - Reconhecimento Digital de Matrículas</h2>"
                          "<p>Copyright &copy; 2011 ESF"
                          "<hr>"
                          "<p>Área de Projecto 12.º - CT4"
                          "<ul>"
                          "<li>Duarte Duarte</li>"
                          "<li>Miguel Mendes</li>"
                          "<li>Marian Giurca</li>"
                          "</ul>"
                          "<p align=right><a href='http://esfundao.pt/'>Escola Secundária do Fundão</a>"));
}

void MainWindow::help()
{
    QMessageBox::information(this, tr("Ajuda"),
                             tr("<h1>Blabla</h1>"
                                "<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer tincidunt vehicula accumsan. Aenean accumsan tellus dui. Phasellus dapibus luctus ligula, a feugiat libero consectetur et. Praesent non urna eget nisl ornare tincidunt sed at diam. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Morbi fringilla lacus sed est scelerisque sagittis. Donec felis urna, tincidunt nec tincidunt vel, rutrum in mauris. Nam id mauris leo. Nullam eget purus et neque accumsan elementum. Praesent faucibus mattis vulputate. Pellentesque luctus elit sollicitudin justo sagittis fermentum. Duis suscipit nunc non magna pellentesque viverra. In hac habitasse platea dictumst. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nunc facilisis, est non volutpat mattis, tortor lorem feugiat mi, at pellentesque lacus lectus a est. Nulla nisl nisi, scelerisque viverra consequat ultricies, convallis interdum leo. Etiam vitae tellus risus. Quisque placerat, sapien nec scelerisque feugiat, massa odio ultrices odio, in tempor tortor sem non ante. Praesent pharetra blandit purus a interdum.</p>"
                                "<ol>"
                                "<li>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</li>"
                                "<li>Mauris et tellus a lorem suscipit lacinia.</li>"
                                "<li>Morbi vehicula mattis elit, quis fringilla velit fringilla vitae.</li>"
                                "<li>Sed vulputate nisl eu neque rhoncus sed tempor lorem accumsan.</li>"
                                "<li>In et felis eget metus dignissim iaculis.</li>"
                                "</ol>"), QMessageBox::Ok);
    // TODO use something else, not a message box
}

void MainWindow::startChoose()
{
    QMessageBox msgBox;
    QPushButton *image = msgBox.addButton(tr("Imagem estática"), QMessageBox::ActionRole);
    QPushButton *dir = msgBox.addButton(tr("Directório"), QMessageBox::ActionRole);
    QPushButton *cam = msgBox.addButton(tr("Câmara de Vídeo"), QMessageBox::ActionRole);
    QPushButton *video = msgBox.addButton(tr("Ficheiro de Vídeo"), QMessageBox::ActionRole);
    QPushButton *cancel = msgBox.addButton(tr("Cancelar"), QMessageBox::RejectRole);

    image->setIcon(QIcon(":images/image.png"));
    dir->setIcon(QIcon(":images/loadimagedir.png"));
    cam->setIcon(QIcon(":images/webcam.png"));
    video->setIcon(QIcon(":images/video.png"));
    cancel->setIcon(QIcon(":images/cancel.png"));
    msgBox.setDefaultButton(image);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Modo"));
    msgBox.setText(tr("Fonte de imagem"));
    msgBox.exec();
    if (msgBox.clickedButton() == image)
        importImage();
    else if (msgBox.clickedButton() == dir)
        importImageDir();
    else if (msgBox.clickedButton() == cam)
        importCam();
    else if (msgBox.clickedButton() == video)
        importVideo();
    else
        return;
}

void MainWindow::takeScreenshot()
{
    if (!camLabel->pixmap())
    {
        QMessageBox::warning(this, tr("Fonte não encontrada"), tr("Não foi possível identificar a imagem ou vídeo."), QMessageBox::Ok);
        return;
    }
    QTime t;
    t.start();

    QString format = "png";
    QString initialPath = QDir::currentPath() + (plate.isEmpty() ? tr("/temp.") : tr("/%1").arg(plate));
    // might want to change "temp" to plate number later (use RegisterPlate)
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar como..."),
        initialPath,
        tr("%1 Portable Network Graphics (*.%2);;Todos os ficheiros (*)").arg(format.toUpper()).arg(format));
    QPixmap(*camLabel->pixmap()).save(fileName, format.toAscii());
    qApp->beep();
    infoList->addItem(tr("Tirar screenshot: %1 ms").arg(t.elapsed()));
}

void MainWindow::enableDisplay()
{
    stopAction->setEnabled(true);
    refreshAction->setEnabled(true);
    startAction->setDisabled(true);
    camGroupBox->show();
    plateGroupBox->show();
    camLabel->show();
    takeScreenshotAction->setEnabled(true);
}

void MainWindow::disableDisplay()
{
    stopAction->setEnabled(false);
    refreshAction->setEnabled(false);
    startAction->setDisabled(false);
    camGroupBox->hide();
    plateGroupBox->hide();
    camLabel->hide();
    takeScreenshotAction->setEnabled(false);
}

void MainWindow::startImage()
{
    QTime t;
    t.start();

    cv::Mat source = cv::imread(fileName.toStdString());
    cv::Mat target = cv::imread(platePath.toStdString());

    rdm::FindPlate a(source);
    a.SetTarget(target);
    a.SetConfidenceMinium(50);

    a.Go();
    cv::Mat procFull = rdm::FindPlate::ConvertToRGB(a.GetFullImage());
    cv::Mat procPlate = rdm::FindPlate::ConvertToRGB(a.GetPlateImage());
    QImage fullImage = a.GetQImageFromMat(procFull);
    camLabel->setPixmap(QPixmap::fromImage(fullImage)); // full image
    QImage plateImage = a.GetQImageFromMat(procPlate);
    plateLabel->setPixmap(QPixmap::fromImage(plateImage)); // plate image

    rdm::FindPlate::SaveImageToHardDrive(procPlate, "plate.jpg");

    enableDisplay();

    infoList->addItem(tr("Processar imagem: %1 ms").arg(t.elapsed()));
    
    // if (!plateImage.isNull())
    OCR();
}

void MainWindow::startImageDir(QString dir)
{
    QDir directory;
    QStringList imageFilters;
    imageFilters << "*.jpg" << "*.png" << "*.bmp" << "*.gif" << "*.tiff";
    directory.setFilter(QDir::Files);
    directory.setNameFilters(imageFilters);
    directory.setPath(dir);

    QStringList list = directory.entryList();

    cv::Mat target = cv::imread(platePath.toStdString());

    QTime t;
    t.start();
    for (int i = 0; i < list.size() && i < 10; i++)
    {
        QString sourceString = dir + "/" + list.at(i);

        cv::Mat source = cv::imread(sourceString.toStdString());
        
        rdm::FindPlate a(source);
        a.SetTarget(target);
        a.SetConfidenceMinium(50);

        a.Go();

        cv::Mat procPlate = a.GetPlateImage(); // this is pretty redundant
        QImage plateImage = a.GetQImageFromMat(procPlate);
        plateLabel->setPixmap(QPixmap::fromImage(plateImage));
        rdm::FindPlate::SaveImageToHardDrive(procPlate, "plate.jpg");

        disableDisplay();

        // if (!plateImage.isNull())
        OCR();
    }
    infoList->addItem(tr("Processar %2 imagens: %1 ms").arg(t.elapsed()).arg(list.size()));
    

}

void MainWindow::stop()
{
    if (mCameraRunning && mCap.isOpened())
    {
        mCap.release();
        mCameraRunning = false;
    }

    QPixmap *nullPixmap;
    nullPixmap = new QPixmap(0,0);
    camLabel->setPixmap(*nullPixmap);
    plateLabel->setPixmap(*nullPixmap);
    camGroupBox->hide();
    plateGroupBox->hide();
    // should delete camGroupBox, plateGroupBox instead
    // delete OpenCV cam etc
    stopAction->setDisabled(true);
    refreshAction->setDisabled(true);
    startAction->setEnabled(true);
    takeScreenshotAction->setDisabled(true);
}

void MainWindow::refresh()
{
    stop();
    startImage();
    // :P
}

void MainWindow::setSettings()
{
    // TODO move to new class (settings.h, settings.cpp)
    settingsWindow = new QWidget(this, Qt::Dialog);
    settingsWindow->setWindowModality(Qt::WindowModal);
    settingsWindow->setWindowTitle(tr("Configurações"));

    QPushButton *loadDirButton;
    loadDirButton = new QPushButton(tr("Pasta para carregar imagens"), settingsWindow);
    loadDirButton->setIcon(QIcon(":images/load.png"));
    connect(loadDirButton, SIGNAL(clicked()), this, SLOT(setLoadDirectory()));
    QHBoxLayout *loadDirLayout;
    loadDirLayout = new QHBoxLayout;
    loadDirLayout->addWidget(loadDirButton);
    loadDirLayout->addWidget(loadDirLabel);

    plateTemplatePathLabel = new QLabel(platePath, settingsWindow);
    QPushButton *plateTemplatePathButton;
    plateTemplatePathButton = new QPushButton(tr("Placa modelo"), settingsWindow);
    plateTemplatePathButton->setIcon(QIcon(":images/plate.png"));
    connect(plateTemplatePathButton, SIGNAL(clicked()), this, SLOT(setPlateTemplatePath()));

    QHBoxLayout *plateTemplateLayout;
    plateTemplateLayout = new QHBoxLayout;
    plateTemplateLayout->addWidget(plateTemplatePathButton);
    plateTemplateLayout->addWidget(plateTemplatePathLabel);

    QGroupBox *dbGroup;
    dbGroup = new QGroupBox(tr("Base de dados"), settingsWindow);

    userLineEdit = new QLineEdit(dbUser,settingsWindow);
    passLineEdit = new QLineEdit(dbPass,settingsWindow);
    portSpinBox = new QSpinBox(settingsWindow);
    serverLineEdit = new QLineEdit(dbServer, settingsWindow);
    typeComboBox = new QComboBox(settingsWindow);
    nameLineEdit = new QLineEdit(dbName, settingsWindow);

    // Default settings
    if (dbUser.isEmpty()) userLineEdit->setText("root");
    if (dbPass.isEmpty()) passLineEdit->setText("");
    if (dbPort.isEmpty()) portSpinBox->setValue(3306);
    if (dbServer.isEmpty()) serverLineEdit->setText("localhost");
    if (dbType.isEmpty()) typeComboBox->setCurrentIndex(0);
    if (dbName.isEmpty()) nameLineEdit->setText("rdm");

    passLineEdit->setEchoMode(QLineEdit::Password);
    portSpinBox->setRange(1,65535);
    portSpinBox->setValue(dbPort.toInt());
    typeComboBox->addItem("MySQL"); // At this moment only MySQL is supported
    typeComboBox->setCurrentIndex(0);

    QFormLayout *dbLayout;
    dbLayout = new QFormLayout;
    dbLayout->addRow(tr("Tipo:"), typeComboBox);
    dbLayout->addRow(tr("Servidor:"), serverLineEdit);
    dbLayout->addRow(tr("Porta:"), portSpinBox);
    dbLayout->addRow(tr("Nome da BD:"), nameLineEdit);
    dbLayout->addRow(tr("Utilizador:"), userLineEdit);
    dbLayout->addRow(tr("Palavra-passe:"), passLineEdit);

    dbStatus = new QLabel;
    QPushButton *dbRefresh;
    dbRefresh = new QPushButton(QIcon(":images/refresh.png"), tr("Actualizar"), dbGroup);
    connect(dbRefresh, SIGNAL(clicked()), this, SLOT(refreshDBSettings()));
    refreshDBSettings();
    QHBoxLayout* dbButLayout = new QHBoxLayout;
    dbButLayout->addWidget(dbRefresh);
    dbButLayout->addWidget(dbStatus);
    dbButLayout->setAlignment(Qt::AlignHCenter);

    QVBoxLayout* dbStLayout = new QVBoxLayout;
    dbStLayout->addLayout(dbLayout);
    dbStLayout->addLayout(dbButLayout);
    dbGroup->setLayout(dbStLayout);

    QPushButton *saveAndExitButton;
    QPushButton *exitButton;

    saveAndExitButton = new QPushButton(tr("Guardar && Sair"),settingsWindow);
    exitButton = new QPushButton(tr("Sair"),settingsWindow);
    saveAndExitButton->setDefault(true);
    saveAndExitButton->setIcon(QIcon(":images/save.png"));
    exitButton->setIcon(QIcon(":images/cancel.png"));

    QHBoxLayout *buttonsLayoutSettings;
    buttonsLayoutSettings = new QHBoxLayout;
    buttonsLayoutSettings->addStretch();
    buttonsLayoutSettings->addWidget(saveAndExitButton);
    buttonsLayoutSettings->addWidget(exitButton);

    layoutSettings = new QVBoxLayout;
    layoutSettings->addLayout(loadDirLayout);
    layoutSettings->addLayout(plateTemplateLayout);
    layoutSettings->addWidget(dbGroup);
    layoutSettings->addStretch();
    layoutSettings->addLayout(buttonsLayoutSettings);

    settingsWindow->setLayout(layoutSettings);

    settingsWindow->show();

    connect(saveAndExitButton,SIGNAL(clicked()),this,SLOT(writeSettings()));
    connect(exitButton,SIGNAL(clicked()),settingsWindow,SLOT(close()));
}

void MainWindow::refreshDBSettings()
{
    QSettings settings("RDMESF", "RDM", this);
    settings.setValue("dbUser", userLineEdit->text());
    settings.setValue("dbPass", passLineEdit->text());
    settings.setValue("dbPort", portSpinBox->value());
    settings.setValue("dbServer", serverLineEdit->text());
    settings.setValue("dbType", typeComboBox->currentText());
    settings.setValue("dbName", nameLineEdit->text());
    dbUser = settings.value("dbUser").toString();
    dbPass = settings.value("dbPass").toString();
    dbPort = settings.value("dbPort").toString();
    dbServer = settings.value("dbServer").toString();
    dbType = settings.value("dbType").toString();
    dbName = settings.value("dbName").toString();

    if (!createConnection())
    {
         dbStatus->setPixmap(QPixmap(":/images/wrong.png"));
         dbStatus->setToolTip(tr("Não conectado"));
         dbViewerAction->setDisabled(true);
         dbViewerAction->setToolTip(tr("Não foi possível ligar à base de dados. Por favor, altere a configuração."));
    }
    else
    {
         dbStatus->setPixmap(QPixmap(":images/correct.png"));
         dbStatus->setToolTip(tr("Conectado"));
         dbViewerAction->setEnabled(true);
         dbViewerAction->setStatusTip(tr("Mostra/adiciona/elimina automóveis registados"));
    }
}

void MainWindow::writeSettings()
{
    QSettings settings("RDMESF", "RDM", this);

    settings.setValue("loadDir", loadDirLabel->text());
    settings.setValue("platePath", plateTemplatePathLabel->text());
    settings.setValue("dbUser", userLineEdit->text());
    settings.setValue("dbPass", passLineEdit->text());
    settings.setValue("dbPort", portSpinBox->value());
    settings.setValue("dbServer", serverLineEdit->text());
    settings.setValue("dbType", typeComboBox->currentText());
    settings.setValue("dbName", nameLineEdit->text());

    settingsWindow->close();

    readSettings(); // Apply settings without needing to restart the program
}

void MainWindow::readSettings()
{
    QSettings settings("RDMESF", "RDM", this);
    loadPath = settings.value("loadDir").toString();
    platePath = settings.value("platePath").toString();
    dbUser = settings.value("dbUser").toString();
    dbPass = settings.value("dbPass").toString();
    dbPort = settings.value("dbPort").toString();
    dbServer = settings.value("dbServer").toString();
    dbType = settings.value("dbType").toString();
    dbName = settings.value("dbName").toString();
}

void MainWindow::clock()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    showTime();
}

void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    clockLabel->setText(tr("Hora actual: <b>") + text + "</b>");
}

void MainWindow::setLoadDirectory()
{
     QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
     QString directory = QFileDialog::getExistingDirectory(this,
         tr("Pasta para carregar imagens"), loadDirLabel->text(), options);
     if (!directory.isEmpty()) // -> QString.isEmpty()
         loadDirLabel->setText(directory);
}

void MainWindow::setPlateTemplatePath()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Placa modelo"), plateTemplatePathLabel->text(),
        tr("Imagems (*.png) (*.jpg)"), &selectedFilter, options);
    if (!fileName.isEmpty())
        plateTemplatePathLabel->setText(fileName);
}

void MainWindow::dbViewer()
{
    dbViewerWin = new DBViewer(this);
    dbViewerWin->show();
}

void MainWindow::OCR()
{
    QTime t;
    t.start();
    rdm::LicensePlateString lic("ERRO: Não encontrada.");
    bool plateExists = false;

    if (!plateLabel->pixmap()->isNull())
    {
        lic.SetPlate(rdm::LicensePlateString::RunOCR().toStdString());
        lic.Normalize();

        lic.SetWarnings();
        if (lic.DBInfoLoad())
            plateExists = true;
        lic.SetSeparator('-');
        infoList->addItem(tr("%1").arg(QString().fromStdString(lic.GetPlate())));
        infoList->addItem(tr("%1").arg(QString().fromStdString(lic.GetPlateWithSep())));

        infoList->addItem(tr("Analise de caracteres: %1 ms").arg(t.elapsed()));
    }
    
    QTableWidgetItem *plateItem = new QTableWidgetItem(QString().fromStdString(lic.GetPlateWithSep()));
    QTableWidgetItem *colorItem = new QTableWidgetItem("N/A");
    QTableWidgetItem *hourItem = new QTableWidgetItem(QTime().currentTime().toString("hh:mm:ss"));
    QTableWidgetItem *dateItem = new QTableWidgetItem(QDateTime().currentDateTime().toString("dd.MM.yyyy"));
    QTableWidgetItem *warnItem = new QTableWidgetItem(rdm::LicensePlateString::GetWarningsText(lic.GetWarnings()));
    QTableWidgetItem *passItem = new QTableWidgetItem("N/A");
    plateTable->setItem(0, 0, plateItem);
    plateTable->setItem(1, 0, colorItem);
    plateTable->setItem(2, 0, hourItem);
    plateTable->setItem(3, 0, dateItem);
    plateTable->setItem(4, 0, warnItem);
    plateTable->setItem(5, 0, passItem);

    if (plateExists)
    {
        QTableWidgetItem *ownerItemE = new QTableWidgetItem(QString().fromStdString(lic.GetOwner()));
        QTableWidgetItem *colorItemE = new QTableWidgetItem(QString().fromStdString(lic.GetColor()));
        QTableWidgetItem *yearItemE = new QTableWidgetItem(QString("%1").arg(lic.GetYear()));
        QTableWidgetItem *brandItemE = new QTableWidgetItem(QString().fromStdString(lic.GetBrand()));
        QTableWidgetItem *modelItemE = new QTableWidgetItem(QString().fromStdString(lic.GetModel()));
        QTableWidgetItem *registItemE = new QTableWidgetItem(QString().fromStdString(lic.GetRegistryTime()));
        extraPlateTable->setItem(0, 0, ownerItemE);
        extraPlateTable->setItem(1, 0, colorItemE);
        extraPlateTable->setItem(2, 0, yearItemE);
        extraPlateTable->setItem(3, 0, brandItemE);
        extraPlateTable->setItem(4, 0, modelItemE);
        extraPlateTable->setItem(5, 0, registItemE);
        extraPlateTable->show();
    }
    else extraPlateTable->hide();

    QTableWidgetItem *plateItemT = new QTableWidgetItem(QString().fromStdString(lic.GetPlateWithSep()));
    QTableWidgetItem *colorItemT = new QTableWidgetItem("N/A");
    QTableWidgetItem *hourItemT = new QTableWidgetItem(QTime().currentTime().toString("hh:mm:ss"));
    QTableWidgetItem *dateItemT = new QTableWidgetItem(QDateTime().currentDateTime().toString("dd.MM.yyyy"));
    QTableWidgetItem *warnItemT = new QTableWidgetItem(rdm::LicensePlateString::GetWarningsText(lic.GetWarnings()));
    int count = listTable->rowCount();
    listTable->setRowCount(count + 1); // this is the reason why we can't enable sorting on this table
    listTable->setItem(count, 0, plateItemT);
    listTable->setItem(count, 1, colorItemT);
    listTable->setItem(count, 2, hourItemT);
    listTable->setItem(count, 3, dateItemT);
    listTable->setItem(count, 4, warnItemT);
    listTable->scrollToItem(plateItemT);

    this->plate = QString().fromStdString(lic.GetPlate());

    plateTable->resizeColumnsToContents();
    extraPlateTable->resizeColumnsToContents();
    listTable->resizeColumnsToContents();
}