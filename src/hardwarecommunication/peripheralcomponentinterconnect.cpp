
#include <hardwarecommunication/peripheralcomponentinterconnect.h>

using namespace MYYOS::drivers;
using namespace MYYOS::common;

namespace MYYOS
{
    namespace hardwarecommunication
    {

        PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
        {
        }

        PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
        {
        }

        string PeripheralComponentInterconnectDeviceDescriptor::TypeString()
        {
            switch (class_id)
            {
                case 0x00: return (subclass_id == 0x01) ? "VGA" : "Legacy";
                case 0x01:
                    switch(subclass_id)
                    {
                        case 0x01:  return "IDE interface";
                        case 0x06:  return "SATA controller";
                        default:    return "Storage";
                    }
                case 0x02: return "Network";                          // Network
                case 0x03: return "Display";
                case 0x04:                                         // Multimedia
                    switch(subclass_id)
                    {
                        case 0x00:  return "Video";
                        case 0x01:
                        case 0x03:  return "Audio";
                        default:    return "Multimedia";
                    }
                case 0x06:                                            // Bridges
                    switch(subclass_id)
                    {
                        case 0x00:  return "Host bridge";
                        case 0x01:  return "ISA bridge";
                        case 0x04:  return "PCI bridge";
                        default:    return "Bridge";
                    }
                case 0x07:
                    switch(subclass_id)
                    {
                        case 0x00:  return "Serial controller";
                        case 0x80:  return "Communication controller";
                    }
                    break;
                case 0x0C:
                    switch(subclass_id)
                    {
                        case 0x03:  return "USB";
                        case 0x05:  return "System Management Bus";
                    }
                    break;
            }
            return "Unknown";
        }





        PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
            : dataPort(0xCFC),
            commandPort(0xCF8)
        {
        }

        PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
        {
        }


        uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t devicenumber, uint16_t function, uint32_t registeroffset)
        {
            uint32_t offset = registeroffset % 0x04; // why not & 0x03 instead?

            commandPort.Write(
                0x1 << 31
                | ((bus            & 0xFF) << 16)
                | ((devicenumber   & 0x1F) << 11)
                | ((function       & 0x07) <<  8)
                | ((registeroffset & 0xFC)));

            return dataPort.Read() >> (8 * offset);
        }

        void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t devicenumber, uint16_t function, uint32_t registeroffset, uint32_t value)
        {
            commandPort.Write(
                0x1 << 31
                | ((bus            & 0xFF) << 16)
                | ((devicenumber   & 0x1F) << 11)
                | ((function       & 0x07) <<  8)
                | ((registeroffset & 0xFC)));

            dataPort.Write(value);
        }



        bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t devicenumber)
        {
            return Read(bus, devicenumber, 0, 0x0E) & (1 << 7);
        }

        PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t devicenumber, uint16_t function)
        {
            PeripheralComponentInterconnectDeviceDescriptor result;

            result.bus          = bus;
            result.devicenumber = devicenumber;
            result.function     = function;

            result.vendor_id    = Read(bus, devicenumber, function, 0x00);
            result.device_id    = Read(bus, devicenumber, function, 0x02);

            result.class_id     = Read(bus, devicenumber, function, 0x0b);
            result.subclass_id  = Read(bus, devicenumber, function, 0x0a);
            result.interface_id = Read(bus, devicenumber, function, 0x09);

            result.revision     = Read(bus, devicenumber, function, 0x08);
            result.interrupt    = Read(bus, devicenumber, function, 0x3c);

            return result;
        }

        BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint32_t bus, uint32_t device, uint32_t function, uint32_t baseaddressregister)
        {
            BaseAddressRegister result;

            uint32_t headertype = Read(bus, device, function, 0x0E);
            if (headertype & 0x3F)
                return result;

            uint32_t bar_value = Read(bus, device, function, 0x10 + 4*baseaddressregister);
            result.type = (bar_value & 1) ? InputOutput : Memory;
            result.address = (uint8_t*) (bar_value & ~0xF); // 16-byte aligned...

            Write(bus, device, function, 0x10 + 4*baseaddressregister, 0xFFFFFFF0 | result.type);

            result.size = Read(bus, device, function, 0x10 + 4*baseaddressregister);
            result.size = (~result.size | 0xF) + 1;

            Write(bus, device, function,  0x10 + 4*baseaddressregister, bar_value);

            return result;
        }

        Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor& device, MemoryManager* memoryManager, InterruptManager* interruptManager, common::OutputStream* errorMessageStream)
        {
            // Select precisely
            switch(device.vendor_id)
            {
                case 0x1022: // Advanced Micro Devices, Inc. (AMD)
                    switch(device.device_id)
                    {
                        case 0x2000: return memoryManager->Instantiate<ethernet::advancedmicrodevices::am79c973>(&device, interruptManager, errorMessageStream);
                    }
                    break;
            }

            // Select generic
            switch (device.class_id)
            {
                case 0x03: // graphics
                    switch(device.subclass_id)
                    {
                        case 0x00: // Video Graphics Array
                            if(device.interface_id == 0x00)
                                return memoryManager->Instantiate<video::VideoGraphicsArray>();
                            break;
                    }
                    break;
            }

            return 0;
        }

        void PeripheralComponentInterconnectController::SelectDrivers(DriverSelectorEventHandler* handler, MemoryManager* memoryManager, InterruptManager* interruptManager, common::OutputStream* errorMessageStream)
        {
            for (int bus = 0; bus < 8; bus++)
            {
                for (int device = 0; device < 32; device++)
                {
                    int functions = DeviceHasFunctions(bus,device) ? 8 : 1;

                    for (int function = 0; function < functions; function++)
                    {
                        PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                        // slot empty
                        if(dev.vendor_id == 0x0 || dev.vendor_id == 0xffff)
                            break;

                        // get port number
                        for (int barnum = 5; barnum >= 0; --barnum)
                        {
                            BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barnum);
                            if (bar.address && (bar.type == InputOutput))
                                dev.portBase = (uint32_t)bar.address;
                        }

                        // instantiate driver and pass it to the handler
                        Driver* driver = GetDriver(dev, memoryManager, interruptManager, errorMessageStream);
                        if(driver != 0)
                            handler->OnDriverSelected(driver);
                    }
                }
            }
        }


    }
}