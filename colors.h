#ifndef COLORS_H
#define COLORS_H // macro to implement color coding  

#include <stdio.h>

// Regular Colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#define ERROR_COLOR   "\033[0;31m"  // Normal Red for errors
#define SUCCESS_COLOR "\033[0;32m"  // Normal Green for success
#define HEAD_COLOR    "\033[0;34m"  // Normal Blue for Headings  & choices
#define INFO_COLOR    "\033[0;33m"  // Normal Yellow for info

#endif

// Functions to print colored text

void print_head(const char *message)
{
    printf(HEAD_COLOR " %s\n" RESET, message);
}
void print_error(const char *message)
{
    printf(ERROR_COLOR "❌ %s\n" RESET, message);
}

void print_success(const char *message)
{
    printf(SUCCESS_COLOR "✅ %s\n" RESET, message);
}

void print_info(const char *message)
{
    printf(INFO_COLOR "\nℹ.%s\n" RESET, message);
}
