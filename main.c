// #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "accountModule/account.h"
#include "loanModule/loan.h"
#include "transactionModule/transaction.h"
#include "reportsModule/reports.h"


void flush_input(){
    int c;
    while((c=getchar()) !='\n' && c!=EOF) {}
}
void show_main_menu() {
    printf("\n");
    printf("\n===============================\n");
    printf("      BANK MANAGEMENT SYSTEM\n");
    printf("===============================\n");
    printf("1. Login\n");
    printf("2. Create Account\n");
    printf("3. Exit\n");
    printf("Enter choice: \n");
}

void show_user_menu() {
    printf("\n");
    printf("\n=========== MAIN MENU ========== \n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Check Balance\n");
    printf("4. Transaction History\n");
    printf("5. Loan Services\n");
    printf("6. Reports\n");
    printf("7. Logout\n");
    printf("Enter choice: \n");
}

void show_reports_menu() {
    printf("\n");
    printf("\n=========== REPORTS MENU ========== \n");
    printf("1. All Accounts Report\n");
    printf("2. All Transactions Report\n");
    printf("3. All Loans Report\n");
    printf("4. Back\n");
    printf("Enter choice: \n");
}

void show_loan_menu() {
    printf("\n");
    printf("\n========== LOAN MENU ========== \n");
    printf("1. Apply for Loan\n");
    printf("2. View Loans\n");
    printf("3. Pay EMI\n");
    printf("4. Back\n");
    printf("Enter choice: \n");
}

void create_account_flow() {
    char name[50];
    char pin[10];
    // getchar();
    flush_input();
    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Set 4-digit PIN: ");
    fgets(pin, sizeof(pin), stdin);
    pin[strcspn(pin, "\n")] = '\0';

    int accno = create_account(name, pin);
    if (accno > 0)
        printf("Account created! Your account number: %d\n", accno);
    else
        printf("Failed to create account.\n");
}

void deposit_flow(int accountNumber) {
    double amt;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amt) != 1) { getchar(); return; }

    if (deposit_to_account(accountNumber, amt)) {
        Account acc; 
        get_account(accountNumber, &acc);
        printf("Deposit successful. New balance: %.2f\n", acc.balance);
    } else printf("Deposit failed.\n");
}

void withdraw_flow(int accountNumber) {
    double amt;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amt) != 1) { getchar(); return; }

    int res = withdraw_from_account(accountNumber, amt);
    if (res == 1) {
        Account acc; 
        get_account(accountNumber, &acc);
        printf("Withdraw successful. New balance: %.2f\n", acc.balance);
    } else if (res == -1) printf("Insufficient balance!\n");
    else printf("Withdraw failed.\n");
}

void check_balance_flow(int accountNumber) {
    Account acc;
    if (get_account(accountNumber, &acc)) {
        printf("Account Number: %d\nName: %s\nBalance: %.2f\n",
         acc.accountNumber, acc.name, acc.balance);
    }
}

void tx_history_flow(int accountNumber) {
    char fname[128];
    snprintf(fname, sizeof(fname), "tx_%d.txt", accountNumber);

    FILE *f = fopen(fname, "r");
    if (!f) { printf("No transaction history.\n"); return; }

    char line[256];
    printf("--- Transaction History ---\n");
    while (fgets(line, sizeof(line), f))
        printf("%s", line);

    fclose(f);
    printf("\n---------------------------\n");
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
                printf("Loan created successfully! Loan ID = %d\n", loanId);
                printf("EMI = %.2f, Total Payable = %.2f\n",
                       l.emi, l.remainingAmount);
            } else printf("Failed to apply loan.\n");
        }
        else if (ch == 2) {
            Loan loans[32];
            int cnt = list_loans_for_account(accountNumber, loans, 32);

            if (cnt == 0) printf("No loans found.\n");
            else {
                for (int i=0; i<cnt; i++) {
                    Loan *l = &loans[i];
                    printf("LoanID:%d Principal:%.2f EMI:%.2f Remaining:%.2f %s\n",
                        l->loanId, l->principal, l->emi,
                        l->remainingAmount, l->active?"(Active)":"(Closed)");
                }
            }
        }
        else if (ch == 3) {
            int loanId; 
            printf("Enter Loan ID to pay EMI: "); 
            scanf("%d", &loanId);

            int res = pay_emi(loanId, accountNumber);

            if (res == 1) printf("EMI paid successfully.\n");
            else if (res == -1) printf("Insufficient funds.\n");
            else printf("Invalid Loan ID.\n");
        }
        else break;
    }
}

void reports_flow() {
    int ch;
    while (1) {
        show_reports_menu();
        if (scanf("%d", &ch) != 1) { getchar(); break; }

        if (ch == 1) report_all_accounts();
        else if (ch == 2) report_all_transactions();
        else if (ch == 3) report_all_loans();
        else break;
    }
}

int main() {
    while (1) {
        show_main_menu();

        int choice;
        if (scanf("%d", &choice) != 1) { getchar(); break; }

        if (choice == 1) {

            int accno; 
            char pin[16];

            printf("Enter Account Number: "); 
            scanf("%d", &accno);

            printf("Enter PIN: ");
            flush_input();
            fgets(pin,sizeof(pin),stdin); 
            pin[strcspn(pin,"\n")]='\0';

            if (authenticate(accno, pin)) {
                printf("Login Successful!\n");

                int user_choice = 0;
                while (1) {
                    show_user_menu();
                    if (scanf("%d", &user_choice) != 1) { getchar(); break; }

                    if (user_choice == 1) deposit_flow(accno);
                    else if (user_choice == 2) withdraw_flow(accno);
                    else if (user_choice == 3) check_balance_flow(accno);
                    else if (user_choice == 4) tx_history_flow(accno);
                    else if (user_choice == 5) loan_services_flow(accno);
                    else if (user_choice == 6) reports_flow();
                    else if (user_choice == 7) break;
                    else printf("Invalid choice\n");
                }
            } else {
                printf("Login failed.\n");
            }
        }

        else if (choice == 2) create_account_flow();
        else if (choice == 3) { printf("Goodbye!\n"); break; }
        else printf("Invalid choice.\n");
    }

    debug_print_accounts();

    return 0;
}
