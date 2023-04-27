assembler:	assembler.o	data.o data.o errors.o error_checking.o 	first_pass.o	alloc.o  files.o	line_checker.o second_pass.o	symbol.o parse.o
	gcc	-g -Wall -ansi -pedantic	assembler.o	data.o errors.o error_checking.o	 first_pass.o	alloc.o files.o	line_checker.o second_pass.o	symbol.o parse.o -lm -o assembler

assembler.o:	assembler.c
	gcc	-c -Wall -ansi -pedantic assembler.c -o assembler.o

data.o:	data.c	
	gcc	-c -Wall -ansi -pedantic data.c -o data.o

errors.o:	errors.c
	gcc -c -Wall -ansi -pedantic	errors.c -o errors.o

error_checking.o:	error_checking.c
	gcc -c -Wall -ansi -pedantic	error_checking.c -o error_checking.o

first_pass.o:	first_pass.c
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o

alloc.o:	alloc.c
	gcc -c -Wall -ansi -pedantic alloc.c  -o alloc.o

files.o:	files.c
	gcc -c -Wall -ansi -pedantic files.c  -o files.o


line_checker.o: line_checker.c
	gcc -c -Wall -ansi -pedantic line_checker.c  -o line_checker.o

second_pass.o:	second_pass.c		
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o

symbol.o	: symbol.c
	gcc -c -Wall -ansi -pedantic	 symbol.c -o symbol.o

parse.o:	parse.c
	gcc -c -Wall -ansi -pedantic	parse.c -o parse.o
