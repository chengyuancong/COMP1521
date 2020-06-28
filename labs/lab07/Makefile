# Makefile for CP1521

CC=gcc
CFLAGS=-std=gnu99 -Wall -Werror
LDFLAGS=-lbsd
#LDFLAGS=

all : myls stat

myls : myls.c
	gcc -o myls myls.c -lbsd

stat : stat.c

clean:
	rm -f myls stat *.o core
