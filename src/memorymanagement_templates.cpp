



#ifndef __MEMORYMANAGEMENT_TEMPLATES_CPP
    #define __MEMORYMANAGEMENT_TEMPLATES_CPP

    namespace MYYOS
    {

        template<class T> T* MemoryManager::Instantiate(uint32_t numberOfElements)
        {
            T* result = (T*)AllocateMemory(sizeof(T)*numberOfElements);
            new (result) T();
            return result;
        }

        template<class T, class P1> T* MemoryManager::Instantiate(P1 p1)
        {
            T* result = (T*)AllocateMemory(sizeof(T));
            new (result) T(p1);
            return result;
        }

        template<class T, class P1, class P2> T* MemoryManager::Instantiate(P1 p1, P2 p2)
        {
            T* result = (T*)AllocateMemory(sizeof(T));
            new (result) T(p1, p2);
            return result;
        }

        template<class T, class P1, class P2, class P3> T* MemoryManager::Instantiate(P1 p1, P2 p2, P3 p3)
        {
            T* result = (T*)AllocateMemory(sizeof(T));
            new (result) T(p1, p2, p3);
            return result;
        }

        template<class T, class P1, class P2, class P3, class P4> T* MemoryManager::Instantiate(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            T* result = (T*)AllocateMemory(sizeof(T));
            new (result) T(p1, p2, p3, p4);
            return result;
        }

    }

#endif