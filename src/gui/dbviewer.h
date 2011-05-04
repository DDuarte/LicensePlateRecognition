#ifndef DBVIEWER_H
#define DBVIEWER_H
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

#include <QDialog>

class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QSqlTableModel;
class QTableView;
class QLabel;
class QHBoxLayout;
class QPushButton;

class DBViewer : public QDialog
{
    Q_OBJECT
public:
    explicit DBViewer(QWidget *parent = 0);

signals:

public slots:

private slots:
    void newCar();
    void newCarSave();

private:
	void newCarReset();
    void createUI();
    void initializeModel(QSqlTableModel *model);
    QTableView *createView(const QString &title, QSqlTableModel *model);

    QWidget *newCarWindow;
    QVBoxLayout *newCarLayout;

    QLabel *registrationLabel;
    QLineEdit *registrationLine;
    QHBoxLayout *registrationLayout;

    QLabel *ownerLabel;
    QLineEdit *ownerLine;
    QHBoxLayout *ownerLayout;

    QLabel *colorLabel;
    QLineEdit *colorLine;
    QHBoxLayout *colorLayout;

    QLabel *yearLabel;
    QSpinBox *yearSpinBox;
    QHBoxLayout *yearLayout;

    QLabel *brandLabel;
    QLineEdit *brandLine;
    QComboBox *brandComboBox;
    QHBoxLayout *brandLayout;

    QLabel *modelLabel;
    QLineEdit *modelLine;
    QHBoxLayout *modelLayout;

    QLabel *warningsLabel;
    QLineEdit *warningsLine;
    QHBoxLayout *warningsLayout;

    QPushButton *newCarExitButton;
    QPushButton *newCarSaveButton;
    QHBoxLayout *buttonsNewCarLayout;

};

#endif // DBVIEWER_H
