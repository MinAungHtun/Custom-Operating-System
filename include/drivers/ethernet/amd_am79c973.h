
#ifndef __DRIVERS__ETHERNET__AMD_AM79C973_H
    #define __DRIVERS__ETHERNET__AMD_AM79C973_H

    #include <hardwarecommunication/devicedescriptor.h>
    #include <hardwarecommunication/port.h>
    #include <hardwarecommunication/interrupts.h>
    #include <drivers/ethernet/ethernet.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace ethernet
            {

                namespace advancedmicrodevices
                {


                    class am79c973 : public EthernetDriver, public hardwarecommunication::InterruptHandler
                    {
                        protected:

                            struct InitializationBlock
                            {
                                uint16_t mode;

                                unsigned reserved1 : 4;
                                unsigned numberOfReceiveBuffers : 4;
                                unsigned reserved2 : 4;
                                unsigned numberOfSendBuffers : 4;
                                uint64_t physicalAddress : 48;
                                uint16_t reserved3;
                                uint64_t logicalAddress;
                                uint32_t receiveBufferDescriptorsAddress;
                                uint32_t sendBufferDescriptorsAddress;
                            } __attribute__((packed));

                            struct BufferDescriptor
                            {
                                uint32_t address;
                                uint32_t flags;
                                uint32_t flags2;
                                uint32_t avail;
                            } __attribute__((packed));


                            hardwarecommunication::Port16Bit mediaAccessControlAddressPort0;
                            hardwarecommunication::Port16Bit mediaAccessControlAddressPort2;
                            hardwarecommunication::Port16Bit mediaAccessControlAddressPort4;
                            hardwarecommunication::Port16Bit registerDataPort;
                            hardwarecommunication::Port16Bit registerAddressPort;
                            hardwarecommunication::Port16Bit resetPort;
                            hardwarecommunication::Port16Bit busControlRegisterDataPort;

                            InitializationBlock initializationBlock;
                            uint8_t sendBufferDescriptorMemory[2048+15];
                            uint8_t receiveBufferDescriptorMemory[2048+15];
                            BufferDescriptor *sendBufferDescriptors;
                            BufferDescriptor *receiveBufferDescriptors;
                            uint8_t sendBuffers[2*1024+15][8]; // 2kb + padding for 16-byte alignment
                            uint8_t receiveBuffers[2*1024+15][8]; // 2kb + padding for 16-byte alignment
                            uint8_t currentSendBuffer;
                            uint8_t currentReceiveBuffer;

                            MediaAccessControlAddress ownMediaAccessControlAddress;
                            volatile bool active;
                            volatile bool initDone;

                            void FetchDataReceived();
                            void FetchDataSent();
                        public:
                            am79c973(hardwarecommunication::DeviceDescriptor* deviceDescriptor, hardwarecommunication::InterruptManager* interruptManager, common::OutputStream* errorMessageStream = 0);
                            ~am79c973();

                            uint32_t Reset();
                            void Initialize();
                            void Activate();
                            void Deactivate();
                            common::string GetVendorName();
                            common::string GetDeviceName();

                            void DoSend(uint8_t* buffer, uint32_t size);
                            MediaAccessControlAddress GetMediaAccessControlAddress();
                            void HandleInterrupt();

                            inline void* operator new(uint32_t, am79c973* p)
                            {
                                return p;
                            }
                    };


                }
            }
        }
    }

#endif
