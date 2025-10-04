
#ifndef __COMMON__FACTORY_H
    #define __COMMON__FACTORY_H

    #include<common/map.h>

    namespace MYYOS
    {
        namespace common
        {

            template<class T> class Instantiator
            {
                public:
                    Instantiator();
                    ~Instantiator();
                    T* Instantiate();
            };

            template<class Key, class T> class Factory
            {
                protected:
                    Map<Key, Instantiator<T>*> Instantiators;
                public:
                    Factory();
                    ~Factory();
                    void Register(Key& key, Instantiator<T>* instantiator);
                    T* Produce(Key&);
            };

        }
    }

    #include "../../src/common/factory_templates.cpp"

#endif