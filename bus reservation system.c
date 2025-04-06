#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Bus {
    int busNumber;
    char source[50];
    char destination[50];
    int totalSeats;
    int availableSeats;
    float fare;
};

struct User {
    char username[50];
    char password[50];
};

void displayMainMenu() {
    printf("\n Main Menu \n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void displayUserMenu() {
    printf("\n User Menu \n");
    printf("1. Book a Ticket\n");
    printf("2. Cancel a Ticket\n");
    printf("3. Check Bus Status\n");
    printf("4. Add a Bus\n");
    printf("5. Logout\n");
    printf("Enter your choice: ");
}

int loginUser(struct User users[], int numUsers, char username[], char password[]) {
	int i;
    for (i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

int registerUser(struct User **users, int *numUsers) {
	int i;
    char username[50], password[50];
    printf("Enter a new username: ");
    scanf("%s", username);
    printf("Enter a new password: ");
    scanf("%s", password);

    for (i = 0; i < *numUsers; i++) {
        if (strcmp((*users)[i].username, username) == 0) {
            printf("Username already exists. Please choose another one.\n");
            return -1;
        }
    }

    struct User *temp = realloc(*users, (*numUsers + 1) * sizeof(struct User));
    if (!temp) {
        printf("Memory allocation failed.\n");
        return -1;
    }
    *users = temp;
    strcpy((*users)[*numUsers].username, username);
    strcpy((*users)[*numUsers].password, password);
    (*numUsers)++;

    printf("Registration successful!\n");
    return *numUsers - 1;
}

void bookTicket(struct Bus buses[], int numBuses) {
	int i;
    int busNumber;
    printf("Available buses\n");
    printf("1. 101 -- City A to City B\n");
    printf("2. 102 -- City C to City D\n");
    printf("3. 103 -- City E to City F\n");
    printf("\nEnter Bus Number: ");
    scanf("%d", &busNumber);

    int busIndex = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex == -1) {
        printf("Bus with Bus Number %d not found.\n", busNumber);
    } else {
        int seatsToBook;
        printf("Enter Number of Seats: ");
        scanf("%d", &seatsToBook);

        if (buses[busIndex].availableSeats < seatsToBook) {
            printf("Sorry, only %d seats are available.\n", buses[busIndex].availableSeats);
        } else {
            buses[busIndex].availableSeats -= seatsToBook;
            float totalFare = buses[busIndex].fare * seatsToBook;
            printf("Booking successful! %d seats booked on Bus Number %d. Total Fare: %.2f\n", seatsToBook, busNumber, totalFare);
        }
    }
}

void cancelTicket(struct Bus buses[], int numBuses) {
	int i;
    int busNumber;
    printf("\nEnter Bus Number: ");
    scanf("%d", &busNumber);

    int busIndex = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex == -1) {
        printf("Bus with Bus Number %d not found.\n", busNumber);
    } else {
        int seatsToCancel;
        printf("Enter Number of Seats to Cancel: ");
        scanf("%d", &seatsToCancel);

        int bookedSeats = buses[busIndex].totalSeats - buses[busIndex].availableSeats;
        if (seatsToCancel > bookedSeats) {
            printf("Error: You can't cancel more seats than were booked.\n");
        } else {
            buses[busIndex].availableSeats += seatsToCancel;
            printf("Cancellation successful! %d seats canceled on Bus Number %d.\n", seatsToCancel, busNumber);
        }
    }
}

void checkBusStatus(struct Bus buses[], int numBuses) {
	int i;
    int busNumber;
    printf("\nEnter Bus Number: ");
    scanf("%d", &busNumber);

    int busIndex = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex != -1) {
        printf("\nBus Number: %d\n", buses[busIndex].busNumber);
        printf("Source: %s\n", buses[busIndex].source);
        printf("Destination: %s\n", buses[busIndex].destination);
        printf("Total Seats: %d\n", buses[busIndex].totalSeats);
        printf("Available Seats: %d\n", buses[busIndex].availableSeats);
        printf("Fare: %.2f\n", buses[busIndex].fare);
    } else {
        printf("Bus with Bus Number %d not found.\n", busNumber);
    }
}

void addBus(struct Bus **buses, int *numBuses) {
    struct Bus *temp = realloc(*buses, (*numBuses + 1) * sizeof(struct Bus));
    if (!temp) {
        printf("Memory allocation failed.\n");
        return;
    }
    *buses = temp;

    printf("\nEnter Bus Number: ");
    scanf("%d", &(*buses)[*numBuses].busNumber);
    printf("Enter Source: ");
    scanf("%s", (*buses)[*numBuses].source);
    printf("Enter Destination: ");
    scanf("%s", (*buses)[*numBuses].destination);
    printf("Enter Total Seats: ");
    scanf("%d", &(*buses)[*numBuses].totalSeats);
    (*buses)[*numBuses].availableSeats = (*buses)[*numBuses].totalSeats;
    printf("Enter Fare: ");
    scanf("%f", &(*buses)[*numBuses].fare);

    (*numBuses)++;
    printf("New bus added successfully!\n");
}

int main() {
    struct User *users = malloc(5 * sizeof(struct User));
    struct User initialUsers[5] = {
        {"user1", "pass1"}, {"user2", "pass2"}, {"user3", "pass3"}, {"user4", "pass4"}, {"user5", "pass5"},
    };
    memcpy(users, initialUsers, 5 * sizeof(struct User));
    int numUsers = 5;

    struct Bus *buses = malloc(3 * sizeof(struct Bus));
    struct Bus initialBuses[3] = {
        {101, "CityA", "CityB", 50, 50, 500.0},
        {102, "CityC", "CityD", 40, 40, 400.0},
        {103, "CityE", "CityF", 30, 30, 300.0},
    };
    memcpy(buses, initialBuses, 3 * sizeof(struct Bus));
    int numBuses = 3;

    int loggedInUserId = -1;

    while (1) {
        if (loggedInUserId == -1) {
            displayMainMenu();
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                char username[50], password[50];
                printf("Enter Username: ");
                scanf("%s", username);
                printf("Enter Password: ");
                scanf("%s", password);

                loggedInUserId = loginUser(users, numUsers, username, password);
                if (loggedInUserId == -1) {
                    printf("Login failed. Please check your username and password.\n");
                } else {
                    printf("Login successful. Welcome, %s!\n", username);
                }
            } else if (choice == 2) {
                loggedInUserId = registerUser(&users, &numUsers);
                if (loggedInUserId != -1) {
                    printf("Login successful. Welcome, %s!\n", users[loggedInUserId].username);
                }
            } else if (choice == 3) {
                printf("Exiting the program.\n");
                break;
            } else {
                printf("Invalid choice. Please try again.\n");
            }
        } else {
            displayUserMenu();
            int userChoice;
            scanf("%d", &userChoice);

            switch (userChoice) {
                case 1:
                    bookTicket(buses, numBuses);
                    break;
                case 2:
                    cancelTicket(buses, numBuses);
                    break;
                case 3:
                    checkBusStatus(buses, numBuses);
                    break;
                case 4:
                    addBus(&buses, &numBuses);
                    break;
                case 5:
                    printf("Logging out.\n");
                    loggedInUserId = -1;
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }

    free(users);
    free(buses);

    return 0;
}

