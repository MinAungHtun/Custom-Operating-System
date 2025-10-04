
#include <hardwarecommunication/devicedescriptor.h>

namespace MYYOS
{
    namespace hardwarecommunication
    {

        DeviceDescriptor::DeviceDescriptor()
        {
            this->portBase = 0;
            this->interrupt = 0;
        }

        DeviceDescriptor::DeviceDescriptor(uint32_t portBase, uint32_t interrupt)
        {
            this->portBase = portBase;
            this->interrupt = interrupt;
        }

        DeviceDescriptor::~DeviceDescriptor()
        {
        }

    }
}
