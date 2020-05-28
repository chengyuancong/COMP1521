# CP1521 Lab02 Makefile

CC=gcc
CFLAGS=-Wall -Werror -std=gnu99

all: add multiply subtract

add : add.o BigNum.o
	$(CC) -o add add.o BigNum.o

multiply : multiply.o BigNum.o
	$(CC) -o multiply multiply.o BigNum.o

subtract : subtract.o BigNum.o
	$(CC) -o subtract subtract.o BigNum.o

add.o : add.c BigNum.h
multiply.o : multiply.c BigNum.h
subtract.o : subtract.c BigNum.h
BigNum.o : BigNum.c BigNum.h


clean :
	rm -f core add add.o multiply multiply.o subtract subtract.o BigNum.o

