
#include<drivers/ethernet/amd_am79c973.h>
#include<drivers/clock/clock.h>

using namespace MYYOS::common;
using namespace MYYOS::hardwarecommunication;

namespace MYYOS
{
    namespace drivers
    {
        namespace ethernet
        {
            namespace advancedmicrodevices
            {


                am79c973::am79c973(DeviceDescriptor* deviceDescriptor, InterruptManager* interruptManager, OutputStream* errorMessageStream)
                :   EthernetDriver(errorMessageStream),
                    InterruptHandler(interruptManager->HardwareInterruptOffset() + deviceDescriptor->interrupt, interruptManager),
                    mediaAccessControlAddressPort0(deviceDescriptor->portBase),
                    mediaAccessControlAddressPort2(deviceDescriptor->portBase + 0x02),
                    mediaAccessControlAddressPort4(deviceDescriptor->portBase + 0x04),
                    registerDataPort(deviceDescriptor->portBase + 0x10),
                    registerAddressPort(deviceDescriptor->portBase + 0x12),
                    resetPort(deviceDescriptor->portBase + 0x14),
                    busControlRegisterDataPort(deviceDescriptor->portBase + 0x16)
                {
                    active = false;
                    initDone = false;

                    ownMediaAccessControlAddress = 0;
                    currentSendBuffer = 0;
                    currentReceiveBuffer = 0;
                }

                am79c973::~am79c973()
                {
                }

#define big_endian_word(x) ((((x) & 0xFF) << 8) | (((x) & 0xFF00) >> 8))


                uint32_t am79c973::Reset()
                {
                    resetPort.Read();
                    resetPort.Write(0);

                    return 10; // for 10 millisecond delay
                }

                void am79c973::Initialize()
                {
                    uint16_t mediaAccessControlAddressPart0 = mediaAccessControlAddressPort0.Read();
                    uint16_t mediaAccessControlAddressPart2 = mediaAccessControlAddressPort2.Read();
                    uint16_t mediaAccessControlAddressPart4 = mediaAccessControlAddressPort4.Read();
                    ownMediaAccessControlAddress = CreateMediaAccessControlAddress(mediaAccessControlAddressPart0 % 256,
                                                                                   mediaAccessControlAddressPart0 / 256,
                                                                                   mediaAccessControlAddressPart2 % 256,
                                                                                   mediaAccessControlAddressPart2 / 256,
                                                                                   mediaAccessControlAddressPart4 % 256,
                                                                                   mediaAccessControlAddressPart4 / 256);


                    // 3. b
                    // set to 32 bit mode
                    registerAddressPort.Write(20);
                    busControlRegisterDataPort.Write(0x102);

                    // 4. STOP Reset
                    registerAddressPort.Write(0);
                    registerDataPort.Write(0x04); // bit no 2 (zero based) is the STOP Bit (see page 125 of the manual)

                    // 6.
                    initializationBlock.mode = 0x0000; // promiscuous mode = false
                    initializationBlock.reserved1 = 0;
                    initializationBlock.numberOfSendBuffers = 3; // 2^3 = 8 buffers
                    initializationBlock.reserved2 = 0;
                    initializationBlock.numberOfReceiveBuffers = 3; // 2^3 = 8 buffers
                    initializationBlock.physicalAddress = GetMediaAccessControlAddress();
                    initializationBlock.reserved3 = 0;
                    initializationBlock.logicalAddress = 0;

                    sendBufferDescriptors = (BufferDescriptor*)((((uint32_t)&sendBufferDescriptorMemory[0]) + 15) & ~((uint32_t)0xF));
                    initializationBlock.sendBufferDescriptorsAddress = (uint32_t)sendBufferDescriptors;
                    receiveBufferDescriptors = (BufferDescriptor*)((((uint32_t)&receiveBufferDescriptorMemory[0]) + 15) & ~((uint32_t)0xF));
                    initializationBlock.receiveBufferDescriptorsAddress = (uint32_t)receiveBufferDescriptors;

                    // 7.
                    for(uint8_t i = 0; i < 8; i++)
                    {
                        // the +15 and & ~0xF are for the stupid 16-byte alignment
                        sendBufferDescriptors[i].address = (((uint32_t)&sendBuffers[i]) + 15) & ~(uint32_t)0xF;
                        sendBufferDescriptors[i].flags =      0x7FF  // length = 2kb
                                                       |     0xF000;
                        sendBufferDescriptors[i].flags2 = 0;
                        sendBufferDescriptors[i].avail = 0;

                        receiveBufferDescriptors[i].address = (((uint32_t)&receiveBuffers[i]) + 15) & ~(uint32_t)0xF ;
                        receiveBufferDescriptors[i].flags =  0xF7FF
                                                          | 0x80000000; // type = receive buffer
                        receiveBufferDescriptors[i].flags2 =0;
                        receiveBufferDescriptors[i].avail = 0;
                    }

                    // 8. Register Initialization Block in Chip
                    registerAddressPort.Write(1);
                    registerDataPort.Write( ((uint32_t)(&initializationBlock)) % 65536 );
                    registerAddressPort.Write(2);
                    registerDataPort.Write( ((uint32_t)(&initializationBlock)) / 65536 );

                }

                void am79c973::Activate()
                {
                    // 9. Enable Interrupts
                    initDone = false;
                    registerAddressPort.Write(0);
                    registerDataPort.Write(0x41);
                    while(!initDone);

                    // 10.
                    registerAddressPort.Write(4);
                    uint32_t temp = registerDataPort.Read();
                    registerAddressPort.Write(4);
                    registerDataPort.Write(temp | 0xC00);

                    // 11. Start
                    registerAddressPort.Write(0);
                    registerDataPort.Write(0x42);

                    active = true;
                }

                void am79c973::Deactivate()
                {
                }



                string am79c973::GetVendorName()
                {
                    return "AMD";
                }

                string am79c973::GetDeviceName()
                {
                    return "PCnet-FAST III (am79c973)";
                }

                void am79c973::DoSend(uint8_t* buffer, uint32_t size)
                {
                    while(!active);

                    int sendDescriptor = currentSendBuffer;
                    currentSendBuffer = (currentSendBuffer + 1) % 8;

                    if(size > 1518)
                        size = 1518;

                    for(uint8_t* src = buffer+size-1, *dst = (uint8_t*)(sendBufferDescriptors[sendDescriptor].address+size-1); src >= buffer; --src, --dst)
                        *dst = *src;

                    sendBufferDescriptors[sendDescriptor].avail = 0;
                    sendBufferDescriptors[sendDescriptor].flags2 = 0; // clear previous error messages
                    sendBufferDescriptors[sendDescriptor].flags = 0x8300F000
                                                                | ((uint16_t)((-size) & 0xFFFF));

                    registerAddressPort.Write(0);
                    registerDataPort.Write(0x48);
                }

                void am79c973::FetchDataReceived()
                {
                    for(;(receiveBufferDescriptors[currentReceiveBuffer].flags & 0x80000000) == 0; currentReceiveBuffer = (currentReceiveBuffer+1)%8)
                    {
                        if(!(receiveBufferDescriptors[currentReceiveBuffer].flags    & 0x40000000)
                            && (receiveBufferDescriptors[currentReceiveBuffer].flags & 0x03000000) == 0x03000000)
                        {
                            uint32_t size = receiveBufferDescriptors[currentReceiveBuffer].flags2 & 0xFFF;
                            if (size > 64)
                                size -= 4;

                            uint8_t* buffer = (uint8_t*)(receiveBufferDescriptors[currentReceiveBuffer].address);
                            FireDataReceived(buffer, size);
                        }

                        receiveBufferDescriptors[currentReceiveBuffer].flags2 = 0;
                        receiveBufferDescriptors[currentReceiveBuffer].flags = 0x8000F7FF;
                    }
                }

                void am79c973::FetchDataSent()
                {
                    /*
                    for(;(receiveBufferDescriptors[currentReceiveBuffer].flags & 0x80000000) == 0; currentReceiveBuffer = (currentReceiveBuffer+1)%8)
                    {
                        if(!(receiveBufferDescriptors[currentReceiveBuffer].flags    & 0x40000000)
                            && (receiveBufferDescriptors[currentReceiveBuffer].flags & 0x03000000) == 0x03000000)
                        {
                            uint32_t size = receiveBufferDescriptors[currentReceiveBuffer].flags2 & 0xFFF;
                            if (size > 64)
                                size -= 4;

                            uint8_t* buffer = (uint8_t*)(receiveBufferDescriptors[currentReceiveBuffer].address);
                            FireDataSent(buffer, size);
                        }

                        receiveBufferDescriptors[currentReceiveBuffer].flags2 = 0;
                        receiveBufferDescriptors[currentReceiveBuffer].flags = 0x8000F7FF;
                    }
                    */
                }

                MediaAccessControlAddress am79c973::GetMediaAccessControlAddress()
                {
                    while(ownMediaAccessControlAddress == 0);
                    return ownMediaAccessControlAddress;
                }

                void am79c973::HandleInterrupt()
                {
                    registerAddressPort.Write(0);
                    uint32_t temp = registerDataPort.Read();

                    if((temp & 0x8000) == 0x8000) ErrorMessage("AMD am79c973 Error\n");
                    if((temp & 0x2000) == 0x2000) ErrorMessage("AMD am79c973 Collision Error\n");
                    if((temp & 0x1000) == 0x1000) ErrorMessage("AMD am79c973 Missed Frame\n");
                    if((temp & 0x0800) == 0x0800) ErrorMessage("AMD am79c973 Memory Error\n");
                    if((temp & 0x0400) == 0x0400) FetchDataReceived();
                    if((temp & 0x0200) == 0x0200) FetchDataSent();

                    // acknowledge
                    registerAddressPort.Write(0);
                    registerDataPort.Write(temp);

                    if((temp & 0x0100) == 0x0100) // initialization done
                        initDone = true;

                }

            }
        }
    }
}
