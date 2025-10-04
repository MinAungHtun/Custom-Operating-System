
#ifndef __PROTOCOLS__ETHERNET__ETHERNETFRAME_H
    #define __PROTOCOLS__ETHERNET__ETHERNETFRAME_H

    #include<common/types.h>
    #include<common/outputstream.h>
    #include<common/map.h>
    #include<drivers/ethernet/ethernet.h>
    #include<memorymanagement.h>

    namespace MYYOS
    {
        namespace protocols
        {
            namespace ethernet
            {

                struct EthernetFrameHeader
                {
                    uint64_t destination   : 48;
                    uint64_t source  : 48;
                    uint16_t type; // IP=0x800, ARP=0x0806
                } __attribute__ ((packed));

                struct EthernetFrameFooter
                {
                    uint32_t checksum;
                } __attribute__ ((packed));




                class EthernetFrameHandler;
                class EthernetFramePayloadHandler
                {
                    friend class EthernetFrameHandler;
                    protected:
                        EthernetFrameHandler* backend;
                        uint16_t handledEthernetFrameType;
                    public:
                        EthernetFramePayloadHandler(EthernetFrameHandler* backend, uint16_t handledEthernetFrameType);
                        ~EthernetFramePayloadHandler();
                        virtual bool HandleEthernetFramePayload(uint8_t* data, uint32_t length);
                        void Send(MYYOS::drivers::ethernet::MediaAccessControlAddress destination, uint8_t* data, uint32_t length);
                };


                class EthernetFrameHandler : public MYYOS::drivers::ethernet::EthernetDriverEventHandler
                {
                    protected:
                        common::Map<uint16_t, EthernetFramePayloadHandler*> handlers;
                        MYYOS::drivers::ethernet::EthernetDriver* backend;
                        common::OutputStream *errorMessages;
                        MemoryManager* memoryManager;
                    public:
                        EthernetFrameHandler(MYYOS::drivers::ethernet::EthernetDriver* backend, common::OutputStream* errorMessages, MemoryManager* memoryManager);
                        ~EthernetFrameHandler();

                        MYYOS::drivers::ethernet::MediaAccessControlAddress GetMediaAccessControlAddress();
                        bool DataReceived(uint8_t* data, uint32_t length);
                        void ConnectEthernetFramePayloadHandler(EthernetFramePayloadHandler* handler);
                        void Send(MYYOS::drivers::ethernet::MediaAccessControlAddress destination, uint16_t frameType, uint8_t* data, uint32_t length);
                };

            }
        }
    }

#endif

