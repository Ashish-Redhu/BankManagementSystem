#ifndef ACCOUNT_H   // Header guard: prevents multiple inclusion of this file
#define ACCOUNT_H

// Define a structure to represent an Account
typedef struct {
    int accountNumber;    // Unique account number
    char name[50];        // Account holder's name (up to 49 characters + null terminator)
    double balance;       // Current balance in the account
} Account;

// Function declarations (prototypes)
// These functions will be implemented in account.c
void createAccount();                      // Create a new account
void updateAccount();                      // Update an existing account
void deleteAccount();                      // Delete an account
Account getAccountByNumber(int accountNumber); // Retrieve account details by account number
void displayAccount(int accountNumber);    // Display account details

#endif // End of header guard