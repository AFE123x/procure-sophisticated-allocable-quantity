Project: MyLittle~~Pony~~Malloc

Due Date: 2/20/24 (11:59PM)

*Participating Students: Arun Felix (ajf277), Kareem Jackson (kkj47)*

---
### Background and Implementation
The heap is a region of memory managed by the run-time system through two functions, malloc()
and free(), which allocate and deallocate portions of the heap for use by client code.
We will model the heap as a sequence of variably sized chunks, where a chunk is a contiguous
sequence of bytes in the heap and all bytes in the heap belong to exactly one chunk. Each chunk has
a size, which is the number of bytes it contains, and may be either allocated (in-use) or deallocated
(free).

---

### What we have to do
We have to implement a working malloc and free replacement.

the difference between our malloc and free compared to the original is that it should be able to have error handling.

the **Rules** our implementation must operate by are:
1. On success, malloc() returns a pointer to the payload of an allocated chunk containing at
least the requested number of bytes. The payload does not overlap any other allocated chunks.
2. The run-time system makes no assumptions about the data written to the payload of a chunk.
In particular, it cannot assume that certain special values are not written to the payload. In
other words, the run-time cannot extract any information by looking at the payload of an
allocated chunk. Conversely, clients may write to any byte received from malloc() without
causing problems for the run-time system.
3. The run-time system never writes to the payload of an allocated chunk. Client code may
assume that data it writes to an object will remain, unchanged, until the object is explicitly
freed.
4. The run-time system never moves or resizes an allocated chunk.
5. The client never reads or writes outside the boundaries of the allocated payloads it receives.
The run-time system can assume that any data it writes to chunk headers or to the payloads
of unallocated chunks will be not be read or updated by client code.

#### Errors we have to detect
- Calling free() with an address not obtained from malloc()
- Calling free() with an address not at the start of a chunk
- Calling free() a second time on the same pointer
- Calling malloc() with a size of 0
- Calling malloc() with a size that is to big
- Calling malloc() with no more free space

---
### How we implemented our code
We use a struct for our metadata. The metadata we contain are the size_t for our size of the data and a char for holding the address for the adjacent block. That char allows for easier access of the next block and does not increase the size of our header. So our header is still of size 8 bytes.

For finding where to put our mallocs, we use first fit. So whatever the first block is that allows for space, we use the space for malloc.

**NOTE** The files: test.c, test, and client.c are only in the files for our testing purposes. they are not included in our makefile. You can ignore them.

**NOTE** When running the makefile there is warnings- these can be ignored. The program runs successfully

### Performance tests made
There are 5 required tests that should be run on our code. 3 of which are provided:

1. malloc() and immediately free() a 1-byte object, 120 times.

2. Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to
deallocate the chunks.

3. Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte
object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects.

The final 2 were created by us:

4. malloc() 101 1 byte objects, free the odd pointers, then free the even pointers. Then malloc() 101 more 1 byte objects then free the even pointers then free everything.

5. malloc() all of the memory and then free it all. but free from the edges inwards

### The average results of the tests

1. 0.000001 seconds
2. 0.000029 seconds
3. 0.000000 seconds
4. 0.000084 seconds
5. 0.000000 seconds



