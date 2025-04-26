#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Added for date and time

#define MAX_USERS 100
#define MAX_BUSES 100

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
    long long mobile;
};

void displayMainMenu() {
    printf("\n\t\t\t\t\t================ MAIN MENU ================\n\n\n");
    printf("\t\t\t\t\t\t1. LOGIN TO YOUR EXISTING ACCOUNT\n\n");
    printf("\t\t\t\t\t\t2. REGISTER A NEW ACCOUNT\n\n");
    printf("\t\t\t\t\t\t3. FORGOT PASSWORD\n\n");
    printf("\t\t\t\t\t\t4. EXIT\n\n");
    printf("\n\t\t\t\t\t=========================================\n\n");
    printf("ENTER YOUR CHOICE: ");
}

void displayUserMenu() {
    printf("\n\t\t\t\t\t========== USER MENU ==========\n\n");
    printf("\t\t\t\t\t\t1. BOOK A TICKET\n\n");
    printf("\t\t\t\t\t\t2. CANCEL A TICKET\n\n");
    printf("\t\t\t\t\t\t3. CHECK BUS STATUS\n\n");
    printf("\t\t\t\t\t\t4. LOGOUT\n\n");
    printf("\n\t\t\t\t\t===============================\n\n");
    printf("ENTER YOUR CHOICE: ");
}

void saveUserToFile(struct User user) {
    FILE *fp = fopen("users.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%s %s %lld\n", user.username, user.password, user.mobile);
        fclose(fp);
    }
}

int loadUsersFromFile(struct User users[]) {
    FILE *fp = fopen("users.txt", "r");
    int count = 0;
    if (fp != NULL) {
        while (fscanf(fp, "%s %s %lld", users[count].username, users[count].password, &users[count].mobile) == 3) {
            count++;
            if (count >= MAX_USERS)
                break;
        }
        fclose(fp);
    }
    return count;
}

int loginUser(struct User users[], int numUsers) {
    char username[50], password[50];
    long long mobile = 0;
    int op, i;

    printf("TO LOGIN WITH MOBILE NUMBER PRESS 1... PRESS 2 TO CONTINUE WITH USERNAME!!\n\n");
    scanf("%d", &op);

    if (op == 1) {
        printf("ENTER YOUR MOBILE NUMBER: ");
        scanf("%lld", &mobile);
        printf("ENTER PASSWORD: ");
        scanf("%s", password);
    } else {
        printf("ENTER USERNAME: ");
        scanf("%s", username);
        printf("ENTER PASSWORD: ");
        scanf("%s", password);
    }

    for (i = 0; i < numUsers; i++) {
        if (((op == 1 && users[i].mobile == mobile) || (op == 2 && strcmp(users[i].username, username) == 0)) &&
            strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

int registerUser(struct User users[], int numUsers) {
    if (numUsers >= MAX_USERS) {
        printf("USER LIMIT REACHED.\n");
        return -1;
    }

    char username[50], password[50];
    long long mobile;
    int i;

    printf("ENTER A NEW USERNAME: ");
    scanf("%s", username);

    for (i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("USERNAME ALREADY EXISTS. TRY ANOTHER ONE...\n");
            return -1;
        }
    }

    printf("ENTER A NEW PASSWORD: ");
    scanf("%s", password);
    printf("ENTER YOUR MOBILE NUMBER: ");
    scanf("%lld", &mobile);

    strcpy(users[numUsers].username, username);
    strcpy(users[numUsers].password, password);
    users[numUsers].mobile = mobile;

    saveUserToFile(users[numUsers]);

    return numUsers;
}

void forgotPassword(struct User users[], int numUsers) {
    long long mobile;
    char newPassword[50];
    int i, found = 0;

    printf("ENTER YOUR REGISTERED MOBILE NUMBER TO RESET PASSWORD: ");
    scanf("%lld", &mobile);

    for (i = 0; i < numUsers; i++) {
        if (users[i].mobile == mobile) {
            printf("ENTER A NEW PASSWORD: ");
            scanf("%s", newPassword);
            strcpy(users[i].password, newPassword);
            printf("PASSWORD RESET SUCCESSFUL. YOUR NEW PASSWORD IS: %s\n", newPassword);
            saveUserToFile(users[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("MOBILE NUMBER NOT FOUND.\n");
    }
}

void bookTicket(struct Bus buses[], int numBuses, struct User currentUser) {
    int busNumber, seatsToBook;
    int i;

    printf("\nAVAILABLE BUSES:\n");
    for (i = 0; i < numBuses; i++) {
        printf("%d -- %s to %s\n", buses[i].busNumber, buses[i].source, buses[i].destination);
    }

    printf("ENTER BUS NUMBER: ");
    scanf("%d", &busNumber);

    int index = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("BUS NOT FOUND.\n");
        return;
    }

    printf("ENTER NUMBER OF SEATS TO BOOK: ");
    scanf("%d", &seatsToBook);

    if (seatsToBook > buses[index].availableSeats) {
        printf("ONLY %d SEATS AVAILABLE.\n", buses[index].availableSeats);
    } else {
        buses[index].availableSeats -= seatsToBook;
        printf("BOOKING SUCCESSFUL!\n");

        // --- Create Receipt ---
        FILE *fp = fopen("ticket.txt", "w");
        if (fp == NULL) {
            printf("ERROR GENERATING RECEIPT.\n");
            return;
        }

        time_t now = time(NULL);
        struct tm *local = localtime(&now);

        // Centering helper
        char *spaces = "                          "; // 26 spaces for centering

        fprintf(fp, "\n%s********** TICKET RECEIPT **********\n", spaces);
        fprintf(fp, "%sNAME           : %s\n", spaces, currentUser.username);
        fprintf(fp, "%sMOBILE NUMBER  : %lld\n", spaces, currentUser.mobile);
        fprintf(fp, "%sBUS NUMBER     : %d\n", spaces, buses[index].busNumber);
        fprintf(fp, "%sROUTE          : %s -> %s\n", spaces, buses[index].source, buses[index].destination);
        fprintf(fp, "%sSEATS BOOKED   : %d\n", spaces, seatsToBook);
        fprintf(fp, "%sTOTAL FARE     : %.2f\n", spaces, buses[index].fare * seatsToBook);
        fprintf(fp, "%sDATE           : %02d-%02d-%04d\n", spaces, local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
        fprintf(fp, "%sTIME           : %02d:%02d:%02d\n", spaces, local->tm_hour, local->tm_min, local->tm_sec);
        fprintf(fp, "%s************************************\n", spaces);

        fclose(fp);

        // --- Open New Window to Display the Ticket ---
        system("start cmd /k type ticket.txt && pause");
    }
}

void cancelTicket(struct Bus buses[], int numBuses) {
    int busNumber, seatsToCancel;
    int i;

    printf("ENTER BUS NUMBER TO CANCEL TICKET: ");
    scanf("%d", &busNumber);

    int index = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("BUS NOT FOUND.\n");
        return;
    }

    printf("ENTER NUMBER OF SEATS TO CANCEL: ");
    scanf("%d", &seatsToCancel);

    int bookedSeats = buses[index].totalSeats - buses[index].availableSeats;
    if (seatsToCancel > bookedSeats) {
        printf("YOU CANNOT CANCEL MORE THAN BOOKED\n");
    } else {
        buses[index].availableSeats += seatsToCancel;
        printf("CANCELLATION SUCCESSFUL.\n");
    }
}

void checkBusStatus(struct Bus buses[], int numBuses) {
    int busNumber, i;

    printf("ENTER A BUS NUMBER TO CHECK STATUS: ");
    scanf("%d", &busNumber);

    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            printf("\nBUS NUMBER: %d\n", buses[i].busNumber);
            printf("ROUTE: %s -> %s\n", buses[i].source, buses[i].destination);
            printf("TOTAL SEATS: %d\n", buses[i].totalSeats);
            printf("AVAILABLE SEATS: %d\n", buses[i].availableSeats);
            printf("FARE: %.2f\n", buses[i].fare);
            return;
        }
    }

    printf("BUS NOT FOUND.\n");
}

int main() {
    struct User users[MAX_USERS];
    int numUsers = loadUsersFromFile(users);

    struct Bus buses[MAX_BUSES];
    int numBuses = 3;

    buses[0] = (struct Bus){101, "CityA", "CityB", 50, 50, 500.0};
    buses[1] = (struct Bus){102, "CityC", "CityD", 40, 40, 400.0};
    buses[2] = (struct Bus){103, "CityE", "CityF", 30, 30, 300.0};

    int loggedInUser = -1;

    while (1) {
        if (loggedInUser == -1) {
            displayMainMenu();
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                loggedInUser = loginUser(users, numUsers);
                if (loggedInUser != -1) {
                    printf("LOGIN SUCCESSFUL. WELCOME %s!\n", users[loggedInUser].username);
                } else {
                    printf("LOGIN FAILED. PLEASE TRY AGAIN.\n");
                }
            } else if (choice == 2) {
                int regIndex = registerUser(users, numUsers);
                if (regIndex != -1) {
                    numUsers++;
                    printf("REGISTRATION SUCCESSFUL. PLEASE LOGIN TO CONTINUE.\n");
                }
            } else if (choice == 3) {
                forgotPassword(users, numUsers);
            } else if (choice == 4) {
                printf("THANK YOU FOR USING THE SYSTEM... GOODBYE!\n");
                break;
            } else {
                printf("INVALID CHOICE.\n");
            }
        } else {
            displayUserMenu();
            int userChoice;
            scanf("%d", &userChoice);

            switch (userChoice) {
                case 1:
                    bookTicket(buses, numBuses, users[loggedInUser]);
                    break;
                case 2:
                    cancelTicket(buses, numBuses);
                    break;
                case 3:
                    checkBusStatus(buses, numBuses);
                    break;
                case 4:
                    printf("LOGGING OUT...\n");
                    loggedInUser = -1;
                    break;
                default:
                    printf("INVALID CHOICE.\n");
            }
        }
    }

    return 0;
}

