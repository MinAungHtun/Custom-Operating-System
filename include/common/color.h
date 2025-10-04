
#ifndef __COMMON__COLOR_H
    #define __COMMON__COLOR_H

    #include<common/types.h>

    namespace MYYOS
    {
        namespace common
        {

            class Color
            {
                public:
                    uint8_t Red;
                    uint8_t Green;
                    uint8_t Blue;
                    uint8_t Alpha;

                    Color();
                    Color(uint8_t Red, uint8_t Green, uint8_t Blue);
                    Color(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);
                    ~Color();
            };

        }
    }

#endif
