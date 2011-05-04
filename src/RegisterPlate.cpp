#include <iostream> // for Write && WriteLine (might want to remove later)
#include "RegisterPlate.h"
#include <string>
#include <cctype> // for std::isdigit
#include <algorithm> // for std::transform
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QString>
#include <QDebug>

namespace rdm {

    RegisterPlate::RegisterPlate():
plate("00AA00")
{
}

RegisterPlate::RegisterPlate(std::string& plate):
plate(plate)
{

}

RegisterPlate::RegisterPlate(std::string& a, std::string& b, std::string& c):
plate(a + b + c)
{

}

RegisterPlate::RegisterPlate(char* plate):
plate(plate)
{

}

RegisterPlate::RegisterPlate(char a[], char b[], char c[])
{
    std::string str1 = a;
    std::string str2 = b;
    std::string str3 = c;
    plate = str1 + str2 + str3;
}

RegisterPlate::~RegisterPlate()
{
}

std::string RegisterPlate::GetPlate() const
{
    return plate;
}

std::string RegisterPlate::GetOwner() const
{
    return owner;
}
std::string RegisterPlate::GetColor() const
{
    return color;
}
int RegisterPlate::GetYear() const
{
    return year;
}
std::string RegisterPlate::GetBrand() const
{
    return brand;
}
std::string RegisterPlate::GetModel() const
{
    return model;
}
std::string RegisterPlate::GetRegistryTime() const
{
    return registryTime;
}

int RegisterPlate::GetWarnings() const
{
    return warnings;
}

std::string RegisterPlate::GetFirstBlock() const
{
    return GetFirstBlock(GetPlate());
}

std::string RegisterPlate::GetSecondBlock() const
{
    return GetSecondBlock(GetPlate());
}

std::string RegisterPlate::GetThirdBlock() const
{
    return GetThirdBlock(GetPlate());
}

void RegisterPlate::Copy(const RegisterPlate& other)
{
    plate = other.plate;
}

RegisterPlate& RegisterPlate::operator = (const RegisterPlate& other)
{
    Copy(other);
    return *this;
}

bool RegisterPlate::operator == (const RegisterPlate& other) const
{
    return this->GetPlate() == other.GetPlate();
}

bool RegisterPlate::operator != (const RegisterPlate& other) const
{
    return ! operator == (other);
}

void RegisterPlate::Write(std::ostream& output /*= std::cout*/) const
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

void RegisterPlate::WriteLine(std::ostream& output /*= std::cout*/) const
{
    Write(output);
    output << std::endl;
}

bool RegisterPlate::IsValid() const
{
    return Valid(GetPlate()) && GetEpoch() != INVALID;
}

std::string RegisterPlate::GetFirstBlock(std::string newPlate)
{
    return newPlate.substr(0, 2);
}

std::string RegisterPlate::GetSecondBlock(std::string newPlate)
{
    return newPlate.substr(2, 2);
}

std::string RegisterPlate::GetThirdBlock(std::string newPlate)
{
    return newPlate.substr(4, 2);
}

bool RegisterPlate::ValidBlock(std::string block)
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

bool RegisterPlate::Valid(std::string plate)
{
    return plate.size() == 6
        && ValidBlock(GetFirstBlock(plate))
        && ValidBlock(GetSecondBlock(plate))
        && ValidBlock(GetThirdBlock(plate));
}

bool RegisterPlate::Valid(std::string a, std::string b, std::string c)
{
    return a.size() == 2
        && b.size() == 2
        && c.size() == 2
        && ValidBlock(a)
        && ValidBlock(b)
        && ValidBlock(c);
}

void RegisterPlate::SetSeparator(char newSeparator)
{
    if (newSeparator == 120) // x
        separator = 0;
    else
        separator = newSeparator;
}

char RegisterPlate::Separator()
{
    return separator;
}

void RegisterPlate::ToUpper()
{
    std::transform(plate.begin(), plate.end(), plate.begin(), ::toupper);
}

RegisterPlate::EpochType RegisterPlate::GetEpoch() const
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

bool RegisterPlate::IsNumericBlock(std::string block)
{
    if (std::isdigit(block[0]) && std::isdigit(block[1]))
        return true;
    else if (!std::isdigit(block[0]) && !std::isdigit(block[1]))
        return false;
    else
        return false;
}

char RegisterPlate::separator = 0;

void RegisterPlate::RemoveNonAlphanumeric()
{
    for (int i = 0; i < static_cast<int>(GetPlate().size()); i++)
        if (!(std::isalnum(plate[i]))) {
            plate.erase(i, 1);
            i--;
        }
}

bool RegisterPlate::DBConnectionCreate()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("rdm");
    if (!db.open()) {
        qDebug() << "Can't open DB";
        return false;
    }
    //qDebug() << "Connected to DB";
    return true;
}

void RegisterPlate::DBConnectionClose()
{
    QSqlDatabase().close();
}

bool RegisterPlate::DBInfoLoad()
{
    QSqlQuery query;
    query.prepare("SELECT `owner`,`color`,`year`,`brand`,`model`,`registryTime`,`warnings` FROM `plates` WHERE registration = :plate" );
    query.bindValue(":plate", QString().fromStdString(GetPlate()));
    if (query.exec())
    {
        if (query.next())
        {
            std::string ownerDb = query.value(0).toString().toStdString();
            SetOwner(*&ownerDb);
            std::string colorDb = query.value(1).toString().toStdString();
            SetColor(*&colorDb);
            int yearDb = query.value(2).toInt();
            SetYear(yearDb);
            std::string brandDb = query.value(3).toString().toStdString();
            SetBrand(*&brandDb);
            std::string modelDb = query.value(4).toString().toStdString();
            SetModel(*&modelDb);
            std::string registryTimeDb = query.value(5).toString().toStdString();
            SetRegistryTime(*&registryTimeDb);
            int warningsDb = query.value(6).toInt();
            SetWarningsRaw(warningsDb);
            return true;
        }
        else
            qDebug() << "No plate " << QString().fromStdString(GetPlate()) << " found in DB";
    }
    else
        qDebug() << "Select query error: " << query.lastError().text();
    return false;
}

void RegisterPlate::DBInfoSave()
{
    if (IsValid())
    {
        QSqlQuery queryDel;
        queryDel.prepare("DELETE FROM `plates` WHERE `registration` = ?");
        queryDel.addBindValue(QString().fromStdString(GetPlate()));
        queryDel.exec();
        qDebug() << "Delete query error: " << queryDel.lastError().text();

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
        queryIns.exec();
        qDebug() << "Insert query error: " << queryIns.lastError().text();
    }
    else
        qDebug() << "Plate " << QString().fromStdString(GetPlate()) << " is not valid. Won't save to DB";

}

void RegisterPlate::SetPlate(const std::string& otherPlate)
{
    plate = otherPlate;
}

void RegisterPlate::SetOwner(const std::string& otherOwner)
{
    owner = otherOwner;
}

void RegisterPlate::SetColor(const std::string& otherColor)
{
    color = otherColor;
}

void RegisterPlate::SetYear(const int& otherYear)
{
    year = otherYear;
}

void RegisterPlate::SetBrand(const std::string& otherBrand)
{
    brand = otherBrand;
}

void RegisterPlate::SetModel(const std::string& otherModel)
{
    model = otherModel;
}

void RegisterPlate::SetRegistryTime(const std::string& otherRegistryTime)
{
    registryTime = otherRegistryTime;
}

void RegisterPlate::SetWarningsRaw(const int& otherWarnings)
{
    warnings = otherWarnings;
}

void RegisterPlate::SetWarnings()
{
    if (!IsValid())
        SetWarnings(PLATE_INVALID);
    if (GetEpoch())
        SetWarnings(PLATE_OUT_OF_CIRCULATION);
}

void RegisterPlate::SetWarnings(const Warnings otherWarnings)
{
    warnings |= static_cast<int>(otherWarnings);
}

void RegisterPlate::RemoveWarnings(const Warnings otherWarnings)
{
    warnings = warnings &~ static_cast<int>(otherWarnings);
}

bool RegisterPlate::IsStolen() const
{
    return GetWarnings() & PLATE_STOLEN; // TODO verify if it's correct
}

bool RegisterPlate::IsOutOfCirculation() const
{
    return GetWarnings() & PLATE_OUT_OF_CIRCULATION;
}

bool RegisterPlate::IsNotAuthorized() const
{
    return GetWarnings() & PLATE_NOT_AUTHORIZED;
}

bool RegisterPlate::IsColorMismatch() const
{
    return GetWarnings() & PLATE_COLOR_MISMATCH;
}

void RegisterPlate::SetNotAutorized()
{
    SetWarnings(PLATE_NOT_AUTHORIZED);
}

void RegisterPlate::SetStolen()
{
    SetWarnings(PLATE_STOLEN);
}

void RegisterPlate::TryToValidate()
{
    // TODO mudar zeros (0) para ós (O) e vice-versa no caso da matrícula ser inválida
    // etc
}

void RegisterPlate::Normalize()
{
    this->RemoveNonAlphanumeric();
    this->ToUpper();
}

}
