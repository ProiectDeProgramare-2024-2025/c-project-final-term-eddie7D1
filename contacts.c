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

//clears the terminal screen depending on OS
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//waits for user to press Enter before continuing
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

//reads contact information from the text file and populates the contacts array
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
            sscanf(line, "Phone   : %[^\n]", contacts[contact_count].phone);

            fgets(line, sizeof(line), fp);
            sscanf(line, "Label   : %[^\n]", contacts[contact_count].label);

            fgets(line, sizeof(line), fp);
            sscanf(line, "Company : %[^\n]", contacts[contact_count].company);

            fgets(line, sizeof(line), fp);
            sscanf(line, "Email   : %[^\n]", contacts[contact_count].email);

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
        printf(RED "Contact list is full!\n" RESET);
        enter();
        return;
    }

    clear_screen();
    printf("--- " GREEN "Add Contact" RESET " ---\n");

    printf("Phone " RED "(mandatory, format: +1234567890)" RESET ": ");
    scanf(" %[^\n]", contacts[contact_count].phone);

    printf("Label " RED "(mandatory, e.g., 'Friend', 'Work')" RESET ": ");
    scanf(" %[^\n]", contacts[contact_count].label);

    printf("Company (optional): ");
    scanf(" %[^\n]", contacts[contact_count].company);

    printf("Email (optional, format: user@example.com): ");
    scanf(" %[^\n]", contacts[contact_count].email);

    contact_count++;
    save_contacts();

    printf(GREEN "Contact added successfully.\n" RESET);
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
        printf(RED "No contacts available.\n" RESET);
    }
    enter();
}

//lets the user choose a contact to delete and updates the saved file
void delete_contact() {
    int index;
    clear_screen();
    printf("--- " RED "Delete Contact" RESET " ---\n");
    if (contact_count == 0) {
        printf(RED "No contacts to delete.\n" RESET);
        enter();
        return;
    }

    display_contacts();
    printf("Enter contact number to delete: ");
    scanf("%d", &index);
    if (index < 1 || index > contact_count) {
        printf(RED "Invalid contact number.\n" RESET);
    } else {
        for (int i = index - 1; i < contact_count - 1; i++) {
            contacts[i] = contacts[i + 1];
        }
        contact_count--;
        save_contacts();
        printf(GREEN "Contact deleted.\n" RESET);
    }
    enter();
}

//so far this function does nothing besides placebo, I might even remove it unless I decide
//to do something with it
void block_number() {
    char number[20];
    clear_screen();
    printf("--- " RED "Block Number" RESET " ---\n");
    printf("Enter number to block: ");
    scanf(" %[^\n]", number);
    printf("Number %s has been blocked.\n", number);
    enter();
}

//this searches for a keyword across all contacts and displays the matches if there are any
void search_contacts() {
    char keyword[50];
    clear_screen();
    printf("--- " CYAN "Search Contacts" RESET " ---\n");
    printf("Enter search keyword: ");
    scanf(" %[^\n]", keyword);
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
        printf(RED "No matching contacts found.\n" RESET);
    }
    enter();
}

//displays the menu and runs selected functions in a loop
void submenu() {
    int option;
    do {
        clear_screen();
        printf("=== " GREEN "Contacts Menu" RESET " ===\n");
        printf("1 - " CYAN "Add Contact" RESET "\n");
        printf("2 - " CYAN "Display Contacts" RESET "\n");
        printf("3 - " CYAN "Delete Contact" RESET "\n");
        printf("4 - " CYAN "Block Number" RESET "\n");
        printf("5 - " CYAN "Search Contacts" RESET "\n");
        printf("0 - " CYAN "Exit" RESET "\n");
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
                printf(GREEN "Goodbye, contact master...\n" RESET);
                break;
            default:
                printf(RED "Invalid option.\n" RESET);
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
