#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Group Members: Izhan & Naqi
// Project: Commercial Building Management System
struct Account {
    char user[30];
    char pass[30];
    char type[10]};
struct Room {
    int id;
    int floor;
    int size;
    float price;
    int status;};
struct BookLog {
    char cName[30];
    int rId;
    float rentAmt;
    float secDeposit;
};
char myType[10];
char myName[30];
// Functions List
void signup();
int login();
void managerDashboard();
void clientDashboard();
void addOfficeSpace();
void showOfficeSpaces();
void matchAndBook();
void showAllBookings();
int main() {
    int option;
    while (1) {printf("\n--- COMMERCIAL BUILDING MANAGEMENT SYSTEM ---\n");
        printf("1. Sign Up (Register)\n");
        printf("2. Sign In (Login)\n");
        printf("3. Exit Program\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        if (option == 1) {
            signup();
        }
        else if (option == 2) {
            if (login() == 1) {
                if (strcmp(myType, "manager") == 0) {
                    managerDashboard();
                } else {
                    clientDashboard();
                }
            }
        } 
        else if (option == 3) {
            printf("Exiting... Goodbye!\n");
            break;
        }
        else {
            printf("Invalid option! Please try again.\n");
        }
    }
    return 0;
}
// 1. Sign Up Function
void signup() {
    FILE *fp = fopen("accounts.txt", "a");
    if (fp == NULL) {
        printf("File Error!\n");
        return;
    }
    struct Account acc;
    int tChoice;
    printf("\n--- SIGN UP ---\n");
    printf("Enter Username (No spaces): ");
    scanf("%s", acc.user);
    printf("Enter Password: ");
    scanf("%s", acc.pass);
    printf("Select Role:\n1. Manager\n2. Client\nChoice (1-2): ");
    scanf("%d", &tChoice);
    if (tChoice == 1) {
        strcpy(acc.type, "manager");
    } else {
        strcpy(acc.type, "client");
    }
    fprintf(fp, "%s %s %s\n", acc.user, acc.pass, acc.type);
    fclose(fp);
    printf("Account created successfully as %s!\n", acc.type);
}
// 2. Login Function
int login() {
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL) {
        printf("No accounts found. Please Sign Up first!\n");
        return 0;
    }
    char uInput[30], pInput[30];
    struct Account temp;
    int success = 0;
    printf("\n--- SIGN IN ---\n");
    printf("Username: ");
    scanf("%s", uInput);
    printf("Password: ");
    scanf("%s", pInput);
    while (fscanf(fp, "%s %s %s", temp.user, temp.pass, temp.type) != EOF) {
        if (strcmp(temp.user, uInput) == 0 && strcmp(temp.pass, pInput) == 0) {
            success = 1;
            strcpy(myType, temp.type);
            strcpy(myName, temp.user);
            break;
        }
    }
    fclose(fp);
    if (success == 1) {
        printf("Login Successful! Welcome, %s.\n", myName);
        return 1;
    } else {
        printf("Invalid Username or Password!\n");
        return 0;
    }
}
// 3. Manager Dashboard
void managerDashboard() {
    int choice;
    while (1) {
        printf("\n--- MANAGER DASHBOARD ---\n");
        printf("1. Add New Office Space\n");
        printf("2. View All Office Spaces\n");
        printf("3. View All Booking Records\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            addOfficeSpace();
        } else if (choice == 2) {
            showOfficeSpaces();
        } else if (choice == 3) {
            showAllBookings();
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid input!\n");
        }
    }
}
// 4. Client Dashboard
void clientDashboard() {
    int choice;
    while (1) {
        printf("\n--- CLIENT DASHBOARD ---\n");
        printf("1. View Available Spaces\n");
        printf("2. Request Office Booking (Matching)\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            showOfficeSpaces();
        } else if (choice == 2) {
            matchAndBook();
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid input!\n");
        }
    }
}
// 5. Add Office (Manager Option)
void addOfficeSpace() {
    FILE *fp = fopen("building_data.txt", "a");
    if (fp == NULL) {
        printf("File Error!\n");
        return;
    }
    struct Room r;
    printf("\nEnter Office ID: ");
    scanf("%d", &r.id);
    printf("Enter Floor Number: ");
    scanf("%d", &r.floor);
    printf("Enter Space Size (Sq Ft): ");
    scanf("%d", &r.size);
    printf("Enter Monthly Rent Price: ");
    scanf("%f", &r.price);
    r.status = 1;
    fprintf(fp, "%d %d %d %.2f %d\n", r.id, r.floor, r.size, r.price, r.status);
    fclose(fp);
    printf("Office Space added successfully!\n");
}
// 6. View Offices
void showOfficeSpaces() {
    FILE *fp = fopen("building_data.txt", "r");
    if (fp == NULL) {
        printf("\nNo office records found.\n");
        return;
    }
    struct Room r;
    printf("\n--- OFFICE SLOTS INVENTORY ---\n");
    printf("ID\tFloor\tSize(SqFt)\tRent\tStatus\n");
    printf("---------------------------------------------\n");
    while (fscanf(fp, "%d %d %d %f %d", &r.id, &r.floor, &r.size, &r.price, &r.status) != EOF) {
        printf("%d\t%d\t%d\t\t%.2f\t", r.id, r.floor, r.size, r.price);
        if (r.status == 1) {
            printf("Available\n");
        } else {
            printf("Booked\n");
        }
    }
    fclose(fp);
}
// 7. Core Logic: Requirement Matching, Notification, & File Rewrite
void matchAndBook() {
    FILE *fp = fopen("building_data.txt", "r");
    if (fp == NULL) {
        printf("No offices registered in the system yet.\n");
        return;
    }
    int fReq, sReq;
    printf("\n--- FIND OFFICE SPACE ---\n");
    printf("Enter Your Preferred Floor: ");
    scanf("%d", &fReq);
    printf("Enter Minimum Size Required (Sq Ft): ");
    scanf("%d", &sReq);
    struct Room list[100]; 
    int total = 0;
    int matchFound = 0;
    int targetIndex = -1;
    while (fscanf(fp, "%d %d %d %f %d", &list[total].id, &list[total].floor, &list[total].size, &list[total].price, &list[total].status) != EOF) {
        if (list[total].floor == fReq && list[total].size >= sReq && list[total].status == 1) {
            if (matchFound == 0) {
                matchFound = 1;
                targetIndex = total; 
            }
        }
        total++;
    }
    fclose(fp);
    // If match found
    if (matchFound == 1) {
        printf("\n=========================================\n");
        printf("!!! BOOKING CONFIRMATION NOTIFICATION !!!\n");
        printf("=========================================\n");
        printf("Match Found! Office ID %d on Floor %d is allocated to you.\n", list[targetIndex].id, list[targetIndex].floor);
        float rent = list[targetIndex].price;
        float security = rent * 2;
        float totalAmount = rent + security;
        printf("\n--- BILLING & RECEIPT DETAILS ---\n");
        printf("Monthly Rent: %.2f\n", rent);
        printf("Security Deposit (2 Months): %.2f\n", security);
        printf("Total Amount Collected: %.2f\n", totalAmount);
        printf("=========================================\n");
        list[targetIndex].status = 0;
        fp = fopen("building_data.txt", "w");
        for (int i = 0; i < total; i++) {
            fprintf(fp, "%d %d %d %.2f %d\n", list[i].id, list[i].floor, list[i].size, list[i].price, list[i].status);
        }
        fclose(fp);
        FILE *logFp = fopen("logs.txt", "a");
        fprintf(logFp, "%s %d %.2f %.2f\n", myName, list[targetIndex].id, rent, security);
        fclose(logFp);
    } else {
        printf("\nSorry, no office space matches your requirements at this moment.\n");
    }
}
// 8. View Bookings (Manager Only)
void showAllBookings() {
    FILE *fp = fopen("logs.txt", "r");
    if (fp == NULL) {
        printf("\nNo booking transaction records found.\n");
        return;
    }
    struct BookLog b;
    printf("\n--- TRANSACTION & BOOKING LOGS ---\n");
    printf("Client\t\tOfficeID\tRent\t\tDeposit\n");
    printf("-----------------------------------------------------------\n");
    while (fscanf(fp, "%s %d %f %f", b.cName, &b.rId, &b.rentAmt, &b.secDeposit) != EOF) {
        printf("%s\t\t%d\t\t%.2f\t\t%.2f\n", b.cName, b.rId, b.rentAmt, b.secDeposit);
    }
    fclose(fp);
}
