CC=gcc

SRCDIR= ./src/
OBJDIR= ./src/
BINDIR= ./

all: main

clean:
	rm -f  ./bonus.exe

main:   
	$(CC) -o $(BINDIR)bonus.exe $(OBJDIR)bonus.c  -lgsl -lgslcblas  -lm
	
 