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
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>

#include "dbviewer.h"

DBViewer::DBViewer(QWidget *parent):
    QDialog(parent)
{
    setWindowTitle(tr("Base de dados"));

    createUI();
}
void DBViewer::initializeModel(QSqlTableModel *model)
{
    model->setTable("plates");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("registration"));
    model->setHeaderData(1, Qt::Horizontal, tr("owner"));
    model->setHeaderData(2, Qt::Horizontal, tr("color"));
    model->setHeaderData(3, Qt::Horizontal, tr("year"));
    model->setHeaderData(4, Qt::Horizontal, tr("brand"));
    model->setHeaderData(5, Qt::Horizontal, tr("model"));
    model->setHeaderData(6, Qt::Horizontal, tr("registry_time"));
    model->setHeaderData(7, Qt::Horizontal, tr("warnings"));
}

QTableView* DBViewer::createView(const QString &title, QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    return view;
}

void DBViewer::createUI()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("plates");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Matrícula"));
    model->setHeaderData(1, Qt::Horizontal, tr("Proprietário"));
    model->setHeaderData(2, Qt::Horizontal, tr("Cor"));
    model->setHeaderData(3, Qt::Horizontal, tr("Ano de compra"));
    model->setHeaderData(4, Qt::Horizontal, tr("Marca"));
    model->setHeaderData(5, Qt::Horizontal, tr("Modelo"));
    model->setHeaderData(6, Qt::Horizontal, tr("Data de registo"));
    model->setHeaderData(7, Qt::Horizontal, tr("Advertência"));

    QTableView *view = new QTableView(this);
    view->setModel(model);

    QPushButton *newCarButton = new QPushButton(tr("Novo Registo"), this);
    newCarButton->setIcon(QIcon(":/images/dbadd.png"));
    newCarButton->setShortcut(QKeySequence::New); // Ctrl + N
    newCarButton->setStatusTip(tr("Registar um novo automóvel na base de dados"));
    connect(newCarButton, SIGNAL(clicked()), this, SLOT(newCar()));

    QPushButton *delCarButton = new QPushButton(tr("Eliminar registo"), this);
    delCarButton->setIcon(QIcon(":/images/dbdel.png"));
    delCarButton->setShortcut(QKeySequence::Delete); // ?
    delCarButton->setStatusTip(tr("Eliminar um registo da base de dados"));
    // connect(delCarButton, SIGNAL(clicked()), view, SLOT(hideRow(1))); find better slot to delete a row
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(newCarButton);
    buttonLayout->addWidget(delCarButton);

    QHBoxLayout *dbViewerLayout = new QHBoxLayout(this);
    dbViewerLayout->addWidget(view);
    dbViewerLayout->addLayout(buttonLayout);

    view->resizeColumnsToContents();
    const int collumnCount = 9;
    int width = 0;
    for (int i = 0; i <= collumnCount; i++)
        width += view->columnWidth(i);
    width += newCarButton->width();

    this->setFixedWidth(width + 105);
}

void DBViewer::newCar()
{
    newCarWindow = new QWidget(this, Qt::Dialog);
    newCarWindow->setWindowTitle(tr("Novo registo"));

    registrationLabel = new QLabel(tr("Matrícula"), newCarWindow);
    registrationLine = new QLineEdit(newCarWindow);
    registrationLine->setMaxLength(6);
    registrationLine->setInputMask(">NN-NN-NN;_");
    registrationLayout = new QHBoxLayout;
    registrationLayout->addWidget(registrationLabel);
    registrationLayout->addWidget(registrationLine);

    ownerLabel = new QLabel(tr("Proprietário"), newCarWindow);
    ownerLine = new QLineEdit(newCarWindow);
    ownerLine->setPlaceholderText(tr("Nome"));
    ownerLayout = new QHBoxLayout;
    ownerLayout->addWidget(ownerLabel);
    ownerLayout->addWidget(ownerLine);

    colorLabel = new QLabel(tr("Cor do automóvel"), newCarWindow);
    colorLine = new QLineEdit(newCarWindow);
    colorLine->setPlaceholderText(tr("Cor"));
    colorLayout = new QHBoxLayout;
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorLine);

    yearLabel = new QLabel(tr("Ano de compra"), newCarWindow);
    yearSpinBox = new QSpinBox(newCarWindow);
    yearSpinBox->setRange(1900, 2011);
    yearSpinBox->setValue(2011);
    yearSpinBox->setWrapping(true);
    yearSpinBox->setAccelerated(true);
    yearLayout = new QHBoxLayout;
    yearLayout->addWidget(yearLabel);
    yearLayout->addWidget(yearSpinBox);

    QStringList brands;
    brands << "Alfa Romeo" << "Aston Martin" << "Audi" << "BMW" << "Bentley"
           << "Cadillac" << "Chevrolet" << "Chrysler" << "Citroën" << "Daewoo"
           << "Daihatsu" << "Dodge" << "Ferrari" << "Fiat" << "Ford" << "Laborghini"
           << "Honda" << "Hyundai" << "Isuzu" << "Jaguar" << "Jeep" << "Kia"
           << "Land Rover" << "Lexus" << "Lotus" << "MG" << "MINI" << "Maserati"
           << "Mazda" << "Mercedes" << "Mercedes-Benz" << "Mitsubishi" << "Nissan"
           << "Opel" << "Peugeot" << "Porsche" << "Proton" << "Renault" << "Rover"
           << "Saab" << "Seat" << "Skoda" << "Smart" << "Ssangyong" << "Subaru"
           << "Suzuki" << "Toyota" << "Vauxhall" << "Volkswagen" << "Volvo"
           << "Outra marca...";
    brandLabel = new QLabel(tr("Marca do automóvel"), newCarWindow);
    brandComboBox = new QComboBox(newCarWindow);
    brandComboBox->addItems(brands);
    brandComboBox->setEditable(true);
    brandComboBox->setAutoCompletion(true);
    brandLayout = new QHBoxLayout;
    brandLayout->addWidget(brandLabel);
    brandLayout->addWidget(brandComboBox);

    modelLabel = new QLabel(tr("Modelo do automóvel"), newCarWindow);
    modelLine = new QLineEdit(newCarWindow);
    modelLine->setPlaceholderText(tr("Modelo"));
    modelLayout = new QHBoxLayout;
    modelLayout->addWidget(modelLabel);
    modelLayout->addWidget(modelLine);

    warningsLabel = new QLabel(tr("Avisos"), newCarWindow);
    warningsLine = new QLineEdit(newCarWindow);
    warningsLine->setPlaceholderText(tr("(Seguro, etc.)"));
    warningsLayout = new QHBoxLayout;
    warningsLayout->addWidget(warningsLabel);
    warningsLayout->addWidget(warningsLine);

    newCarSaveButton = new QPushButton(tr("Guardar"), newCarWindow);
    newCarSaveButton->setDefault(true);
    newCarSaveButton->setIcon(QIcon(":images/save.png"));
    newCarExitButton = new QPushButton(tr("Sair"), newCarWindow);
    newCarExitButton->setIcon(QIcon(":images/cancel.png"));
    buttonsNewCarLayout = new QHBoxLayout;
    buttonsNewCarLayout->addStretch();
    buttonsNewCarLayout->addWidget(newCarSaveButton);
    buttonsNewCarLayout->addWidget(newCarExitButton);
    connect(newCarSaveButton, SIGNAL(clicked()), this, SLOT(newCarSave()));
    connect(newCarExitButton, SIGNAL(clicked()), newCarWindow, SLOT(close()));
    connect(newCarSaveButton, SIGNAL(clicked()), this, SLOT(hideRow(2)));

    newCarLayout = new QVBoxLayout(newCarWindow);
    newCarLayout->addLayout(registrationLayout);
    newCarLayout->addLayout(ownerLayout);
    newCarLayout->addLayout(colorLayout);
    newCarLayout->addLayout(yearLayout);
    newCarLayout->addLayout(brandLayout);
    newCarLayout->addLayout(modelLayout);
    newCarLayout->addLayout(warningsLayout);
    newCarLayout->addStretch();
    newCarLayout->addLayout(buttonsNewCarLayout);
    newCarWindow->setLayout(newCarLayout);

    newCarWindow->show();
}

void DBViewer::newCarSave()
{
    if (registrationLine->text().remove("-").isEmpty())
    {
        QMessageBox::warning(newCarWindow, tr("Erro"),
            tr("Tem de introduzir uma matrícula"), QMessageBox::Ok);
        return;
    }
    if (ownerLine->text().isEmpty())
    {
        QMessageBox::warning(newCarWindow, tr("Erro"),
            tr("Tem de introduzir o nome do proprietário"), QMessageBox::Ok);
        return;
    }
    if (colorLine->text().isEmpty())
    {
        QMessageBox::warning(newCarWindow, tr("Erro"),
            tr("Tem de introduzir uma cor"), QMessageBox::Ok);
        return;
    }
    if (yearSpinBox->text().isEmpty())
    {
        QMessageBox::warning(newCarWindow, tr("Erro"),
            tr("Tem de introduzir o ano de compra do automóvel"), QMessageBox::Ok);
        return;
    }
    if (brandComboBox->currentText().isEmpty())
    {
        QMessageBox::warning(newCarWindow, tr("Erro"),
            tr("Tem de introduzir a marca do automóvel"), QMessageBox::Ok);
        return;
    }
    if (modelLine->text().isEmpty())
    {
        QMessageBox::warning(newCarWindow, tr("Erro"),
            tr("Tem de introduzir o modelo do automóvel"), QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    if (query.exec(tr(
        "INSERT INTO `plates` (`registration`, `owner`, `color`, `year`, `brand`, `model`, `warnings`) VALUES "
        "('%1', '%2', '%3', '%4', '%5', '%6', '%7');")
        .arg(registrationLine->text().remove("-")) // 1
        .arg(ownerLine->text())                    // 2
        .arg(colorLine->text())                    // 3
        .arg(yearSpinBox->text())                  // 4
        .arg(brandComboBox->currentText())         // 5
        .arg(modelLine->text())                    // 6
        .arg(warningsLine->text())))               // 7
    {
        QMessageBox::information(newCarWindow, tr("Sucesso"),
            tr("Registo de automóvel adicionado."), QMessageBox::Ok);
        newCarReset();
    }
    else
        QMessageBox::critical(newCarWindow, tr("Erro"),
        tr("Occoreu um erro:</br> %1").arg(query.lastError().text()), QMessageBox::Cancel);
}

void DBViewer::newCarReset()
{
    registrationLine->setText("");
    ownerLine->setText("");
    colorLine->setText("");
    yearSpinBox->setValue(2011);
    brandComboBox->setCurrentIndex(0);
    modelLine->setText("");
    warningsLine->setText("");
}
