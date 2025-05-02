#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100
#define FILE_NAME "contacts.txt"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

typedef struct {
    char phone[20];
    char label[50];
    char company[50];
    char email[50];
} Contact;

Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void enter() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

//function to write all current contacts to contacts.txt in a neat and tidy way,
//makes reading the txt easier
void save_contacts() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < contact_count; i++) {
        fprintf(fp, "Contact #%d\n", i + 1);
        fprintf(fp, "Phone   : %s\n", contacts[i].phone);
        fprintf(fp, "Label   : %s\n", contacts[i].label);
        fprintf(fp, "Company : %s\n", contacts[i].company);
        fprintf(fp, "Email   : %s\n", contacts[i].email);
        fprintf(fp, "----------------------\n");
    }

    fclose(fp);
}

void load_contacts() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        return;
    }

    contact_count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Contact", 7) == 0) {
            if (contact_count >= MAX_CONTACTS) break;

            fgets(line, sizeof(line), fp);
            sscanf(line, "Phone   : %[^"]", contacts[contact_count].phone);

            fgets(line, sizeof(line), fp);
            sscanf(line, "Label   : %[^"]", contacts[contact_count].label);

            fgets(line, sizeof(line), fp);
            sscanf(line, "Company : %[^"]", contacts[contact_count].company);

            fgets(line, sizeof(line), fp);
            sscanf(line, "Email   : %[^"]", contacts[contact_count].email);

            fgets(line, sizeof(line), fp);
            contact_count++;
        }
    }

    fclose(fp);
}

//void add_contact() is used to input new contact details
//afterwards it adds the contact to the array and saves it to file
void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Contact list is full!\n");
        enter();
        return;
    }

    clear_screen();
    printf("--- Add Contact ---\n");

    printf("Phone (mandatory, format: +1234567890): ");
    scanf(" %[^"]", contacts[contact_count].phone);

    printf("Label (mandatory, e.g., 'Friend', 'Work'): ");
    scanf(" %[^"]", contacts[contact_count].label);

    printf("Company (optional): ");
    scanf(" %[^"]", contacts[contact_count].company);

    printf("Email (optional, format: user@example.com): ");
    scanf(" %[^"]", contacts[contact_count].email);

    contact_count++;
    save_contacts();

    printf("Contact added successfully.\n");
    enter();
}

//displays the list of all the contacts
void display_contacts() {
    clear_screen();
    printf("--- Contact List ---\n");
    for (int i = 0; i < contact_count; i++) {
        printf(GREEN "Contact #%d\n" RESET, i + 1);
        printf("Phone   : " CYAN "%s\n" RESET, contacts[i].phone);
        printf("Label   : " CYAN "%s\n" RESET, contacts[i].label);
        printf("Company : %s\n", contacts[i].company);
        printf("Email   : " CYAN "%s\n\n" RESET, contacts[i].email);
    }
    if (contact_count == 0) {
        printf("No contacts available.\n");
    }
    enter();
}

//lets the user choose a contact to delete and updates the saved file
void delete_contact() {
    int index;
    clear_screen();
    printf("--- Delete Contact ---\n");
    if (contact_count == 0) {
        printf("No contacts to delete.\n");
        enter();
        return;
    }

    display_contacts();
    printf("Enter contact number to delete: ");
    scanf("%d", &index);
    if (index < 1 || index > contact_count) {
        printf("Invalid contact number.\n");
    } else {
        for (int i = index - 1; i < contact_count - 1; i++) {
            contacts[i] = contacts[i + 1];
        }
        contact_count--;
        save_contacts();
        printf("Contact deleted.\n");
    }
    enter();
}

//so far this function does nothing besides placebo, I might even remove it unless I decide
//to do something with it
void block_number() {
    char number[20];
    clear_screen();
    printf("--- Block Number ---\n");
    printf("Enter number to block: ");
    scanf(" %[^"]", number);
    printf("Number %s has been blocked.\n", number);
    enter();
}

//this searches for a keyword across all contacts and displays the matches if there are any
void search_contacts() {
    char keyword[50];
    clear_screen();
    printf("--- Search Contacts ---\n");
    printf("Enter search keyword: ");
    scanf(" %[^"]", keyword);
    int found = 0;
    for (int i = 0; i < contact_count; i++) {
        if (strstr(contacts[i].phone, keyword) || strstr(contacts[i].label, keyword) ||
            strstr(contacts[i].company, keyword) || strstr(contacts[i].email, keyword)) {
            printf(GREEN "Contact #%d\n" RESET, i + 1);
            printf("Phone   : " CYAN "%s\n" RESET, contacts[i].phone);
            printf("Label   : " CYAN "%s\n" RESET, contacts[i].label);
            printf("Company : %s\n", contacts[i].company);
            printf("Email   : " CYAN "%s\n\n" RESET, contacts[i].email);
            found = 1;
        }
    }
    if (!found) {
        printf("No matching contacts found.\n");
    }
    enter();
}

void submenu() {
    int option;
    do {
        clear_screen();
        printf("=== Contacts Menu ===\n");
        printf("1 - Add Contact\n");
        printf("2 - Display Contacts\n");
        printf("3 - Delete Contact\n");
        printf("4 - Block Number\n");
        printf("5 - Search Contacts\n");
        printf("0 - Exit\n");
        printf("=====================\n");
        printf("Select an option: ");
        scanf("%d", &option);

        switch (option) {
        case 1: add_contact(); break;
        case 2: display_contacts(); break;
        case 3: delete_contact(); break;
        case 4: block_number(); break;
        case 5: search_contacts(); break;
        case 0:
            clear_screen();
            printf("Goodbye, contact master...\n");
            break;
        default:
            printf("Invalid option.\n");
            enter();
        }
    } while (option != 0);
}

//loads contacts from the file when the program starts then displays the submenu function
int main() {
    load_contacts();
    submenu();
    return 0;
}
