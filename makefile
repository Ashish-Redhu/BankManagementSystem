CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -IaccountModule -ItransactionModule -IloanModule

SRC = main.c \
      accountModule/account.c \
      transactionModule/transaction.c \
      loanModule/loan.c

OBJ = $(SRC:.c=.o)

TARGET = bank

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
