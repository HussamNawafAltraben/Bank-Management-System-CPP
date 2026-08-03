#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>



using namespace std;
const string ClientsFileName = "Clients2.txt";

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool   MarkForDelete = false; // علامة تُستخدم لتحديد ما إذا كان العميل مطلوب حذفه أم لا
};

// تعريف الـ Enum لخيارات القائمة الرئيسية
enum enMainMenueOptions {
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransaction = 6,
    eExit = 7
};

enum enTransactionMenueOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalance = 3,
    eMainMenue = 4
};

vector<string> SplitString(string S1, string Delim) {
    vector<string> vString;
    size_t pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord); // إضافة الكلمة المستخرجة للفيكتور
        }
        S1.erase(0, pos + Delim.length()); // مسح الجزء المقروء للمتابعة
    }
    if (S1 != "") {
        vString.push_back(S1); // إضافة الكلمة الأخيرة المتبقية في السطر
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") {
    sClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    if (vClientData.size() >= 5) {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]); // تحويل النص إلى رقم عشري (double)
    }
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance); // تحويل الرقم إلى نص
    return stClientRecord;
}

vector<sClient> LoadCleintsDataFromFile(string FileName) {
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // فتح الملف في وضع القراءة (Read Mode)

    if (MyFile.is_open()) {
        string Line;
        while (getline(MyFile, Line)) {
            if (Line.empty()) { // تجاهل أي أسطر فارغة بالخطأ داخل الملف
                continue;
            }
            vClients.push_back(ConvertLinetoRecord(Line)); // تحويل السطر وإضافته للفيكتور
        }
        MyFile.close(); // إغلاق الملف بعد الانتهاء
    }
    return vClients;
}

void PrintClientCard(sClient Client) {
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number  : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance << endl;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client) {
    for (sClient C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C; // تمرير بيانات العميل التي تم العثور عليها عبر المرجع (Reference)
            return true;
        }
    }
    return false; // إرجاع false إذا لم يتم العثور على الحساب
}

string ReadClientAccountNumber() {
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber ? ";
    cin >> AccountNumber;
    return AccountNumber;
}

vector<sClient> SaveClientsDataToFile(string FileName, vector<sClient> vClients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out); // فتح الملف في وضع الكتابة الفوقية (Overwrite Mode)

    string DataLine;

    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);

        cout << "\n\nAre You sure you want delete this client? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found .";
        return false;
    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n \n Enter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "\n \n Enter Name ? ";
    getline(cin, Client.Name);

    cout << "\n \n Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "\n \n Enter AccountBalance ? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Do you want to update the Client data ?\n";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "The Client updated successfully";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found .";
        return false;
    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void PrintClientRecordLine(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(30) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecordBalanceLine(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
}

void GoBackToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
}

void AddClients()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void PrintAllClientsData(vector <sClient> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(30) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sClient Client : vClients)
    {
        PrintClientRecordLine(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

double ReadDepositAmount()
{
    double DepositAmount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> DepositAmount;
    return DepositAmount;
}

bool Deposit(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want to perform this transaction? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            double DepositAmount = ReadDepositAmount();

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance += DepositAmount;
                    cout << "\nDone Successfully, New Balance is: " << C.AccountBalance << endl;
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found .\n";
        return false;
    }
    return false;
}

double ReadWithdrawAmount()
{
    double WithdrawAmount = 0;
    cout << "\nPlease enter Withdraw amount? ";
    cin >> WithdrawAmount;
    return WithdrawAmount;
}

bool Withdraw(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        double WithdrawAmount = ReadWithdrawAmount();

        // التكرار والتحقق من أن المبلغ لا يتجاوز الرصيد
        while (WithdrawAmount > Client.AccountBalance)
        {
            cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance;
            cout << "\nPlease enter another amount? ";
            cin >> WithdrawAmount;
        }

        cout << "\nAre you sure you want to perform this transaction? y/n ? ";
        cin >> Answer;

        if (tolower(Answer) == 'y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance -= WithdrawAmount;
                    cout << "\nDone Successfully, New Balance is: " << C.AccountBalance << endl;
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found.\n";
        return false;
    }
    return false;
}

void ShowDepositScreen()
{
    cout << "=======================================================\n";
    cout << "                  Deposit Screen \n";
    cout << "=======================================================\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    Deposit(AccountNumber, vClients);
}

void ShowWithdrawScreen() {
    cout << "=======================================================\n";
    cout << "                  Withdraw Screen\n";
    cout << "=======================================================\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    Withdraw(AccountNumber, vClients);
}

void PrintTotalBalances(vector <sClient> vClients)
{
    double TotalBalances = 0;

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t Total Balances = " << TotalBalances << endl;
}

void ShowTotalBalancesScreen()
{
    cout << "=======================================================\n";
    cout << "                 Total Balances Screen\n";
    cout << "=======================================================\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    PrintTotalBalances(vClients);
}

void Transaction() {
    short ChoiceNumber = 0;

    do
    {
        system("cls");
        cout << "=======================================================\n";
        cout << "                  Transactions Menue Screen \n";
        cout << "=======================================================\n";
        cout << "                [1] Deposit.\n";
        cout << "                [2] Withdraw.\n";
        cout << "                [3] Total Balance .\n";
        cout << "                [4] Main Menue.\n";
        cout << "=======================================================\n";
        cout << "Choose what do you want to do ? [1 to 4]? ";
        cin >> ChoiceNumber;

        enTransactionMenueOptions TransactionMenueOptions = (enTransactionMenueOptions)ChoiceNumber;

        switch (TransactionMenueOptions)
        {
        case enTransactionMenueOptions::eDeposit:
            system("cls");
            ShowDepositScreen();
            GoBackToTransactionMenue();
            break;

        case enTransactionMenueOptions::eWithdraw:
            system("cls");
            ShowWithdrawScreen();
            GoBackToTransactionMenue();
            break;

        case enTransactionMenueOptions::eTotalBalance:
            system("cls");
            ShowTotalBalancesScreen();
            GoBackToTransactionMenue();
            break;

        case enTransactionMenueOptions::eMainMenue:
            break;
        }

    } while (ChoiceNumber != enTransactionMenueOptions::eMainMenue);
}

void MainScreen()
{
    short ChoiceNumber = 0;

    do
    {
        system("cls");
        cout << "=======================================================\n";
        cout << "                  Main Menue Screen \n";
        cout << "=======================================================\n";
        cout << "                [1] Show Client Lists.\n";
        cout << "                [2] Add New Client.\n";
        cout << "                [3] Delete Client.\n";
        cout << "                [4] Update Client Info.\n";
        cout << "                [5] Find Client.\n";
        cout << "                [6] Transaction .\n";
        cout << "                [7] Exit .\n";
        cout << "=======================================================\n";
        cout << "Choose what do you want to do ? [1 to 7]? ";
        cin >> ChoiceNumber;

        enMainMenueOptions MainMenueOption = (enMainMenueOptions)ChoiceNumber;
        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        sClient Client;

        switch (MainMenueOption)
        {
        case enMainMenueOptions::eListClients:
            system("cls");
            PrintAllClientsData(vClients);
            GoBackToMainMenue();
            break;

        case enMainMenueOptions::eAddNewClient:
            system("cls");
            cout << "\n-----------------------------------\n";
            cout << "\t Add New Client   \n";
            cout << "-----------------------------------\n";
            AddClients();
            GoBackToMainMenue();
            break;

        case enMainMenueOptions::eDeleteClient:
            system("cls");
            cout << "\n-----------------------------------\n";
            cout << "\t Delete Client   \n";
            cout << "-----------------------------------\n";
            DeleteClientByAccountNumber(ReadClientAccountNumber(), vClients);
            GoBackToMainMenue();
            break;

        case enMainMenueOptions::eUpdateClient:
            system("cls");
            cout << "\n-----------------------------------\n";
            cout << "\t Update Client Info \n";
            cout << "-----------------------------------\n";
            UpdateClientByAccountNumber(ReadClientAccountNumber(), vClients);
            GoBackToMainMenue();
            break;

        case enMainMenueOptions::eFindClient:
            system("cls");
            cout << "\n-----------------------------------\n";
            cout << "\t Find Client \n";
            cout << "-----------------------------------\n";
            if (FindClientByAccountNumber(ReadClientAccountNumber(), vClients, Client))
            {
                PrintClientCard(Client);
            }
            else
            {
                cout << "\nClient is not found !!\n";
            }
            GoBackToMainMenue();
            break;

        case enMainMenueOptions::eTransaction:
            system("cls");
            Transaction();
            break;

        case enMainMenueOptions::eExit:
            system("cls");
            cout << "\nProgram Ends :-)\n";
            break;
        }

    } while (ChoiceNumber != enMainMenueOptions::eExit);
}

int main()
{
    MainScreen();
    system("pause>0");
    return 0;
}