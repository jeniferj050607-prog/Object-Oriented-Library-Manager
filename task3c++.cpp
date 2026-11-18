
//include
#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

public:
    Book(int id, string title, string author,
         int totalCopies, int availableCopies = -1);

    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    bool isAvailable() const;
    bool borrowCopy();
    bool returnCopy();

    void display() const;
};

#endif


#ifndef MEMBER_H
#define MEMBER_H

#include <string>
using namespace std;

class Member {
private:
    int id;
    string name;
    string email;

public:
    Member(int id, string name, string email);

    int getId() const;
    string getName() const;
    string getEmail() const;

    void display() const;
};

#endif


#ifndef LOAN_H
#define LOAN_H

class Loan {
private:
    int id;
    int bookId;
    int memberId;
    bool active;

public:
    Loan(int id, int bookId, int memberId,
         bool active = true);

    int getId() const;
    int getBookId() const;
    int getMemberId() const;

    bool isActive() const;
    void markReturned();
};

#endif

#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Member.h"
#include "Loan.h"

#include <vector>
#include <string>
using namespace std;

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;

    int nextLoanId;
    string dataDirectory;

    Book* findBook(int id);
    Member* findMember(int id);
    Loan* findLoan(int id);

public:
    Library(string directory);

    bool addBook(Book book);
    bool addMember(Member member);

    void listBooks() const;
    void listMembers() const;

    void searchBooks(string keyword) const;
    void showAvailableBooks() const;

    bool issueBook(int bookId, int memberId);
    bool returnBook(int loanId);

    void listLoans() const;

    void saveData() const;
    void loadData();
};

#endif

//src
#include "Book.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Book::Book(int id, string title, string author,
           int totalCopies, int availableCopies) {

    if (id <= 0 || title.empty() || author.empty() ||
        totalCopies < 0) {
        throw invalid_argument("Invalid book details");
    }

    if (availableCopies == -1)
        availableCopies = totalCopies;

    if (availableCopies < 0 ||
        availableCopies > totalCopies) {
        throw invalid_argument("Invalid available copies");
    }

    this->id = id;
    this->title = title;
    this->author = author;
    this->totalCopies = totalCopies;
    this->availableCopies = availableCopies;
}

int Book::getId() const {
    return id;
}

string Book::getTitle() const {
    return title;
}

string Book::getAuthor() const {
    return author;
}

int Book::getTotalCopies() const {
    return totalCopies;
}

int Book::getAvailableCopies() const {
    return availableCopies;
}

bool Book::isAvailable() const {
    return availableCopies > 0;
}

bool Book::borrowCopy() {
    if (availableCopies > 0) {
        availableCopies--;
        return true;
    }
    return false;
}

bool Book::returnCopy() {
    if (availableCopies < totalCopies) {
        availableCopies++;
        return true;
    }
    return false;
}

void Book::display() const {
    cout << "\nBook ID: " << id;
    cout << "\nTitle: " << title;
    cout << "\nAuthor: " << author;
    cout << "\nAvailable Copies: "
         << availableCopies << "/" << totalCopies;
    cout << "\n-------------------------\n";
}


#include "Member.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Member::Member(int id, string name, string email) {

    if (id <= 0 || name.empty() || email.empty() ||
        email.find('@') == string::npos) {
        throw invalid_argument("Invalid member details");
    }

    this->id = id;
    this->name = name;
    this->email = email;
}

int Member::getId() const {
    return id;
}

string Member::getName() const {
    return name;
}

string Member::getEmail() const {
    return email;
}

void Member::display() const {
    cout << "\nMember ID: " << id;
    cout << "\nName: " << name;
    cout << "\nEmail: " << email;
    cout << "\n-------------------------\n";
}


#include "Loan.h"
#include <stdexcept>

Loan::Loan(int id, int bookId, int memberId,
           bool active) {

    if (id <= 0 || bookId <= 0 || memberId <= 0) {
        throw std::invalid_argument("Invalid loan details");
    }

    this->id = id;
    this->bookId = bookId;
    this->memberId = memberId;
    this->active = active;
}

int Loan::getId() const {
    return id;
}

int Loan::getBookId() const {
    return bookId;
}

int Loan::getMemberId() const {
    return memberId;
}

bool Loan::isActive() const {
    return active;
}

void Loan::markReturned() {
    active = false;
}


#include "Library.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

// Constructor
Library::Library(string directory) {
    dataDirectory = directory;
    nextLoanId = 1;
    fs::create_directories(dataDirectory);
}

// Find book
Book* Library::findBook(int id) {
    for (auto& book : books) {
        if (book.getId() == id)
            return &book;
    }
    return nullptr;
}

// Find member
Member* Library::findMember(int id) {
    for (auto& member : members) {
        if (member.getId() == id)
            return &member;
    }
    return nullptr;
}

// Find loan
Loan* Library::findLoan(int id) {
    for (auto& loan : loans) {
        if (loan.getId() == id)
            return &loan;
    }
    return nullptr;
}

// Add book
bool Library::addBook(Book book) {
    if (findBook(book.getId())) {
        cout << "Book ID already exists!\n";
        return false;
    }

    books.push_back(book);
    saveData();
    cout << "Book added successfully!\n";
    return true;
}

// Add member
bool Library::addMember(Member member) {
    if (findMember(member.getId())) {
        cout << "Member ID already exists!\n";
        return false;
    }

    members.push_back(member);
    saveData();
    cout << "Member added successfully!\n";
    return true;
}

// Display books
void Library::listBooks() const {
    if (books.empty()) {
        cout << "No books available.\n";
        return;
    }

    for (const auto& book : books)
        book.display();
}

// Display members
void Library::listMembers() const {
    if (members.empty()) {
        cout << "No members found.\n";
        return;
    }

    for (const auto& member : members)
        member.display();
}

// Search books
void Library::searchBooks(string keyword) const {
    transform(keyword.begin(), keyword.end(),
              keyword.begin(), [](unsigned char c) {
                  return static_cast<char>(tolower(c));
              });

    bool found = false;

    for (const auto& book : books) {
        string title = book.getTitle();
        string author = book.getAuthor();

        transform(title.begin(), title.end(), title.begin(),
                  [](unsigned char c) {
                      return static_cast<char>(tolower(c));
                  });

        transform(author.begin(), author.end(), author.begin(),
                  [](unsigned char c) {
                      return static_cast<char>(tolower(c));
                  });

        if (title.find(keyword) != string::npos ||
            author.find(keyword) != string::npos) {
            book.display();
            found = true;
        }
    }

    if (!found)
        cout << "No matching books found.\n";
}

// Show available books
void Library::showAvailableBooks() const {
    bool found = false;

    for (const auto& book : books) {
        if (book.isAvailable()) {
            book.display();
            found = true;
        }
    }

    if (!found)
        cout << "No books currently available.\n";
}

// Issue book
bool Library::issueBook(int bookId, int memberId) {
    Book* book = findBook(bookId);

    if (!book) {
        cout << "Book not found!\n";
        return false;
    }

    if (!findMember(memberId)) {
        cout << "Member not found!\n";
        return false;
    }

    if (!book->borrowCopy()) {
        cout << "Book is not available!\n";
        return false;
    }

    loans.emplace_back(nextLoanId, bookId, memberId);

    int issuedId = nextLoanId;
    nextLoanId++;

    saveData();

    cout << "Book issued successfully!\n";
    cout << "Loan ID: " << issuedId << "\n";

    return true;
}

// Return book
bool Library::returnBook(int loanId) {
    Loan* loan = findLoan(loanId);

    if (!loan) {
        cout << "Loan not found!\n";
        return false;
    }

    if (!loan->isActive()) {
        cout << "Book already returned!\n";
        return false;
    }

    Book* book = findBook(loan->getBookId());

    if (!book || !book->returnCopy()) {
        cout << "Unable to return book.\n";
        return false;
    }

    loan->markReturned();
    saveData();

    cout << "Book returned successfully!\n";
    return true;
}

// Display loans
void Library::listLoans() const {
    if (loans.empty()) {
        cout << "No loan records found.\n";
        return;
    }

    for (const auto& loan : loans) {
        cout << "\nLoan ID: " << loan.getId();
        cout << "\nBook ID: " << loan.getBookId();
        cout << "\nMember ID: " << loan.getMemberId();

        cout << "\nStatus: "
             << (loan.isActive() ? "Issued" : "Returned");

        cout << "\n-------------------------\n";
    }
}

// Save data
void Library::saveData() const {
    ofstream bookFile(dataDirectory + "/books.txt");
    ofstream memberFile(dataDirectory + "/members.txt");
    ofstream loanFile(dataDirectory + "/loans.txt");

    if (!bookFile || !memberFile || !loanFile) {
        cerr << "Error saving data!\n";
        return;
    }

    for (const auto& book : books) {
        bookFile << book.getId() << " "
                 << quoted(book.getTitle()) << " "
                 << quoted(book.getAuthor()) << " "
                 << book.getTotalCopies() << " "
                 << book.getAvailableCopies() << "\n";
    }

    for (const auto& member : members) {
        memberFile << member.getId() << " "
                   << quoted(member.getName()) << " "
                   << quoted(member.getEmail()) << "\n";
    }

    for (const auto& loan : loans) {
        loanFile << loan.getId() << " "
                 << loan.getBookId() << " "
                 << loan.getMemberId() << " "
                 << loan.isActive() << "\n";
    }

    if (!bookFile || !memberFile || !loanFile)
        cerr << "Error writing data!\n";
}

// Load data
void Library::loadData() {
    books.clear();
    members.clear();
    loans.clear();
    nextLoanId = 1;

    string line;

    // Load books
    ifstream bookFile(dataDirectory + "/books.txt");

    while (getline(bookFile, line)) {
        try {
            istringstream input(line);

            int id, total, available;
            string title, author;

            if (!(input >> id >> quoted(title)
                       >> quoted(author) >> total >> available))
                continue;

            books.emplace_back(
                id, title, author, total, available
            );

        } catch (const exception&) {
            cerr << "Invalid book record skipped.\n";
        }
    }

    // Load members
    ifstream memberFile(dataDirectory + "/members.txt");

    while (getline(memberFile, line)) {
        try {
            istringstream input(line);

            int id;
            string name, email;

            if (!(input >> id >> quoted(name)
                       >> quoted(email)))
                continue;

            members.emplace_back(id, name, email);

        } catch (const exception&) {
            cerr << "Invalid member record skipped.\n";
        }
    }

    // Load loans
    ifstream loanFile(dataDirectory + "/loans.txt");

    while (getline(loanFile, line)) {
        try {
            istringstream input(line);

            int id, bookId, memberId;
            bool active;

            if (!(input >> id >> bookId >> memberId >> active))
                continue;

            if (!findBook(bookId) || !findMember(memberId))
                continue;

            loans.emplace_back(id, bookId, memberId, active);

            nextLoanId = max(nextLoanId, id + 1);

        } catch (const exception&) {
            cerr << "Invalid loan record skipped.\n";
        }
    }

    cout << "Library data loaded successfully!\n";
}



#include "Library.h"

#include <iostream>
#include <limits>
using namespace std;

int readInt(string message) {
    int value;

    while (true) {
        cout << message;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Please enter a valid number!\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(string message) {
    string value;
    cout << message;
    getline(cin, value);
    return value;
}

int main() {
    Library library("data");

    library.loadData();

    int choice;

    do {
        cout << "\n================================\n";
        cout << "       LIBRARY MANAGEMENT\n";
        cout << "================================\n";

        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Display Books\n";
        cout << "4. Display Members\n";
        cout << "5. Search Book\n";
        cout << "6. Available Books\n";
        cout << "7. Issue Book\n";
        cout << "8. Return Book\n";
        cout << "9. Display Loans\n";
        cout << "0. Exit\n";

        cout << "================================\n";

        choice = readInt("Enter your choice: ");

        try {
            switch (choice) {

                case 1: {
                    int id = readInt("Enter Book ID: ");
                    string title = readText("Enter Book Title: ");
                    string author = readText("Enter Author Name: ");
                    int copies = readInt("Enter Number of Copies: ");

                    Book book(id, title, author, copies);
                    library.addBook(book);
                    break;
                }

                case 2: {
                    int id = readInt("Enter Member ID: ");
                    string name = readText("Enter Member Name: ");
                    string email = readText("Enter Email: ");

                    Member member(id, name, email);
                    library.addMember(member);
                    break;
                }

                case 3:
                    library.listBooks();
                    break;

                case 4:
                    library.listMembers();
                    break;

                case 5: {
                    string keyword = readText("Enter title or author: ");
                    library.searchBooks(keyword);
                    break;
                }

                case 6:
                    library.showAvailableBooks();
                    break;

                case 7: {
                    int bookId = readInt("Enter Book ID: ");
                    int memberId = readInt("Enter Member ID: ");

                    library.issueBook(bookId, memberId);
                    break;
                }

                case 8: {
                    int loanId = readInt("Enter Loan ID: ");
                    library.returnBook(loanId);
                    break;
                }

                case 9:
                    library.listLoans();
                    break;

                case 0:
                    library.saveData();
                    cout << "Thank you for using Library Manager!\n";
                    break;

                default:
                    cout << "Invalid choice! Try again.\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }

    } while (choice != 0);

    return 0;
}


//data
101 "The Alchemist" "Paulo Coelho" 3 3
102 "Wings of Fire" "A P J Abdul Kalam" 2 2
103 "Clean Code" "Robert C Martin" 1 1

201 "Ananya" "ananya@example.com"
202 "Rahul" "rahul@example.com"

