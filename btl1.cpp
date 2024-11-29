#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Lop user dai dien cho nguoi dung (admin or client)
class User {
public:
    string username;
    bool isAdmin;

    User(string name, bool admin = false) : username(name), isAdmin(admin) {}
};

// Lop Event dai dien cho mot su kien
class Event {
private:
    string eventName;
    string date;
    int totalTickets;
    int ticketsSold;

public:
    Event(string name, string d, int total)
        : eventName(name), date(d), totalTickets(total), ticketsSold(0) {}

    string getEventName() const {
        return eventName;
    }

    string getDate() const {
        return date;
    }

    int getRemainingTickets() const {
        return totalTickets - ticketsSold;
    }

    bool sellTicket() {
        if (ticketsSold < totalTickets) {
            ticketsSold++;
            return true;
        } else {
            cout << "Het ve cho su kien nay.\n";
            return false;
        }
    }

    bool cancelTicket() {
        if (ticketsSold > 0) {
            ticketsSold--;
            return true;
        } else {
            cout << "Khong co ve nao de huy.\n";
            return false;
        }
    }

    void displayEventInfo() const {
        cout << "Ten su kien : " << eventName << "\n";
        cout << "Ngay to chuc : " << date << "\n";
        cout << "Tong so ve : " << totalTickets << "\n";
        cout << "So ve con lai: " << getRemainingTickets() << "\n";
    }

    void saveToFile(ofstream& file) const {
        file << eventName << "," << date << "," << totalTickets << "," << ticketsSold << "\n";
    }
};

// Lop Ticket dai dien cho mot ve
class Ticket {
private:
    string customerName;
    Event* event;

public:
    Ticket(string name, Event* e)
        : customerName(name), event(e) {}

    string getCustomerName() const {
        return customerName;
    }

    Event* getEvent() const {
        return event;
    }

    void displayTicketInfo() const {
        cout << "Ten khach hang : " << customerName << "\n";
        cout << "Thong tin su kien :\n";
        event->displayEventInfo();
    }

    void saveToFile(ofstream& file) const {
        file << customerName << "," << event->getEventName() << "\n";
    }
};

// Lop TicketManager de quan ly su kien va ve
class TicketManager {
private:
    list<Event> events;   
    list<Ticket> tickets;  

public:
    void addEvent(string name, string date, int totalTickets) {
        events.push_back(Event(name, date, totalTickets));
        cout << "Da them su kien : " << name << "\n";
    }

    Event* findEvent(string name) {
        for (auto& event : events) {
            if (event.getEventName() == name) {
                return &event;
            }
        }
        cout << "Khong tim thay su kien.\n";
        return nullptr;
    }

    void buyTicket(string customerName, string eventName) {
        Event* event = findEvent(eventName);
        if (event != nullptr && event->sellTicket()) {
            tickets.push_back(Ticket(customerName, event));
            cout << "Mua ve thanh cong cho " << customerName << "!\n";
        }
    }

    void cancelTicket(string customerName, string eventName) {
        Event* event = findEvent(eventName);
        if (event != nullptr && event->cancelTicket()) {
            auto it = find_if(tickets.begin(), tickets.end(), [&](Ticket& t) {
                return t.getCustomerName() == customerName && t.getEvent()->getEventName() == eventName;
            });

            if (it != tickets.end()) {
                tickets.erase(it);
                cout << "Huy ve thanh cong cho " << customerName << "!\n";
            }
        }
    }

    void displayAllTickets() const {
        if (tickets.empty()) {
            cout << "Chua co ve nao duoc ban.\n";
        } else {
            for (const auto& ticket : tickets) {
                ticket.displayTicketInfo();
                cout << "-------------------\n";
            }
        }
    }

    void displayEvents() const {
        if (events.empty()) {
            cout << "Chua co su kien nao.\n";
        } else {
            for (const auto& event : events) {
                event.displayEventInfo();
                cout << "-------------------\n";
            }
        }
    }

    void saveData() {
        ofstream eventFile("events.txt");
        for (const auto& event : events) {
            event.saveToFile(eventFile);
        }
        eventFile.close();

        ofstream ticketFile("tickets.txt");
        for (const auto& ticket : tickets) {
            ticket.saveToFile(ticketFile);
        }
        ticketFile.close();

        cout << "Du lieu da duoc luu vao file.\n";
    }
};

int main() {
    TicketManager manager;
    manager.addEvent("Concert A", "2024-11-20", 100);
    manager.addEvent("Concert B", "2024-12-14", 50);
    manager.addEvent("Concert C","2024-7-14",1000);

    int choice;
    string userType;
    cout << "Ban la quan tri vien (admin) hay khach hang (user)? ";
    cin >> userType;
    cin.ignore();

    if (userType == "admin") {
        do {
            cout << "\n--- Quan Li Su Kien (Admin) ---\n";
            cout << "1. Xem danh sach su kien\n";
            cout << "2. Them su kien\n";
            cout << "3. Luu du lieu\n";
            cout << "0. Thoat\n";
            cout << "Lua chon cua ban: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    manager.displayEvents();
                    break;
                }
                case 2: {
                    string eventName, date;
                    int totalTickets;
                    cout << "Nhap ten su kien: ";
                    getline(cin, eventName);
                    cout << "Nhap ngay to chuc: ";
                    getline(cin, date);
                    cout << "Nhap tong so ve: ";
                    cin >> totalTickets;
                    manager.addEvent(eventName, date, totalTickets);
                    break;
                }
                case 3: {
                    manager.saveData();
                    break;
                }
                case 0:
                    cout << "Thoat chuong trinh.\n";
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
            }
        } while (choice != 0);
    } else {
        do {
            cout << "\n--- He thong quan li ve (User) ---\n";
            cout << "1. Xem danh sach su kien \n";
            cout << "2. Mua ve\n";
            cout << "3. Huy ve\n";
            cout << "4. Xem tat ca ve da ban\n";
            cout << "0. Thoat\n";
            cout << "Lua chon cua ban: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    manager.displayEvents();
                    break;
                }
                case 2: {
                    string customerName, eventName;
                    cout << "Nhap ten khach hang: ";
                    getline(cin, customerName);
                    cout << "Nhap ten su kien: ";
                    getline(cin, eventName);
                    manager.buyTicket(customerName, eventName);
                    break;
                }
                case 3: {
                    string customerName, eventName;
                    cout << "Nhap ten khach hang: ";
                    getline(cin, customerName);
                    cout << "Nhap ten su kien: ";
                    getline(cin, eventName);
                    manager.cancelTicket(customerName, eventName);
                    break;
                }
                case 4: {
                    manager.displayAllTickets();
                    break;
                }
                case 0:
                    cout << "Thoat chuong trinh.\n";
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
            }
        } while (choice != 0);
    }

    return 0;
}
