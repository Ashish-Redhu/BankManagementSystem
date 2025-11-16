# BankManagementSystem
Application built for Demo POC, C-language. 

## Project Structure
```
BankManagementSystem/
│
├── main.c
├── accountModule/
│   ├── account.h
│   ├── account.c
│   └── accounts.dat
├── transactionModule/
│   ├── transaction.h
│   ├── transaction.c
│   └── transactions.dat
├── loanModule/
│   ├── loan.h
│   ├── loan.c
│   └── loans.dat
├── reportsModule/
│   ├── reports.h
│   ├── reports.c
│   └── reports.dat (optional)
├── build/
│   ├── compiledfiles...
└── Makefile
```


🧪 Optional: Compile Without Make

```
gcc main.c \
    accountModule/account.c \
    transactionModule/transaction.c \
    loanModule/loan.c \
    reportsModule/reports.c \
    -o BankManagementSystem

```
run the program manually 

```
./BankManagementSystem

```

## Contributors
- @Ashish-Redhu 
- @mayank5515


