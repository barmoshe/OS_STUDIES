all:clean lexcmp lencmp loopcmp  nicecmp
lexcmp:
	gcc -o lexcmp lexcmp.c -Wall
lencmp:
	gcc -o lencmp lencmp.c -Wall
loopcmp:
	gcc -o loopcmp loopcmp.c -Wall
nicecmp:
	gcc -o nicecmp nicecmp.c -Wall
	
clean: 
	rm -f lexcmp.o lexcmp
	rm -f lencmp.o lencmp
	rm -f loopcmp.o loopcmp	
	rm -f nicecmp.o nicecmp
	rm -f try.o nicecmp

