# Carnegie WaterMelon Notes - DMA Concepts

***What is DMA?***
- DMA, which is ***Dynamic Memory Allocation*** allows one to acquire virtual memory at runtime. 
    - This area of memory is known as the Heap. 

### malloc/free function

malloc and free be like: 

```c
void* malloc(size_t size);
void free(void* p)
```
***malloc***
- If malloc sucessfully finds memory to allocate, it will return the address to a memory block of the ***least size byte***
- If malloc failed, like I failed my family, it will return NULL.
***free***
- free returns the block pointed by B back to the pool of available memory. 

### Malloc padding

- alignment is very important, When the CPU reads from memory, you don't want to read from memory multiple times, cuz it comes at the cost of clock cycles. 

- This is where padding comes in place.
    - With padding, we place variables in spaces that the variable size divides
        - For example, we can only put ints in addresses that are multiples of 4. 

