test: test.o ghee.o
	gcc -o test test.o ghee.o

test.o:  
	gcc -c test.c

ghee.o:
	gcc -c ghee.c

# This line means nothing