all: grafo.o main.o
	gcc grafo.o main.o -o main

run: main
	./main

main.o: main.c grafo.o
	gcc -c main.c

grafo.o: grafo.c
	gcc -c grafo.c

clean:
	rm *.o main
