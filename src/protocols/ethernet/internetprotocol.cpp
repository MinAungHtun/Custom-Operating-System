
#include <protocols/ethernet/internetprotocol.h>

using namespace MYYOS::drivers::ethernet;

namespace MYYOS
{
    namespace protocols
    {
        namespace ethernet
        {

            InternetProtocolAddressResolver::InternetProtocolAddressResolver(InternetProtocolHandler* handler)
            {
                handler->RegisterInternetProtocolAddressResolver(this);
            }

            InternetProtocolAddressResolver::~InternetProtocolAddressResolver()
            {
            }

            MediaAccessControlAddress InternetProtocolAddressResolver::Resolve(InternetProtocolAddress)
            {
                return 0xFFFFFFFFFFFF; // the broadcast address
            }

            void InternetProtocolAddressResolver::Store(InternetProtocolAddress, MediaAccessControlAddress)
            {
            }





            InternetProtocolPayloadHandler::InternetProtocolPayloadHandler(InternetProtocolHandler* backend, uint8_t handledInternetProtocolProtocolNumber)
            {
                this->backend = backend;
                this->handledInternetProtocolProtocolNumber = handledInternetProtocolProtocolNumber;
                backend->ConnectInternetProtocolPayloadHandler(this);
            }

            InternetProtocolPayloadHandler::~InternetProtocolPayloadHandler()
            {
            }

            bool InternetProtocolPayloadHandler::HandleInternetProtocolPayload(InternetProtocolAddress, InternetProtocolAddress, uint8_t*, uint32_t)
            {
                return false;
            }

            void InternetProtocolPayloadHandler::Send(InternetProtocolAddress destination, uint8_t* data, uint32_t length)
            {
                backend->Send(destination, this->handledInternetProtocolProtocolNumber, data, length);
            }





            InternetProtocolHandler::InternetProtocolHandler(EthernetFrameHandler* backend, InternetProtocolAddress ownInternetProtocolAddress,
                                                             InternetProtocolAddress defaultGatewayInternetProtocolAddress, SubnetMask subnetMask,
                                                             common::OutputStream* errorMessages, MemoryManager* memoryManager)
              : EthernetFramePayloadHandler(backend, 0x0800)
            {
                this->ownInternetProtocolAddress = ownInternetProtocolAddress;
                this->defaultGatewayInternetProtocolAddress = defaultGatewayInternetProtocolAddress;
                this->subnetMask = subnetMask;
                this->errorMessages = errorMessages;
                this->memoryManager = memoryManager;
            }

            InternetProtocolHandler::~InternetProtocolHandler()
            {
            }

            InternetProtocolAddress InternetProtocolHandler::GetInternetProtocolAddress()
            {
                return ownInternetProtocolAddress;
            }

            MYYOS::drivers::ethernet::MediaAccessControlAddress InternetProtocolHandler::GetMediaAccessControlAddress()
            {
                return backend->GetMediaAccessControlAddress();
            }

            bool InternetProtocolHandler::HandleEthernetFramePayload(uint8_t* data, uint32_t length)
            {
                if(length < sizeof(InternetProtocolHeader))
                    return false;

                bool sendBack = false;
                InternetProtocolHeader* header = (InternetProtocolHeader*)data;
                common::Map<uint8_t, InternetProtocolPayloadHandler*>::iterator handlerIterator = handlers.find(header->protocol);
                if(handlerIterator != handlers.end())
                    sendBack = handlerIterator->second->HandleInternetProtocolPayload(header->sourceAddress, header->destinationAddress,
                                                                                      data + 4*header->headerLength, length-4*header->headerLength);

                if(sendBack)
                {
                    // header->identification should be set safely
                    header->timeToLive = 255;
                    uint32_t temp = header->destinationAddress;
                    header->destinationAddress = header->sourceAddress;
                    header->sourceAddress = temp;
                    header->headerChecksum = Checksum((uint16_t*)data, 4*header->headerLength);
                }

                return sendBack;
            }

            void InternetProtocolHandler::ConnectInternetProtocolPayloadHandler(InternetProtocolPayloadHandler* handler)
            {
                handlers.insert(handler->handledInternetProtocolProtocolNumber, handler);
            }

            InternetProtocolAddress InternetProtocolHandler::Parse(common::string address)
            {
                uint8_t digits[4];

                uint8_t currentDigit = 0;
                for(int i = 0; i < 4; i++)
                    digits[i] = 0;
                for(char* i = (char*)address; *i != '\0'; ++i)
                {
                    if('0' <= *i && *i <= '9')
                    {
                        digits[currentDigit] = digits[currentDigit] * 10 + *i - '0';
                    }
                    else
                    {
                        if(currentDigit >= 3)
                            break;
                        currentDigit++;
                    }

                }

                return CreateInternetProtocolAddress(digits[0], digits[1], digits[2], digits[3]);
            }

            InternetProtocolAddress InternetProtocolHandler::CreateInternetProtocolAddress(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
            {
                InternetProtocolAddress result = digit4;
                result = (result << 8) | digit3;
                result = (result << 8) | digit2;
                result = (result << 8) | digit1;
                return result;
            }

            SubnetMask InternetProtocolHandler::CreateSubnetMask(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
            {
                return (SubnetMask)CreateInternetProtocolAddress(digit1, digit2, digit3, digit4);
            }


            void InternetProtocolHandler::RegisterInternetProtocolAddressResolver(InternetProtocolAddressResolver* resolver)
            {
                this->resolver = resolver;
            }


            void InternetProtocolHandler::Send(InternetProtocolAddress destination, uint8_t protocol, uint8_t* data, uint32_t length)
            {
                uint8_t* buffer = memoryManager->AllocateMemory(sizeof(InternetProtocolHeader) + length);
                InternetProtocolHeader* header = (InternetProtocolHeader*)(buffer);

                header->version = 4;
                header->headerLength = sizeof(InternetProtocolHeader)/4;
                header->typeOfService = 0;
                header->totalLength = length+sizeof(InternetProtocolHeader);
                header->totalLength = ((header->totalLength&0x00FF)<<8) | ((header->totalLength&0xFF00)>>8); // big endian
                header->identification = 0x0100; // FIXME meh
                header->flagsAndOffset = 0x0040; // FIXME doesn't work for packets > 1500 bytes
                header->timeToLive = 0x40;
                header->protocol = protocol;

                header->destinationAddress = destination;
                header->sourceAddress = GetInternetProtocolAddress();

                header->headerChecksum = 0;
                header->headerChecksum = Checksum((uint16_t*)buffer, sizeof(InternetProtocolHeader));

                for(uint8_t *src = data+length-1, *dst = buffer+sizeof(InternetProtocolHeader)+length-1; src >= data; --src, --dst)
                    *dst = *src;

                // send directly to the destination (if the destination is in the same subnet)
                InternetProtocolAddress route = destination;
                // or send to default gateway (if the destination is outside our subnet)
                if((destination & subnetMask) != (ownInternetProtocolAddress & subnetMask))
                    route = defaultGatewayInternetProtocolAddress;

                backend->Send(resolver->Resolve(route), this->handledEthernetFrameType, buffer, length+sizeof(InternetProtocolHeader));
            }


#define big_endian_word(x)  ((((x)&0xFF00)>>8) | (((x)&0x00FF)<<8))

            uint16_t InternetProtocolHandler::Checksum(uint16_t* buffer, uint32_t sizeInBytes)
            {
                uint32_t checksum = 0;
                uint16_t* data_word = (uint16_t*) buffer;

                if (sizeInBytes % 2)
                    checksum += (uint16_t) (((char*) buffer)[sizeInBytes - 1] << 8);

                for (uint32_t i = 0; i < (sizeInBytes / 2); i++)
                    checksum += big_endian_word(data_word[i]);

                while (checksum & 0xFFFF0000)
                    checksum = (checksum & 0xFFFF) + (checksum >> 16);

                return big_endian_word(~checksum);
            }


        }
    }
}
