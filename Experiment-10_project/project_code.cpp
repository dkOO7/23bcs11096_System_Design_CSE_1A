#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
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
        cout << "Order Published -> ID: " << order.orderId
             << " | Customer: " << order.customerName
             << " | Item: " << order.item << endl;
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
};

int main() {
    KafkaQueue kafkaTopic;
    Dashboard dashboard;

    kafkaTopic.publishOrder(Order(101, "Dikshay", "Burger", 199));
    kafkaTopic.publishOrder(Order(102, "Arjun", "Pizza", 349));
    kafkaTopic.publishOrder(Order(103, "Rahul", "Pasta", 249));
    kafkaTopic.publishOrder(Order(104, "Priya", "Biryani", 299));

    while (kafkaTopic.hasOrders()) {
        Order currentOrder = kafkaTopic.consumeOrder();

        cout << "\nNew Order Received:\n";
        cout << "Order ID: " << currentOrder.orderId
             << " | Customer: " << currentOrder.customerName
             << " | Item: " << currentOrder.item
             << " | Amount: " << currentOrder.amount << endl;

        int choice_Dikshay;
        cout << "Enter 1 to Accept, 0 to Reject: ";
        cin >> choice_Dikshay;

        if (choice_Dikshay == 1) {
            dashboard.acceptOrder(currentOrder);
            cout << "Order Accepted Successfully!\n";
        } else {
            dashboard.rejectOrder(currentOrder);
            cout << "Order Rejected Successfully!\n";
        }
    }

    dashboard.showAcceptedOrders();
    dashboard.showRejectedOrders();

    return 0;
}
