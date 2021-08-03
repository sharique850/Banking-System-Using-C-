#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
using namespace std;
#define MIN_BALANCE 500

class InsufficientFunds{};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;
public:
    Account(){}
    Account(string fname,string lname,float balance);
    long getAccNo(){return accountNumber;}
    string getFirstName(){return firstName;}
    string getLastName(){return lastName;}
    float getBalance(){return balance;}
    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);
};

long Account::NextAccountNumber=0;

class Bank
{
private:
    map<long,Account> accounts;
public:
    Bank();
    Account OpenAccount(string fname,string lname,float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber,float amount);
    Account Withdraw(long accountNumber,float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};

int main()
{
    Bank b;
    Account acc;
    int choice;
    string fname,lname;

    long accountNumber;
    float balance;
    float amount;
    cout<<"\t\t\t\t\t***Banking System***\n"<<endl;
    cout<<"\t\tWelcome to the Codo Bank where you will get all banking facilities..."<<endl;
    do
    {
        cout<<"\n\t\tSelect one option below ";
        cout<<"\n\t\t1 Open an Account";
        cout<<"\n\t\t2 Balance Enquiry";
        cout<<"\n\t\t3 Deposit";
        cout<<"\n\t\t4 Withdrawal";
        cout<<"\n\t\t5 Close an Account";
        cout<<"\n\t\t6 Show All Accounts";
        cout<<"\n\t\t7 Quit";
        cout<<"\n\n\t\tEnter your choice: ";
        cin>>choice;
        switch(choice)
        {
            case 1:
                cout<<"\t\tEnter First Name: ";
                cin>>fname;
                cout<<"\t\tEnter Last Name: ";
                cin>>lname;
                cout<<"\t\tEnter Initil Balance: ";
                cin>>balance;
                acc=b.OpenAccount(fname,lname,balance);
                cout<<endl<<"\t\tCongratulation Account is Created"<<endl;
                cout<<acc;
                break;

            case 2:
                cout<<"\t\tEnter Account Number:";
                cin>>accountNumber;
                acc=b.BalanceEnquiry(accountNumber);
                cout<<endl<<"\t\tYour Account Details"<<endl;
                cout<<acc;
                break;

            case 3:
                cout<<"\t\tEnter Account Number:";
                cin>>accountNumber;
                cout<<"\t\tEnter Balance:";
                cin>>amount;
                acc=b.Deposit(accountNumber, amount);
                cout<<endl<<"\t\tAmount is Deposited"<<endl;
                cout<<acc;
                break;

            case 4:
                cout<<"\t\tEnter Account Number:";
                cin>>accountNumber;
                cout<<"\t\tEnter Balance:";
                cin>>amount;
                acc=b.Withdraw(accountNumber, amount);
                cout<<endl<<"\t\tAmount Withdrawn"<<endl;
                cout<<acc;
                break;

            case 5:
                cout<<"\t\tEnter Account Number:";
                cin>>accountNumber;
                b.CloseAccount(accountNumber);
                cout<<endl<<"\t\tAccount is Closed"<<endl;
                cout<<acc;

            case 6:
                b.ShowAllAccounts();
                break;

            case 7: break;
            
            default:
                cout<<"\n\t\tEnter corret choice";
                exit(0);
        }
    }while(choice!=7);
return 0;
}

Account::Account(string fname,string lname,float balance)
{
    NextAccountNumber++;
    accountNumber=NextAccountNumber;
    firstName=fname;
    lastName=lname;
    this->balance=balance;
}
void Account::Deposit(float amount)
{
    balance+=amount;
}
void Account::Withdraw(float amount)
{
    if(balance-amount<MIN_BALANCE)
    throw InsufficientFunds();
    balance-=amount;
}
void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber=accountNumber;
}
long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}
ofstream & operator<<(ofstream &ofs,Account &acc)
{
    ofs<<acc.accountNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastName<<endl;
    ofs<<acc.balance<<endl;
    return ofs;
}
ifstream & operator>>(ifstream &ifs,Account &acc)
{
    ifs>>acc.accountNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastName;
    ifs>>acc.balance;
    return ifs;
}
ostream & operator<<(ostream &os,Account &acc)
{
    os<<"\t\tFirst Name:"<<acc.getFirstName()<<endl;
    os<<"\t\tLast Name:"<<acc.getLastName()<<endl;
    os<<"\t\tAccount Number:"<<acc.getAccNo()<<endl;
    os<<"\t\tBalance:"<<acc.getBalance()<<endl;
    return os;
}
Bank::Bank()
{
    Account account;
    ifstream infile;

    infile.open("Bank.data");
    if(!infile)
    {
        //cout<<"Error in Opening! File Not Found!!"<<endl;
        return;
        }
    while(!infile.eof())
    {
        infile>>account;
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }
Account::setLastAccountNumber(account.getAccNo());
infile.close();
}

Account Bank::OpenAccount(string fname,string lname,float balance)
{
    ofstream outfile;
    Account account(fname,lname,balance);
    accounts.insert(pair<long,Account>(account.getAccNo(),account));
    outfile.open("Bank.data", ios::trunc);
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    return itr->second;
}

Account Bank::Deposit(long accountNumber,float amount)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdraw(long accountNumber,float amount)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    cout<<"\t\tAccount Deleted"<<itr->second;
    accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        cout<<"\t\tAccount "<<itr->first<<endl<<itr->second<<endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
    outfile<<itr->second;
    }
    outfile.close();
}