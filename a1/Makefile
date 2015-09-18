CC = gcc
C_FLAGS = -Wall -Wextra

all: main

main: main.o
	$(CC) main.o -o main

main.o: main.c
	$(CC) -c $(C_FLAGS) main.c

clean:
	rm -f main main.o