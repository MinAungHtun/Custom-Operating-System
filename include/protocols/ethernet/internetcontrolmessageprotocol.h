
#ifndef __PROTOCOLS__ETHERNET__INTERNETCONTROLMESSAGEPROTOCOL_H
    #define __PROTOCOLS__ETHERNET__INTERNETCONTROLMESSAGEPROTOCOL_H

    #include<common/types.h>
    #include<protocols/ethernet/internetprotocol.h>

    namespace MYYOS
    {
        namespace protocols
        {
            namespace ethernet
            {

                struct InternetControlMessageProtocolHeader
                {
                    uint8_t type;          // ICMP packet type
                    uint8_t code;          // Type sub code
                    uint16_t checksum;
                    uint32_t data;
                } __attribute__((packed));


                class InternetControlMessageProtocolHandler : public InternetProtocolPayloadHandler
                {
                    public:
                        InternetControlMessageProtocolHandler(InternetProtocolHandler* backend);
                        ~InternetControlMessageProtocolHandler();

                        bool HandleInternetProtocolPayload(InternetProtocolAddress remoteInternetProtocolAddress,
                                                           InternetProtocolAddress localInternetProtocolAddress,uint8_t* data, uint32_t length);
                        void RequestEchoReply(InternetProtocolAddress remoteInternetProtocolAddress);
                        static uint16_t Checksum(uint16_t* buffer, uint32_t length);
                };

            }
        }
    }

#endif
