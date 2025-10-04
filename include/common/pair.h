
#ifndef __COMMON__PAIR_H
    #define __COMMON__PAIR_H

    namespace MYYOS
    {
        namespace common
        {

            template<class First, class Second> class Pair
            {
                public:
                    First first;
                    Second second;

                    Pair();
                    Pair(First, Second);
                    ~Pair();
            };



        }
    }

    #include "../../src/common/pair_templates.cpp"

#endif