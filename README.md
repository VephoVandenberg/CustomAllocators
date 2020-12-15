# Custom Memory Allocators
Here are my custom memory allocators. I write them for educational purposes.
Each implementation differ from another, so don't blame if one is worse than another.
Just doing different implementations for myself for pure self-education.

MemoryAllocatar1-2 will work only in UNIX systems. Why? Because only in Linux/MacOS(I'm not sure about MacOS) I can access heap without any problems using sbrk() function, so this feature can't be achieved in Windows.
