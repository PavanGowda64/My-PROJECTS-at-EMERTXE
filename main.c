/*-----------------------------------------------------------------------------------------------------
Name:Pavan Gowda A.Y
Date:18-08-2025
Batch:17A
Name of the project:ADRESSBOOK
-------------------------------------------------------------------------------------------------------*/


#include <stdio.h>
#include "contact.h"

int main() {
    int choice,sortcriteria;
    AddressBook addressBook;
    addressBook.contactCount = 0;
    initialize(&addressBook); // Initialize the address book
    loadContactsFromFile(&addressBook);//Load contacts fn
    do {
       m: 
        print_head("\n=========== Address Book Menu ===========");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");		
        printf("7. Exit\n");
        print_head("Enter your choice: ");
        if(scanf("%d", &choice) != 1) 
        {
            print_error("Invalid input... Please enter a number only.\n");
            while (getchar() != '\n');
            goto m;
        }
        
        switch (choice) {
            case 1:
                createContact(&addressBook);   //Create contacts
                break;
            case 2:
                searchContact(&addressBook);   //Search contacts
                break;
            case 3:
                editContact(&addressBook);     //Edit contacts
                break;
            case 4:
                deleteContact(&addressBook);    //Delete contact
                break;
            case 5:          
                listContacts(&addressBook,sortcriteria);    //List contact
                break;
            case 6:
                print_success("Saved..\n");
                saveContactsToFile(&addressBook);//save contacts fn
                break;
            case 7:
                    saveContactsToFile(&addressBook);//save contacts fn
                    print_success("Bye...Have a nice day !!");
                    
                    return 0;
                    
            default:
                print_error("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
    
       return 0;
}
