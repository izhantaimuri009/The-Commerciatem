#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct login_data {
char user[30];
char pass[30];
char role[15]; };

struct office {
int id;
int floor;
int size;
float rent;
int status;};

struct log_sheet {
char c_name[30];
int o_id;
float r_amt;
float deposit;};
char cur_role[15];
char cur_user[30];
void signup();
int login();
void manager_menu();
void client_menu();
void add_office();
void view_offices();
void check_and_book();
void view_logs();

int main() {
int choice;

while (1) {
printf("\n WELCOME TO Commercial Building Management System \n");
printf("1.Register\n");
printf("2.Login\n");
printf("3.Exit\n");
printf("Enter Choice: ");
scanf("%d", &choice);

if(choice == 1) {
signup();} 
else if (choice == 2) {
if(login() == 1) {
if(strcmp(cur_role, "manager") == 0) {
manager_menu();}
else {
client_menu();}}} 
else if (choice == 3) {
printf("CLOSING...\n");
break;} 
else {
printf("wrong option! try again.\n");}}
return 0;}

void signup(){
FILE *f1 = fopen("accounts.txt", "a");
if (f1 == NULL) {
printf("error opening file\n");
return;}

struct login_data d;
int r_ch;
printf("\n--- REGISTER NOW ---\n");
printf("username: ");
scanf("%s", d.user);
printf("password: ");
scanf("%s", d.pass);
printf("1. Manager\n2. Client\nchoose role: ");
scanf("%d", &r_ch);
if(r_ch == 1) {
strcpy(d.role, "manager");}
else {
strcpy(d.role, "client");}
fprintf(f1, "%s %s %s\n", d.user, d.pass, d.role);
fclose(f1);
printf("done! account created.\n");}

int login() {
FILE *f1 = fopen("accounts.txt", "r");
if (f1 == NULL) {
printf("No accounts yet. signup first.\n");
return 0;}

char u[30], p[30];
struct login_data temp;
int found = 0;
printf("\n--- LOGIN ---\n");
printf("Username: ");
scanf("%s", u);
printf("Password: ");
scanf("%s", p);
while (fscanf(f1, "%s %s %s", temp.user, temp.pass, temp.role) != EOF) {
if (strcmp(temp.user, u) == 0 && strcmp(temp.pass, p) == 0) {
found = 1;
strcpy(cur_role, temp.role);
strcpy(cur_user, temp.user);
break;}}
fclose(f1);
if (found == 1) {
printf("login pass! welcome %s\n", cur_user);
return 1;}
else {
printf("invalid login creditentials!\n");
return 0;}}

void manager_menu() {
int choice;
while (1) {
printf("\n--- MANAGER AREA ---\n");
printf("1. Add Office Slot\n");
printf("2. View All Slots\n");
printf("3. View Bookings\n");
printf("4. Logout\n");
printf("choice: ");
scanf("%d", &choice);

if (choice == 1) {
add_office();}
else if (choice == 2) {view_offices();}
else if (choice == 3) {
view_logs();}
else if(choice == 4){
break;}
else{
printf("invalid input\n");}}}
void client_menu(){
int choice;

while (1) {
printf("\n--- CLIENT AREA ---\n");
printf("1. View Available Offices\n");
printf("2. Search & Book Office\n");
printf("3. Logout\n");
printf("choice: ");
scanf("%d", &choice);
if (choice == 1) {
view_offices();}
else if (choice == 2) {
check_and_book();}
else if (choice == 3) {
break;}
else {
printf("invalid input\n");}}}
void add_office() {
FILE *f2 = fopen("building_data.txt", "a");
if (f2 == NULL) {
printf("file error\n");
return;}
struct office o;
printf("\nOffice ID: ");
scanf("%d", &o.id);
printf("Floor Number: ");
scanf("%d", &o.floor);
printf("Size (Sq Ft): ");
scanf("%d", &o.size);
printf("Monthly Rent: ");
scanf("%f", &o.rent);
o.status = 1;
fprintf(f2, "%d %d %d %.2f %d\n", o.id, o.floor, o.size, o.rent, o.status);
fclose(f2);
printf("office space saved successfully\n");}
void view_offices() {
FILE *f2 = fopen("building_data.txt", "r");
if (f2 == NULL) {
printf("\nno records.\n");
return;}
int ch = fgetc(f2);
if (ch == EOF) {
printf("\nNo offices registered yet!\n");
fclose(f2);
return;}
rewind(f2);
struct office o;
printf("\nID\tFloor\tSize\tRent\tStatus\n");
printf("----------------------------------------\n"); 
while (fscanf(f2, "%d %d %d %f %d", &o.id, &o.floor, &o.size, &o.rent, &o.status) != EOF) {
printf("%d\t%d\t%d\t%.2f\t", o.id, o.floor, o.size, o.rent);
if (o.status == 1) {
printf("Available\n");}
else {
printf("Booked\n");}}
fclose(f2);}
void check_and_book() {
FILE *f2 = fopen("building_data.txt", "r");
if (f2 == NULL) {
printf("no building data found\n");
return;}
int f, s;
printf("\nEnter Floor Preference: ");
scanf("%d", &f);
printf("Enter Min Size Required: ");
scanf("%d", &s);
struct office arr[100]; 
int count = 0;
int match = 0;
int idx = -1;
while (fscanf(f2, "%d %d %d %f %d", &arr[count].id, &arr[count].floor, &arr[count].size, &arr[count].rent, &arr[count].status) != EOF) {
if (arr[count].floor == f && arr[count].size >= s && arr[count].status == 1) {
if (match == 0) {
match = 1;
idx = count; }}
count++;}
fclose(f2);
if (match == 1) {
char confirm;
printf("\nMatch Found! Office ID %d on Floor %d is available.\n", arr[idx].id, arr[idx].floor);
printf("Do you want to confirm this booking? (y/n): ");
fflush(stdin);
scanf(" %c", &confirm);
if (confirm == 'y' || confirm == 'Y') {
printf("\n!!! BOOKING CONFIRMATION NOTIFICATION !!!\n");
float rt = arr[idx].rent;
float sec = rt * 2;
float total = rt + sec;
printf("\n--- BILLING DETAILS ---\n");
printf("Rent: %.2f\n", rt);
printf("Security Deposit: %.2f\n", sec);
printf("Total Collected: %.2f\n", total);
arr[idx].status = 0;
f2 = fopen("building_data.txt", "w");
for (int i = 0; i < count; i++) {
fprintf(f2, "%d %d %d %.2f %d\n", arr[i].id, arr[i].floor, arr[i].size, arr[i].rent, arr[i].status);}
fclose(f2);
FILE *f3 = fopen("logs.txt", "a");
fprintf(f3, "%s %d %.2f %.2f\n", cur_user, arr[idx].id, rt, sec);
fclose(f3);
printf("\nBooking successfully processed!\n");}
else {
printf("\nBooking cancelled by user.\n");}}
else {
printf("\n NOT AVAILABLE \n");}}
void view_logs() {
FILE *f3 = fopen("logs.txt", "r");
if (f3 == NULL) {
printf("\nno transactions logs.\n");
return;}
struct log_sheet l;
printf("\nclient\tOfficeID\tRent\tDeposit\n");
printf("----------------------------------------\n");
while (fscanf(f3, "%s %d %f %f", l.c_name, &l.o_id, &l.r_amt, &l.deposit) != EOF) {
printf("%s\t%d\t\t%.2f\t%.2f\n", l.c_name, l.o_id, l.r_amt, l.deposit);}
fclose(f3);}
