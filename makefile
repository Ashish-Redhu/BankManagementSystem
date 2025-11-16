CC = gcc
CFLAGS = -Wall -Wextra -IaccountModule -ItransactionModule -IloanModule -IreportsModule

SRC = main.c \
      accountModule/account.c \
      transactionModule/transaction.c \
      loanModule/loan.c \
      reportsModule/reports.c

OBJ = $(SRC:.c=.o)

TARGET = bank

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm

clean:
	rm -f $(OBJ) $(TARGET)

