#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STUDENT_FILE "student.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentroll[10];
char currentuser[50];
char currentrole[10];

/* FUNCTION DECLARATIONS */
int loginsystem();
void mainmenu();
void adminmenu();
void staffmenu();
void guestmenu();
void usermenu();

/* CRUD Operations */
void addstudent();
void displaystudents();
void searchstudent();
void updatestudent();
void deletestudent();
void calculateaverage();  /* NEW FUNCTION - Calculate average marks */

/*-------------------- MAIN --------------- */
int main()
{
    /* Ensure files exist */
    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Creating default credentials.txt...\n");
        fp = fopen(CREDENTIAL_FILE, "w");
        fprintf(fp, "admin admin123 ADMIN\n");
        fprintf(fp, "staff staff123 STAFF\n");
        fprintf(fp, "guest guest123 GUEST\n");
        fprintf(fp, "student student123 USER\n");
        fclose(fp);
    } else {
        fclose(fp);
    }

    fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        fp = fopen(STUDENT_FILE, "w");
        fclose(fp);
    } else {
        fclose(fp);
    }

    if (loginsystem())
    {
        mainmenu();
    }
    else
    {
        printf("\n Access denied. Exiting...\n");
    }

    return 0;
}

int loginsystem()
{
    char username[50], password[50];
    char fileuser[50], filepass[50], filerole[10];

    printf("\n====================================\n");
    printf(" STUDENT MANAGEMENT SYSTEM \n");
    printf("====================================\n");
    printf("===== LOGIN =====\n");
    printf("Username : ");
    scanf("%s", username);
    printf("Password : ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp)
    {
        printf("ERROR : credentials.txt not found! \n");
        return 0;
    }

    int found = 0;
    while (fscanf(fp, "%s %s %s", fileuser, filepass, filerole) == 3)
    {
        if (strcmp(username, fileuser) == 0 && strcmp(password, filepass) == 0)
        {
            strcpy(currentrole, filerole);
            strcpy(currentuser, fileuser);
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}

void mainmenu()
{
    printf("\nLogin Successful! Welcome, %s (%s)\n", currentuser, currentrole);

    if (strcmp(currentrole, "ADMIN") == 0)
        adminmenu();
    else if (strcmp(currentrole, "STAFF") == 0)
        staffmenu();
    else if (strcmp(currentrole, "GUEST") == 0)
        guestmenu();
    else
        usermenu();
}

/*-------------------- CRUD OPERATIONS -------------------*/
void addstudent() {
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "a");

    printf("\n--- Add Student ---\n");
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student added successfully!\n");
}

void displaystudents() {
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    printf("\n--- Student Records ---\n");
    printf("%-10s %-20s %-10s\n", "Roll", "Name", "Marks");
    printf("----------------------------------------\n");

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        printf("%-10d %-20s %-10.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
}

void searchstudent() {
    int roll, found = 0;
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    printf("\n--- Search Student ---\n");
    printf("Enter Roll Number to search: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            printf("\nStudent Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    if (!found) printf("Student with Roll %d not found.\n", roll);
    fclose(fp);
}

void updatestudent() {
    int roll, found = 0;
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("\n--- Update Student ---\n");
    printf("Enter Roll Number to update: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            printf("Current Data: %d %s %.2f\n", s.roll, s.name, s.marks);
            printf("Enter New Name: ");
            scanf("%s", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
            printf("Record updated.\n");
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
    } else {
        remove("temp.txt");
        printf("Student not found.\n");
    }
}

void deletestudent() {
    int roll, found = 0;
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("\n--- Delete Student ---\n");
    printf("Enter Roll Number to delete: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        if (s.roll == roll) {
            printf("Deleting Record: %d %s %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            continue;  /* Skip writing this record to temp */
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Student deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("Student not found.\n");
    }
}


void calculateaverage() {
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    
    if (!fp) {
        printf("\nNo student records found.\n");
        return;
    }
    
    float totalmarks = 0.0;
    int count = 0;
    float average = 0.0;
    
    printf("\n--- Calculate Average Marks ---\n");
    
    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) != EOF) {
        totalmarks += s.marks;
        count++;
    }
    
    fclose(fp);
    
    if (count == 0) {
        printf("No students in the database.\n");
    } else {
        average = totalmarks / count;
        printf("Total Students: %d\n", count);
        printf("Total Marks: %.2f\n", totalmarks);
        printf("Average Marks: %.2f\n", average);
    }
}

/*-------------------- MENUS -------------------*/
void adminmenu()
{
    int choice;
    do
    {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Calculate Average Marks\n");
        printf("7. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: addstudent(); break;
            case 2: displaystudents(); break;
            case 3: searchstudent(); break;
            case 4: updatestudent(); break;
            case 5: deletestudent(); break;
            case 6: calculateaverage(); break;
            case 7: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); break;
        }
    } while (1);
}

void staffmenu()
{
    int choice;
    do
    {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Calculate Average Marks\n");
        printf("5. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: displaystudents(); break;
            case 2: searchstudent(); break;
            case 3: updatestudent(); break;
            case 4: calculateaverage(); break;
            case 5: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); break;
        }
    } while (1);
}

void guestmenu()
{
    int choice;
    do
    {
        printf("\n===== GUEST MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Calculate Average Marks\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: displaystudents(); break;
            case 2: searchstudent(); break;
            case 3: calculateaverage(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); break;
        }
    } while (1);
}

void usermenu()
{
    int choice;
    do
    {
        printf("\n===== STUDENT MENU =====\n");
        printf("1. View All Students (Public Info)\n");
        printf("2. View Average Marks\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: displaystudents(); break;
            case 2: calculateaverage(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); break;
        }
    } while (1);
}
