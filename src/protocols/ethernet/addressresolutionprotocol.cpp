
#include<protocols/ethernet/addressresolutionprotocol.h>

using namespace MYYOS::drivers::ethernet;

namespace MYYOS
{
    namespace protocols
    {
        namespace ethernet
        {



            AddressResolutionProtocolHandler::AddressResolutionProtocolHandler(EthernetFrameHandler* backend, InternetProtocolHandler* internetProtocolHandler,  common::OutputStream* errorMessages)
                : EthernetFramePayloadHandler(backend, 0x0806),
                  InternetProtocolAddressResolver(internetProtocolHandler)
            {
                this->internetProtocolHandler = internetProtocolHandler;
                this->errorMessages = errorMessages;
            }

            AddressResolutionProtocolHandler::~AddressResolutionProtocolHandler()
            {
            }

            bool AddressResolutionProtocolHandler::HandleEthernetFramePayload(uint8_t* data, uint32_t length)
            {
                if(length < sizeof(AddressResolutionProtocolHeader))
                    return false;

                AddressResolutionProtocolHeader* header = (AddressResolutionProtocolHeader*)(data);
                if(header->hardwareType == 0x0100)                // ethernet
                {
                    if(header->protocol == 0x0008                 // address resolution protocol message, regarding internet protocol and it is for me
                        && header->hardwareAddressSize == 6
                        && header->protocolAddressSize == 4
                        && header->destinationInternetProtocolAddress == internetProtocolHandler->GetInternetProtocolAddress())
                    {
                        switch(header->command)
                        {
                            case 0x0200: // response
                                addressCache.insert((InternetProtocolAddress)(header->sourceInternetProtocolAddress),
                                                    (MediaAccessControlAddress)(header->sourceMediaAccessControlAddress));
                                return false;
                            case 0x0100: // request
                                header->command = 0x0200; // make a response out of it
                                header->destinationInternetProtocolAddress = header->sourceInternetProtocolAddress;
                                header->sourceInternetProtocolAddress = internetProtocolHandler->GetInternetProtocolAddress();
                                header->destinationMediaAccessControlAddress = header->sourceMediaAccessControlAddress;
                                header->sourceMediaAccessControlAddress = internetProtocolHandler->GetMediaAccessControlAddress();
                                return true;
                        }
                    }
                }
                return false;
            }

            void AddressResolutionProtocolHandler::RequestMediaAccessControlAddress(InternetProtocolAddress internetProtocolAddress)
            {
                AddressResolutionProtocolHeader arp;
                arp.hardwareType = 0x0100;
                arp.protocol = 0x0008; // big endian for 0x0800=IPv4
                arp.hardwareAddressSize = 6; // media access control address has 6 bytes
                arp.protocolAddressSize = 4; // internet protocol v4 address has 4 bytes
                arp.command = 0x0100; // request

                arp.sourceMediaAccessControlAddress = backend->GetMediaAccessControlAddress();
                arp.sourceInternetProtocolAddress = internetProtocolHandler->GetInternetProtocolAddress();
                arp.destinationMediaAccessControlAddress = EthernetDriver::CreateMediaAccessControlAddress(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF); // broadcast address
                arp.destinationInternetProtocolAddress = internetProtocolAddress;

                EthernetFramePayloadHandler::Send(arp.destinationMediaAccessControlAddress, (uint8_t*)&arp, sizeof(AddressResolutionProtocolHeader));
            }

            MediaAccessControlAddress AddressResolutionProtocolHandler::Resolve(InternetProtocolAddress address)
            {
                volatile common::Map<InternetProtocolAddress, MediaAccessControlAddress>::iterator cacheEntry = addressCache.find(address);
                if(cacheEntry == addressCache.end())
                    RequestMediaAccessControlAddress(address);

                while(cacheEntry == addressCache.end()) // FIXME: should timeout
                    cacheEntry = addressCache.find(address);

                return cacheEntry->second;
            }

            void AddressResolutionProtocolHandler::Store(InternetProtocolAddress internetProtocolAddress, MediaAccessControlAddress mediaAccessControlAddress)
            {
                addressCache.insert(internetProtocolAddress, mediaAccessControlAddress);
            }

        }
    }
}

