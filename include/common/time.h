
#ifndef __COMMON__TIME_H
    #define __COMMON__TIME_H

    namespace MYYOS
    {
        namespace common
        {

            struct Time
            {
                uint16_t year;
                uint8_t month;
                uint8_t day;

                uint8_t hour;
                uint8_t minute;
                uint8_t second;
            };

        }
    }

#endif
