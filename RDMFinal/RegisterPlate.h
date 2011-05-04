#ifndef _H_RegisterPlate
#define _H_RegisterPlate

namespace rdm {

    class RegisterPlate
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
            PLATE_INVALID               = 0x00000001, // Matr�cula inv�lida
            PLATE_OUT_OF_CIRCULATION    = 0x00000002, // Matr�cula fora de circula��o
            PLATE_COLOR_MISMATCH        = 0x00000004, // Cor do autom�vel n�o coincide com registo
            PLATE_STOLEN                = 0x00000008, // Matr�cula na lista de autom�veis roubados
            PLATE_NOT_AUTHORIZED        = 0x00000010, // Autom�vel n�o autorizado
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
        int warnings; // TODO add enum for different warnings

    private: // static data members
        static char separator;
    public:
        RegisterPlate();
        RegisterPlate(std::string& plate); // pre Valid(plate);
        RegisterPlate(char* plate); // pre Valid(plate);
        RegisterPlate(std::string& a, std::string& b, std::string& c); // pre Valid(a, b, c);
        RegisterPlate(char a[], char b[], char c[]); // pre Valid(a, b, c);
        virtual ~RegisterPlate();

        virtual std::string GetPlate() const;
        virtual std::string GetOwner() const;
        virtual std::string GetColor() const;
        virtual int         GetYear() const;
        virtual std::string GetBrand() const;
        virtual std::string GetModel() const;
        virtual std::string GetRegistryTime() const;
        virtual int         GetWarnings() const;

        virtual EpochType GetEpoch() const;

        virtual void SetPlate(const std::string& otherPlate);
        virtual void SetOwner(const std::string& otherOwner);
        virtual void SetColor(const std::string& otherColor);
        virtual void SetYear(const int& otherYear);
        virtual void SetBrand(const std::string& otherBrand);
        virtual void SetModel(const std::string& otherModel);
        virtual void SetRegistryTime(const std::string& otherRegistryTime);
        virtual void SetWarningsRaw(const int& otherWarnings);
        virtual void SetWarnings(const Warnings otherWarnings);
        virtual void SetWarnings();
        virtual void RemoveWarnings(const Warnings otherWarnings);
        virtual void SetNotAutorized();
        virtual void SetStolen();

        virtual void ToUpper();
        virtual void RemoveNonAlphanumeric();

        virtual void Normalize();

        virtual std::string GetFirstBlock() const;
        virtual std::string GetSecondBlock() const;
        virtual std::string GetThirdBlock() const;

        virtual void Copy(const RegisterPlate& other);
        virtual RegisterPlate& operator = (const RegisterPlate& other);

        virtual bool operator == (const RegisterPlate& other) const;
        virtual bool operator != (const RegisterPlate& other) const;

        virtual void Write(std::ostream& output = std::cout) const;
        virtual void WriteLine(std::ostream& output = std::cout) const;

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

        static bool DBConnectionCreate();
        static void DBConnectionClose();

    };
}

#endif // _H_RegisterPlate
