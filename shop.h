#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

enum class OrderStatus {
    Pending,
    Completed,
    Cancelled
};

string statusToString(OrderStatus status);

enum class PaymentMethod {
    None,
    CashOnDelivery,
    CardOnline
};

string paymentToString(PaymentMethod method);

class Product {
public:
    string name;
    double price;
    int quantity;

    Product(string n, double p, int q = 0);
};

class Order {
private:
    vector<Product> products;
    string date;
    OrderStatus status;
    double discount;

    PaymentMethod paymentMethod;
    bool isPaid;

public:
    Order(string d);

    void addProduct(const Product& p);
    void removeProduct(const string& name);
    void updateQuantity(const string& name, int qty);
    void setDiscount(double d);

    double getTotal() const;
    double getTotalWithDiscount() const;

    void completeOrder();

    void setPayment(PaymentMethod method);

    void printReceipt() const;
    void saveReceipt(const string& filename);
};
