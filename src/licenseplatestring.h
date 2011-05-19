#ifndef LICENSEPLATESTRING_H
#define LICENSEPLATESTRING_H
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

namespace rdm {

    class LicensePlateString
    {
    public: // enums
        enum EpochType {
            PRE1992 = 0, // AA-00-00
            PRE2005 = 1, // 00-00-AA
            POS2005 = 2, // 00-AA-00
            NYI     = 3, // AA-00-AA
            INVALID = 4,
        };
        enum Warnings
        {
            PLATE_INVALID               = 0x00000001, // Matrícula inválida
            PLATE_OUT_OF_CIRCULATION    = 0x00000002, // Matrícula fora de circulação
            PLATE_COLOR_MISMATCH        = 0x00000004, // Cor do automóvel não coincide com registo
            PLATE_STOLEN                = 0x00000008, // Matrícula na lista de automóveis roubados
            PLATE_NOT_AUTHORIZED        = 0x00000010, // Automóvel não autorizado
            PLATE_PLACEHOLDER_1         = 0x00000020,
            PLATE_PLACEHOLDER_2         = 0x00000040,
            PLATE_PLACEHOLDER_3         = 0x00000080,
            PLATE_PLACEHOLDER_4         = 0x00000100,
            PLATE_PLACEHOLDER_5         = 0x00000200,
            PLATE_PLACEHOLDER_6         = 0x00000400,
        };
    private: // data members
        std::string plate;
        // sql fields:
        std::string owner;
        std::string color;
        int year;
        std::string brand;
        std::string model;
        std::string registryTime;
        int warnings;

    private: // static data members
        static char separator;
    public:
        LicensePlateString();
        LicensePlateString(std::string& plate); // pre Valid(plate);
        LicensePlateString(char* plate); // pre Valid(plate);
        LicensePlateString(std::string& a, std::string& b, std::string& c); // pre Valid(a, b, c);
        LicensePlateString(char a[], char b[], char c[]); // pre Valid(a, b, c);
        LicensePlateString(QString plate); // pre Valid(plate.toStdString());
        virtual ~LicensePlateString();

        virtual std::string GetPlate() const;
        virtual std::string GetPlateWithSep() const;
        virtual std::string GetOwner() const;
        virtual std::string GetColor() const;
        virtual int         GetYear() const;
        virtual std::string GetBrand() const;
        virtual std::string GetModel() const;
        virtual std::string GetRegistryTime() const;
        virtual int         GetWarnings() const;
        virtual EpochType   GetEpoch() const;

        virtual void SetPlate(const std::string& otherPlate);
        virtual void SetOwner(const std::string& otherOwner);
        virtual void SetColor(const std::string& otherColor);
        virtual void SetYear(const int& otherYear);
        virtual void SetBrand(const std::string& otherBrand);
        virtual void SetModel(const std::string& otherModel);
        virtual void SetRegistryTime(const std::string& otherRegistryTime);
        virtual void SetWarningsRaw(const int otherWarnings);
        virtual void SetWarnings(const Warnings otherWarnings);
        virtual void SetWarnings();
        virtual void SetWarnings(const int otherWarnings);
        virtual void RemoveWarnings(const Warnings otherWarnings);
        virtual void SetNotAutorized();
        virtual void SetStolen();

        virtual void ToUpper();
        virtual void RemoveNonAlphanumeric();

        virtual void Normalize();

        virtual std::string GetFirstBlock() const;
        virtual std::string GetSecondBlock() const;
        virtual std::string GetThirdBlock() const;

        virtual void Copy(const LicensePlateString& other);
        virtual LicensePlateString& operator = (const LicensePlateString& other);

        virtual bool operator == (const LicensePlateString& other) const;
        virtual bool operator != (const LicensePlateString& other) const;

        virtual bool IsValid() const;
        virtual bool IsStolen() const;
        virtual bool IsNotAuthorized() const;
        virtual bool IsOutOfCirculation() const;
        virtual bool IsColorMismatch() const;

        virtual bool DBInfoLoad(); // returns true if plate exists
        virtual void DBInfoSave();

        virtual void TryToValidate();

    public: // static
        static std::string GetFirstBlock(std::string newPlate);
        static std::string GetSecondBlock(std::string newPlate);
        static std::string GetThirdBlock(std::string newPlate);

        static bool ValidBlock(std::string block);
        static bool Valid(std::string newPlate);
        static bool Valid(std::string a, std::string b, std::string c);
        static void SetSeparator(char newSeparator); // char x = empty separator
        static char Separator();
        static bool IsNumericBlock(std::string block); // pre ValidBlock(block);

        static QString RunOCR();

        static QString GetWarningsText(int warnings);

        static std::string RemoveNonAlphanumeric(std::string string);
    };
}

#endif // REGISTERPLATE_H
