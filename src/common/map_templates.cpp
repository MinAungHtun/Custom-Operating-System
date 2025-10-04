
#ifndef __COMMON__MAP_TEMPLATES_CPP
    #define __COMMON__MAP_TEMPLATES_CPP

    #include<common/types.h>

    namespace MYYOS
    {
        namespace common
        {

            template<class Key, class Value> Map<Key,Value>::Map()
            {
            }

            template<class Key, class Value> Map<Key,Value>::~Map()
            {
            }

            template<class Key, class Value> Value& Map<Key,Value>::operator[](Key key)
            {
                return find(key)->second;
            }

            template<class Key, class Value>  typename Map<Key,Value>::iterator Map<Key,Value>::begin()
            {
                return elements.begin();
            }

            template<class Key, class Value>  typename Map<Key,Value>::iterator Map<Key,Value>::end()
            {
                return elements.end();
            }

            template<class Key, class Value> typename Map<Key,Value>::iterator Map<Key,Value>::find(Key key)
            {
                for(iterator i = begin(); i != end(); ++i)
                    if(i->first == key)
                        return i;
                return end();
            }

            template<class Key, class Value> bool Map<Key,Value>::empty()
            {
                return elements.empty();
            }





            template<class Key, class Value> void Map<Key,Value>::insert(Key key, Value value)
            {
                iterator it = find(key);
                if(it == end())
                    elements.push_back(Pair<Key,Value>(key, value));
                else
                    it->second = value;
            }

            template<class Key, class Value> void Map<Key,Value>::erase(Key key)
            {
                elements.erase(find(key));
            }





            template<class Key, class Value> void Map<Key,Value>::Iterate(MapIterationHandler<Key,Value>* handler)
            {
                for(iterator i = begin(); i != end(); ++i)
                    handler->OnRead(i->first, i->second);
                handler->OnEndOfStream();
            }

            template<class Key, class Value> void Map<Key,Value>::Iterate(void callback(Key&, Value&))
            {
                for(iterator i = begin(); i != end(); ++i)
                    callback(i->fist, i->second);
            }

        }
    }

#endif