mymalloc: mymalloc.o memgrind.o
	gcc -o mymalloc -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla mymalloc.o memgrind.o
memtest: mymalloc.o memtest.o
	gcc -o memtest -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla mymalloc.o memtest.o
mymalloc.o: mymalloc.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla  mymalloc.c
memgrind.o: memgrind.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla memgrind.c
memtest.o: memtest.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla memtest.c
cleanmymymalloc:
	rm *.o
	rm mymalloc
cleanmemtest:
	rm *.o
	rm memtest


# the command I ran for testing on 2/20/24: \
gcc mymalloc.c memgrind.c -g -O2 -fsanitize=address,undefined -o memgrinding