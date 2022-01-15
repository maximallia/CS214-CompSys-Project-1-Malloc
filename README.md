# CS214-CompSys-Project-1-Malloc

Summary:

In this assignment, you will implement malloc() and free() library calls for dynamic memory allocation that detect common programming and usage errors.

Malloc( size_t size ) is a system library call that (hopefully) returns a pointer to a block of memory of at least the requested size. This memory comes from a main memory resource managed by the operating system. The free( void * ) function informs the operating system that you are done with a given block of dynamically-allocated memory, and that it can reclaim it for other uses.

You will use a large array to simulate main memory (static char myblock[4096]). Your malloc() function will return pointers to this large array and your free() function will let your code know that a previously-allocated region can be reclaimed and used for other purposes. Programmers can easily make some very debilitating errors when using dynamic memory. Your versions of malloc() and free() will detect these errors and will react nicely by not allowing a user to do Bad Things. Your malloc() function should use a “first free” algorithm to select blocks of memory to allocate.

MORE INFO: Asst1.pdf
