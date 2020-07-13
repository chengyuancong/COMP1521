# Makefile for Week 06 struct lecture exercises


CC=gcc
CFLAGS=-Wall -Werror
BINS=studentAdd1 studentAdd2 studentList1 studentList2 studentSeek1 studentSeek2 studentUpdate1

all: $(BINS)

studentAdd1 : studentAdd1.o Student.o
studentAdd1.o : studentAdd1.c Student.h
studentList1 : studentList1.o Student.o
studentList1.o : studentList1.c Student.h
studentAdd2 : studentAdd2.o Student.o
studentAdd2.o : studentAdd2.c Student.h
studentList2 : studentList2.o Student.o
studentList2.o : studentList2.c Student.h
studentSeek1 : studentSeek1.o Student.o
studentSeek1.o : studentSeek1.c Student.h
studentSeek2 : studentSeek2.o Student.o
studentSeek2.o : studentSeek2.c Student.h
studentUpdate1: studentUpdate1.o Student.o
studentUpdate1.o : studentUpdate1.c Student.h
Student.o : Student.c Student.h

clean :
	rm -f $(BINS) a.out core *.o