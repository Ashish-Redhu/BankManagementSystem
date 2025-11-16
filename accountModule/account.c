// // #include <stdio.h>      // For input/output functions like printf, scanf, fopen, etc.
// // #include <stdlib.h>     // For functions like exit(), malloc(), free()
// // #include <string.h>     // For string handling functions like strcpy, strcmp
// // #include "account.h"    // Include our header file for Account structure and function prototypes

// // #define FILE_NAME "accountModule/accounts.dat"   
// // // We provide the path in this way so that on program execution it search for files in accountModule/ folder only.

// // // ---------------- Helper Function ----------------
// // // This function saves a single account record to the file in binary format
// // void saveAccount(Account acc) {
// //     FILE *fp = fopen(FILE_NAME, "ab"); // Open file in append-binary mode
// //     if (!fp) {                         // If file opening fails
// //         printf("Error opening file!\n");
// //         return;
// //     }
// //     fwrite(&acc, sizeof(Account), 1, fp); // Write one Account structure to file
// //     fclose(fp);                            // Close the file
// // }

// // // ---------------- Create Account ----------------
// // void createAccount() {
// //     Account acc; // Create a variable of type Account

// //     // Take input from user
// //     printf("Enter Account Number: ");
// //     scanf("%d", &acc.accountNumber);

// //     printf("Enter Name: ");
// //     scanf(" %[^\n]", acc.name); // Read string with spaces until newline

// //     printf("Enter Initial Balance: ");
// //     scanf("%lf", &acc.balance);

// //     // Save the account to file
// //     saveAccount(acc);
// //     printf("Account created successfully!\n");
// // }

// // // ---------------- Update Account ----------------
// // void updateAccount() {
// //     int accNum;
// //     printf("Enter Account Number to update: ");
// //     scanf("%d", &accNum);

// //     FILE *fp = fopen(FILE_NAME, "rb+"); // Open file in read/write binary mode. 
// //     if (!fp) {
// //         printf("Error opening file!\n");
// //         return;
// //     }

// //     Account acc;
// //     int found = 0;

// //     // Read each account from file until we find the matching account number
// //     while (fread(&acc, sizeof(Account), 1, fp)) {
// //         if (acc.accountNumber == accNum) {
// //             // Found the account, now update details
// //             printf("Enter new name: ");
// //             scanf(" %[^\n]", acc.name);
// //             printf("Enter new balance: ");
// //             scanf("%lf", &acc.balance);

// //             // Move file pointer back by one record to overwrite the old data
// //             fseek(fp, -sizeof(Account), SEEK_CUR);
// //             fwrite(&acc, sizeof(Account), 1, fp); // Write updated account
// //             found = 1;
// //             break;
// //         }
// //     }
// //     fclose(fp);

// //     if (found)
// //         printf("Account updated successfully!\n");
// //     else
// //         printf("Account not found!\n");
// // }

// // // ---------------- Delete Account ----------------
// // void deleteAccount() {
// //     int accNum;
// //     printf("Enter Account Number to delete: ");
// //     scanf("%d", &accNum);

// //     FILE *fp = fopen(FILE_NAME, "rb");       // Open original file for reading
// //     FILE *temp = fopen("temp.dat", "wb");    // Open temporary file for writing
// //     if (!fp || !temp) {
// //         printf("Error opening file!\n");
// //         return;
// //     }

// //     Account acc;
// //     int found = 0;

// //     // Copy all accounts except the one to delete into temp file
// //     while (fread(&acc, sizeof(Account), 1, fp)) {
// //         if (acc.accountNumber != accNum) {
// //             fwrite(&acc, sizeof(Account), 1, temp);
// //         } else {
// //             found = 1; // Mark that we found the account to delete
// //         }
// //     }
// //     fclose(fp);
// //     fclose(temp);

// //     // Replace original file with temp file
// //     remove(FILE_NAME);
// //     rename("temp.dat", FILE_NAME);

// //     if (found)
// //         printf("Account deleted successfully!\n");
// //     else
// //         printf("Account not found!\n");
// // }

// // // ---------------- Get Account By Number ----------------
// // Account getAccountByNumber(int accountNumber) {
// //     FILE *fp = fopen(FILE_NAME, "rb"); // Open file for reading
// //     Account acc = {0, "", 0.0};        // Initialize empty account

// //     if (!fp) {
// //         printf("Error opening file!\n");
// //         return acc;
// //     }

// //     // Search for account in file
// //     while (fread(&acc, sizeof(Account), 1, fp)) {
// //         if (acc.accountNumber == accountNumber) {
// //             fclose(fp);
// //             return acc; // Return account if found
// //         }
// //     }
// //     fclose(fp);

// //     acc.accountNumber = -1; // Indicates account not found
// //     return acc;
// // }

// // // ---------------- Display Account ----------------
// // void displayAccount(int accountNumber) {
// //     Account acc = getAccountByNumber(accountNumber);
// //     if (acc.accountNumber == -1) {
// //         printf("Account not found!\n");
// //     } else {
// //         printf("\nAccount Details:\n");
// //         printf("Account Number: %d\n", acc.accountNumber);
// //         printf("Name: %s\n", acc.name);
// //         printf("Balance: %.2lf\n", acc.balance);
// //     }
// // }

#include "account.h"
#include <stdlib.h>
#include <string.h>

// File path where all account records are stored in binary format
#define ACCOUNTS_FILE "accountModule/accounts.dat"

// ---------------------------------------------------------------------------
// next_account_number()
// Reads all accounts from the file and returns the next available account number.
// If file doesn't exist, it starts numbering from 1001.
// ---------------------------------------------------------------------------
int next_account_number() {
    FILE *f = fopen(ACCOUNTS_FILE, "rb");   // Open file for reading in binary
    if (!f) return 1001;                    // If no file, first account starts at 1001

    Account a;
    int last = 1000;                        // Start tracking account numbers

    // Read each account and track the highest account number
    while (fread(&a, sizeof(Account), 1, f) == 1) {
        if (a.accountNumber > last)
            last = a.accountNumber;
    }

    fclose(f);
    return last + 1;                        // Next account number
}

// ---------------------------------------------------------------------------
// create_account()
// Creates a new account with provided name and pin, initializes balance to 0,
// generates a new account number, and stores the account in binary file.
// Returns the account number on success or -1 on failure.
// ---------------------------------------------------------------------------
int create_account(const char *name, const char *pin) {
    Account a;
    a.accountNumber = next_account_number();   // Generate next account number

    // Copy name safely (avoid buffer overflow)
    strncpy(a.name, name, sizeof(a.name) - 1);
    a.name[sizeof(a.name) - 1] = '\0';

    // Copy PIN safely
    strncpy(a.pin, pin, sizeof(a.pin) - 1);
    a.pin[sizeof(a.pin) - 1] = '\0';

    a.balance = 0.0;                            // Default balance

    // Open accounts file in append-binary mode
    FILE *f = fopen(ACCOUNTS_FILE, "ab");
    if (!f) return -1;                          // Failed to open file

    fwrite(&a, sizeof(Account), 1, f);          // Write the new account
    fclose(f);

    return a.accountNumber;                     // Return generated number
}

// ---------------------------------------------------------------------------
// authenticate()
// Validates if the provided accountNumber + PIN combination exists in file.
// Returns 1 if valid, 0 otherwise.
// ---------------------------------------------------------------------------
int authenticate(int accountNumber, const char *pin) {
    Account a;
    FILE *f = fopen(ACCOUNTS_FILE, "rb");       // Open for read
    if (!f) return 0;                           // File doesn't exist → auth fails

    // Search for the account
    while (fread(&a, sizeof(Account), 1, f) == 1) {
        if (a.accountNumber == accountNumber) { // Account found
            fclose(f);
            return (strcmp(a.pin, pin) == 0);   // Compare PINs → return result
        }
    }

    fclose(f);
    return 0;                                   // Account not found or mismatch
}

// ---------------------------------------------------------------------------
// get_account()
// Retrieves account details by accountNumber and stores in 'out' struct.
// Returns 1 if found, 0 otherwise.
// ---------------------------------------------------------------------------
int get_account(int accountNumber, Account *out) {
    Account a;
    FILE *f = fopen(ACCOUNTS_FILE, "rb");
    if (!f) return 0;

    // Scan file for matching account number
    while (fread(&a, sizeof(Account), 1, f) == 1) {
        if (a.accountNumber == accountNumber) {
            if (out) *out = a;   // Copy data to output struct
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

// ---------------------------------------------------------------------------
// update_account()
// Overwrites an existing account record by matching accountNumber.
// Opens file in read+write mode, locates target, and replaces it.
// Returns 1 if successful, 0 otherwise.
// ---------------------------------------------------------------------------
int update_account(const Account *acc) {
    FILE *f = fopen(ACCOUNTS_FILE, "rb+");   // Read+Write file
    if (!f) return 0;

    Account a;

    // Read file record-by-record
    while (fread(&a, sizeof(Account), 1, f) == 1) {
        if (a.accountNumber == acc->accountNumber) {  // Match found
            // Move cursor back one record to overwrite current item
            fseek(f, -((long)sizeof(Account)), SEEK_CUR);

            // Write updated account
            fwrite(acc, sizeof(Account), 1, f);

            fclose(f);
            return 1;                               // Update successful
        }
    }

    fclose(f);
    return 0;                                       // Account not found
}

// ---------------------------------------------------------------------------
// debug_print_accounts()
// Prints all accounts for debugging/testing purposes.
// Lists account number, name, PIN, and balance.
// ---------------------------------------------------------------------------
void debug_print_accounts() {
    FILE *f = fopen("accountModule/accounts.dat", "rb");
    if (!f) {
        printf("No accounts file.\n");
        return;
    }

    Account a;
    printf("\n--- DEBUG ACCOUNTS ---\n");

    // Read all accounts and print them
    while (fread(&a, sizeof(Account), 1, f) == 1) {
        printf("Acc: %d | Name: %s | PIN: '%s' | Balance: %.2f\n",
                a.accountNumber, a.name, a.pin, a.balance);
    }

    fclose(f);
}