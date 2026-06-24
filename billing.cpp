#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <limits>
#include <string>
#include <algorithm> 
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
};

// Converts a string to all lowercase letters for easy comparison
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

            Bill b(item, rate, quant);

            ofstream out("Inventory.txt", ios::app);
            if (!out.is_open()) {
                cout << "\tError: Can't open Inventory file!" << endl;
            } else {
                out << "\t" << b.getItem() << " : " << b.getRate() << " : " << b.getQuant() << endl;
                cout << "\tSuccess! '" << b.getItem() << "' added to master inventory." << endl;
            }
            out.close();
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
    
    // Ensure we start with a fresh cart for a new customer
    remove("Receipt.txt"); 

    while (!close) {
        cout << "\n\t--- SHOPPING CART ---" << endl;
        cout << "\t1. Buy Item" << endl;
        cout << "\t2. Checkout & Print Final Bill" << endl;
        
        int choice = getValidInt("\tEnter Choice: ");

        if (choice == 1) {
            string searchItem;
            cout << "\tEnter Item Name to Buy: ";
            getline(cin >> ws, searchItem);
            
            ifstream in("Inventory.txt");
            if (!in.is_open()) {
                cout << "\tError: Inventory empty! Please add items first." << endl;
                Sleep(2000);
                system("cls");
                continue;
            }

            ofstream out("Inventory_Temp.txt");
            ofstream receipt("Receipt.txt", ios::app); 

            string line;
            bool found = false;

            while (getline(in, line)) {
                if (line.empty()) continue; 

                stringstream ss(line);
                string itemName;
                int itemRate, itemQuant;
                char delimiter;
                
                ss >> ws;
                getline(ss, itemName, ':');
                
                while (!itemName.empty() && isspace(itemName.back())) {
                    itemName.pop_back();
                }

                ss >> itemRate >> delimiter >> itemQuant;

                if (toLowerCase(searchItem) == toLowerCase(itemName)) {
                    found = true;
                    
                    // Display available stock and ask for quantity ONLY if item is found
                    cout << "\n\t-> Item Found! Available Stock: " << itemQuant << endl;
                    int buyQuant = getValidInt("\t-> Enter Quantity to Buy: ");

                    if (buyQuant <= itemQuant) {
                        int amount = itemRate * buyQuant;
                        
                        cout << "\t[ADDED TO CART] " << buyQuant << "x " << itemName << " - $" << amount << endl;
                        
                        itemQuant -= buyQuant;
                        totalAmount += amount;

                        receipt << "\t" << itemName << " \t$" << itemRate << " \tx" << buyQuant << " \t$" << amount << endl;
                        out << "\t" << itemName << " : " << itemRate << " : " << itemQuant << endl;
                    } else {
                        cout << "\t[ERROR] Insufficient stock! Transaction cancelled." << endl;
                        out << line << endl; // Write original back to inventory
                    }
                } else {
                    out << line << endl;
                }
            }
            
            if (!found) {
                cout << "\n\t[ERROR] '" << searchItem << "' Not Available in Inventory!" << endl;
            }
            
            in.close();
            out.close();
            receipt.close();
            
            remove("Inventory.txt");
            rename("Inventory_Temp.txt", "Inventory.txt");
            
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

    ifstream readReceipt("Receipt.txt");
    if (readReceipt.is_open()) {
        string receiptLine;
        while (getline(readReceipt, receiptLine)) {
            cout << receiptLine << endl; 
        }
        readReceipt.close();
    } else {
        cout << "\t(No items purchased)" << endl;
    }

    cout << "\t===========================================" << endl;
    cout << "\t TOTAL AMOUNT DUE : \t\t$" << totalAmount << endl;
    cout << "\t===========================================" << endl;
    cout << "\t       Thanks For Shopping With Us!        " << endl << endl;
    
    remove("Receipt.txt"); 
    
    system("pause"); 
}

int main() {
    bool exit = false;
    
    while (!exit) {
        system("cls");
        cout << "\t======================================" << endl;
        cout << "\t     SUPER MARKET BILLING SYSTEM" << endl;
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
            cout << "\n\tShutting down system. Goodbye!\n" << endl;
            Sleep(2000);
        } else {
            cout << "\tInvalid option! Please choose 1, 2, or 3." << endl;
            Sleep(1500);
        }
    }
    return 0;
}