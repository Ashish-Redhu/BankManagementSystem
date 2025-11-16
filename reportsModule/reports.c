#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>   // for FindFirstFile, FindNextFile

#include "../accountModule/account.h"
#include "../transactionModule/transaction.h"
#include "../loanModule/loan.h"
#include "reports.h"

// ----------------------------
// 1. Show all accounts
// ----------------------------
void report_all_accounts() {
    FILE *fp = fopen("accountModule/accounts.dat", "rb");
    if (!fp) {
        printf("\n[ERROR] Could not open accounts.dat\n");
        return;
    }

    Account acc;
    printf("\n============================\n");
    printf("   ALL ACCOUNTS REPORT\n");
    printf("============================\n");

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        printf("\nAccount Number : %d", acc.accountNumber);
        printf("\nName           : %s", acc.name);
        printf("\nBalance        : %.2f\n", acc.balance);
        printf("----------------------------");
    }

    fclose(fp);
}

// ----------------------------
// 2. Show all transactions
// ----------------------------
void report_all_transactions() {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile("tx_*.txt", &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("\n[INFO] No transaction files found.\n");
        return;
    }

    printf("\n============================\n");
    printf("  ALL TRANSACTIONS REPORT\n");
    printf("============================\n");

    do {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s", findData.cFileName);

        FILE *fp = fopen(filename, "r");
        if (!fp) continue;

        printf("\n--- File: %s ---\n", filename);

        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
        }

        fclose(fp);
        printf("\n----------------------------\n");

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}
// ----------------------------
// 3. Show all loans
// ----------------------------
void report_all_loans() {
    FILE *fp = fopen("loanModule/loans.dat", "rb");
    if (!fp) {
        printf("\n[ERROR] Could not open loans.dat\n");
        return;
    }

    Loan loan;

    printf("\n============================\n");
    printf("       ALL LOANS REPORT\n");
    printf("============================\n");

    while (fread(&loan, sizeof(Loan), 1, fp) == 1) {
        printf("\nLoan ID         : %d", loan.loanId);
        printf("\nAccount Number  : %d", loan.accountNumber);
        printf("\nLoan Amount     : %.2f", loan.principal);
        printf("\nRemaining Amount: %.2f", loan.remainingAmount);
        printf("\nStatus          : %s", loan.active ? "ACTIVE" : "PAID");
        printf("\n----------------------------");
    }

    fclose(fp);
}