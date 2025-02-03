#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_EMAIL_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_LINE_LENGTH 150
#define MAX_ROUTE_LENGTH 100
#define MAX_BUS_SEATS 100
#define BUS_ENTRY_LINES 6

void bookTicket();
// int checkUserLogin(const char *email, const char *password);
// int getBusDetails(FILE *fp, int selectedBusNumber, int *totalSeats, int *availableSeats, float *seatPrice);
// void saveBookingHistory(const char *email, int busNumber, const char *route, int seatsBooked, float totalPrice);
int busExists(int busNumber);
void addBus();
void editBus();
void deleteBus();
struct User {
    char email[50];
    char password[20];
};
// Structure to hold bus information
struct Bus {
    int busNumber;
    char route[MAX_ROUTE_LENGTH];
    int totalSeats;
    int availableSeats;
    float seatPrice;
};
struct Ticket {
    int ticketID;
    int busNumber;
    int seatNumber;
    char passengerName[50];
};
void drawBus() {
    printf("\033[1;35m\t\t\t\t  ________________\033[0m");
    printf("\n\033[1;32m\t\t\t\t /                \\\033[0m\n");
    printf("\033[1;32m\t\t\t\t/      DIU BUS     \\\033[0m\n");
    printf("\033[1;36m\t\t\t\t\\_____OO____OO_____/\033[0m");
    
}

int isLoggedIn = 0; // To track login status
char loggedInEmail[100]; // To store the logged-in user's email
bool isValidEmail(const char *email) {
    char *atPos = strstr(email, "@gmail.com");
    return atPos != NULL && strlen(atPos) == 10; 
}

bool emailExists(const char *email, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return false; 
    }

    char line[150];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, email)) { 
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false;
}


void registration() {
    struct User user;
    printf("\n\n\033[1;105m\t\t\t\t      Registration       \033[0m\n\n");
    printf("\033[1;46mEnter Your Email :\033[0m");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = '\0'; 

    
    if (!isValidEmail(user.email)) {
        printf("\033[1;31mError: Email address is not valid. It must end with '@gmail.com'\033[0m\n");
        return;
    }

    if (emailExists(user.email, "user.txt")) {
        printf("\033[1;31mError: Email address already exists.\033[0m\n");
        return;
    }

    printf("\033[1;46mEnter Your Password:\033[0m");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = '\0'; 

    
    FILE *fp = fopen("user.txt", "a");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file.\033[0m\n");
        return;
    }
    fprintf(fp, "Email Address:%s\n", user.email);
    fprintf(fp, "Password:%s\n", user.password);
    fclose(fp);

    printf("\n\n\033[1;32m\t\t\tRegistration successful!\033[0m\n");
    
}

void login() {
    char email[50], password[50];
    printf("\n\n\033[1;105m\t\t\t\t      Login       \033[0m\n\n");
    printf("\033[1;46mEnter Your Email :\033[0m");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    printf("\033[1;46mEnter Your Password:\033[0m");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; 

    FILE *fp = fopen("user.txt", "r");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file.\033[0m\n");
        return;
    }

    char line[150];
    char fileEmail[50], filePassword[50];
    int loginSuccess = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "Email Address:%49s", fileEmail) == 1) {
            fgets(line, sizeof(line), fp); // Read the next line for password
            sscanf(line, "Password:%49s", filePassword);

            if (strcmp(email, fileEmail) == 0 && strcmp(password, filePassword) == 0) {
                loginSuccess = 1;
                break;
            }
        }
    }
    fclose(fp);
    if (loginSuccess) {
        printf("\033[1;32m\t\t\t\tLogin successful!\033[0m\n");
    } else {
        printf("\033[1;31m\t\t\t\tInvalid email or password.\033[0m\n");
    }
}
void addBus() {
    struct Bus newBus;
    FILE *fp;

    printf("\n\n\033[1;105m\t\t\t\t      Add New Bus       \033[0m\n\n");

    printf("\033[1;46mEnter Bus Number: \033[0m");
    scanf("%d", &newBus.busNumber);
    while (getchar() != '\n'); // Clear input buffer

    // Check if bus number already exists
    if (busExists(newBus.busNumber)) {
        printf("\033[1;31mError: Bus number %d already exists. Please enter a unique bus number.\033[0m\n", newBus.busNumber);
        return;
    }

    printf("\033[1;46mEnter Route (e.g., CityA-CityB): \033[0m");
    fgets(newBus.route, sizeof(newBus.route), stdin);
    newBus.route[strcspn(newBus.route, "\n")] = '\0'; // Remove newline

    printf("\033[1;46mEnter Total Seats: \033[0m");
    scanf("%d", &newBus.totalSeats);
    while (getchar() != '\n'); // Clear input buffer

    // Ensure the total seats do not exceed the max limit (100)
    if (newBus.totalSeats > MAX_BUS_SEATS) {
        printf("\033[1;31mError: Bus cannot have more than %d seats.\033[0m\n", MAX_BUS_SEATS);
        return;
    }

    printf("\033[1;46mEnter Seat Price: \033[0m");
    scanf("%f", &newBus.seatPrice);
    while (getchar() != '\n'); // Clear input buffer

    // Initialize available seats as total seats
    newBus.availableSeats = newBus.totalSeats;

    // Open file to append bus details
    fp = fopen("buses.txt", "a");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file.\033[0m\n");
        return;
    }

    // Write bus details to file
    fprintf(fp, "Bus Number: %d\n", newBus.busNumber);
    fprintf(fp, "Route: %s\n", newBus.route);
    fprintf(fp, "Total Seats: %d\n", newBus.totalSeats);
    fprintf(fp, "Available Seats: %d\n", newBus.availableSeats);
    fprintf(fp, "Seat Price: %.2f\n", newBus.seatPrice);
    fprintf(fp, "---------------------------\n"); // Separator for better readability

    fclose(fp);

    printf("\n\033[1;32mBus added successfully!\033[0m\n");
}

// Function to check if a bus number already exists in the buses.txt file
void editBus() {
    int targetBusNumber, found = 0;
    struct Bus bus;
    FILE *fp, *temp;
    char line[150];

    printf("\n\n\033[1;105m\t\t\t\t      Edit Bus Details       \033[0m\n\n");

    printf("\033[1;46mEnter the Bus Number to Edit: \033[0m");
    scanf("%d", &targetBusNumber);
    while (getchar() != '\n'); // Clear input buffer

    // Open the original file in read mode
    fp = fopen("buses.txt", "r");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file.\033[0m\n");
        return;
    }

    // Open a temporary file to store updated records
    temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("\033[1;31mError: Unable to open temporary file.\033[0m\n");
        fclose(fp);
        return;
    }

    // Read the file line by line and process each bus entry
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "Bus Number: %d", &bus.busNumber) == 1 && bus.busNumber == targetBusNumber) {
            // Bus found, read remaining details
            fgets(line, sizeof(line), fp); // Read route
            sscanf(line, "Route: %[^\n]", bus.route);
            fgets(line, sizeof(line), fp); // Read total seats
            sscanf(line, "Total Seats: %d", &bus.totalSeats);
            fgets(line, sizeof(line), fp); // Read available seats
            sscanf(line, "Available Seats: %d", &bus.availableSeats);
            fgets(line, sizeof(line), fp); // Read seat price
            sscanf(line, "Seat Price: %f", &bus.seatPrice);

            // Allow admin to modify the bus details
            printf("\033[1;46mEditing details for Bus Number: %d\033[0m\n", bus.busNumber);

            // Route input
            printf("\033[1;46mEnter New Route (current: %s): \033[0m", bus.route);
            fgets(bus.route, sizeof(bus.route), stdin);
            bus.route[strcspn(bus.route, "\n")] = '\0';

            // Total seats input
            do {
                printf("\033[1;46mEnter New Total Seats (current: %d): \033[0m", bus.totalSeats);
                scanf("%d", &bus.totalSeats);
                while (getchar() != '\n'); // Clear input buffer

                if (bus.totalSeats <= 0 || bus.totalSeats > MAX_BUS_SEATS) {
                    printf("\033[1;31mError: Please enter a valid number of seats (1 - %d).\033[0m\n", MAX_BUS_SEATS);
                }
            } while (bus.availableSeats < 0 || bus.availableSeats > bus.totalSeats);

            // Available seats input
            do {
                printf("\033[1;46mEnter New Available Seats (current: %d): \033[0m", bus.availableSeats);
                scanf("%d", &bus.availableSeats);
                while (getchar() != '\n'); // Clear input buffer

                if (bus.availableSeats < 0 || bus.availableSeats > bus.totalSeats) {
                    printf("\033[1;31mError: Available seats must be between 0 and Total Seats (%d).\033[0m\n", bus.totalSeats);
                }
            } while (bus.availableSeats < 0 || bus.availableSeats > bus.totalSeats);

            // Seat price input
            do {
                printf("\033[1;46mEnter New Seat Price (current: %.2f): \033[0m", bus.seatPrice);
                scanf("%f", &bus.seatPrice);
                while (getchar() != '\n'); // Clear input buffer

                if (bus.seatPrice <= 0) {
                    printf("\033[1;31mError: Seat price must be a positive value.\033[0m\n");
                }
            } while (bus.seatPrice <= 0);

            // Write the updated details to the temporary file
            fprintf(temp, "Bus Number: %d\n", bus.busNumber);
            fprintf(temp, "Route: %s\n", bus.route);
            fprintf(temp, "Total Seats: %d\n", bus.totalSeats);
            fprintf(temp, "Available Seats: %d\n", bus.availableSeats);
            fprintf(temp, "Seat Price: %.2f\n", bus.seatPrice);
            fprintf(temp, "---------------------------\n");

            printf("\033[1;32mBus details updated successfully!\033[0m\n");
            found = 1;
        } else {
            // Write unchanged lines to the temporary file
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace the original file with the updated file
    if (found) {
        remove("buses.txt");
        rename("temp.txt", "buses.txt");
    } else {
        printf("\033[1;31mBus with number %d not found.\033[0m\n", targetBusNumber);
        remove("temp.txt");
    }
}
void deleteBus() {
    int targetBusNumber, found = 0;
    FILE *fp, *temp;
    char line[MAX_LINE_LENGTH];
    int busNumber;

    printf("\n\n\033[1;105m\t\t\t\t      Delete Bus       \033[0m\n\n");

    printf("\033[1;46mEnter the Bus Number to Delete: \033[0m");
    scanf("%d", &targetBusNumber);
    while (getchar() != '\n'); // Clear input buffer

    // Open the original file in read mode
    fp = fopen("buses.txt", "r");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file.\033[0m\n");
        return;
    }

    // Open a temporary file to store remaining records
    temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("\033[1;31mError: Unable to open temporary file.\033[0m\n");
        fclose(fp);
        return;
    }

    // Read the file line by line and process each bus entry
    while (fgets(line, sizeof(line), fp)) {
        // Check if the current line contains the bus number
        if (sscanf(line, "Bus Number: %d", &busNumber) == 1 && busNumber == targetBusNumber) {
            // Bus found, skip this entry (don't write to temp file)
            found = 1;

            // Skip the next 5 lines related to this bus entry (Bus details and separator)
            for (int i = 0; i < BUS_ENTRY_LINES - 1; i++) {
                fgets(line, sizeof(line), fp);
            }

            printf("\033[1;32mBus with number %d deleted successfully!\033[0m\n", targetBusNumber);
        } else {
            // Write the current line to the temporary file
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace the original file with the updated file
    if (found) {
        remove("buses.txt");
        rename("temp.txt", "buses.txt");
    } else {
        printf("\033[1;31mBus with number %d not found.\033[0m\n", targetBusNumber);
        remove("temp.txt");
    }
}

void viewAllBuses() {
    FILE *fp;
    char line[150];
    int isEmpty = 1;

    printf("\n\n\033[1;105m\t\t\t\t      All Buses       \033[0m\n\n");

    // Open the file in read mode
    fp = fopen("buses.txt", "r");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file. Make sure 'buses.txt' exists.\033[0m\n");
        return;
    }

    // Read and display each line from the file
    while (fgets(line, sizeof(line), fp)) {
        isEmpty = 0; // File is not empty
        printf("\033[1;37m%s\033[0m", line);
    }
    fclose(fp);

    if (isEmpty) {
        printf("\033[1;33mNo buses found. Please add buses first.\033[0m\n");
    }
}
void searchBusByRoute() {
    char targetRoute[100];
    FILE *fp;
    char line[150];
    int found = 0;

    printf("\n\n\033[1;105m\t\t\t\t    Search Bus by Route     \033[0m\n\n");

    // Prompt the user for the route
    printf("\033[1;46mEnter the Route (e.g., City A to City B): \033[0m");
    fgets(targetRoute, sizeof(targetRoute), stdin);
    targetRoute[strcspn(targetRoute, "\n")] = '\0'; // Remove trailing newline

    // Open the file in read mode
    fp = fopen("buses.txt", "r");
    if (!fp) {
        printf("\033[1;31mError: Unable to open file. Make sure 'buses.txt' exists.\033[0m\n");
        return;
    }

    printf("\n\033[1;33mSearching for buses on route: %s\033[0m\n\n", targetRoute);

    // Read the file line by line
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Route:") && strstr(line, targetRoute)) {
            found = 1;
            printf("\033[1;37m%s\033[0m", line); // Display the route line

            // Display the next 4 lines (bus details)
            for (int i = 0; i < 4; i++) {
                if (fgets(line, sizeof(line), fp)) {
                    printf("\033[1;37m%s\033[0m", line);
                }
            }
            printf("\033[1;36m---------------------------\033[0m\n");
        }
    }
    fclose(fp);

    if (!found) {
        printf("\033[1;31mNo buses found for the specified route.\033[0m\n");
    }
}

int getNewTicketID() {
    FILE *idFile;
    int lastTicketID = 0;

    // Open the file to read the last ticket ID
    idFile = fopen("last_ticket_id.txt", "r");
    if (idFile != NULL) {
        fscanf(idFile, "%d", &lastTicketID);
        fclose(idFile);
    }

    // Increment the last ticket ID
    int newTicketID = lastTicketID + 1;

    // Save the new ticket ID back to the file
    idFile = fopen("last_ticket_id.txt", "w");
    if (idFile == NULL) {
        printf("\033[1;31mError: Unable to save the ticket ID.\033[0m\n");
        return -1;
    }
    fprintf(idFile, "%d", newTicketID);
    fclose(idFile);

    return newTicketID;
}

int busExists(int busNumber) {
    FILE *fp = fopen("buses.txt", "r");
    if (!fp) return 0;

    struct Bus tempBus;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "Bus Number: %d", &tempBus.busNumber) == 1) {
            if (tempBus.busNumber == busNumber) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void updateAvailableSeats(int busNumber, int change) {
    FILE *fp = fopen("buses.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("\033[1;31mError: Unable to update bus details.\033[0m\n");
        return;
    }

    struct Bus tempBus;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        fprintf(temp, "%s", line); // Copy other lines
        if (sscanf(line, "Bus Number: %d", &tempBus.busNumber) == 1 && tempBus.busNumber == busNumber) {
            found = 1;
            fgets(line, sizeof(line), fp); // Read route
            fprintf(temp, "%s", line);

            fgets(line, sizeof(line), fp); // Read total seats
            fprintf(temp, "%s", line);

            fgets(line, sizeof(line), fp); // Read available seats
            sscanf(line, "Available Seats: %d", &tempBus.availableSeats);
            tempBus.availableSeats += change;
            fprintf(temp, "Available Seats: %d\n", tempBus.availableSeats);

            fgets(line, sizeof(line), fp); // Read seat price
            fprintf(temp, "%s", line);
        }
    }
    fclose(fp);
    fclose(temp);

    if (found) {
        remove("buses.txt");
        rename("temp.txt", "buses.txt");
    } else {
        printf("\033[1;31mError: Bus not found.\033[0m\n");
    }
}
// Function to check if the seat is already booked
int isSeatBooked(int busNumber, int seatNumber) {
    FILE *ticketFile = fopen("tickets.txt", "r");
    if (!ticketFile) {
        printf("\033[1;31mError: Unable to check seat availability.\033[0m\n");
        return 0;
    }

    char line[256];
    int fileBusNumber, fileSeatNumber;

    while (fgets(line, sizeof(line), ticketFile)) {
        if (sscanf(line, "Bus Number: %d", &fileBusNumber) == 1 && fileBusNumber == busNumber) {
            // Read next lines to check seat number
            fgets(line, sizeof(line), ticketFile);
            if (sscanf(line, "Seat Number: %d", &fileSeatNumber) == 1 && fileSeatNumber == seatNumber) {
                fclose(ticketFile);
                return 1; // Seat is already booked
            }
        }
    }

    fclose(ticketFile);
    return 0; // Seat is available
}

void bookTicket() {
    struct Ticket newTicket;
    FILE *ticketFile;
    
    printf("\n\033[1;105m\t\t\t\t      Book Ticket       \033[0m\n\n");
    
    // Input bus number
    printf("\033[1;46mEnter Bus Number: \033[0m");
    scanf("%d", &newTicket.busNumber);
    while (getchar() != '\n'); // Clear input buffer

    // Check if the bus exists
    if (!busExists(newTicket.busNumber)) {
        printf("\033[1;31mError: Bus number %d does not exist.\033[0m\n", newTicket.busNumber);
        return;
    }

    // Input passenger name
    printf("\033[1;46mEnter Passenger Name: \033[0m");
    fgets(newTicket.passengerName, sizeof(newTicket.passengerName), stdin);
    newTicket.passengerName[strcspn(newTicket.passengerName, "\n")] = '\0'; // Remove newline

    // Input seat number
    printf("\033[1;46mEnter Seat Number: \033[0m");
    scanf("%d", &newTicket.seatNumber);

    // Check if the seat is already booked
    if (isSeatBooked(newTicket.busNumber, newTicket.seatNumber)) {
        printf("\033[1;31mError: Seat number %d on bus number %d is already booked.\033[0m\n", newTicket.seatNumber, newTicket.busNumber);
        return;
    }

    // Generate unique ticket ID
    newTicket.ticketID = getNewTicketID();

    // Save ticket details
    ticketFile = fopen("tickets.txt", "a");
    if (!ticketFile) {
        printf("\033[1;31mError: Unable to save ticket details.\033[0m\n");
        return;
    }

    fprintf(ticketFile, "Ticket ID: %d\n", newTicket.ticketID);
    fprintf(ticketFile, "Bus Number: %d\n", newTicket.busNumber);
    fprintf(ticketFile, "Seat Number: %d\n", newTicket.seatNumber);
    fprintf(ticketFile, "Passenger Name: %s\n", newTicket.passengerName);
    fprintf(ticketFile, "---------------------------\n");
    fclose(ticketFile);

    // Update available seats
    updateAvailableSeats(newTicket.busNumber, -1);

    printf("\n\033[1;32mTicket booked successfully! Ticket ID: %d\033[0m\n", newTicket.ticketID);
}


void cancelTicket() {
    int ticketID;
    FILE *ticketFile, *tempFile;
    struct Ticket tempTicket;
    char line[256];
    int found = 0;

    printf("\n\033[1;105m\t\t\t\t      Cancel Ticket       \033[0m\n\n");

    printf("\033[1;46mEnter Ticket ID to Cancel: \033[0m");
    scanf("%d", &ticketID);

    ticketFile = fopen("tickets.txt", "r");
    tempFile = fopen("temp.txt", "w");

    if (!ticketFile || !tempFile) {
        printf("\033[1;31mError: Unable to process ticket cancellation.\033[0m\n");
        return;
    }

    while (fgets(line, sizeof(line), ticketFile)) {
        if (sscanf(line, "Ticket ID: %d", &tempTicket.ticketID) == 1 && tempTicket.ticketID == ticketID) {
            found = 1;
            fgets(line, sizeof(line), ticketFile); // Skip Bus Number
            sscanf(line, "Bus Number: %d", &tempTicket.busNumber);

            fgets(line, sizeof(line), ticketFile); // Skip Seat Number
            sscanf(line, "Seat Number: %d", &tempTicket.seatNumber);

            fgets(line, sizeof(line), ticketFile); // Skip Passenger Name

            fgets(line, sizeof(line), ticketFile); // Skip separator
            updateAvailableSeats(tempTicket.busNumber, 1);
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(ticketFile);
    fclose(tempFile);

    if (found) {
        remove("tickets.txt");
        rename("temp.txt", "tickets.txt");
        printf("\n\033[1;32mTicket cancelled successfully!\033[0m\n");
    } else {
        printf("\033[1;31mError: Ticket ID not found.\033[0m\n");
    }
}

void printTicket() {
    int ticketID;
    FILE *ticketFile;
    struct Ticket tempTicket;
    char line[256];
    int found = 0;

    printf("\n\033[1;105m\t\t\t\t      Print Ticket       \033[0m\n\n");

    printf("\033[1;46mEnter Ticket ID to Print: \033[0m");
    scanf("%d", &ticketID);

    ticketFile = fopen("tickets.txt", "r");
    if (!ticketFile) {
        printf("\033[1;31mError: Unable to open ticket file.\033[0m\n");
        return;
    }

    while (fgets(line, sizeof(line), ticketFile)) {
        if (sscanf(line, "Ticket ID: %d", &tempTicket.ticketID) == 1 && tempTicket.ticketID == ticketID) {
            found = 1;
            printf("\n\033[1;104mTicket Details:\033[0m\n");
            printf("%s", line);
            fgets(line, sizeof(line), ticketFile);
            printf("%s", line);
            fgets(line, sizeof(line), ticketFile);
            printf("%s", line);
            fgets(line, sizeof(line), ticketFile);
            printf("%s", line);
        }
    }

    fclose(ticketFile);

    if (!found) {
        printf("\033[1;31mError: Ticket ID not found.\033[0m\n");
    }

}
void AdminPanel() {
    struct User admin;
    char adminKey[20];
    const char correctAdminKey[] = "securekey123"; // Predefined admin key for security
    int adminChoice;

    // Ask for the admin key for verification
    printf("\033[1;46mEnter the Admin Key:\033[0m ");
    fgets(adminKey, sizeof(adminKey), stdin);
    adminKey[strcspn(adminKey, "\n")] = '\0';

    // Verify the admin key
    if (strcmp(adminKey, correctAdminKey) != 0) {
        printf("\033[1;31mError: Invalid Admin Key.\033[0m\n");
        return;
    }
    

    while (1) {
        // Admin Panel Menu
        printf("\n\n\033[1;31m\t\t\t         Welcome to Admin Panel!\033[0m\n");
        drawBus();
        printf("\n\n\033[1;105m\t\t\t\t   Admin Panel Menu   \033[0m\n");
         printf("\033[1;30m\t\t\t\t  [Enter Your Choice]\033[0m\n\n");
        printf("\033[1;80m[1] Add Bus\033[0m\n");
        printf("\033[1;80m[2] Edit Bus\033[0m\n");
        printf("\033[1;80m[3] Delete Bus\033[0m\n");
        printf("\033[1;80m[4] View All Buses\033[0m\n");
        printf("\033[1;80m[5] Search Bus by Route\033[0m\n");
        printf("\033[1;80m[0] Return to Main Menu\033[0m\n");

        printf("\n\033[1;46mEnter Your Choice:\033[0m ");
        scanf("%d", &adminChoice);
        while (getchar() != '\n'); // Clear input buffer

        switch (adminChoice) {
            case 1:
                addBus();
                break;

            case 2:
                editBus();
                break;

            case 3:
                deleteBus();
                break;

            case 4:
                viewAllBuses();
                break;

            case 5:
                searchBusByRoute();
                break;

            case 0:
                printf("\033[1;32mReturning to the main menu...\033[0m\n");
                return;

            default:
                printf("\033[1;33mInvalid choice. Please try again.\033[0m\n");
        }

        printf("\n\033[1;36mPress any key to continue...\033[0m");
        getch();
    }
}
void UserPanel() {
    int userChoice;

    while (1) {
        printf("\n\n\033[1;31m\t\t\t     Welcome to User Panel!\033[0m\n");
        drawBus();
        printf("\n\n\033[1;105m\t\t\t\t   User Panel Menu   \033[0m\n");
        printf("\033[1;30m\t\t\t\t  [Enter Your Choice]\033[0m\n\n");
        printf("\033[1;80m[1] Registration\033[0m\n");
        printf("\033[1;80m[2] Login\033[0m\n");
        printf("\033[1;80m[3] View All Buses\033[0m\n");
        printf("\033[1;80m[4] Search Bus by route\033[0m\n");
        printf("\033[1;80m[5] Book Ticket\033[0m\n");
        printf("\033[1;80m[6] Print ticket\033[0m\n");
        printf("\033[1;80m[7] Cancel Ticket\033[0m\n");
        printf("\033[1;80m[0] Return to Main Menu\033[0m\n");

        printf("\n\033[1;46mEnter Your Choice:\033[0m ");
        scanf("%d", &userChoice);
        while (getchar() != '\n'); // Clear input buffer

        switch (userChoice) {

            case 1:
                registration();
                break;
            case 2:
                login();
                break;
            case 3:
                viewAllBuses();
                break;
            case 4:
                searchBusByRoute();
                break;

            case 5:
                bookTicket();
                break;

            case 6:
                printTicket();
                break;

            case 7:
                  cancelTicket();
                  break;

            case 0:
                printf("\033[1;32mReturning to the main menu...\033[0m\n");
                return;

            default:
                printf("\033[1;33mInvalid choice. Please try again.\033[0m\n");
        }

        printf("\n\033[1;36mPress any key to continue...\033[0m");
        getch();
    }
}
int main() {
    int choice;

    while (1) {
        printf("\n");
        printf("\033[1;31m\t\t\t      WELCOME TO Ticket Counter\033[0m\n");
        drawBus();
        printf("\n\n\033[1;37m\t\t\t\t     Hello User!\033[0m\n");
        printf("\033[1;37m\t\t\t         Welcome to Our System!\033[0m\n");
        printf("\n\n\033[1;105m\t\t\t\t      Main Menu       \033[0m\n");
        printf("\033[1;30m\t\t\t\t  [Enter Your Choice]\033[0m\n\n");
        printf("\033[1;80m[1] Admin Panel\033[0m\n");
        printf("\033[1;80m[2] User Panel\033[0m\n");
        printf("\033[1;80m[0] Logout\033[0m\n");

        printf("\n\033[1;46mEnter Your Choice:\033[0m ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                AdminPanel(); // Register as Admin
                break;

            case 2:
                UserPanel(); // Register as User
                break;

            case 0:
                printf("\033[1;32mExiting the system. Goodbye!\033[0m\n");
                return 0;

            default:
                printf("\033[1;33mInvalid choice. Try again.\033[0m\n");
        }

        printf("\n\033[1;36mPress any key to continue...\033[0m");
        getch();
    }
}