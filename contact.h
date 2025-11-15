#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[20];
    char phone[10];
    char email[30];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook,int );
void initialize(AddressBook *addressBook);

void saveContactsToFile(AddressBook *addressBook);
void loadContactsFromFile(AddressBook *addressBook);

void print_head(const char *text);
void print_success(const char *text);
void print_error(const char *text);


int validate_name(char name[]);
int validate_phone_no(char phone_no[]);
int validate_email(char email[]);

int isUniquePhone(AddressBook *addressBook,char *phone);
int is_unique_email(AddressBook *addressBook, char email[]);

void  search_by_name(AddressBook *addressBook,char *);
void search_by_phone_no(AddressBook *addressBook);
void search_by_email(AddressBook *addressBook);
#endif
