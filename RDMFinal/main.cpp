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

#include <QtGui/QApplication>
#include <QSplashScreen>
#include "mainwindow.h"
#include "QPlastiqueStyle"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    a.setOrganizationName("ESF");
    a.setApplicationName("RDM");
    a.setApplicationVersion("0.3");
    QApplication::setStyle(new QPlastiqueStyle);
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->show();
    splash->showMessage(QObject::tr("A ligar à base de dados"), Qt::AlignRight | Qt::AlignTop, Qt::white);
    createConnection();
    splash->showMessage(QObject::tr("A iniciar o programa..."), Qt::AlignRight | Qt::AlignTop, Qt::white);
    MainWindow w;
    w.show();
    splash->finish(&w);

    delete splash;
    return a.exec();
}
