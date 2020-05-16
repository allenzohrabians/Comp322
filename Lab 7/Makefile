ALL:
	gcc -c matrix_gen.c -Wall -Wextra
	gcc -o matrix_gen matrix_gen.o -lm
	gcc -c matrix_add.c -Wall -Wextra -lrt
	gcc -o matrix_add matrix_add.o -lm -lrt

REM:
	rm matrix_gen matrix_gen.o
	rm matrix_add matrix_add.o
