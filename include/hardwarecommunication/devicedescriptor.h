
#ifndef __HARDWARECOMMUNICATION__DEVICEDESCRIPTOR_H
    #define __HARDWARECOMMUNICATION__DEVICEDESCRIPTOR_H

    #include <common/types.h>

    namespace MYYOS
    {
        namespace hardwarecommunication
        {

            class DeviceDescriptor
            {
                public:
                    uint32_t portBase;
                    uint32_t interrupt;

                    DeviceDescriptor();
                    DeviceDescriptor(uint32_t port_base, uint32_t interrupt);
                    ~DeviceDescriptor();
            };

        }
    }

#endif
