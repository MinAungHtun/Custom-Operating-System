
#ifndef __COMMON__PAIR_TEMPLATES_CPP
    #define __COMMON__PAIR_TEMPLATES_CPP

    namespace MYYOS
    {
        namespace common
        {

            template<class First, class Second> Pair<First,Second>::Pair()
            {
            }
            
            template<class First, class Second> Pair<First,Second>::Pair(First first, Second second)
            {
                this->first = first;
                this->second = second;
            }

            template<class First, class Second> Pair<First,Second>::~Pair()
            {
            }

        }
    }

#endif