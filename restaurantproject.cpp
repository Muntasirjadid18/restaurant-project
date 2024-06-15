#include <iostream>
#include <string>

using namespace std;

class Dish {
public:
    string name;
    double price;
    Dish* left;
    Dish* right;

    Dish(const string& n, double p) : name(n), price(p), left(nullptr), right(nullptr) {}
};

struct Order {
    string name;
    int quantity;
    double price;
};

class Restaurant {
private:
    Dish* menuRoot; // Root of the BST for menu items
    Order orders[100]; // Array to store orders
    int orderSize; // Current number of orders

    Dish* insertDish(Dish* root, const string& name, double price) {
        if (root == nullptr) {
            return new Dish(name, price);
        }
        if (name < root->name) {
            root->left = insertDish(root->left, name, price);
        } else if (name > root->name) {
            root->right = insertDish(root->right, name, price);
        }
        return root;
    }

    void displayMenu(Dish* root) const {
        if (root == nullptr) {
            return;
        }
        displayMenu(root->left);
        cout << root->name << " - $" << root->price << endl;
        displayMenu(root->right);
    }

    Dish* searchDish(Dish* root, const string& name) const {
        if (root == nullptr || root->name == name) {
            return root;
        }
        if (name < root->name) {
            return searchDish(root->left, name);
        }
        return searchDish(root->right, name);
    }

    void deleteTree(Dish* root) {
        if (root == nullptr) {
            return;
        }
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }

public:
    Restaurant() : menuRoot(nullptr), orderSize(0) {}

    ~Restaurant() {
        deleteTree(menuRoot);
    }

    void addDish(const string& name, double price) {
        menuRoot = insertDish(menuRoot, name, price);
    }

    void displayMenu() const {
        if (menuRoot == nullptr) {
            cout << "The menu is empty.\n";
            return;
        }
        cout << "\n\n\n------------------ |||Restaurant Menu||| ------------------\n";
        displayMenu(menuRoot);
        cout << "-----------------------------------------------------\n";
    }

    void takeOrder() {
        if (menuRoot == nullptr) {
            cout << "The menu is empty. No dishes to order.\n";
            return;
        }
        string dishName;
        cout << "Enter the dish name you want to order: ";
        cin.ignore();
        getline(cin, dishName);

        Dish* dish = searchDish(menuRoot, dishName);
        if (dish == nullptr) {
            cout << "Dish not found. Please try again.\n";
            return;
        }

        int orderQuantity;
        cout << "Enter the quantity you want to order: ";
        cin >> orderQuantity;

        bool found = false;
        for (int i = 0; i < orderSize; ++i) {
            if (orders[i].name == dishName) {
                orders[i].quantity += orderQuantity;
                found = true;
                break;
            }
        }
        if (!found && orderSize < 100) {
            orders[orderSize++] = {dishName, orderQuantity, dish->price};
        }

        cout << "Added " << orderQuantity << " of " << dishName << " to your order.\n";
    }

    void generateBill() {
        if (orderSize == 0) {
            cout << "No orders placed.\n";
            return;
        }
        cout << "\n\n\n------------------ |Your Bill| ------------------\n";
        double total = 0;
        for (int i = 0; i < orderSize; ++i) {
            double dishTotal = orders[i].price * orders[i].quantity;
            total += dishTotal;
            cout << orders[i].name << " - " << orders[i].quantity << " x $" << orders[i].price << " = $" << dishTotal << "\n";
        }
        cout << "Total: $" << total << "\n";
        cout << "-----------------------------------------------\n";
        orderSize = 0; // Clear orders after billing
    }
};

int main() {
    Restaurant restaurant;
    restaurant.addDish("Burger", 8.99);
    restaurant.addDish("Pasta", 12.50);
    restaurant.addDish("Pizza", 10.99);
    restaurant.addDish("Salad", 5.99);
    restaurant.addDish("Sandwich", 7.99);
    restaurant.addDish("Soup", 15.99);
    restaurant.addDish("Steak", 25.00);

    while (true) {
        restaurant.displayMenu(); // Display menu always
        cout << "\n\n\n------------------ ||Restaurant|| ------------------\n";
        cout << "1. Take Order\n";
        cout << "2. Generate Bill\n";
        cout << "3. Exit\n";
        cout << "-----------------------------------------------\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                restaurant.takeOrder();
                break;
            case 2:
                restaurant.generateBill();
                break;
            case 3:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
