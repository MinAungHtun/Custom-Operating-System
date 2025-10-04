
#ifndef __PROTOCOLS__ETHERNET__ADDRESSRESOLUTIONPROTOCOL_H
    #define __PROTOCOLS__ETHERNET__ADDRESSRESOLUTIONPROTOCOL_H

    #include<common/types.h>
    #include<common/map.h>
    #include<protocols/ethernet/ethernetframe.h>
    #include<protocols/ethernet/internetprotocol.h>
    #include<common/outputstream.h>

    namespace MYYOS
    {
        namespace protocols
        {
            namespace ethernet
            {

                struct AddressResolutionProtocolHeader
                {
                    uint16_t hardwareType;
                    uint16_t protocol;
                    uint8_t hardwareAddressSize;
                    uint8_t protocolAddressSize;
                    uint16_t command;

                    uint64_t sourceMediaAccessControlAddress : 48;
                    uint32_t sourceInternetProtocolAddress;
                    uint64_t destinationMediaAccessControlAddress : 48;
                    uint32_t destinationInternetProtocolAddress;

                } __attribute__((packed));


                class AddressResolutionProtocolHandler : public EthernetFramePayloadHandler, public InternetProtocolAddressResolver
                {
                    protected:
                        InternetProtocolHandler* internetProtocolHandler;
                        common::Map<InternetProtocolAddress, MYYOS::drivers::ethernet::MediaAccessControlAddress> addressCache;
                        common::OutputStream* errorMessages;
                    public:
                        AddressResolutionProtocolHandler(EthernetFrameHandler* backend, InternetProtocolHandler* internetProtocolHandler,
                            common::OutputStream* errorMessages
                        );
                        ~AddressResolutionProtocolHandler();

                        void RequestMediaAccessControlAddress(InternetProtocolAddress internetProtocolAddress);
                        bool HandleEthernetFramePayload(uint8_t* data, uint32_t length);
                        MYYOS::drivers::ethernet::MediaAccessControlAddress Resolve(InternetProtocolAddress address);
                        void Store(InternetProtocolAddress internetProtocolAddress, MYYOS::drivers::ethernet::MediaAccessControlAddress mediaAccessControlAddress);
                };

            }
        }
    }

#endif
