mymalloc: mymalloc.o memgrind.o
	gcc -o mymalloc mymalloc.o memgrind.o
mymalloc.o: mymalloc.c
	gcc -c mymalloc.c
memgrind.o: memgrind.c
	gcc -c memgrind.c
clean:
	rm *.o mymalloc
