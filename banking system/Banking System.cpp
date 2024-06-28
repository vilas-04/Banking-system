#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

// Transaction class
class Transaction {
private:
    int transactionId;
    int accountId;
    double amount;
    string type; // deposit, withdrawal, transfer
    time_t timestamp;

public:
    Transaction(int transId, int accId, double amt, string typ)
        : transactionId(transId), accountId(accId), amount(amt), type(typ) {
        timestamp = time(0); // current timestamp
    }

    void display() const {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Account ID: " << accountId << endl;
        cout << "Amount: " << amount << endl;
        cout << "Type: " << type << endl;
        cout << "Timestamp: " << asctime(localtime(&timestamp));
    }
};

// Account class
class Account {
private:
    int accountId;
    int customerId;
    double balance;
    vector<Transaction> transactions;

public:
    Account(int accId, int custId, double initialBalance)
        : accountId(accId), customerId(custId), balance(initialBalance) {}

    void deposit(double amount) {
        balance += amount;
        Transaction trans(transactions.size() + 1, accountId, amount, "deposit");
        transactions.push_back(trans);
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            Transaction trans(transactions.size() + 1, accountId, amount, "withdrawal");
            transactions.push_back(trans);
            return true;
        }
        return false;
    }

    void transfer(double amount, Account& targetAccount) {
        if (withdraw(amount)) {
            targetAccount.deposit(amount);
            Transaction trans(transactions.size() + 1, accountId, amount, "transfer");
            transactions.push_back(trans);
        }
    }

    double getBalance() const {
        return balance;
    }

    void displayTransactions() const {
        for (size_t i = 0; i < transactions.size(); ++i) {
            transactions[i].display();
        }
    }

    int getAccountId() const {
        return accountId;
    }
};

// BankingSystem class
class BankingSystem {
private:
    vector<Account> accounts;
    int nextAccountId;

public:
    BankingSystem() : nextAccountId(1) {}

    void addAccount(int custId, double initialBalance) {
        Account acc(nextAccountId++, custId, initialBalance);
        accounts.push_back(acc);
    }

    bool deposit(int accId, double amount) {
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i].getAccountId() == accId) {
                accounts[i].deposit(amount);
                return true;
            }
        }
        return false;
    }

    bool withdraw(int accId, double amount) {
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i].getAccountId() == accId) {
                return accounts[i].withdraw(amount);
            }
        }
        return false;
    }

    void transfer(int fromAccId, int toAccId, double amount) {
        Account* fromAcc = 0; // or NULL
        Account* toAcc = 0;   // or NULL

        // Find both accounts
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i].getAccountId() == fromAccId) {
                fromAcc = &accounts[i];
            }
            if (accounts[i].getAccountId() == toAccId) {
                toAcc = &accounts[i];
            }
        }

        // Perform transfer if both accounts are found
        if (fromAcc && toAcc) {
            fromAcc->transfer(amount, *toAcc);
        }
    }

    void displayAccountDetails(int accId) {
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i].getAccountId() == accId) {
                cout << "Account ID: " << accId << endl;
                cout << "Balance: " << accounts[i].getBalance() << endl;
                cout << "Transaction History:" << endl;
                accounts[i].displayTransactions();
                return;
            }
        }
        cout << "Account not found." << endl;
    }
};

int main() {
    // Example usage
    BankingSystem bank;

    // Add accounts
    bank.addAccount(1, 1000);
    bank.addAccount(2, 500);

    // Perform transactions
    bank.deposit(1, 500);
    bank.withdraw(2, 200);
    bank.transfer(1, 2, 300);

    // Display account details
    cout << "Account 1 details:" << endl;
    bank.displayAccountDetails(1);

    cout << "\nAccount 2 details:" << endl;
    bank.displayAccountDetails(2);

    return 0;
}

