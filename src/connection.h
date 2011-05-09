#ifndef CONNECTION_H
#define CONNECTION_H
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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include "gui/mainwindow.h"

static bool createConnection()
{
    QString type;
    if (MainWindow().dbType == "MySQL") type = "QMYSQL";

    QSqlDatabase db = QSqlDatabase::addDatabase(type);
    db.setHostName(MainWindow().dbServer);
    db.setPort(MainWindow().dbPort.toInt());
    db.setUserName(MainWindow().dbUser);
    db.setPassword(MainWindow().dbPass);
    db.setDatabaseName(MainWindow().dbName);
    bool ok = db.open();
    if (!ok) {
        QMessageBox::critical(0, qApp->tr("Não é possível abrir a BD"),
                              qApp->tr("Não é possível estabelecer uma conexão à base de dados\n"
                                       "Por favor, altere as configurações."), QMessageBox::Ok);
        return false;
    }
    else
        return true;
}

#endif // CONNECTION_H
