#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accountModule/account.h"
#include "loanModule/loan.h"
#include "transactionModule/transaction.h"


void pause_and_clear(){
    printf("Press Enter to continue ...");
    getchar();
}

void show_main_menu(){
    printf("===============================");
    printf(" BANK MANAGEMENT SYSTEM" );
    printf("=============================== \n");
    printf("1. Login \n");
    printf("2. Create Account \n");
    printf("3. Exit \n");
    printf("Enter choice: ");
}

void show_user_menu(){
    printf("=========== MAIN MENU ==========");
    printf("1. Deposit \n");
    printf("2. Withdraw \n");
    printf("3. Check Balance \n");
    printf("4. Transaction History \n");
    printf("5. Loan Services \n");
    printf("6. Logout \n");
    printf("Enter choice: ");
}

void show_loan_menu(){
    printf(" ========== LOAN MENU ========== \n");
    printf("1. Apply for Loan \n");
    printf("2. View Active Loans \n");
    printf("3. Pay EMI \n");
    printf("4. Loan History \n");
    printf("5. Back \n");
    printf("Enter choice: ");
}

void create_account_flow() {
    char name[50];
    char pin[10];
    getchar();
    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "")] = '␀';
    printf("Set 4-digit PIN: ");
    fgets(pin, sizeof(pin), stdin);
    pin[strcspn(pin, "")] = '␀';
    int accno = create_account(name, pin);
    if (accno > 0) {
    printf("Account created! Your account number: %d", accno);
    }
    else {
    printf("Failed to create account.");
    }
}

void deposit_flow(int accountNumber) {
    double amt;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amt) != 1) { getchar(); return; }
    int res = deposit_to_account(accountNumber, amt);
    if (res) {
    Account acc; get_account(accountNumber, &acc);
    printf("Deposit successful. New balance: %.2f", acc.balance);
    } 
    else printf("Deposit failed.");
}

void withdraw_flow(int accountNumber) {
    double amt;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amt) != 1) { getchar(); return; }
    int res = withdraw_from_account(accountNumber, amt);
    if (res == 1) {
    Account acc; get_account(accountNumber, &acc);
    printf("Withdraw successful. New balance: %.2f", acc.balance);
    } else if (res == -1) printf("Insufficient balance!");
    else printf("Withdraw failed.");
}

void check_balance_flow(int accountNumber) {
    Account acc;
    if (get_account(accountNumber, &acc)) {
    printf("Account Number: %dName: %sBalance: %.2f", acc.accountNumber, acc.name, acc.balance);
    }
}

void tx_history_flow(int accountNumber) {
    char fname[64];
    snprintf(fname, sizeof(fname), "tx_%d.txt", accountNumber);
    FILE *f = fopen(fname, "r");
    if (!f) { printf("No transaction history."); 
        return; 
    }
    char line[256];
    printf("--- Transaction History ---");
    while (fgets(line, sizeof(line), f)) printf("%s", line);
    fclose(f);
    printf("---------------------------");
}

void loan_services_flow(int accountNumber) {
    int ch;
    while (1) {
    show_loan_menu();
    if (scanf("%d", &ch) != 1) { getchar(); break; }
    if (ch == 1) {
    double amount; int months; double rate;
    printf("Enter loan amount: "); scanf("%lf", &amount);
    printf("Enter duration (months): "); scanf("%d", &months);
    printf("Enter annual interest rate (%%): "); scanf("%lf", &rate);
    int loanId;
    if (apply_loan(accountNumber, amount, rate, months, &loanId)) {
    Loan l; 
    get_loan(loanId, &l);
    printf("Loan created successfully! Loan ID = %d", loanId);
    printf("EMI = %.2f, Total Payable = %.2f\n", l.emi, l.remainingAmount);
    } else printf("Failed to apply loan.\n");
    } else if (ch == 2) {
    Loan loans[32];
    int cnt = list_loans_for_account(accountNumber, loans, 32);
    if (cnt == 0) { printf("No loans found.\n"); }
    else {
    for (int i=0;i<cnt;i++) {
    Loan *l = &loans[i];
    printf("LoanID:%d Principal:%.2f EMI:%.2f Remaining:%.2f %s\n",
    l->loanId, l->principal, l->emi, l->remainingAmount, l->active?"(Active)":"(Closed)");
    }
    }
} else if (ch == 3) {
    int loanId; printf("Enter Loan ID to pay EMI: "); scanf("%d", &loanId);
    int res = pay_emi(loanId, accountNumber);
    if (res == 1) printf("EMI paid successfully.\n");
    else if (res == -1) printf("Insufficient funds to pay EMI.\n");
    else printf("Failed to pay EMI. Check Loan ID.\n");
} else if (ch == 4) {
    Loan loans[64];
    int total = list_loans_for_account(accountNumber, loans, 64);
    if (total == 0) { printf("No loan history.\n"); }
    else {
    for (int i=0;i<total;i++) {
    Loan *l = &loans[i];
    printf("ID:%d Principal:%.2f Rate:%.2f Months:%d EMI:%.2f Remaining:%.2f %s\n",
    l->loanId, l->principal, l->interestRate, l->months, l->emi, l->remainingAmount, l->active?"(Active)":"(Closed)");
    }
    }
    } else break;
    }
}

int main(){
    while(1){
        show_main_menu();
        int choice;
        if(scanf("%d",&choice)!=1){
            getchar();
            break;
        }
        if(choice==1){
            int accno;
            char pin[16];
            printf("Enter Account Number: ");
            scanf("%d",&accno);
            printf("Enter PIN: ");
            scanf("%s",pin);
            
            if(authenticate(accno,pin)){
                printf("Login Successful!\n");

                int user_choice=0;
                while(1){
                    show_user_menu();
                    if(scanf("%d",&user_choice)!=1){
                        getchar();
                        break;
                    }
                    if(user_choice==1) deposit_flow(accno);
                    else if(user_choice==2) withdraw_flow(accno);
                    else if(user_choice==3) check_balance_flow(accno);
                    else if(user_choice==4) tx_history_flow(accno);
                    else if(user_choice==5) loan_services_flow(accno);
                    else if(user_choice==6) break;
                    else printf("Invalid Choice\n");
                }
            }
            else{
                printf("Login Failed.\n");
            }
        }
        else if(choice==2){
            create_account_flow();
        }
        else if(choice==3){
            printf("Goodbye!");
            break;
        }
        else printf("Invalid choice.\n");

    }
    return 0;
}