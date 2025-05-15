#include <iostream>
#include <stack>
#include <string>

using namespace std;

struct Ticket {
    string title;
    string description;
    string notes;
    string attachments;
    pair<string, int> requester;
    int priority;
}; 


void CreateTicket(stack<Ticket>& tickets) {
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
    stack<Ticket> tickets;

    char choice;
    do {
        CreateTicket(tickets);

        cout << "Do you want to create another ticket? (Y/N): ";
        cin >> choice;
        cin.ignore(); 

        cout << endl;
    } while (choice == 'Y' || choice == 'y');

    int priority;
    cout << "Enter the priority of the request (1-3), where 4 is an exception: ";
    cin >> priority;
    while (priority < 1 || priority > 4) {
        cout << "Please enter a correct priority rating: ";
        cin >> priority;
    }

    cout << "\nTickets created:\n";
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

    return 0;
}
