#include <bits/stdc++.h>
using namespace std;


//  srp class to create account
//  user -> account

//  bal = 0;
//  user id


class Account {
public:
    int id;
    int balance;

    Account(int id) {
        this->id = id;
        balance = 0;
    }
};

class Transactions {
public:
    virtual void debit(int amount) = 0;
    virtual void credit(int amount) = 0;
    virtual void get_balance() = 0;
};

class CriticalTransactions : public Transactions {
private:
    Account* A1;
public:
    CriticalTransactions(Account* a) {
        A1 = a;
    }

    void debit(int amount) override {
        if (A1->balance - amount < 0) {
            cout << "Insufficient balance\n";
            return;
        }
        A1->balance -= amount;
    }

    void credit(int amount) override {
        A1->balance += amount;
    }

    void get_balance() override {
        cout << "Balance: " << A1->balance << endl;
    }
};

int main() {
    int id;
    cout << "Enter Account ID: ";
    cin >> id;

    Account* curr_acc = new Account(id);
    CriticalTransactions c1(curr_acc);
    int choice;

    while (true) {
        cout << "\n Dashboard \n";
        cout << "1: Credit\n2: Debit\n3: View Balance\n4: Exit\n";
        cin >> choice;

        if (choice == 4) break;

        int amount;

        switch (choice) {
            case 1:
                cout << "Enter amount: ";
                cin >> amount;
                c1.credit(amount);
                break;

            case 2:
                cout << "Enter amount: ";
                cin >> amount;
                c1.debit(amount);
                break;

            case 3:
                c1.get_balance();
                break;

            default:
                cout << "Invalid choice\n";
        }
    }

    return 0;
}
