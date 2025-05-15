#include <iostream>
#include <ctime>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stack>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Ticket {
    string title;
    string description;
    string notes;
    string attachments;
    pair<string, int> requester;
    int priority;
};

void CreateTicket(stack<Ticket>& tickets, const string& ipAddress, const string& dateTime) {
    Ticket ticket;

    cout << "Please enter your credentials: <Name_MiddleName> ";
    getline(cin, ticket.requester.first);

    while (ticket.requester.first.empty()) {
        cout << "Please re-enter your name: ";
        getline(cin, ticket.requester.first);
    }

    cout << "<EmployeeNumber/ID> ";
    cin >> ticket.requester.second;

    while (ticket.requester.second <= 0 || ticket.requester.second > 600) {
        cout << "Please re-enter your ID: ";
        cin >> ticket.requester.second;
    }

    cin.ignore();

    cout << "Please add a title to your request: ";
    getline(cin, ticket.title);

    cout << "Please provide a description for the request: ";
    getline(cin, ticket.description);

    cout << "Is there any additional information that should be included? ";
    getline(cin, ticket.notes);

    cout << "Would you like to provide any attachments? ";
    getline(cin, ticket.attachments);

    cout << "Enter the priority of the request (1-3), where 4 is an exception: ";
    cin >> ticket.priority;
    while (ticket.priority < 1 || ticket.priority > 4) {
        cout << "Please enter a correct priority rating: ";
        cin >> ticket.priority;
    }

    tickets.push(ticket);

    // Displaying the IP
    cout << "IP Address: " << ipAddress << endl;
    cout << "Date and Time: " << dateTime << endl;
}

void PrintRequest(const Ticket& ticket) {
    cout << "The individual who made the Request is: " << ticket.requester.first
        << " with ID " << ticket.requester.second << endl;
    cout << "The Title of the request is: " << ticket.title << endl;
    cout << "The Description says: " << ticket.description << endl;
    cout << "As additional notes: " << ticket.notes << endl;
    cout << "Attachments: " << ticket.attachments << endl;

    cout << "Priority: ";
    switch (ticket.priority) {
    case 1:
        cout << "Highest";
        break;
    case 2:
        cout << "Medium";
        break;
    case 3:
        cout << "Low";
        break;
    default:
        cout << "To be reviewed";
    }
    cout << endl;
}

void PrintAllRequests(const stack<Ticket>& tickets) {
    stack<Ticket> tempStack = tickets;

    while (!tempStack.empty()) {
        const Ticket& ticket = tempStack.top();
        PrintRequest(ticket);
        cout << "-------------------------------\n";
        tempStack.pop();
    }
}

void DeleteTicket(stack<Ticket>& tickets) {
    if (tickets.empty()) {
        cout << "No tickets to delete." << endl;
        return;
    }

    cout << "Deleting the latest ticket from the database." << endl;
    tickets.pop();
    cout << "Latest ticket deleted successfully." << endl;
}

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock" << endl;
        return 1;
    }


    time_t currentTime = time(0);
    struct tm timeInfo {};
    localtime_s(&timeInfo, &currentTime);
    char dateTime[26];
    if (strftime(dateTime, sizeof(dateTime), "%c", &timeInfo) == 0) {
        cerr << "Failed to format date and time" << endl;
        WSACleanup();
        return 1;
    }

    // that's 4 the local host name
    char hostName[NI_MAXHOST];
    if (gethostname(hostName, NI_MAXHOST) != 0) {
        cerr << "Failed to get local host name" << endl;
        WSACleanup();
        return 1;
    }

    // Here we r gonna take the IP 
    struct addrinfo* result = nullptr;
    struct addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(hostName, nullptr, &hints, &result) != 0) {
        cerr << "Failed to get address info" << endl;
        WSACleanup();
        return 1;
    }

    // searching for IPv4/v6
    char ipBuffer[INET6_ADDRSTRLEN];
    struct sockaddr_in* sa = nullptr;
    for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        if (ptr->ai_family == AF_INET) {
            sa = (struct sockaddr_in*)ptr->ai_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), ipBuffer, INET_ADDRSTRLEN);
            break;
        }
        else if (ptr->ai_family == AF_INET6) {
            // Skipping IPv6
            continue;
        }
    }

    // here we will store the tickets
    stack<Ticket> tickets;

    char choice;
    do {
        CreateTicket(tickets, ipBuffer, dateTime);

        cout << "Do you want to create another ticket? (Y/N): ";
        cin >> choice;
        cin.ignore();

        cout << endl;
    } while (choice == 'Y' || choice == 'y'); 

    cout << "Tickets created:\n";
    PrintAllRequests(tickets); 

    char deleteChoice;
    cout << "Do you want to delete a ticket from the database? (Y/N): ";
    cin >> deleteChoice;
    cin.ignore();

    if (deleteChoice == 'Y' || deleteChoice == 'y') {
        DeleteTicket(tickets);
        cout << "Remaining tickets:\n";
        PrintAllRequests(tickets);
    }

    freeaddrinfo(result);
    WSACleanup();

    return 0;
}
