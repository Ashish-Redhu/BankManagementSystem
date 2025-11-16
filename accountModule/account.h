// #ifndef ACCOUNT_H   // Header guard: prevents multiple inclusion of this file
// #define ACCOUNT_H
// #include "../transactionModule/transaction.h"
// // Define a structure to represent an Account
// typedef struct {
//     int accountNumber;    // Unique account number
//     char name[50];        // Account holder's name (up to 49 characters + null terminator)
//     double balance;       // Current balance in the account

//     Transaction transactions[100];
//     int t_count;

// } Account;

// // Function declarations (prototypes)
// // These functions will be implemented in account.c
// void createAccount();                      // Create a new account
// void updateAccount();                      // Update an existing account
// void deleteAccount();                      // Delete an account
// Account getAccountByNumber(int accountNumber); // Retrieve account details by account number
// void displayAccount(int accountNumber);    // Display account details

// #endif // End of header guard
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdio.h>

typedef struct {
    int accountNumber;
    char name[50];
    char pin[10];
    double balance;
} Account;

// account file operations (binary file storage)
int create_account(const char *name, const char *pin);
int authenticate(int accountNumber, const char *pin);
int get_account(int accountNumber, Account *out);
int update_account(const Account *acc);
int next_account_number();
void debug_print_accounts();
#endif // ACCOUNT_H
