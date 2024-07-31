# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -std=c99 -fsanitize=address -fsanitize=undefined -Wvla -O2

# Targets and files
TARGETS = memgrind memtest
OBJS = mymalloc.o memgrind.o memtest.o
EXEC = memgrind memtest

# Default target
all: $(TARGETS)

# Build memgrind
memgrind: mymalloc.o memgrind.o
	$(CC) -o $@ $(CFLAGS) mymalloc.o memgrind.o

# Build memtest
memtest: mymalloc.o memtest.o
	$(CC) -o $@ $(CFLAGS) mymalloc.o memtest.o

# Compile mymalloc.o
mymalloc.o: mymalloc.c
	$(CC) -c $(CFLAGS) mymalloc.c

# Compile memgrind.o
memgrind.o: memgrind.c
	$(CC) -c $(CFLAGS) memgrind.c

# Compile memtest.o
memtest.o: memtest.c
	$(CC) -c $(CFLAGS) memtest.c

# Clean up files
clean:
	rm -f $(OBJS) $(EXEC)

# Clean specific targets
cleanmemgrind: memgrind
	rm -f memgrind.o mymalloc.o memgrind

cleanmemtest: memtest
	rm -f memtest.o mymalloc.o memtest

# Clean all targets
cleanall: clean
	rm -f memtest memgrind
