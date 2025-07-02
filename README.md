# Library Management System (C++)

A beginner-friendly Library Management System built using C++ that allows administrators to manage books and members, perform borrow/return operations, and log transactions — all stored in simple CSV files.

---

## 🚀 Features

* ✅ Add/Remove Books
* ✅ Add/Remove Members
* ✅ Borrow & Return Books
* ✅ View All Books & Members
* ✅ Search Books by Title, Author, or ISBN
* ✅ Save & Load Data using CSV files
* ✅ Record Transactions with Date
* ✅ Admin Authentication

---

## 🗂 File Structure

| File               | Purpose                             |
| ------------------ | ----------------------------------- |
| `main.cpp`         | Main C++ program                    |
| `books.csv`        | Stores book records                 |
| `members.csv`      | Stores member details               |
| `transactions.csv` | Stores borrow/return logs with date |

---

## 🔧 How to Use

### 1. **Compile and Run**

```bash
g++ main.cpp -o library
./library
```

### 2. **Login**

Enter admin password:

```
admin123
```

### 3. **Choose Options from Menu**

```
1. Manage Books
2. Manage Members
3. Borrow Book
4. Return Book
5. View All Books
6. View All Members
7. Search Books
8. Exit
```

---

## 📦 Data Persistence

* Data is automatically saved to CSV files on exit.
* On restart, the data is loaded back from these files.

---

## 📌 Requirements

* C++11 or higher
* No external libraries needed

---

## 🔒 Admin Login

* Default Password: `admin123`
* (Can be changed in `Library` class variable `ADMIN_PASS`)

---

## 📈 Future Improvements

* ✅ Due date & fine system
* ✅ GUI interface with Qt/SFML
* ✅ SQLite/MySQL database integration
* ✅ Role-based user login (Admin/Student)
* ✅ Book categories, sorting, pagination, etc.

---

## 🧠 Educational Value

This project is perfect for C++ learners to understand:

* Object-Oriented Programming (OOP)
* File handling in C++ (CSV)
* STL (`vector`, `string`, etc.)
* Basic error handling and input validation

---

## 📝 Author

* Developed by: Chetan Palta
* Last updated: July 2025

---

## 📄 License

This project is open-source and free to use for educational purposes.
