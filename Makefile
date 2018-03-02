CC = gcc
OPT = -O2 -Wall
LIB = 

all: my_regex

my_regex: my_regex.c
	$(CC) $(OPT) my_regex.c -o my_regex

clean:
	rm -f my_regex
