#include "transaction.h"
#include "accountModule/account.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

void log_transaction(int accountNumber,const char* type,double amount){
 
    //buffer
    char fname[64];
    //creates a filename string (does not create a file)
    snprintf(fname,sizeof(fname),"tx_%d.txt",accountNumber);
    //create (or opens) the file for appending
    FILE *f=fopen(fname,"a");
    if(!f)return;
    //time(NULL) gets current UNIX timestamp
    time_t t=time(NULL);
    //localtime() converts it to a human-readable structure: year month etc
    struct tm *tm=localtime(&t);
    char timestr[64];
    //format the time into readable string : 2025-03-08 14:55:22
    strftime(timestr,sizeof(timestr),"%Y-%m-%d %H:%M:%S",tm);
    //appends log text into that file (ex) -> [2025-03-08 14:55:22] DEPOSIT: 200.00
    fprintf(f,"[%s] %s: %.2f",timestr,type,amount);
    //flushes data and releases the file handle
    fclose(f);
}

int deposit_to_account(int accountNumber,double amount){
    if(amount<=0)return 0;
    Account acc;
    if(!get_account(accountNumber,&acc)) return 0;
    acc.balance+=amount;
    if(!update_account(&acc))return 0;
    log_transaction(accountNumber,"Deposit",amount);
    return 1;
}

int withdraw_from_account(int accountNumber,double amount){
    if(amount<=0)return 0;
    Account acc;
    if(!get_account(accountNumber,&acc))return 0;
    if(acc.balance<amount)return -1; //insufficent funds
    acc.balance-=amount;
    if(!update_account(&acc))return 0;
    log_transaction(accountNumber,"Withdraw",amount);
    return 1;
}