mymalloc: mymalloc.o memgrind.o
	gcc -o mymalloc -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla mymalloc.o memgrind.o
	rm *.o
mymalloc.o: mymalloc.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla  mymalloc.c
memgrind.o: memgrind.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla memgrind.c
clean:
	rm mymalloc
