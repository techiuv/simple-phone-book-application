#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store contact information
typedef struct {
    char name[50];
    char phone[15];
    char email[50];
} Contact;

// Function prototypes
void addContact();
void listContacts();
void searchContact();
void deleteContact();
void clearBuffer();

// File to store contacts
const char *filename = "phonebook.dat";

int main() {
    int choice;

    while (1) {
        printf("\nPhonebook Application\n");
        printf("1. Add Contact\n");
        printf("2. List Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                listContacts();
                break;
            case 3:
                searchContact();
                break;
            case 4:
                deleteContact();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

void addContact() {
    FILE *fp;
    Contact c;

    fp = fopen(filename, "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Name: ");
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = 0;

    printf("Enter Phone Number: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    c.phone[strcspn(c.phone, "\n")] = 0;

    printf("Enter Email: ");
    fgets(c.email, sizeof(c.email), stdin);
    c.email[strcspn(c.email, "\n")] = 0;

    fwrite(&c, sizeof(Contact), 1, fp);
    fclose(fp);

    printf("Contact added successfully!\n");
}

void listContacts() {
    FILE *fp;
    Contact c;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nContacts List:\n");
    printf("---------------------------------------------------\n");
    while (fread(&c, sizeof(Contact), 1, fp)) {
        printf("Name: %s\n", c.name);
        printf("Phone: %s\n", c.phone);
        printf("Email: %s\n", c.email);
        printf("---------------------------------------------------\n");
    }
    fclose(fp);
}

void searchContact() {
    FILE *fp;
    Contact c;
    char searchName[50];
    int found = 0;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the name of the contact to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    while (fread(&c, sizeof(Contact), 1, fp)) {
        if (strcmp(c.name, searchName) == 0) {
            printf("\nContact Found:\n");
            printf("Name: %s\n", c.name);
            printf("Phone: %s\n", c.phone);
            printf("Email: %s\n", c.email);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Contact not found.\n");
    }

    fclose(fp);
}

void deleteContact() {
    FILE *fp, *tempFp;
    Contact c;
    char deleteName[50];
    int found = 0;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    tempFp = fopen("temp.dat", "wb");
    if (tempFp == NULL) {
        printf("Error opening temporary file.\n");
        fclose(fp);
        return;
    }

    printf("Enter the name of the contact to delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    deleteName[strcspn(deleteName, "\n")] = 0;

    while (fread(&c, sizeof(Contact), 1, fp)) {
        if (strcmp(c.name, deleteName) == 0) {
            found = 1;
            printf("Contact '%s' deleted successfully.\n", c.name);
        } else {
            fwrite(&c, sizeof(Contact), 1, tempFp);
        }
    }

    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove(filename);
        rename("temp.dat", filename);
    } else {
        printf("Contact not found.\n");
        remove("temp.dat");
    }
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
