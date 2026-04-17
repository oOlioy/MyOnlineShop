#include "shop.h"

string statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::Pending: return "Pending";
        case OrderStatus::Completed: return "Completed";
        case OrderStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

string paymentToString(PaymentMethod method) {
    switch (method) {
        case PaymentMethod::CashOnDelivery: return "Cash on delivery";
        case PaymentMethod::CardOnline: return "Card online";
        default: return "None";
    }
}

Product::Product(string n, double p, int q)
    : name(n), price(p), quantity(q) {}

Order::Order(string d)
    : date(d), status(OrderStatus::Pending), discount(0),
      paymentMethod(PaymentMethod::None), isPaid(false) {}

void Order::addProduct(const Product& p) {
    for (auto& prod : products) {
        if (prod.name == p.name) {
            prod.quantity += p.quantity;
            return;
        }
    }
    products.push_back(p);
}

void Order::removeProduct(const string& name) {
    products.erase(
        remove_if(products.begin(), products.end(),
            [&](const Product& p) { return p.name == name; }),
        products.end()
    );
}

void Order::updateQuantity(const string& name, int qty) {
    for (auto& p : products) {
        if (p.name == name) {
            p.quantity = qty;
            return;
        }
    }
}

void Order::setDiscount(double d) {
    discount = d;
}

double Order::getTotal() const {
    double sum = 0;
    for (const auto& p : products)
        sum += p.price * p.quantity;
    return sum;
}

double Order::getTotalWithDiscount() const {
    return getTotal() * (1 - discount / 100.0);
}

void Order::setPayment(PaymentMethod method) {
    paymentMethod = method;
    isPaid = (method == PaymentMethod::CardOnline);
}

void Order::completeOrder() {
    status = OrderStatus::Completed;
}

void Order::printReceipt() const {
    cout << "\n========== RECEIPT ==========\n";
    cout << "Date: " << date << "\n";
    cout << "Status: " << statusToString(status) << "\n";
    cout << "Payment: " << paymentToString(paymentMethod) << "\n";
    cout << "Paid: " << (isPaid ? "Yes" : "No") << "\n";
    cout << "-----------------------------\n";

    for (const auto& p : products) {
        cout << p.name << " | "
             << p.price << " x "
             << p.quantity << " = "
             << p.price * p.quantity << "\n";
    }

    cout << "-----------------------------\n";
    cout << "Subtotal: " << getTotal() << "\n";
    cout << "Discount: " << discount << "%\n";
    cout << "Total: " << getTotalWithDiscount() << "\n";
    cout << "=============================\n";
}

void Order::saveReceipt(const string& filename) {
    ofstream file(filename);

    file << "========== RECEIPT ==========\n";
    file << "Date: " << date << "\n";
    file << "Status: " << statusToString(status) << "\n";
    file << "Payment: " << paymentToString(paymentMethod) << "\n";
    file << "Paid: " << (isPaid ? "Yes" : "No") << "\n";
    file << "-----------------------------\n";

    for (const auto& p : products) {
        file << p.name << " | "
             << p.price << " x "
             << p.quantity << " = "
             << p.price * p.quantity << "\n";
    }

    file << "-----------------------------\n";
    file << "Subtotal: " << getTotal() << "\n";
    file << "Discount: " << discount << "%\n";
    file << "Total: " << getTotalWithDiscount() << "\n";
    file << "=============================\n";
}

vector<Product> loadItems(const string& filename) {
    vector<Product> items;
    ifstream file(filename);

    string name;
    double price;

    while (file >> name >> price) {
        items.emplace_back(name, price);
    }

    return items;
}

void showItems(const vector<Product>& items) {
    cout << "\n===== AVAILABLE PRODUCTS =====\n";
    for (const auto& p : items) {
        cout << p.name << " | Price: " << p.price << endl;
    }
    cout << "==============================\n";
}

Product findItem(const vector<Product>& items, const string& name) {
    for (const auto& p : items) {
        if (p.name == name)
            return p;
    }
    return Product("", 0);
}

void showMenu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Add products\n";
    cout << "2. Remove product\n";
    cout << "3. Update quantity\n";
    cout << "4. Set discount\n";
    cout << "5. Finish order\n";
    cout << "Choose: ";
}

// Виправлена допоміжна функція для безпечного читання int
int readInt() {
    int value;
    while (!(cin >> value)) {
        cin.clear();                // скидаємо стан помилки
        cin.ignore(1000, '\n');     // очищаємо буфер
        cout << "Invalid input. Try again: ";
    }
    cin.ignore(1000, '\n');         // очищаємо залишок рядка після числа
    return value;
}

int main() {
    vector<Product> items = loadItems("items.txt");
    showItems(items);

    Order order("2026-04-17");

    int choice;

    while (true) {
        showMenu();
        choice = readInt(); // <-- використовуємо безпечне читання

        if (choice == 1) {
            string name;
            int qty;

            while (true) {
                cout << "\nEnter product name (or 0 to stop): ";
                cin >> name;
                cin.ignore(1000, '\n'); // очищаємо буфер після назви

                if (name == "0") break;

                Product item = findItem(items, name);

                if (item.name == "") {
                    cout << "Product not found!\n";
                    continue;
                }

                cout << "Enter quantity: ";
                qty = readInt();

                item.quantity = qty;
                order.addProduct(item);
            }
        }
        else if (choice == 2) {
            string name;
            cout << "Enter product to remove: ";
            cin >> name;
            cin.ignore(1000, '\n');
            order.removeProduct(name);
        }
        else if (choice == 3) {
            string name;
            int qty;
            cout << "Enter product: ";
            cin >> name;
            cin.ignore(1000, '\n');
            cout << "Enter new quantity: ";
            qty = readInt();
            order.updateQuantity(name, qty);
        }
        else if (choice == 4) {
            double d;
            cout << "Enter discount: ";
            while (!(cin >> d)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Try again: ";
            }
            cin.ignore(1000, '\n');
            order.setDiscount(d);
        }
        else if (choice == 5) {
            int payChoice;

            cout << "\nSelect payment method:\n";
            cout << "1. Cash on delivery\n";
            cout << "2. Card online\n";
            cout << "Choose: ";
            payChoice = readInt();

            if (payChoice == 1)
                order.setPayment(PaymentMethod::CashOnDelivery);
            else
                order.setPayment(PaymentMethod::CardOnline);

            order.completeOrder();
            order.printReceipt();
            order.saveReceipt("receipt.txt");

            cout << "\nSaved to receipt.txt\n";
            break;
        }
        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
