CC=gcc
CFLAGS= -c -Wall

all: piping.o
	$(CC) piping.o -o piping
piping.o: piping.c 
	$(CC) $(CFLAGS) piping.c
clean:
	rm -rf *o piping