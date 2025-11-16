#ifndef LOAN_H
#define LOAN_H

#include <stdio.h>

typedef struct
{
    int loanId;
    int accountNumber;
    double principal;
    double interestRate; // annual percent
    int months;
    double emi;
    double remainingAmount;
    int active; // 1 active , 0 closed
} Loan;

int apply_loan(int accountNumber, double principal, double annualInterestPercent, int months, int *outLoanId);
int list_loans_for_account(int accountNumber, Loan *outLoans, int maxLoans);
int get_loan(int loanId, Loan *out);
int pay_emi(int loanId, int accountNumber);
int update_loan(const Loan *ln);
int next_loan_id();

double calculate_emi(double principal, double annualInterestPercent, int months);

#endif