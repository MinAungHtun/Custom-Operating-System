
#ifndef __HARDWARECOMMUNICATION__PERIPHERALCOMPONENTINTERCONNECT_H
    #define __HARDWARECOMMUNICATION__PERIPHERALCOMPONENTINTERCONNECT_H

    #include <common/types.h>
    #include <common/string.h>
    #include <hardwarecommunication/devicedescriptor.h>
    #include <hardwarecommunication/bus.h>
    #include <hardwarecommunication/port.h>
    #include <drivers/driver.h>

    #include <drivers/ethernet/amd_am79c973.h>
    #include <drivers/video/videographicsarray.h>


    namespace MYYOS
    {
        namespace hardwarecommunication
        {

            enum BaseAddressRegisterType
            {
                Memory = 0,
                InputOutput = 1
            };

            class BaseAddressRegister
            {
                public:
                    uint8_t* address;
                    uint32_t size;
                    BaseAddressRegisterType type;
            };

            class PeripheralComponentInterconnectDeviceDescriptor : public DeviceDescriptor
            {
                friend class PeripheralComponentInterconnectController;
                public:
                    uint16_t bus;
                    uint16_t devicenumber;
                    uint16_t function;

                    uint16_t vendor_id;
                    uint16_t device_id;

                    uint8_t class_id;
                    uint8_t subclass_id;
                    uint8_t interface_id;

                    uint8_t revision;

                public:

                    PeripheralComponentInterconnectDeviceDescriptor();
                    ~PeripheralComponentInterconnectDeviceDescriptor();
                    common::string TypeString();
            };

            class PeripheralComponentInterconnectController : public drivers::DriverSelector
            {
                protected:
                    Port32Bit dataPort;
                    Port32Bit commandPort;
                    uint32_t Read(uint16_t bus, uint16_t devicenumber, uint16_t function, uint32_t registeroffset);
                    void Write(uint16_t bus, uint16_t devicenumber, uint16_t function, uint32_t registeroffset, uint32_t value);

                    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t devicenumber, uint16_t function);
                    BaseAddressRegister GetBaseAddressRegister(uint32_t bus, uint32_t device, uint32_t function, uint32_t baseaddressregister);
                    bool DeviceHasFunctions(uint16_t bus, uint16_t devicenumber);
                    drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor& device, MemoryManager* memoryManager, InterruptManager* interruptManager, common::OutputStream* errorMessageStream);

                public:
                    PeripheralComponentInterconnectController();
                    ~PeripheralComponentInterconnectController();

                    void SelectDrivers(drivers::DriverSelectorEventHandler* handler, MemoryManager* memoryManager, InterruptManager* interruptManager, common::OutputStream* errorMessageStream);
            };

        }
    }

#endif
