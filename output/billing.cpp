#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <limits>
#include <string>
#include <algorithm> 
#include <unordered_map>
#include <vector>
#include <cctype>    

using namespace std;

class Bill {
private:
    string Item;
    int Rate, Quantity;
public:
    Bill() : Item(""), Rate(0), Quantity(0) { }
    Bill(string item, int rate, int quant) : Item(item), Rate(rate), Quantity(quant) {}

    string getItem() const { return Item; }
    int getRate() const { return Rate; }
    int getQuant() const { return Quantity; }
    
    // New methods to handle stock entirely in memory
    void reduceQuant(int amount) { Quantity -= amount; }
    void addQuant(int amount) { Quantity += amount; }
};

// Global Inventory Map - Replaces continuous disk reads
unordered_map<string, Bill> inventory;

// Converts a string to all lowercase letters for O(1) key lookups
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Helper function to safely get positive integers from the user
int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\tInvalid input. Please enter a positive number.\n";
        } else {
            return value;
        }
    }
}

// Loads inventory into RAM at startup O(N)
void loadInventory() {
    ifstream in("Inventory.txt");
    if (!in.is_open()) return; // If file doesn't exist yet, just return

    string line, itemName;
    int itemRate, itemQuant;
    char delimiter;

    while (getline(in, line)) {
        if (line.empty()) continue; 

        stringstream ss(line);
        ss >> ws;
        getline(ss, itemName, ':');
        
        // Trim trailing spaces from item name
        while (!itemName.empty() && isspace(itemName.back())) {
            itemName.pop_back();
        }

        ss >> itemRate >> delimiter >> itemQuant;
        
        // Store in hash map with lowercase key
        inventory[toLowerCase(itemName)] = Bill(itemName, itemRate, itemQuant);
    }
    in.close();
}

// Saves RAM state back to disk O(N)
void saveInventory() {
    ofstream out("Inventory.txt", ios::trunc); // Overwrite existing file
    for (const auto& pair : inventory) {
        out << "\t" << pair.second.getItem() << " : " << pair.second.getRate() << " : " << pair.second.getQuant() << endl;
    }
    out.close();
}

void addItem() {
    bool close = false;
    while (!close) {
        cout << "\n\t--- INVENTORY MANAGEMENT ---" << endl;
        cout << "\t1. Add New Item to Stock" << endl;
        cout << "\t2. Return to Main Menu" << endl;
        
        int choice = getValidInt("\tEnter Choice: ");

        if (choice == 1) {
            system("cls");
            string item;
            int rate, quant;

            cout << "\tEnter Item Name: ";
            getline(cin >> ws, item); 
            
            rate = getValidInt("\tEnter Rate Of Item: ");
            quant = getValidInt("\tEnter Quantity To Add: ");

            string key = toLowerCase(item);
            
            // O(1) Check: Does item already exist?
            if (inventory.find(key) != inventory.end()) {
                inventory[key].addQuant(quant); // Add to existing stock
            } else {
                inventory[key] = Bill(item, rate, quant); // Create new stock
            }

            saveInventory(); // Sync changes to disk

            cout << "\tSuccess! '" << item << "' added/updated in master inventory." << endl;
            Sleep(2000);
            system("cls");
            
        } else if (choice == 2) {
            system("cls");
            close = true;
        } else {
            cout << "\tInvalid choice. Please select 1 or 2." << endl;
            Sleep(1500);
            system("cls");
        }
    }
}

void printBill() {
    system("cls");
    int totalAmount = 0;
    bool close = false;
    
    // In-Memory Shopping Cart - Replaces Receipt.txt
    vector<string> cartReceipt; 

    while (!close) {
        cout << "\n\t--- SHOPPING CART ---" << endl;
        cout << "\t1. Buy Item" << endl;
        cout << "\t2. Checkout & Print Final Bill" << endl;
        
        int choice = getValidInt("\tEnter Choice: ");

        if (choice == 1) {
            string searchItem;
            cout << "\tEnter Item Name to Buy: ";
            getline(cin >> ws, searchItem);
            
            string key = toLowerCase(searchItem);
            
            // O(1) Fast Lookup in Hash Map
            if (inventory.find(key) != inventory.end()) {
                int available = inventory[key].getQuant();
                
                if (available == 0) {
                    cout << "\n\t[ERROR] '" << inventory[key].getItem() << "' is currently out of stock!" << endl;
                } else {
                    cout << "\n\t-> Item Found! Available Stock: " << available << endl;
                    int buyQuant = getValidInt("\t-> Enter Quantity to Buy: ");

                    if (buyQuant <= available) {
                        int amount = inventory[key].getRate() * buyQuant;
                        
                        cout << "\t[ADDED TO CART] " << buyQuant << "x " << inventory[key].getItem() << " - $" << amount << endl;
                        
                        inventory[key].reduceQuant(buyQuant); // Reduce RAM stock instantly
                        totalAmount += amount;

                        // Add receipt line to memory vector
                        cartReceipt.push_back("\t" + inventory[key].getItem() + " \t$" + to_string(inventory[key].getRate()) + " \tx" + to_string(buyQuant) + " \t$" + to_string(amount));
                    } else {
                        cout << "\t[ERROR] Insufficient stock! Transaction cancelled." << endl;
                    }
                }
            } else {
                cout << "\n\t[ERROR] '" << searchItem << "' Not Available in Inventory!" << endl;
            }
            
            Sleep(2500);
            system("cls");
            
        } else if (choice == 2) {
            close = true;
            cout << "\tProceeding to checkout..." << endl;
            Sleep(1500);
        } else {
            cout << "\tInvalid choice. Please select 1 or 2." << endl;
            Sleep(1500);
            system("cls");
        }
    }
    
    // --- FINAL CHECKOUT & RECEIPT PRINTING ---
    system("cls");
    cout << "\n\n\t===========================================" << endl;
    cout << "\t             SUPER MARKET                  " << endl;
    cout << "\t             CUSTOMER RECEIPT              " << endl;
    cout << "\t===========================================" << endl;
    cout << "\tITEM \t\tRATE \tQTY \tAMOUNT" << endl;
    cout << "\t-------------------------------------------" << endl;

    if (!cartReceipt.empty()) {
        for (const string& line : cartReceipt) {
            cout << line << endl; 
        }
    } else {
        cout << "\t(No items purchased)" << endl;
    }

    cout << "\t===========================================" << endl;
    cout << "\t TOTAL AMOUNT DUE : \t\t$" << totalAmount << endl;
    cout << "\t===========================================" << endl;
    cout << "\t       Thanks For Shopping With Us!        " << endl << endl;
    
    saveInventory(); // Sync final updated inventory to disk once checkout is done
    
    system("pause"); 
}

int main() {
    bool exit = false;
    
    // Load existing inventory into memory at launch
    loadInventory();
    
    while (!exit) {
        system("cls");
        cout << "\t======================================" << endl;
        cout << "\t       SUPER MARKET BILLING SYSTEM" << endl;
        cout << "\t======================================" << endl;
        cout << "\t\t1. Add Inventory Items" << endl;
        cout << "\t\t2. New Customer Checkout" << endl;
        cout << "\t\t3. Exit System" << endl;
        
        int val = getValidInt("\t\tEnter Choice: ");

        if (val == 1) {
            system("cls");
            addItem();
        } else if (val == 2) {
            printBill();
        } else if (val == 3) {
            system("cls");
            exit = true;
            saveInventory(); // Final safety save before shutting down
            cout << "\n\tShutting down system. Goodbye!\n" << endl;
            Sleep(2000);
        } else {
            cout << "\tInvalid option! Please choose 1, 2, or 3." << endl;
            Sleep(1500);
        }
    }
    return 0;
}