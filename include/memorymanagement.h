
#ifndef __MEMORYMANAGEMENT_H
    #define __MEMORYMANAGEMENT_H

    #include<common/types.h>

    namespace MYYOS
    {

        class MemoryManager
        {
            protected:
                uint8_t* startOfManagedMemory;
                uint8_t* endOfManagedMemory;
                uint8_t* startOfFreeMemory;

            public:

                MemoryManager(uint8_t* startOfFreeMemory, uint32_t sizeOfManagedMemory);
                ~MemoryManager();

                uint8_t* AllocateMemory(uint32_t numberOfBytes);
                void DeallocateMemory(uint8_t* pointer);

                template<class T> T* Instantiate(uint32_t numberOfElements=1);
                template<class T, class P1> T* Instantiate(P1);
                template<class T, class P1, class P2> T* Instantiate(P1, P2);
                template<class T, class P1, class P2, class P3> T* Instantiate(P1, P2, P3);
                template<class T, class P1, class P2, class P3, class P4> T* Instantiate(P1, P2, P3, P4);
        };

    }

    #include "../src/memorymanagement_templates.cpp"

#endif