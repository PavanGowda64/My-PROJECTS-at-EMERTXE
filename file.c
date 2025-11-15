#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv", "w");
    if (fptr == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    fprintf(fptr, "%d\n", addressBook->contactCount); // Save count
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr,"%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv", "r");
    if (fptr == NULL)
    {
        perror("Error opening file for reading");
        addressBook->contactCount = 0;
        return;
    }

    fscanf(fptr, "%d\n", &addressBook->contactCount); // Read count
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fptr, " %[^,],%[^,],%[^\n]",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    fclose(fptr);
}
