#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>

// ---------------------------------------------
// Transaction structure (shared between modules)
// ---------------------------------------------
typedef struct {
    int accountNumber;
    char type[20];
    double amount;
    char timestamp[30];
} Transaction;

// Log a transaction
void log_transaction(int accountNumber, const char *type, double amount);

// High-level operations
int deposit_to_account(int accountNumber, double amount);
int withdraw_from_account(int accountNumber, double amount);

#endif // TRANSACTION_H
