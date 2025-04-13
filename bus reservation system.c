#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("\n\t\t\t\t\t========== Main Menu ==========\t\t\t\t\t\n\n");
    printf("\t\t\t\t\t\t1. Login\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t2. Register\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t3. Forgot Password\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t4. Exit\t\t\t\t\t\t\n");
    printf("\n\t\t\t\t\t===============================\t\t\t\t\t\n");
    printf("Enter your choice: ");
}

void displayUserMenu() {
    printf("\n\t\t\t\t\t========== User Menu ==========\t\t\t\t\t\n\n");
    printf("\t\t\t\t\t\t1. Book a Ticket\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t2. Cancel a Ticket\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t3. Check Bus Status\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t4. Logout\t\t\t\t\t\t\n");
    printf("\n\t\t\t\t\t===============================\t\t\t\t\t\n");
    printf("Enter your choice: ");
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

    printf("To login with mobile number Press 1... Press 2 to continue with username!!\n");
    scanf("%d", &op);

    if (op == 1) {
        printf("Enter Mobile Number: ");
        scanf("%lld", &mobile);
        printf("Enter Password: ");
        scanf("%s", password);
    } else {
        printf("Enter Username: ");
        scanf("%s", username);
        printf("Enter Password: ");
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
        printf("User limit reached.\n");
        return -1;
    }

    char username[50], password[50];
    long long mobile;
    int i;

    printf("Enter a new username: ");
    scanf("%s", username);

    for (i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Try another one.\n");
            return -1;
        }
    }

    printf("Enter a new password: ");
    scanf("%s", password);
    printf("Enter your mobile number: ");
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

    printf("Enter your registered mobile number to reset password: ");
    scanf("%lld", &mobile);

    for (i = 0; i < numUsers; i++) {
        if (users[i].mobile == mobile) {
            printf("Mobile number found. Enter a new password: ");
            scanf("%s", newPassword);
            strcpy(users[i].password, newPassword);
            printf("Password reset successful. Your new password is: %s\n", newPassword);
            saveUserToFile(users[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Mobile number not found.\n");
    }
}

void bookTicket(struct Bus buses[], int numBuses) {
    int busNumber, seatsToBook;
    int i;

    printf("\nAvailable Buses:\n");
    for (i = 0; i < numBuses; i++) {
        printf("%d -- %s to %s\n", buses[i].busNumber, buses[i].source, buses[i].destination);
    }

    printf("Enter Bus Number: ");
    scanf("%d", &busNumber);

    int index = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Bus not found.\n");
        return;
    }

    printf("Enter number of seats to book: ");
    scanf("%d", &seatsToBook);

    if (seatsToBook > buses[index].availableSeats) {
        printf("Only %d seats available.\n", buses[index].availableSeats);
    } else {
        buses[index].availableSeats -= seatsToBook;
        printf("Booking successful! Total Fare: %.2f\n", buses[index].fare * seatsToBook);
    }
}

void cancelTicket(struct Bus buses[], int numBuses) {
    int busNumber, seatsToCancel;
    int i;

    printf("Enter Bus Number to cancel ticket: ");
    scanf("%d", &busNumber);

    int index = -1;
    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Bus not found.\n");
        return;
    }

    printf("Enter number of seats to cancel: ");
    scanf("%d", &seatsToCancel);

    int bookedSeats = buses[index].totalSeats - buses[index].availableSeats;
    if (seatsToCancel > bookedSeats) {
        printf("You can't cancel more than booked.\n");
    } else {
        buses[index].availableSeats += seatsToCancel;
        printf("Cancellation successful.\n");
    }
}

void checkBusStatus(struct Bus buses[], int numBuses) {
    int busNumber, i;

    printf("Enter Bus Number to check status: ");
    scanf("%d", &busNumber);

    for (i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            printf("\nBus Number: %d\n", buses[i].busNumber);
            printf("Route: %s -> %s\n", buses[i].source, buses[i].destination);
            printf("Total Seats: %d\n", buses[i].totalSeats);
            printf("Available Seats: %d\n", buses[i].availableSeats);
            printf("Fare: %.2f\n", buses[i].fare);
            return;
        }
    }

    printf("Bus not found.\n");
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
                    printf("Login successful. Welcome, %s!\n", users[loggedInUser].username);
                } else {
                    printf("Login failed. Please try again.\n");
                }
            } else if (choice == 2) {
                int regIndex = registerUser(users, numUsers);
                if (regIndex != -1) {
                    numUsers++;
                    printf("Registration successful. Please login to continue.\n");
                }
            } else if (choice == 3) {
                forgotPassword(users, numUsers);
            } else if (choice == 4) {
                printf("Thank you for using the system... Goodbye!\n");
                break;
            } else {
                printf("Invalid choice.\n");
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
                    printf("Logging out...\n");
                    loggedInUser = -1;
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        }
    }

    return 0;
}

