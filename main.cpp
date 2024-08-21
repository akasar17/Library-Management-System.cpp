#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool isBorrowed;
    time_t dueDate;
};

vector<Book> loadBooks() {
    vector<Book> books;
    ifstream infile("books.dat", ios::binary);
    Book book;
    while (infile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        books.push_back(book);
    }
    infile.close();
    return books;
}

void saveBooks(const vector<Book>& books) {
    ofstream outfile("books.dat", ios::binary);
    for (const auto& book : books) {
        outfile.write(reinterpret_cast<const char*>(&book), sizeof(Book));
    }
    outfile.close();
}

void addBook(vector<Book>& books) {
    Book book;
    cout << "Enter book ID: ";
    cin >> book.id;
    cin.ignore();  // ignore the newline character
    cout << "Enter book title: ";
    getline(cin, book.title);
    cout << "Enter book author: ";
    getline(cin, book.author);
    book.isBorrowed = false;
    book.dueDate = 0;
    books.push_back(book);
    saveBooks(books);
    cout << "Book added successfully!" << endl;
}

void displayBooks(const vector<Book>& books) {
    cout << "ID\tTitle\tAuthor\tStatus" << endl;
    for (const auto& book : books) {
        cout << book.id << '\t' << book.title << '\t' << book.author << '\t';
        if (book.isBorrowed) {
            cout << "Borrowed";
        } else {
            cout << "Available";
        }
        cout << endl;
    }
}

void borrowBook(vector<Book>& books) {
    int id;
    cout << "Enter book ID to borrow: ";
    cin >> id;
    for (auto& book : books) {
        if (book.id == id && !book.isBorrowed) {
            book.isBorrowed = true;
            book.dueDate = time(0) + 14 * 24 * 3600; // due in 14 days
            saveBooks(books);
            cout << "Book borrowed successfully!" << endl;
            return;
        }
    }
    cout << "Book not available or already borrowed!" << endl;
}

void returnBook(vector<Book>& books) {
    int id;
    cout << "Enter book ID to return: ";
    cin >> id;
    for (auto& book : books) {
        if (book.id == id && book.isBorrowed) {
            book.isBorrowed = false;
            double overdueDays = difftime(time(0), book.dueDate) / (60 * 60 * 24);
            double fine = max(0.0, overdueDays * 5); // $5 per day overdue
            book.dueDate = 0;
            saveBooks(books);
            cout << "Book returned successfully! Fine: $" << fine << endl;
            return;
        }
    }
    cout << "Book was not borrowed or does not exist!" << endl;
}

int main() {
    vector<Book> books = loadBooks();
    int choice;
    do {
        cout << "Library Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Display Books" << endl;
        cout << "3. Borrow Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addBook(books);
                break;
            case 2:
                displayBooks(books);
                break;
            case 3:
                borrowBook(books);
                break;
            case 4:
                returnBook(books);
                break;
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    } while (choice != 5);
    return 0;
}
