



#include <memorymanagement.h>

namespace MYYOS
{

    MemoryManager::MemoryManager(uint8_t* startOfManagedMemory, uint32_t sizeOfManagedMemory)
    {
        this->startOfManagedMemory = startOfManagedMemory;
        startOfFreeMemory = startOfManagedMemory;
        endOfManagedMemory = startOfManagedMemory + sizeOfManagedMemory;
    }

    MemoryManager::~MemoryManager()
    {
    }

    uint8_t* MemoryManager::AllocateMemory(uint32_t numberOfBytes)
    {
        if(startOfFreeMemory + numberOfBytes >= endOfManagedMemory)
            return 0;
        uint8_t* result = startOfFreeMemory;
        startOfFreeMemory += numberOfBytes;
        return result;
    }

    void MemoryManager::DeallocateMemory(uint8_t*)
    {
        
    }

}

