# CustomAllocator
Here are custom memory allocators.
Each implementation differ from another, so don't blame if one is worse than another.
Just doing different implementations for myself.

MemoryAllocatar1-2 will work only in UNIX systems. Why? Because only in Linux/MacOS I can access heap without any problems using sbrk() function, this can't be achieved in Windows.
