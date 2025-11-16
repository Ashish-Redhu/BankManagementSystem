#include "loan.h"
#include "../accountModule/account.h"
#include "../transactionModule/transaction.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LOANS_FILE "loanModule/loans.dat"


static int loans_file_exists(){
    FILE *f=fopen(LOANS_FILE,"rb");
    if(!f) return 0;
    fclose(f);
    return 1;
}

int next_loan_id(){
    FILE *f=fopen(LOANS_FILE,"rb");
    if(!f) return 1;
    Loan l;
    int last=0;
    while(fread(&l,sizeof(Loan),1,f)==1){
        if(l.loanId>last)last=l.loanId;
    }
    fclose(f);
    return last+1;
}

double calculate_emi(double principal,double annualInterestPercent,int months){
    if(months<=0) return 0.0;
    double r=(annualInterestPercent / 12.0) / 100.0; //monthly rate
    if(r==0.0) return principal/months;
    double numerator=principal * r * pow(1+r,months);
    double denom=pow(1+r,months)-1;
    return numerator/denom;
}

int apply_loan(int accountNumber, double principal, double annualInterestPercent, int months, int *outLoanId) {
// check account exists
Account acc;
if (!get_account(accountNumber, &acc)) return 0;


Loan l;
l.loanId = next_loan_id();
l.accountNumber = accountNumber;
l.principal = principal;
l.interestRate = annualInterestPercent;
l.months = months;
l.emi = calculate_emi(principal, annualInterestPercent, months);
l.remainingAmount = l.emi * months; // total payable
l.active = 1;


FILE *f = fopen(LOANS_FILE, "ab");
if (!f) return 0;
fwrite(&l, sizeof(Loan), 1, f);
fclose(f);


if (outLoanId) *outLoanId = l.loanId;
// Credit the principal to account balance using transaction module
if (!deposit_to_account(accountNumber, principal)) return 0;


// Log loan disbursal as a separate entry
char buf[128];
snprintf(buf, sizeof(buf), "Loan Disbursal (ID:%d)", l.loanId);
log_transaction(accountNumber, buf, principal);


return 1;
}

int apply_loan(int accountNumber,double principal,double annualInterestPercent, int months, int *outLoanId){

    //check account exists

    Account acc;
    if(!get_account(accountNumber,&acc))return 0;

    Loan l;
    l.loanId=next_loan_id();
    l.accountNumber=accountNumber;
    l.principal=principal;
    l.interestRate=annualInterestPercent;
    l.months=months;
    l.emi=calculate_emi(principal,annualInterestPercent,months);
    l.remainingAmount=l.emi * months; // total payable
    l.active=1;

    FILE *f=fopen(LOANS_FILE,"ab");
    if(!f) return 0;
    fwrite(&l,sizeof(Loan),1,f);
    fclose(f);

    if(outLoanId) *outLoanId=l.loanId;
    // Credit the principal to account balance using transaction module
    if(!deposit_to_account(accountNumber,principal))return 0;

    // Log loan disbursal as a separate entry
    char buf[128];
    snprintf(buf,sizeof(buf),"Loan Disbursal (ID:%d)",l.loanId);
    log_transaction(accountNumber,buf,principal);
 
    return 1;
}


int list_loans_for_account(int accountNumber,Loan *outloans, int maxLoans){
    FILE *f=fopen(LOANS_FILE,"rb");
    if(!f) return 0;

    Loan l;

    int cnt=0;
    while(fread(&l,sizeof(Loan),1,f)==1){
        if(l.accountNumber==accountNumber){
            if(cnt<maxLoans && outloans) outloans[cnt]=1;
            cnt++;
        }
    }
    fclose(f);
    return cnt;
}

