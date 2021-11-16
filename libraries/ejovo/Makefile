all: compile exe

compile: ejovo_matrix.c piece.c test.c
	gcc -o test test.c ejovo_matrix.c piece.c

 exe: compile
	./test