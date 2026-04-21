#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

class Order {
public:
    int orderId;
    string customerName;
    string item;
    double amount;
    string status;

    Order(int id, string name, string food, double price) {
        orderId = id;
        customerName = name;
        item = food;
        amount = price;
        status = "PENDING";
    }
};

class KafkaQueue {
private:
    queue<Order> orderQueue;

public:
    void publishOrder(Order order) {
        orderQueue.push(order);
        cout << "\nOrder Published Successfully!\n";
        cout << "Order ID: " << order.orderId
             << " | Customer: " << order.customerName
             << " | Item: " << order.item
             << " | Amount: " << order.amount << endl;
    }

    bool hasOrders() {
        return !orderQueue.empty();
    }

    Order consumeOrder() {
        Order currentOrder = orderQueue.front();
        orderQueue.pop();
        return currentOrder;
    }
};

class Dashboard {
private:
    vector<Order> acceptedOrders;
    vector<Order> rejectedOrders;

public:
    void acceptOrder(Order order) {
        order.status = "ACCEPTED";
        acceptedOrders.push_back(order);
    }

    void rejectOrder(Order order) {
        order.status = "REJECTED";
        rejectedOrders.push_back(order);
    }

    void showAcceptedOrders() {
        cout << "\n===== Accepted Orders =====\n";

        if (acceptedOrders.empty()) {
            cout << "No accepted orders yet.\n";
            return;
        }

        for (auto &order : acceptedOrders) {
            cout << "Order ID: " << order.orderId
                 << " | Customer: " << order.customerName
                 << " | Item: " << order.item
                 << " | Amount: " << order.amount
                 << " | Status: " << order.status << endl;
        }
    }

    void showRejectedOrders() {
        cout << "\n===== Rejected Orders =====\n";

        if (rejectedOrders.empty()) {
            cout << "No rejected orders yet.\n";
            return;
        }

        for (auto &order : rejectedOrders) {
            cout << "Order ID: " << order.orderId
                 << " | Customer: " << order.customerName
                 << " | Item: " << order.item
                 << " | Amount: " << order.amount
                 << " | Status: " << order.status << endl;
        }
    }

    void showSummary() {
        cout << "\n===== Dashboard Summary =====\n";
        cout << "Total Accepted Orders: " << acceptedOrders.size() << endl;
        cout << "Total Rejected Orders: " << rejectedOrders.size() << endl;
    }
};

int main() {
    KafkaQueue kafkaTopic;
    Dashboard dashboard;

    int totalOrders;

    cout << "Enter total number of orders to publish: ";
    cin >> totalOrders;

    for (int i = 0; i < totalOrders; i++) {
        int id;
        string customerName;
        string item;
        double amount;

        cout << "\nEnter details for Order " << i + 1 << endl;

        cout << "Enter Order ID: ";
        cin >> id;

        cin.ignore();

        cout << "Enter Customer Name: ";
        getline(cin, customerName);

        cout << "Enter Food Item: ";
        getline(cin, item);

        cout << "Enter Amount: ";
        cin >> amount;

        kafkaTopic.publishOrder(Order(id, customerName, item, amount));
    }

    while (kafkaTopic.hasOrders()) {
        Order currentOrder = kafkaTopic.consumeOrder();

        cout << "\n====================================\n";
        cout << "New Order Received:\n";
        cout << "Order ID: " << currentOrder.orderId
             << " | Customer: " << currentOrder.customerName
             << " | Item: " << currentOrder.item
             << " | Amount: " << currentOrder.amount << endl;

        int choice;
        cout << "Enter 1 to Accept, 0 to Reject: ";
        cin >> choice;

        if (choice == 1) {
            dashboard.acceptOrder(currentOrder);
            cout << "Order Accepted Successfully!\n";
        } else {
            dashboard.rejectOrder(currentOrder);
            cout << "Order Rejected Successfully!\n";
        }
    }

    dashboard.showSummary();
    dashboard.showAcceptedOrders();
    dashboard.showRejectedOrders();

    return 0;
}
