Project: MyLittleMalloc

Due Date: 2/20/24 (11:59PM)

*Participating Students: Arun (ajf277), Kareem(kkj47)*

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

the difference between our malloc and free is that it should be able to have error handling

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
4. The run-time system never moves or resizes an allocated chunk. 1
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


### Differences between our code
with our implementation of malloc we use a full linked list struct with previous and next nodes for ease of implementation.

because we are using this style of formatting for our structure, it ends up being (padded) to size 32. This size is more than wanted but because we have the nodes to point to previous and next it is a lot easier to get from header to header forwards and backwards.















---
### Mary (*netid to be added*) said this

---