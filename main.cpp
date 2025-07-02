#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

class Book {
private:
    string isbn, title, author;
    int available;
public:
    Book(string i = "", string t = "", string a = "", int q = 1)
        : isbn(i), title(t), author(a), available(q) {}

    string getISBN() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getAvailable() const { return available; }

    bool borrow() { if (available > 0) { available--; return true; } return false; }
    void returns() { available++; }

    void display() const {
        cout << "ISBN: " << isbn << "\nTitle: " << title
             << "\nAuthor: " << author << "\nAvailable: " << available << endl;
    }

    string toCSV() const {
        return isbn + "," + title + "," + author + "," + to_string(available);
    }
};

class Member {
private:
    string id, name;
    vector<string> borrowedBooks;
public:
    Member(string i = "", string n = "") : id(i), name(n) {}

    string getID() const { return id; }
    string getName() const { return name; }

    bool borrowBook(const string &isbn) {
        if (borrowedBooks.size() < 5) {
            borrowedBooks.push_back(isbn);
            return true;
        }
        return false;
    }

    bool returnBook(const string &isbn) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            return true;
        }
        return false;
    }

    void display() const {
        cout << "Member ID: " << id << "\nName: " << name
             << "\nBooks Borrowed: " << borrowedBooks.size() << endl;
    }

    string toCSV() const {
        return id + "," + name;
    }
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    const string ADMIN_PASS = "admin123";

    Book* findBook(const string &isbn) {
        for (auto &book : books)
            if (book.getISBN() == isbn) return &book;
        return nullptr;
    }

    Member* findMember(const string &id) {
        for (auto &member : members)
            if (member.getID() == id) return &member;
        return nullptr;
    }

    string currentDate() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", now);
        return buf;
    }

    void logTransaction(const string &memberId, const string &isbn, const string &type) {
        ofstream log("transactions.csv", ios::app);
        log << memberId << "," << isbn << "," << type << "," << currentDate() << "\n";
        log.close();
    }

public:
    bool authenticate(const string &pass) const {
        return pass == ADMIN_PASS;
    }

    void searchBooks() {
        cout << "Search by 1. Title 2. Author 3. ISBN\nChoice: ";
        string input;
        int choice;
        getline(cin, input);
        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input!\n";
            return;
        }
        string query;
        cout << "Enter search text: ";
        getline(cin, query);
        transform(query.begin(), query.end(), query.begin(), ::tolower);

        for (const auto &book : books) {
            string field = (choice == 1) ? book.getTitle() : (choice == 2) ? book.getAuthor() : book.getISBN();
            string temp = field;
            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            if (temp.find(query) != string::npos)
                book.display(), cout << "-----------------\n";
        }
    }

    void manageBook() {
        cout << "\n1. Add Book\n2. Remove Book\nChoice: ";
        string input;
        int choice;
        getline(cin, input);
        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input!\n";
            return;
        }

        string isbn, title, author;
        cout << "Enter ISBN: "; getline(cin, isbn);

        if (choice == 1) {
            if (findBook(isbn)) {
                cout << "Book with ISBN already exists.\n";
                return;
            }
            cout << "Enter Title: "; getline(cin, title);
            cout << "Enter Author: "; getline(cin, author);
            books.emplace_back(isbn, title, author);
            cout << "New book added successfully!\n";
        } else {
            auto it = remove_if(books.begin(), books.end(), [&](const Book& b) { return b.getISBN() == isbn; });
            if (it != books.end()) {
                books.erase(it, books.end());
                cout << "Book removed successfully!\n";
            } else {
                cout << "Book not found!\n";
            }
        }
    }

    void manageMember() {
        cout << "\n1. Add Member\n2. Remove Member\nChoice: ";
        string input;
        int choice;
        getline(cin, input);
        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input!\n";
            return;
        }

        string id, name;
        cout << "Enter Member ID: "; getline(cin, id);

        if (choice == 1) {
            if (findMember(id)) {
                cout << "Member ID already exists.\n";
                return;
            }
            cout << "Enter Name: "; getline(cin, name);
            members.emplace_back(id, name);
            cout << "New member added successfully!\n";
        } else {
            auto it = remove_if(members.begin(), members.end(), [&](const Member& m) { return m.getID() == id; });
            if (it != members.end()) {
                members.erase(it, members.end());
                cout << "Member removed successfully!\n";
            } else {
                cout << "Member not found!\n";
            }
        }
    }

    void borrowBook() {
        string memberId, isbn;
        cout << "Enter Member ID: "; getline(cin, memberId);
        cout << "Enter ISBN: "; getline(cin, isbn);

        Member* member = findMember(memberId);
        Book* book = findBook(isbn);

        if (member && book && book->borrow() && member->borrowBook(isbn)) {
            logTransaction(memberId, isbn, "Borrow");
            cout << "Book borrowed successfully!\n";
        } else {
            cout << "Borrow operation failed!\n";
        }
    }

    void returnBook() {
        string memberId, isbn;
        cout << "Enter Member ID: "; getline(cin, memberId);
        cout << "Enter ISBN: "; getline(cin, isbn);

        Member* member = findMember(memberId);
        Book* book = findBook(isbn);

        if (member && book && member->returnBook(isbn)) {
            book->returns();
            logTransaction(memberId, isbn, "Return");
            cout << "Book returned successfully!\n";
        } else {
            cout << "Return operation failed!\n";
        }
    }

    void displayBooks() const {
        if (books.empty()) { cout << "No books in library.\n"; return; }
        for (const auto &book : books) { book.display(); cout << "-----------------\n"; }
    }

    void displayMembers() const {
        if (members.empty()) { cout << "No members registered.\n"; return; }
        for (const auto &member : members) { member.display(); cout << "-----------------\n"; }
    }

    void saveData() {
        ofstream bout("books.csv"), mout("members.csv");
        for (auto &b : books) bout << b.toCSV() << "\n";
        for (auto &m : members) mout << m.toCSV() << "\n";
        bout.close();
        mout.close();
    }

    void loadData() {
        ifstream bin("books.csv"), min("members.csv");
        string line;
        while (getline(bin, line)) {
            stringstream ss(line);
            string isbn, title, author, qty;
            getline(ss, isbn, ','); getline(ss, title, ','); getline(ss, author, ','); getline(ss, qty);
            books.emplace_back(isbn, title, author, stoi(qty));
        }
        while (getline(min, line)) {
            stringstream ss(line);
            string id, name;
            getline(ss, id, ','); getline(ss, name);
            members.emplace_back(id, name);
        }
    }
};

void showMenu() {
    cout << "\n===== Library System =====\n"
         << "1. Manage Books\n"
         << "2. Manage Members\n"
         << "3. Borrow Book\n"
         << "4. Return Book\n"
         << "5. View All Books\n"
         << "6. View All Members\n"
         << "7. Search Books\n"
         << "8. Exit\n"
         << "Choice: ";
}

int main() {
    Library lib;
    lib.loadData();

    string pass;
    cout << "Enter Admin Password: ";
    getline(cin, pass);

    if (!lib.authenticate(pass)) {
        cout << "Authentication failed!\n";
        return 0;
    }

    string input;
    int choice;
    while (true) {
        showMenu();
        getline(cin, input);

        if (input.empty()) {
            cout << "Please enter a valid number.\n";
            continue;
        }

        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: lib.manageBook(); break;
            case 2: lib.manageMember(); break;
            case 3: lib.borrowBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.displayBooks(); break;
            case 6: lib.displayMembers(); break;
            case 7: lib.searchBooks(); break;
            case 8: lib.saveData(); return 0;
            default: cout << "Invalid choice!\n";
        }
    }

    return 0;
}
