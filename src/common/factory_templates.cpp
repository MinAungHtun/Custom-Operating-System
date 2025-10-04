
#ifndef __COMMON__FACTORY_TEMPLATES_CPP
    #define __COMMON__FACTORY_TEMPLATES_CPP

    namespace MYYOS
    {
        namespace common
        {

            template<class T> Instantiator<T>::Instantiator()
            {
            }

            template<class T> Instantiator<T>::~Instantiator()
            {
            }

            template<class T> T* Instantiator<T>::Instantiate()
            {
                return 0;
            }



            template<class Key, class T> Factory<Key,T>::Factory()
            {
            }

            template<class Key, class T> Factory<Key,T>::~Factory()
            {
                Instantiators.clear();
            }

            template<class Key, class T> void Factory<Key,T>::Register(Key& key, Instantiator<T>* instantiator)
            {
                if(instantiator != 0)
                    Instantiators.insert(key, instantiator);
            }

            template<class Key, class T> T* Factory<Key,T>::Produce(Key& key)
            {
                Map<Key,Instantiator<T>*>::iterator it = Instantiators.find(key);
                if(it == Instantiators.end())
                    return 0;
                return it->second->Instantiate();
            }

        }
    }

#endif