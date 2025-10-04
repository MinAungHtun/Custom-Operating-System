// public domain

typedef void (*constructor)();

// Included in the linker file

extern "C" constructor start_ctors;

extern "C" constructor end_ctors;

extern "C" void initialiseConstructors();

// This invokes the constructor for global and static objects

void initialiseConstructors()

{
    
    for (constructor* i = &start_ctors; i != &end_ctors; ++i)
        (*i)();

}
