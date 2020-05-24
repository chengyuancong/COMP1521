# CP1521 Lab02 Makefile

CC=gcc
CFLAGS=-Wall -Werror -std=gnu99

add : add.o BigNum.o
	$(CC) -o add add.o BigNum.o

add.o : add.c BigNum.h
BigNum.o : BigNum.c BigNum.h


clean :
	rm -f core add add.o BigNum.o

