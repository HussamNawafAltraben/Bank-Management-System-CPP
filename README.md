# 🏦 Bank Management System (C++)

A comprehensive console-based **Bank Management System** written in C++. This application handles client data persistence using text file storage and provides a complete suite of banking operations including client management and transaction processing.

---

## ✨ Features

* 👥 **Client Management:**
  * **Show Client List:** Displays formatted tables of all registered bank clients.
  * **Add New Client:** Easily add new records with validation.
  * **Delete Client:** Securely mark and remove client accounts.
  * **Update Client Info:** Modify existing account details (Name, Phone, PinCode, Balance).
  * **Find Client:** Quick lookup for client details by Account Number.

* 💳 **Transactions Menu:**
  * **Deposit:** Add funds to any client account with instant balance updates.
  * **Withdraw:** Perform withdrawals with strict balance checks (prevents overdrawing).
  * **Total Balances:** Generates a full summary report of total assets in the bank.

---

## 🛠️ Concepts & Technologies Used

* **Language:** C++
* **File Handling:** Read/Write operations using `std::fstream` for persistent data storage (`Clients2.txt`).
* **Data Structures:** `std::vector` for dynamic memory management and struct arrays.
* **OOP Concepts:** Encapsulation via `struct` definitions (`sClient`).
* **Enums & Control Flow:** Strongly typed `enum` switch handling for menus.
* **String Manipulation:** Custom delimiter parsing (`SplitString`) using `std::string` functions.

1. **Clepository:**
   ```bash
   git clone [https://github.com/HussamNawafAltraben/Bank-Management-System-CPP.git](https://github.com/HussamNawafAltraben/Bank-Management-System-CPP.git)
