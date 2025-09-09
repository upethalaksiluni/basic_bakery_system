#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <map>
#include <sstream>

using namespace std;

class Product;
class Order;
class Customer;
class Admin;

class Product {
private:
    static int nextID;
public:
    int productID;
    string name;
    string category;
    double price;
    int stock;
    
    Product(string n, string cat, double p, int s) 
        : productID(nextID++), name(n), category(cat), price(p), stock(s) {}
    
    void displayProduct() const {
        cout << left << setw(5) << productID 
             << setw(20) << name 
             << setw(15) << category
             << setw(10) << fixed << setprecision(2) << price
             << setw(10) << stock << endl;
    }
    
    bool updateStock(int quantity) {
        if (stock >= quantity) {
            stock -= quantity;
            return true;
        }
        return false;
    }
    
    void addStock(int quantity) {
        stock += quantity;
    }
    
    bool isLowStock(int threshold = 5) const {
        return stock <= threshold;
    }
};

int Product::nextID = 1001;

class OrderItem {
public:
    Product* product;
    int quantity;
    double itemTotal;
    
    OrderItem(Product* p, int q) : product(p), quantity(q) {
        itemTotal = p->price * q;
    }
    
    void displayOrderItem() const {
        cout << left << setw(20) << product->name
             << setw(10) << quantity
             << setw(10) << fixed << setprecision(2) << product->price
             << setw(10) << itemTotal << endl;
    }
};

class Order {
private:
    static int nextOrderID;
public:
    int orderID;
    vector<OrderItem> items;
    double subtotal;
    double tax;
    double discount;
    double total;
    string timestamp;
    string customerName;
    
    Order(string custName = "Guest") : orderID(nextOrderID++), customerName(custName) {
        subtotal = tax = discount = total = 0.0;
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back();
    }
    
    void addItem(Product* product, int quantity) {
        for (auto& item : items) {
            if (item.product->productID == product->productID) {
                item.quantity += quantity;
                item.itemTotal = item.product->price * item.quantity;
                calculateTotal();
                return;
            }
        }
        items.push_back(OrderItem(product, quantity));
        calculateTotal();
    }
    
    bool removeItem(int productID) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->product->productID == productID) {
                items.erase(it);
                calculateTotal();
                return true;
            }
        }
        return false;
    }
    
    void calculateTotal(double taxRate = 0.05, double discountAmount = 0.0) {
        subtotal = 0.0;
        for (const auto& item : items) {
            subtotal += item.itemTotal;
        }
        tax = subtotal * taxRate;
        discount = discountAmount;
        total = subtotal + tax - discount;
    }
    
    void displayOrder() const {
        cout << "\n========== ORDER SUMMARY ==========\n";
        cout << "Order ID: " << orderID << endl;
        cout << "Customer: " << customerName << endl;
        cout << "Date: " << timestamp << endl;
        cout << "-----------------------------------\n";
        cout << left << setw(20) << "Item" 
             << setw(10) << "Qty" 
             << setw(10) << "Price" 
             << setw(10) << "Total" << endl;
        cout << "-----------------------------------\n";
        
        for (const auto& item : items) {
            item.displayOrderItem();
        }
        
        cout << "-----------------------------------\n";
        cout << right << setw(30) << "Subtotal: $" << fixed << setprecision(2) << subtotal << endl;
        cout << right << setw(30) << "Tax: $" << tax << endl;
        cout << right << setw(30) << "Discount: $" << discount << endl;
        cout << right << setw(30) << "TOTAL: $" << total << endl;
        cout << "===================================\n";
    }
    
    void printReceipt() const {
        cout << "\n========== RECEIPT ==========\n";
        cout << "Sweet Delights Bakery\n";
        cout << "123 Baker Street\n";
        cout << "Phone: (555) 123-CAKE\n";
        cout << "-----------------------------\n";
        displayOrder();
        cout << "\nThank you for your purchase!\n";
        cout << "Have a sweet day!\n";
        cout << "=============================\n";
    }
};

int Order::nextOrderID = 5001;

class Customer {
public:
    string name;
    string phone;
    vector<Order> orderHistory;
    int loyaltyPoints;
    
    Customer(string n, string p) : name(n), phone(p), loyaltyPoints(0) {}
    
    void addOrder(const Order& order) {
        orderHistory.push_back(order);
        loyaltyPoints += static_cast<int>(order.total / 10);
    }
    
    void displayCustomerInfo() const {
        cout << "\nCustomer: " << name << endl;
        cout << "Phone: " << phone << endl;
        cout << "Loyalty Points: " << loyaltyPoints << endl;
        cout << "Total Orders: " << orderHistory.size() << endl;
    }
    
    void displayOrderHistory() const {
        cout << "\n========== ORDER HISTORY ==========\n";
        for (const auto& order : orderHistory) {
            cout << "Order #" << order.orderID << " - " << order.timestamp 
                 << " - Total: $" << fixed << setprecision(2) << order.total << endl;
        }
        cout << "===================================\n";
    }
};

class Inventory {
private:
    vector<Product> products;
    
public:
    void addProduct(const Product& product) {
        products.push_back(product);
    }
    
    void removeProduct(int productID) {
        products.erase(
            remove_if(products.begin(), products.end(),
                [productID](const Product& p) { return p.productID == productID; }),
            products.end()
        );
    }
    
    Product* findProduct(int productID) {
        for (auto& product : products) {
            if (product.productID == productID) {
                return &product;
            }
        }
        return nullptr;
    }
    
    void displayAllProducts() const {
        cout << "\n========== BAKERY MENU ==========\n";
        cout << left << setw(5) << "ID" 
             << setw(20) << "Name" 
             << setw(15) << "Category"
             << setw(10) << "Price"
             << setw(10) << "Stock" << endl;
        cout << "------------------------------------------------\n";
        
        for (const auto& product : products) {
            product.displayProduct();
        }
        cout << "================================\n";
    }
    
    void displayByCategory(const string& category) const {
        cout << "\n========== " << category << " ==========\n";
        cout << left << setw(5) << "ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price"
             << setw(10) << "Stock" << endl;
        cout << "-----------------------------------\n";
        
        for (const auto& product : products) {
            if (product.category == category) {
                product.displayProduct();
            }
        }
        cout << "===========================\n";
    }
    
    void checkLowStock() const {
        cout << "\n========== LOW STOCK ALERT ==========\n";
        bool found = false;
        for (const auto& product : products) {
            if (product.isLowStock()) {
                product.displayProduct();
                found = true;
            }
        }
        if (!found) {
            cout << "All products are well stocked!\n";
        }
        cout << "=====================================\n";
    }
    
    vector<Product>& getProducts() { return products; }
};

class SalesReport {
private:
    vector<Order> allOrders;
    
public:
    void addOrder(const Order& order) {
        allOrders.push_back(order);
    }
    
    void displayDailySales() const {
        double totalSales = 0.0;
        int totalOrders = 0;
        
        cout << "\n========== DAILY SALES REPORT ==========\n";
        for (const auto& order : allOrders) {
            cout << "Order #" << order.orderID << " - " 
                 << order.customerName << " - $" 
                 << fixed << setprecision(2) << order.total << endl;
            totalSales += order.total;
            totalOrders++;
        }
        cout << "----------------------------------------\n";
        cout << "Total Orders: " << totalOrders << endl;
        cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
        cout << "Average Order Value: $" << (totalOrders > 0 ? totalSales/totalOrders : 0) << endl;
        cout << "=======================================\n";
    }
    
    void displayMostSoldItems() const {
        map<string, int> itemSales;
        
        for (const auto& order : allOrders) {
            for (const auto& item : order.items) {
                itemSales[item.product->name] += item.quantity;
            }
        }
        
        cout << "\n========== MOST SOLD ITEMS ==========\n";
        vector<pair<string, int>> sortedItems(itemSales.begin(), itemSales.end());
        sort(sortedItems.begin(), sortedItems.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });
        
        for (const auto& item : sortedItems) {
            cout << left << setw(30) << item.first << item.second << " units" << endl;
        }
        cout << "====================================\n";
    }
};

class BakerySystem {
private:
    Inventory inventory;
    SalesReport salesReport;
    vector<Customer> customers;
    Order currentOrder;
    bool isAdminMode;
    
public:
    BakerySystem() : isAdminMode(false), currentOrder("Guest") {
        initializeProducts();
    }
    
    void initializeProducts() {
        inventory.addProduct(Product("Chocolate Cake", "Cakes", 25.99, 10));
        inventory.addProduct(Product("Vanilla Cupcake", "Cakes", 3.50, 24));
        inventory.addProduct(Product("Croissant", "Pastries", 2.75, 15));
        inventory.addProduct(Product("Apple Pie", "Pastries", 12.99, 8));
        inventory.addProduct(Product("Whole Wheat Bread", "Bread", 4.50, 12));
        inventory.addProduct(Product("Sourdough", "Bread", 5.99, 6));
        inventory.addProduct(Product("Chocolate Chip Cookie", "Cookies", 1.99, 30));
        inventory.addProduct(Product("Coffee", "Drinks", 2.99, 20));
        inventory.addProduct(Product("Hot Chocolate", "Drinks", 3.49, 15));
    }
    
    void displayMainMenu() {
        cout << "\n========== SWEET DELIGHTS BAKERY ==========\n";
        cout << "1. Customer Mode\n";
        cout << "2. Admin Mode\n";
        cout << "3. Exit\n";
        cout << "==========================================\n";
        cout << "Select option: ";
    }
    
    void displayCustomerMenu() {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "1. Browse Menu\n";
        cout << "2. Browse by Category\n";
        cout << "3. Add Item to Cart\n";
        cout << "4. Remove Item from Cart\n";
        cout << "5. View Current Order\n";
        cout << "6. Checkout & Pay\n";
        cout << "7. Register as Customer\n";
        cout << "8. Back to Main Menu\n";
        cout << "==================================\n";
        cout << "Select option: ";
    }
    
    void displayAdminMenu() {
        cout << "\n========== ADMIN MENU ==========\n";
        cout << "1. Add Product\n";
        cout << "2. Remove Product\n";
        cout << "3. Update Product Stock\n";
        cout << "4. Update Product Price\n";
        cout << "5. View All Products\n";
        cout << "6. Check Low Stock\n";
        cout << "7. View Sales Report\n";
        cout << "8. View Most Sold Items\n";
        cout << "9. Back to Main Menu\n";
        cout << "==============================\n";
        cout << "Select option: ";
    }
    
    void customerMode() {
        int choice;
        while (true) {
            displayCustomerMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    inventory.displayAllProducts();
                    break;
                case 2:
                    browseByCategory();
                    break;
                case 3:
                    addItemToCart();
                    break;
                case 4:
                    removeItemFromCart();
                    break;
                case 5:
                    currentOrder.displayOrder();
                    break;
                case 6:
                    checkout();
                    break;
                case 7:
                    registerCustomer();
                    break;
                case 8:
                    return;
                default:
                    cout << "Invalid option! Please try again.\n";
            }
        }
    }
    
    void browseByCategory() {
        cout << "\nAvailable Categories:\n";
        cout << "1. Cakes\n2. Pastries\n3. Bread\n4. Cookies\n5. Drinks\n";
        cout << "Select category (1-5): ";
        
        int choice;
        cin >> choice;
        
        string categories[] = {"", "Cakes", "Pastries", "Bread", "Cookies", "Drinks"};
        if (choice >= 1 && choice <= 5) {
            inventory.displayByCategory(categories[choice]);
        } else {
            cout << "Invalid category selection!\n";
        }
    }
    
    void addItemToCart() {
        inventory.displayAllProducts();
        
        int productID, quantity;
        cout << "Enter Product ID: ";
        cin >> productID;
        cout << "Enter Quantity: ";
        cin >> quantity;
        
        Product* product = inventory.findProduct(productID);
        if (product) {
            if (product->stock >= quantity) {
                currentOrder.addItem(product, quantity);
                cout << "Item added to cart successfully!\n";
            } else {
                cout << "Insufficient stock! Available: " << product->stock << endl;
            }
        } else {
            cout << "Product not found!\n";
        }
    }
    
    void removeItemFromCart() {
        if (currentOrder.items.empty()) {
            cout << "Cart is empty!\n";
            return;
        }
        
        currentOrder.displayOrder();
        cout << "Enter Product ID to remove: ";
        int productID;
        cin >> productID;
        
        if (currentOrder.removeItem(productID)) {
            cout << "Item removed successfully!\n";
        } else {
            cout << "Item not found in cart!\n";
        }
    }
    
    void checkout() {
        if (currentOrder.items.empty()) {
            cout << "Cart is empty! Add items before checkout.\n";
            return;
        }
        
        double discount = 0.0;
        cout << "Apply discount? (y/n): ";
        char applyDiscount;
        cin >> applyDiscount;
        
        if (applyDiscount == 'y' || applyDiscount == 'Y') {
            cout << "Enter discount amount: $";
            cin >> discount;
        }
        
        currentOrder.calculateTotal(0.05, discount);
        currentOrder.displayOrder();
        
        cout << "\nConfirm order? (y/n): ";
        char confirm;
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            for (const auto& item : currentOrder.items) {
                item.product->updateStock(item.quantity);
            }
            
            salesReport.addOrder(currentOrder);
            
            currentOrder.printReceipt();
            
            currentOrder = Order("Guest");
            cout << "\nOrder completed successfully!\n";
        } else {
            cout << "Order cancelled!\n";
        }
    }
    
    void registerCustomer() {
        string name, phone;
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter phone number: ";
        getline(cin, phone);
        
        customers.push_back(Customer(name, phone));
        currentOrder.customerName = name;
        
        cout << "Customer registered successfully!\n";
    }
    
    void adminMode() {
        string password;
        cout << "Enter admin password: ";
        cin >> password;
        
        if (password != "admin123") {
            cout << "Incorrect password!\n";
            return;
        }
        
        int choice;
        while (true) {
            displayAdminMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    addProduct();
                    break;
                case 2:
                    removeProduct();
                    break;
                case 3:
                    updateStock();
                    break;
                case 4:
                    updatePrice();
                    break;
                case 5:
                    inventory.displayAllProducts();
                    break;
                case 6:
                    inventory.checkLowStock();
                    break;
                case 7:
                    salesReport.displayDailySales();
                    break;
                case 8:
                    salesReport.displayMostSoldItems();
                    break;
                case 9:
                    return;
                default:
                    cout << "Invalid option! Please try again.\n";
            }
        }
    }
    
    void addProduct() {
        string name, category;
        double price;
        int stock;
        
        cout << "Enter product name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter category: ";
        getline(cin, category);
        cout << "Enter price: $";
        cin >> price;
        cout << "Enter initial stock: ";
        cin >> stock;
        
        inventory.addProduct(Product(name, category, price, stock));
        cout << "Product added successfully!\n";
    }
    
    void removeProduct() {
        inventory.displayAllProducts();
        cout << "Enter Product ID to remove: ";
        int productID;
        cin >> productID;
        
        inventory.removeProduct(productID);
        cout << "Product removed successfully!\n";
    }
    
    void updateStock() {
        inventory.displayAllProducts();
        cout << "Enter Product ID: ";
        int productID;
        cin >> productID;
        
        Product* product = inventory.findProduct(productID);
        if (product) {
            cout << "Current stock: " << product->stock << endl;
            cout << "Enter quantity to add: ";
            int quantity;
            cin >> quantity;
            product->addStock(quantity);
            cout << "Stock updated successfully! New stock: " << product->stock << endl;
        } else {
            cout << "Product not found!\n";
        }
    }
    
    void updatePrice() {
        inventory.displayAllProducts();
        cout << "Enter Product ID: ";
        int productID;
        cin >> productID;
        
        Product* product = inventory.findProduct(productID);
        if (product) {
            cout << "Current price: $" << product->price << endl;
            cout << "Enter new price: $";
            cin >> product->price;
            cout << "Price updated successfully!\n";
        } else {
            cout << "Product not found!\n";
        }
    }
    
    void run() {
        int choice;
        cout << "Welcome to Sweet Delights Bakery Management System!\n";
        
        while (true) {
            displayMainMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    customerMode();
                    break;
                case 2:
                    adminMode();
                    break;
                case 3:
                    cout << "Thank you for using Sweet Delights Bakery System!\n";
                    return;
                default:
                    cout << "Invalid option! Please try again.\n";
            }
        }
    }
};

int main() {
    BakerySystem bakery;
    bakery.run();
    return 0;
}