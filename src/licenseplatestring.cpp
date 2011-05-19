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

#include <string>
#include <cctype> // for std::isdigit
#include <algorithm> // for std::transform
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QString>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "licenseplatestring.h"

namespace rdm
{

    LicensePlateString::LicensePlateString():
        plate("00AA00"),
        owner(""),
        color(""),
        year(0),
        brand(""),
        model(""),
        registryTime(""),
        warnings(0)
    {
    }
    
    LicensePlateString::LicensePlateString(std::string& plate):
        plate(plate),
        owner(""),
        color(""),
        year(0),
        brand(""),
        model(""),
        registryTime(""),
        warnings(0)
    {
    }
    
    LicensePlateString::LicensePlateString(std::string& a, std::string& b, std::string& c):
        plate(a + b + c),
        owner(""),
        color(""),
        year(0),
        brand(""),
        model(""),
        registryTime(""),
        warnings(0)
    {
    }
    
    LicensePlateString::LicensePlateString(char* plate):
        plate(plate),
        owner(""),
        color(""),
        year(0),
        brand(""),
        model(""),
        registryTime(""),
        warnings(0)
    {
    }
    
    LicensePlateString::LicensePlateString(char a[], char b[], char c[]):
        owner(""),
        color(""),
        year(0),
        brand(""),
        model(""),
        registryTime(""),
        warnings(0)
    {
        std::string str1 = a;
        std::string str2 = b;
        std::string str3 = c;
        plate = str1 + str2 + str3;
    }

    LicensePlateString::LicensePlateString(QString plate):
        plate(plate.toStdString()),
        owner(""),
        color(""),
        year(0),
        brand(""),
        model(""),
        registryTime(""),
        warnings(0)
    {
    }


    LicensePlateString::~LicensePlateString()
    {
    }
    
    std::string LicensePlateString::GetPlate() const
    {
        return plate;
    }

    std::string LicensePlateString::GetPlateWithSep() const
    {
        if (plate.size() < static_cast<int>(6))
            return std::string("ERRO: Não encontrada.");
        char s = Separator();
        std::string result;
        if (s == 0) // no separator
            return plate;
        else
            result = plate.substr(0, 2) + s +
                plate.substr(2, 2) + s +
                plate.substr(4, 2);
        return result;
    }
    
    std::string LicensePlateString::GetOwner() const
    {
        return owner;
    }
    std::string LicensePlateString::GetColor() const
    {
        return color;
    }
    int LicensePlateString::GetYear() const
    {
        return year;
    }
    std::string LicensePlateString::GetBrand() const
    {
        return brand;
    }
    std::string LicensePlateString::GetModel() const
    {
        return model;
    }
    std::string LicensePlateString::GetRegistryTime() const
    {
        return registryTime;
    }
    
    int LicensePlateString::GetWarnings() const
    {
        return warnings;
    }
    
    std::string LicensePlateString::GetFirstBlock() const
    {
        return GetFirstBlock(GetPlate());
    }
    
    std::string LicensePlateString::GetSecondBlock() const
    {
        return GetSecondBlock(GetPlate());
    }
    
    std::string LicensePlateString::GetThirdBlock() const
    {
        return GetThirdBlock(GetPlate());
    }
    
    void LicensePlateString::Copy(const LicensePlateString& other)
    {
        plate = other.plate;
    }
    
    LicensePlateString& LicensePlateString::operator = (const LicensePlateString& other)
    {
        Copy(other);
        return *this;
    }
    
    bool LicensePlateString::operator == (const LicensePlateString& other) const
    {
        return this->GetPlate() == other.GetPlate();
    }
    
    bool LicensePlateString::operator != (const LicensePlateString& other) const
    {
        return ! operator == (other);
    }
    
    /* void LicensePlateString::Write(std::ostream& output /*= std::cout*\/) const
    {
        char s = Separator();
        if (s == 0)
            output << GetPlate().substr(0,2)
            + GetPlate().substr(2,2)
            + GetPlate().substr(4,2);
        else
            output << GetPlate().substr(0,2) + s
            + GetPlate().substr(2,2) + s
            + GetPlate().substr(4,2);
    }
    
    void LicensePlateString::WriteLine(std::ostream& output /*= std::cout*\/) const
    {
        Write(output);
        output << std::endl;
    } */
    
    bool LicensePlateString::IsValid() const
    {
        return Valid(GetPlate()) && GetEpoch() != INVALID;
    }
    
    std::string LicensePlateString::GetFirstBlock(std::string newPlate)
    {
        if (newPlate.size() >= static_cast<int>(6))
            return newPlate.substr(0, 2);
        return "";
    }
    
    std::string LicensePlateString::GetSecondBlock(std::string newPlate)
    {
        if (newPlate.size() >= static_cast<int>(6))
            return newPlate.substr(2, 2);
        return "";
    }
    
    std::string LicensePlateString::GetThirdBlock(std::string newPlate)
    {
        if (newPlate.size() >= static_cast<int>(6))
            return newPlate.substr(4, 2);
        return "";
    }
    
    bool LicensePlateString::ValidBlock(std::string block)
    {
        // Se o tamanho da string for diferente de 2, não é válido
        if (block.size() != 2)
            return false;
    
        // No caso do primeiro carácter ser um digito:
        // será válido se o segundo carácter também for um digito.
        if (std::isdigit(block[0]))
            if (std::isdigit(block[1]))
                return true;
            else
                return false;
        // No caso do primeiro carácter ser uma letra:
        // será válido se o segundo carácter também uma letra.
        else
            if (std::isdigit(block[1]))
                return false;
            else
                return true;
    }
    
    bool LicensePlateString::Valid(std::string plate)
    {
        return plate.size() == 6
            && ValidBlock(GetFirstBlock(plate))
            && ValidBlock(GetSecondBlock(plate))
            && ValidBlock(GetThirdBlock(plate));
    }
    
    bool LicensePlateString::Valid(std::string a, std::string b, std::string c)
    {
        return a.size() == 2
            && b.size() == 2
            && c.size() == 2
            && ValidBlock(a)
            && ValidBlock(b)
            && ValidBlock(c);
    }
    
    void LicensePlateString::SetSeparator(char newSeparator)
    {
        if (newSeparator == 120) // x
            separator = 0;
        else
            separator = newSeparator;
    }
    
    char LicensePlateString::Separator()
    {
        return separator;
    }
    
    void LicensePlateString::ToUpper()
    {
        std::transform(plate.begin(), plate.end(), plate.begin(), ::toupper);
    }
    
    LicensePlateString::EpochType LicensePlateString::GetEpoch() const
    {
        std::string a = GetFirstBlock();
        std::string b = GetSecondBlock();
        std::string c = GetThirdBlock();
    
        if (!IsNumericBlock(a) && IsNumericBlock(b) && IsNumericBlock(c))
            return PRE1992;
        else if (IsNumericBlock(a) && IsNumericBlock(b) && !IsNumericBlock(c))
            return PRE2005;
        else if (IsNumericBlock(a) && !IsNumericBlock(b) && IsNumericBlock(c))
            return POS2005;
        else if (!IsNumericBlock(a) && IsNumericBlock(b) && !IsNumericBlock(c))
            return NYI; // A ser usado no futuro
        else
            return INVALID;
    }
    
    bool LicensePlateString::IsNumericBlock(std::string block)
    {
        if (block.empty())
            return false;
        if (block[0] <= 0 || block[1] <= 0) // char with negative value
            return false;                   // are not cool
        if (std::isdigit(block[0]) && std::isdigit(block[1]))
            return true;
        else if (!std::isdigit(block[0]) && !std::isdigit(block[1]))
            return false;
        else
            return false;
    }
    
    char LicensePlateString::separator = 0;
    
    void LicensePlateString::RemoveNonAlphanumeric()
    {
        SetPlate(RemoveNonAlphanumeric(GetPlate()));
    }

    std::string LicensePlateString::RemoveNonAlphanumeric(std::string string)
    {
        if (string.size() <= 0)
            return "";
        for (int i = 0; i < static_cast<int>(string.size()); i++)
            if (string[i] <= static_cast<int>(0) ||
                string[i] >= static_cast<int>(256) ||
                !std::isalnum(string[i]))
            {
                string.erase(i, 1);
                i--;
            }
        return string;
    }
    
    bool LicensePlateString::DBInfoLoad()
    {
        QSqlQuery query;
        query.prepare("SELECT `owner`,`color`,`year`,`brand`,`model`,`registryTime`,`warnings` FROM `plates` WHERE registration = :plate" );
        query.bindValue(":plate", QString().fromStdString(GetPlate()));
        if (query.exec())
        {
            if (query.next())
            {
                std::string ownerDb = query.value(0).toString().toStdString();
                SetOwner(ownerDb);
                std::string colorDb = query.value(1).toString().toStdString();
                SetColor(colorDb);
                int yearDb = query.value(2).toInt();
                SetYear(yearDb);
                std::string brandDb = query.value(3).toString().toStdString();
                SetBrand(brandDb);
                std::string modelDb = query.value(4).toString().toStdString();
                SetModel(modelDb);
                std::string registryTimeDb = query.value(5).toString().toStdString();
                SetRegistryTime(registryTimeDb);
                int warningsDb = query.value(6).toInt();
                // SetWarningsRaw(warningsDb);
                SetWarnings(warningsDb);
                return true;
            }
            else
                qDebug() << "No plate " << QString().fromStdString(GetPlate()) << " found in DB";
        }
        else
            qDebug() << "Select query error: " << query.lastError().text();
        return false;
    }
    
    void LicensePlateString::DBInfoSave()
    {
        if (IsValid())
        {
            QSqlQuery queryDel;
            queryDel.prepare("DELETE FROM `plates` WHERE `registration` = ?");
            queryDel.addBindValue(QString().fromStdString(GetPlate()));
            if (!queryDel.exec())
            {
                qDebug() << "Delete query error: " << queryDel.lastError().text();
                return;
            }
    
            QSqlQuery queryIns;
            queryIns.prepare("INSERT INTO `plates` (`registration`,`owner`,`color`,`year`,`brand`,`model`,`warnings`) VALUES "
                "(?, ?, ?, ?, ?, ?, ?)");
            queryIns.addBindValue(QString().fromStdString(GetPlate()));
            queryIns.addBindValue(QString().fromStdString(GetOwner()));
            queryIns.addBindValue(QString().fromStdString(GetColor()));
            queryIns.addBindValue(GetYear());
            queryIns.addBindValue(QString().fromStdString(GetBrand()));
            queryIns.addBindValue(QString().fromStdString(GetModel()));
            queryIns.addBindValue(GetWarnings());
            if (!queryIns.exec())
            {
                qDebug() << "Insert query error: " << queryIns.lastError().text();
                return;
            }
        }
        else
            qDebug() << "Plate " << QString().fromStdString(GetPlate()) << " is not valid. Won't save to DB";
    
    }
    
    void LicensePlateString::SetPlate(const std::string& otherPlate)
    {
        plate = otherPlate;
    }
    
    void LicensePlateString::SetOwner(const std::string& otherOwner)
    {
        owner = otherOwner;
    }
    
    void LicensePlateString::SetColor(const std::string& otherColor)
    {
        color = otherColor;
    }
    
    void LicensePlateString::SetYear(const int& otherYear)
    {
        year = otherYear;
    }
    
    void LicensePlateString::SetBrand(const std::string& otherBrand)
    {
        brand = otherBrand;
    }
    
    void LicensePlateString::SetModel(const std::string& otherModel)
    {
        model = otherModel;
    }
    
    void LicensePlateString::SetRegistryTime(const std::string& otherRegistryTime)
    {
        registryTime = otherRegistryTime;
    }
    
    void LicensePlateString::SetWarningsRaw(const int otherWarnings)
    {
        warnings = otherWarnings;
    }
    
    void LicensePlateString::SetWarnings()
    {
        if (!IsValid())
            SetWarnings(PLATE_INVALID);
        else
            RemoveWarnings(PLATE_INVALID);
        if (GetEpoch() == NYI)
            SetWarnings(PLATE_OUT_OF_CIRCULATION);
        else
            RemoveWarnings(PLATE_OUT_OF_CIRCULATION);
    }
    
    void LicensePlateString::SetWarnings(const Warnings otherWarnings)
    {
        warnings |= static_cast<int>(otherWarnings);
    }

    void LicensePlateString::SetWarnings(const int otherWarnings)
    {
        warnings |= otherWarnings;
    }
    
    void LicensePlateString::RemoveWarnings(const Warnings otherWarnings)
    {
        warnings = warnings &~ static_cast<int>(otherWarnings);
    }
    
    bool LicensePlateString::IsStolen() const
    {
        return GetWarnings() & PLATE_STOLEN;
    }
    
    bool LicensePlateString::IsOutOfCirculation() const
    {
        return GetWarnings() & PLATE_OUT_OF_CIRCULATION;
    }
    
    bool LicensePlateString::IsNotAuthorized() const
    {
        return GetWarnings() & PLATE_NOT_AUTHORIZED;
    }
    
    bool LicensePlateString::IsColorMismatch() const
    {
        return GetWarnings() & PLATE_COLOR_MISMATCH;
    }
    
    void LicensePlateString::SetNotAutorized()
    {
        SetWarnings(PLATE_NOT_AUTHORIZED);
    }
    
    void LicensePlateString::SetStolen()
    {
        SetWarnings(PLATE_STOLEN);
    }
    
    void LicensePlateString::TryToValidate()
    {
        // TODO mudar zeros (0) para ós (O) e vice-versa no caso da matrícula ser inválida
        // etc
    }
    
    void LicensePlateString::Normalize()
    {
        this->RemoveNonAlphanumeric();
        if (plate.size() > 6)
            plate.erase(6);
        this->ToUpper();
    }

    QString LicensePlateString::RunOCR()
    {
        QString program = "tesseract.exe";
        QStringList arguments;
        arguments << "plate.jpg" << "plate" << "-l" << "eng";

        QProcess *tesseractProc = new QProcess();
        tesseractProc->start(program, arguments);
        tesseractProc->waitForFinished();
        tesseractProc->terminate();

        QFile file("plate.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Ficheiro não encontrado";
            return QString("");
        }
        QTextStream stream(&file);
        QString line;
        
        while (!stream.atEnd())
        {
            line = stream.readLine();
            line = QString().fromStdString(RemoveNonAlphanumeric(line.simplified().toStdString()));
            if (!line.isEmpty())
                break;
        }
        file.close();
        return line;
    }

    QString LicensePlateString::GetWarningsText(int warnings)
    {
        QString result("");
        qDebug() << warnings;
        if (warnings & PLATE_INVALID)
            return result = "Inválida."; // if invalid no need to check for other warnings
        if (warnings & PLATE_OUT_OF_CIRCULATION)
            result += "Fora de circulação.";
        if (warnings & PLATE_COLOR_MISMATCH)
            result += "Cor difere.";
        if (warnings & PLATE_STOLEN)
            result += "Roubada.";
        if (warnings & PLATE_NOT_AUTHORIZED)
            result += "Não autorizada.";
        
        if (result.isEmpty())
            result = "N/A";
        else
            result.replace(".", ". ");
        return result;
    }

}
