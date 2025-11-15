#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"  //Contains all fn declarations
#include "file.h"  //contains load & save fn
#include "populate.h" //contains all  dummy contacts
#include <ctype.h>
#include "colors.h" //to implement user defined colored texts

                                                    /*THE MAIN FUNCTIONS */

void listContacts(AddressBook *addressBook, int sortCriteria)           //List contact
{

    // If sortCriteria not passed or invalid, ask the user
    if (sortCriteria < 1 || sortCriteria > 3)
    {
        printf("\nChoose sorting criteria:\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        print_head("Enter choice: ");
        scanf("%d", &sortCriteria);
    }

    Contact temp;
    int i, j;

    // Bubble sort
    for (i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            int cmp = 0;

            if (sortCriteria == 1) // Sort by Name
                cmp = strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name);
            else if (sortCriteria == 2) // Sort by Phone
                cmp = strcasecmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone);
            else if (sortCriteria == 3) // Sort by Email
                cmp = strcasecmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email);
            else
            {
                print_error("Invalid sort criteria.\n");
                return;
            }

            if (cmp > 0) // Swap if out of order
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }

    // Display sorted contacts in table format
    printf("-------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-30s\n", "S.No", "Name", "Phone", "Email");
    printf("-------------------------------------------------------------------------------\n");

    for (i = 0; i < addressBook->contactCount; i++)
    {
        printf("%-5d %-20s %-15s %-30s\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    printf("-------------------------------------------------------------------------------\n");
}

void initialize(AddressBook *addressBook)            //Dummy contacts for testing
{
    addressBook->contactCount = 0;
    populateAddressBook(addressBook); // Dummy contacts

    loadContactsFromFile(addressBook); // Load contacts from file
}

void saveAndExit(AddressBook *addressBook)          //saving contacts fn
{
    saveContactsToFile(addressBook); // Save contacts to file

    exit(EXIT_SUCCESS); // Exit the program
}

void createContact(AddressBook *addressBook)        //Creating contacts fn
{
    char name[50], email[20], phone_no[50];
    int add_more = 1;

    while (add_more)
    {
        // NAME
        while (1)
        {
            printf("Enter the name: ");
            scanf(" %20[^\n]", name); // read upto 20 charecters

            if (validate_name(name))
            {
                print_success("Name added successfully");
                break;
            }
            else
            {
                print_error("Invalid name. Please try again.");
            }
        }

        // PHONE
        while (1)
        {
            printf("Enter the Phone no: ");
            scanf(" %11[^\n]", phone_no);

            if (!validate_phone_no(phone_no))
            {
                print_error("Invalid phone number it must be exact 10 digits. Try again.");
                continue;
            }
            if (!isUniquePhone(addressBook, phone_no))
            {
                print_error("Phone number already exists. Try another.");
                continue;
            }
            print_success("Phone number added successfully");
            break;
        }

        // EMAIL
        while (1)
        {
            printf("Enter E-mail: ");
            scanf(" %19[^\n]", email);

            if (!validate_email(email))
            {
                print_error("Invalid email format check these correct formats->abc@xyz.com  or user123@domain.in Try again.");
                continue;
            }
            if (!is_unique_email(addressBook, email))
            {
                print_error("Email already exists. Try another.");
                continue;
            }
            print_success("Email added successfully");
            break;
        }

        // Store contact
        strcpy(addressBook->contacts[addressBook->contactCount].name, name);
        strcpy(addressBook->contacts[addressBook->contactCount].phone, phone_no);
        strcpy(addressBook->contacts[addressBook->contactCount].email, email);
        addressBook->contactCount++;

        // Ask userif more contacts should be added
        print_info("Do you want to add one more contact? (1 = YES, 0 = NO): ");
        scanf("%d", &add_more);
    }
}

void searchContact(AddressBook *addressBook)        //Search Contact fn
{
    int search_choice;
    print_head("\nChoose searching criteria:\n"); // Choosing  criterias
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter choice: ");
    scanf("%d", &search_choice);

    switch (search_choice)
    {
    case 1:
        char name[20];
        search_by_name(addressBook, name);
        break;

    case 2:
        search_by_phone_no(addressBook);
        break;

    case 3:
        search_by_email(addressBook);
        break;
    }
}

/*void editContact(AddressBook *addressBook)          //Edit contact fn
{
    char old_name[20], new_name[20];
    int match_indices[100], match_count = 0, count = 0, i = 0;

    printf("Enter the name to edit: ");
    scanf(" %20[^\n]", old_name);

    // Collect all matching indices
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasecmp(old_name, addressBook->contacts[i].name) == 0)
        {
            match_indices[match_count++] = i;
        }
    }

    if (match_count == 0)
    {
        print_error("Name not found");
        return;
    }

    // If multiple matches, ask user to choose
    if (match_count > 1)
    {
        printf("\nMultiple contacts found:\n");
        printf("---------------------------------------------------------------\n");
        printf(BLUE "%-5s %-20s %-15s %-30s\n" RESET, "SlNo", "Name", "Phone", "Email");
        printf("---------------------------------------------------------------\n");
        for (i = 0; i < match_count; i++)
        {
            int idx = match_indices[i];
            printf("%-5d %-20s %-15s %-30s\n",
                   i + 1,
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }

        int choice;
        do
        {
            printf("Enter Sl.No to edit: ");
            scanf("%d", &choice);
            if (choice < 1 || choice > match_count)
            {
                print_error("Invalid selection");
                continue;
            }
            break;
        } while (1);

        count = match_indices[choice - 1];
    }
    else
    {
        count = match_indices[0];
    }

    // Edit the selected contact
    printf("Enter the new name: ");
    scanf(" %20[^\n]", new_name);
    strcpy(addressBook->contacts[count].name, new_name);

    print_success("Name modified successfully");
}*/
void editContact(AddressBook *addressBook)          // Edit contact fn
{
    char searchValue[50];
    int match_indices[100], match_count = 0, count = 0, i = 0, choice;

    if (addressBook->contactCount == 0)
    {
        print_error("No contacts to edit.\n");
        return;
    }

    printf("\nEdit contact by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter value to search: ");
    scanf(" %[^\n]", searchValue);

    // Collect all matching indices
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if ((choice == 1 && strcasecmp(searchValue, addressBook->contacts[i].name) == 0) ||
            (choice == 2 && strcasecmp(searchValue, addressBook->contacts[i].phone) == 0) ||
            (choice == 3 && strcasecmp(searchValue, addressBook->contacts[i].email) == 0))
        {
            match_indices[match_count++] = i;
        }
    }

    if (match_count == 0)
    {
        print_error("No matching contact found.\n");
        return;
    }

    // If multiple matches, ask user to choose
    if (match_count > 1)
    {
        printf("\nMultiple contacts found:\n");
        printf("---------------------------------------------------------------\n");
        printf(BLUE "%-5s %-20s %-15s %-30s\n" RESET, "SlNo", "Name", "Phone", "Email");
        printf("---------------------------------------------------------------\n");
        for (i = 0; i < match_count; i++)
        {
            int idx = match_indices[i];
            printf("%-5d %-20s %-15s %-30s\n",
                   i + 1,
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }

        int sel;
        do
        {
            printf("Enter Sl.No to edit: ");
            scanf("%d", &sel);
            if (sel < 1 || sel > match_count)
            {
                print_error("Invalid selection.\n");
                continue;
            }
            count = match_indices[sel - 1];
            break;
        } while (1);
    }
    else
    {
        count = match_indices[0];
    }

    // Ask which field to edit
    int fieldChoice;
    printf("\nWhat do you want to edit?\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &fieldChoice);

    char newValue[50];
    printf("Enter new value: ");
    scanf(" %[^\n]", newValue);

    switch (fieldChoice)
    {
        case 1:
            strcpy(addressBook->contacts[count].name, newValue);
            print_success("Name updated successfully.\n");
            break;
        case 2:
            strcpy(addressBook->contacts[count].phone, newValue);
            print_success("Phone updated successfully.\n");
            break;
        case 3:
            strcpy(addressBook->contacts[count].email, newValue);
            print_success("Email updated successfully.\n");
            break;
        default:
            print_error("Invalid choice.\n");
            break;
    }
}


void deleteContact(AddressBook *addressBook)        //Delete contact fn
{
    if (addressBook->contactCount == 0)
    {
        print_error("No contacts to delete.\n");
        return;
    }

    int choice;
    char input[50];
    print_head("\nDelete contact by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter value to delete: ");
    scanf(" %[^\n]", input);

    int matchIndexes[100]; // store indexes of matches
    int matchCount = 0;

    // Find all matches
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((choice == 1 && strcasecmp(addressBook->contacts[i].name, input) == 0) ||
            (choice == 2 && strcasecmp(addressBook->contacts[i].phone, input) == 0) ||
            (choice == 3 && strcasecmp(addressBook->contacts[i].email, input) == 0))
        {
            matchIndexes[matchCount++] = i;
        }
    }

    if (matchCount == 0)
    {
        print_error("Contact not found.\n");
        return;
    }

    int deleteIndex;

    if (matchCount > 1)
    {
        printf(YELLOW "\nMultiple contacts found:\n" RESET);
        printf("---------------------------------------------------------------\n");
        printf("%-5s %-20s %-15s %-30s\n", BLUE "SlNo", "Name", "Phone", "Email" RESET);
        printf("---------------------------------------------------------------\n");
        for (int j = 0; j < matchCount; j++)
        {
            int idx = matchIndexes[j];
            printf("%-5d %-20s %-15s %-30s\n",
                   j + 1,
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }

        printf("Enter the Sl.No of the contact to delete: ");
        int selectedSlNo;
        scanf("%d", &selectedSlNo);

        if (selectedSlNo < 1 || selectedSlNo > matchCount)
        {
            print_error("Invalid selection.\n");
            return;
        }

        deleteIndex = matchIndexes[selectedSlNo - 1];
    }
    else
    {
        deleteIndex = matchIndexes[0];
    }

    // Shift elements to remove the found contact
    for (int i = deleteIndex; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->contactCount--;
    print_success("Contact deleted successfully.\n");
}

                                        /*THE SUB FUNCTIONS*/

int validate_name(char name[]) // validate Name
{
    int i = 0;
    while (name[i] != '\0')
    {
        if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z') || (name[i] == ' ' && i != 0) || (name[i] == '.' && i != 0)) // only allow alphabets & spaces
        {
            i++;
            continue;
        }
        return 0;
    }

    return 1;
}

int validate_phone_no(char phone_no[]) // Validate phone no
{
    int i = 0;

    // Count digits
    while (phone_no[i] != '\0')
    {
        if (phone_no[i] < '0' || phone_no[i] > '9')
        {
            return 0; //  Invalid if not a digit
        }
        i++;
    }

    // Phone number must be exactly 10 digits
    if (i != 10)
        return 0;

    return 1;
}

int validate_email(char email[])      //Validate email
{
    int i = 0, at_found = 0, dot_found = 0;

    //  Email id must be a lowercase letter
    if (!(email[i] >= 'a' && email[i] <= 'z'))
        return 0;

    while (email[i] != '\0')
    {

        //  Allowed characters: lowercase, digits, underscore, dot, @
        if (!((email[i] >= 'a' && email[i] <= 'z') ||
              (email[i] >= '0' && email[i] <= '9') ||
              (email[i] == '_') ||
              (email[i] == '.') ||
              (email[i] == '@')))
        {
            return 0; // invalid character
        }

        // '@' handling
        if (email[i] == '@')
        {
            if (at_found || i == 0) // multiple '@' or starts with '@'
                return 0;
            at_found = 1;
        }

        // '.' handling
        if (email[i] == '.')
        {
            if (!at_found) // dot before '@' is not allowed
                return 0;
            dot_found = 1;
        }

        i++;
    }

    // Must contain exactly one '@' and at least one '.' after it
    if (!at_found || !dot_found)
        return 0;

    // Domain ending check
    char *dot = strrchr(email, '.');
    if (dot == NULL)
        return 0;

    if (strcmp(dot, ".com") == 0 || strcmp(dot, ".in") == 0 || strcmp(dot, ".org") == 0)
    // must contain  one of these ending
        return 1;

    return 0;
}

int isUniquePhone(AddressBook *addressBook, char *phone)     //unique ph.no check
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            return 0; //  Phone number already exists
        }
    }
    return 1; // Phone number is unique
}

int is_unique_email(AddressBook *addressBook, char email[])    //unique ph.no check
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0)
            return 0; // already exists
    }
    return 1; // unique
}



void search_by_name(AddressBook *addressBook, char name[])        //Searching by name
{

    int res;

    do
    {
        printf("Enter the Name u wanna search : \n");
        scanf(" %[^\n]", name);
        res = validate_name(name);
        res ? 1 : print_error("Invalid Name\n");
    } while (!res);

    int flag = 0;
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strncasecmp(name, addressBook->contacts[i].name, strlen(name)) == 0)
        {
            printf("%s %s %s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            flag = 1;
        }
    }
    if (flag == 0)
    {
        print_error("No Name is  found\n");

        return;
    }
    return;
}
void search_by_phone_no(AddressBook *addressBook)        //Searching by ph.no
{
    int res;
    char phone_no[10];
    do
    {
        printf("Enter the Phone .no u wanna search : \n");
        scanf(" %[^\n]", phone_no);
        res = validate_phone_no(phone_no);
        res ? 1 : print_error("Invalid ph.no\n");
    } while (!res);
    int flag = 0;
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(phone_no, addressBook->contacts[i].phone) == 0)
        {
            printf("%s %s %s", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            flag = 1;
        }
    }
    if (flag == 0)
    {
        print_error("No Ph.no is found\n");

        return;
    }
}
void search_by_email(AddressBook *addressBook)           //Searching by name
{
    int res;
    char email[20];
    do
    {
        printf("Enter the Email ID u wanna search : \n");
        scanf(" %[^\n]", email);
        res = validate_email(email);
        res ? 1 : print_error("Invalid email ID\n");
    } while (!res);

    int flag = 0;
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(email, addressBook->contacts[i].email) == 0)
        {
            printf("%s %s %s", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            flag = 1;
        }
    }
    if (flag == 0)
    {
        print_error("No Email is found\n");

        return;
    }
}