all: lista.o main.o
	gcc lista.o main.o -o main

run: main
	./main

main.o: main.c lista.o
	gcc -c main.c

lista.o: lista.c
	gcc -c lista.c

clean:
	rm *.o main
