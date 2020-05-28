# Makefile for CP1521 Lab03

CC=gcc
CFLAGS=-Wall -Werror -std=c99

all : watb maf bits

bits : bits.c

watb : where_are_the_bits.o
	$(CC) -o watb where_are_the_bits.o

maf : make_a_float.o
	$(CC) -o maf make_a_float.o

clean :
	rm -f watb maf where_are_the_bits.o make_a_float.o bits core


