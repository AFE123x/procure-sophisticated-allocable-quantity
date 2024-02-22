memgrind: mymalloc.o memgrind.o
	gcc -o memgrind -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla mymalloc.o memgrind.o -O2 
memtest: mymalloc.o memtest.o
	gcc -o memtest -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla mymalloc.o memtest.o -O2
mymalloc.o: mymalloc.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla  mymalloc.c -O2
memgrind.o: memgrind.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla memgrind.c -O2
memtest.o: memtest.c
	gcc -c -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla memtest.c -O2
cleanmemgrind: memgrind
	rm memgrind.o
	rm mymalloc.o
	rm memgrind
cleanmemtest: memtest
	rm memtest.o
	rm mymalloc.o
	rm mytest
cleanall: memgrind memtest
	rm *.o
	rm memtest
	rm memgrind

# the command I ran for testing on 2/20/24: \
gcc mymalloc.c memgrind.c -g -O2 -fsanitize=address,undefined -o memgrinding
