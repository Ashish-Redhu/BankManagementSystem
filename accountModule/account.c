// #include <stdio.h>      // For input/output functions like printf, scanf, fopen, etc.
// #include <stdlib.h>     // For functions like exit(), malloc(), free()
// #include <string.h>     // For string handling functions like strcpy, strcmp
// #include "account.h"    // Include our header file for Account structure and function prototypes

// #define FILE_NAME "accountModule/accounts.dat"   
// // We provide the path in this way so that on program execution it search for files in accountModule/ folder only.

// // ---------------- Helper Function ----------------
// // This function saves a single account record to the file in binary format
// void saveAccount(Account acc) {
//     FILE *fp = fopen(FILE_NAME, "ab"); // Open file in append-binary mode
//     if (!fp) {                         // If file opening fails
//         printf("Error opening file!\n");
//         return;
//     }
//     fwrite(&acc, sizeof(Account), 1, fp); // Write one Account structure to file
//     fclose(fp);                            // Close the file
// }

// // ---------------- Create Account ----------------
// void createAccount() {
//     Account acc; // Create a variable of type Account

//     // Take input from user
//     printf("Enter Account Number: ");
//     scanf("%d", &acc.accountNumber);

//     printf("Enter Name: ");
//     scanf(" %[^\n]", acc.name); // Read string with spaces until newline

//     printf("Enter Initial Balance: ");
//     scanf("%lf", &acc.balance);

//     // Save the account to file
//     saveAccount(acc);
//     printf("Account created successfully!\n");
// }

// // ---------------- Update Account ----------------
// void updateAccount() {
//     int accNum;
//     printf("Enter Account Number to update: ");
//     scanf("%d", &accNum);

//     FILE *fp = fopen(FILE_NAME, "rb+"); // Open file in read/write binary mode. 
//     if (!fp) {
//         printf("Error opening file!\n");
//         return;
//     }

//     Account acc;
//     int found = 0;

//     // Read each account from file until we find the matching account number
//     while (fread(&acc, sizeof(Account), 1, fp)) {
//         if (acc.accountNumber == accNum) {
//             // Found the account, now update details
//             printf("Enter new name: ");
//             scanf(" %[^\n]", acc.name);
//             printf("Enter new balance: ");
//             scanf("%lf", &acc.balance);

//             // Move file pointer back by one record to overwrite the old data
//             fseek(fp, -sizeof(Account), SEEK_CUR);
//             fwrite(&acc, sizeof(Account), 1, fp); // Write updated account
//             found = 1;
//             break;
//         }
//     }
//     fclose(fp);

//     if (found)
//         printf("Account updated successfully!\n");
//     else
//         printf("Account not found!\n");
// }

// // ---------------- Delete Account ----------------
// void deleteAccount() {
//     int accNum;
//     printf("Enter Account Number to delete: ");
//     scanf("%d", &accNum);

//     FILE *fp = fopen(FILE_NAME, "rb");       // Open original file for reading
//     FILE *temp = fopen("temp.dat", "wb");    // Open temporary file for writing
//     if (!fp || !temp) {
//         printf("Error opening file!\n");
//         return;
//     }

//     Account acc;
//     int found = 0;

//     // Copy all accounts except the one to delete into temp file
//     while (fread(&acc, sizeof(Account), 1, fp)) {
//         if (acc.accountNumber != accNum) {
//             fwrite(&acc, sizeof(Account), 1, temp);
//         } else {
//             found = 1; // Mark that we found the account to delete
//         }
//     }
//     fclose(fp);
//     fclose(temp);

//     // Replace original file with temp file
//     remove(FILE_NAME);
//     rename("temp.dat", FILE_NAME);

//     if (found)
//         printf("Account deleted successfully!\n");
//     else
//         printf("Account not found!\n");
// }

// // ---------------- Get Account By Number ----------------
// Account getAccountByNumber(int accountNumber) {
//     FILE *fp = fopen(FILE_NAME, "rb"); // Open file for reading
//     Account acc = {0, "", 0.0};        // Initialize empty account

//     if (!fp) {
//         printf("Error opening file!\n");
//         return acc;
//     }

//     // Search for account in file
//     while (fread(&acc, sizeof(Account), 1, fp)) {
//         if (acc.accountNumber == accountNumber) {
//             fclose(fp);
//             return acc; // Return account if found
//         }
//     }
//     fclose(fp);

//     acc.accountNumber = -1; // Indicates account not found
//     return acc;
// }

// // ---------------- Display Account ----------------
// void displayAccount(int accountNumber) {
//     Account acc = getAccountByNumber(accountNumber);
//     if (acc.accountNumber == -1) {
//         printf("Account not found!\n");
//     } else {
//         printf("\nAccount Details:\n");
//         printf("Account Number: %d\n", acc.accountNumber);
//         printf("Name: %s\n", acc.name);
//         printf("Balance: %.2lf\n", acc.balance);
//     }
// }

// ------------- Mayank's Code ------------- //

#include "../accountModule/account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "../accountModule/accounts.dat"

static int file_exists(const char * fname){
    FILE *f=fopen(fname,"rb");
    if(!f)return 0;
    fclose(f);
    return 1;
}

int next_account_number(){
    FILE *f= fopen(FILE_NAME,"rb");
    if(!f) return 1001; //start
    Account a;
    int last =1000;

    while(fread(&a,sizeof(Account),1,f)==1){
        if(a.accountNumber>last)last=a.accountNumber;
    }
    fclose(f);
    return last+1;
}

int create_account(const char*name,const char*pin){
    Account a;
    a.accountNumber=next_account_number();
    strncpy(a.name,name,sizeof(a.name)-1);
    a.name[sizeof(a.name)-1]='␀'
}

int get_account(int accountNumber,Account * out){
    Account a;
    FILE *f=fopen(FILE_NAME,"rb");
    if(!f) return 0;
    while(fread(&a,sizeof(Account),1,f)==1){
        if(a.accountNumber==accountNumber){
            if(out) *out = a;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int update_account(const Account *acc){
    FILE *f=fopen(FILE_NAME,"rb+");
    if(!f)return 0;
    Account a;
    while(fread(&a,sizeof(Account),1,f)==1){
        if(a.accountNumber==acc->accountNumber){
            fseek(f,-((long)sizeof(Account)),SEEK_CUR);
            fwrite(acc,sizeof(Account),1,f);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}