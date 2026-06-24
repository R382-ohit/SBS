# Super Market Billing System

A robust, console-based C++ application designed to manage supermarket inventory and process customer checkouts. This system features persistent file storage, real-time stock validation, and structured receipt generation.

## 🚀 Features

* **Inventory Management:** Add new items, rates, and stock quantities to a permanent master inventory.
* **Smart Shopping Cart:** * Case-insensitive item searching (e.g., "apple", "Apple", and "APPLE" all match).
  * Support for multi-word item names (e.g., "Ice Cream").
  * Real-time stock verification to prevent overselling.
* **Receipt Generation:** Automatically calculates total amounts and prints a cleanly formatted final bill for the customer.
* **Persistent Storage:** Utilizes C++ file handling (`fstream`) to maintain an `Inventory.txt` database and a temporary `Receipt.txt` for active sessions.
* **Robust Error Handling:** Prevents infinite loops and crashes by strictly validating user input (blocking negative numbers and character inputs where integers are expected).

## 🛠️ Tech Stack

* **Language:** C++
* **Libraries:** `<iostream>`, `<fstream>`, `<sstream>`, `<windows.h>`, `<string>`, `<algorithm>`
* **Platform:** Windows (utilizes Windows-specific console commands like `system("cls")` and `Sleep()`)

## 📂 How It Works

The application operates using two primary text files that are generated automatically in the same directory as the executable:

1. `Inventory.txt`: The master database. It permanently stores all items, their prices, and current stock levels.
2. `Receipt.txt`: A temporary session file. It records the items a customer adds to their cart during the "Checkout" phase and is deleted once the final bill is printed.

## ⚙️ Getting Started

### Prerequisites
* A Windows operating system.
* A C++ compiler (such as MinGW/GCC) or an IDE like Visual Studio, Code::Blocks, or Dev-C++.

### Compilation & Execution (Command Line)

1. Clone this repository or download the source code file.
2. Open your terminal or command prompt and navigate to the folder containing the code.
3. Compile the code using `g++`:
   ```bash
   g++ billing.cpp -o billing.exe
