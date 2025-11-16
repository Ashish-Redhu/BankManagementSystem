#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>

void log_transaction(int accountNumber, const char *type, double amount);

int deposit_to_account(int accountNumber, double amount);
int withdraw_from_account(int accountNumber, double amount);
#endif // TRANSACTION_H