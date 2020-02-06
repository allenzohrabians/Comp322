ALL:
	gcc -c Lab0.c -Wall -Wextra
	gcc -o Lab0 Lab0.o -lm

REM:
	rm Lab0 Lab0.o
