// #include "loan.h"
// #include "../accountModule/account.h"
// #include "../transactionModule/transaction.h"
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>

// #define LOANS_FILE "loanModule/loans.dat"


// static int loans_file_exists(){
//     FILE *f=fopen(LOANS_FILE,"rb");
//     if(!f) return 0;
//     fclose(f);
//     return 1;
// }

// int next_loan_id(){
//     FILE *f=fopen(LOANS_FILE,"rb");
//     if(!f) return 1;
//     Loan l;
//     int last=0;
//     while(fread(&l,sizeof(Loan),1,f)==1){
//         if(l.loanId>last)last=l.loanId;
//     }
//     fclose(f);
//     return last+1;
// }

// double calculate_emi(double principal,double annualInterestPercent,int months){
//     if(months<=0) return 0.0;
//     double r=(annualInterestPercent / 12.0) / 100.0; //monthly rate
//     if(r==0.0) return principal/months;
//     double numerator=principal * r * pow(1+r,months);
//     double denom=pow(1+r,months)-1;
//     return numerator/denom;
// }

// int apply_loan(int accountNumber, double principal, double annualInterestPercent, int months, int *outLoanId) {
// // check account exists
// Account acc;
// if (!get_account(accountNumber, &acc)) return 0;


// Loan l;
// l.loanId = next_loan_id();
// l.accountNumber = accountNumber;
// l.principal = principal;
// l.interestRate = annualInterestPercent;
// l.months = months;
// l.emi = calculate_emi(principal, annualInterestPercent, months);
// l.remainingAmount = l.emi * months; // total payable
// l.active = 1;


// FILE *f = fopen(LOANS_FILE, "ab");
// if (!f) return 0;
// fwrite(&l, sizeof(Loan), 1, f);
// fclose(f);


// if (outLoanId) *outLoanId = l.loanId;
// // Credit the principal to account balance using transaction module
// if (!deposit_to_account(accountNumber, principal)) return 0;


// // Log loan disbursal as a separate entry
// char buf[128];
// snprintf(buf, sizeof(buf), "Loan Disbursal (ID:%d)", l.loanId);
// log_transaction(accountNumber, buf, principal);


// return 1;
// }



// int list_loans_for_account(int accountNumber, Loan *outLoans, int maxLoans)
// {
//     FILE *f = fopen(LOANS_FILE, "rb");
//     if (!f) return 0;

//     Loan l;
//     int cnt = 0;

//     while (fread(&l, sizeof(Loan), 1, f) == 1) {
//         if (l.accountNumber == accountNumber) {
//             if (cnt < maxLoans && outLoans)
//                 outLoans[cnt] = l; // FIXED
//             cnt++;
//         }
//     }

//     fclose(f);
//     return cnt;
// }

#include "loan.h"
#include "../accountModule/account.h"
#include "../transactionModule/transaction.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LOANS_FILE "loanModule/loans.dat"


// -----------------------------
// Utility
// -----------------------------
static int loans_file_exists() {
    FILE *f = fopen(LOANS_FILE, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}

int next_loan_id() {
    FILE *f = fopen(LOANS_FILE, "rb");
    if (!f) return 1;

    Loan l;
    int last = 0;

    while (fread(&l, sizeof(Loan), 1, f) == 1) {
        if (l.loanId > last) last = l.loanId;
    }

    fclose(f);
    return last + 1;
}


// -----------------------------
// EMI Calculation
// -----------------------------
double calculate_emi(double principal, double annualInterestPercent, int months) {
    if (months <= 0) return 0.0;

    double r = (annualInterestPercent / 12.0) / 100.0; // monthly rate

    if (r == 0.0) return principal / months;

    double numerator = principal * r * pow(1 + r, months);
    double denom = pow(1 + r, months) - 1;

    return numerator / denom;
}


// -----------------------------
// APPLY LOAN
// -----------------------------
int apply_loan(int accountNumber, double principal, double annualInterestPercent, int months, int *outLoanId) {

    // Check account exists
    Account acc;
    if (!get_account(accountNumber, &acc)) return 0;

    Loan l;
    l.loanId = next_loan_id();
    l.accountNumber = accountNumber;
    l.principal = principal;
    l.interestRate = annualInterestPercent;
    l.months = months;
    l.emi = calculate_emi(principal, annualInterestPercent, months);
    l.remainingAmount = l.emi * months;
    l.active = 1;

    FILE *f = fopen(LOANS_FILE, "ab");
    if (!f) return 0;
    fwrite(&l, sizeof(Loan), 1, f);
    fclose(f);

    if (outLoanId) *outLoanId = l.loanId;

    // Credit principal to account
    if (!deposit_to_account(accountNumber, principal)) return 0;

    // Log disbursal
    char buf[128];
    snprintf(buf, sizeof(buf), "Loan Disbursal (ID:%d)", l.loanId);
    log_transaction(accountNumber, buf, principal);

    return 1;
}


// -----------------------------
// LIST LOANS FOR ACCOUNT
// -----------------------------
int list_loans_for_account(int accountNumber, Loan *outLoans, int maxLoans) {
    FILE *f = fopen(LOANS_FILE, "rb");
    if (!f) return 0;

    Loan l;
    int cnt = 0;

    while (fread(&l, sizeof(Loan), 1, f) == 1) {
        if (l.accountNumber == accountNumber) {
            if (cnt < maxLoans && outLoans)
                outLoans[cnt] = l;
            cnt++;
        }
    }

    fclose(f);
    return cnt;
}


// -----------------------------
// GET LOAN BY ID
// -----------------------------
int get_loan(int loanId, Loan *out) {
    FILE *f = fopen(LOANS_FILE, "rb");
    if (!f) return 0;

    Loan l;

    while (fread(&l, sizeof(Loan), 1, f) == 1) {
        if (l.loanId == loanId) {
            if (out) *out = l;
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}


// -----------------------------
// UPDATE (OVERWRITE) LOAN RECORD
// -----------------------------
int update_loan(const Loan *ln) {
    FILE *f = fopen(LOANS_FILE, "rb+");
    if (!f) return 0;

    Loan l;

    while (fread(&l, sizeof(Loan), 1, f) == 1) {
        if (l.loanId == ln->loanId) {
            fseek(f, -sizeof(Loan), SEEK_CUR);
            fwrite(ln, sizeof(Loan), 1, f);
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}


// -----------------------------
// PAY EMI
// -----------------------------
int pay_emi(int loanId, int accountNumber) {
    Loan l;

    if (!get_loan(loanId, &l)) return 0;
    if (!l.active) return 0;
    if (l.accountNumber != accountNumber) return 0;

    // Debit the emi amount
    if (!withdraw_from_account(accountNumber, l.emi)) return 0;

    // Deduct EMI from remaining
    l.remainingAmount -= l.emi;
    if (l.remainingAmount < 1) {
        l.remainingAmount = 0;
        l.active = 0;  // loan closed
    }

    update_loan(&l);

    // Log EMI payment
    char buf[128];
    snprintf(buf, sizeof(buf), "EMI Payment (Loan ID:%d)", loanId);
    log_transaction(accountNumber, buf, l.emi);

    return 1;
}
