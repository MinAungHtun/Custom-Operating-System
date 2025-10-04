
#include <protocols/ethernet/userdatagramprotocol.h>

namespace MYYOS
{
    namespace protocols
    {
        namespace ethernet
        {

            UserDatagramProtocolPayloadHandler::UserDatagramProtocolPayloadHandler()
            {
            }

            UserDatagramProtocolPayloadHandler::~UserDatagramProtocolPayloadHandler()
            {
            }

            void UserDatagramProtocolPayloadHandler::HandleUserDatagramProtocolPayload(UserDatagramProtocolSocket*, uint8_t*, uint32_t)
            {
            }





            UserDatagramProtocolSocket::UserDatagramProtocolSocket()
            {
            }

            UserDatagramProtocolSocket::~UserDatagramProtocolSocket()
            {
            }

            void UserDatagramProtocolSocket::HandleUserDatagramProtocolPayload(uint8_t* data, uint32_t length)
            {
                for(common::Vector<UserDatagramProtocolPayloadHandler*>::iterator handler = handlers.begin(); handler != handlers.end(); handler++)
                    (*handler)->HandleUserDatagramProtocolPayload(this, data, length);
            }

            void UserDatagramProtocolSocket::Disconnect()
            {
                backend->Disconnect(this);
            }

            void UserDatagramProtocolSocket::Send(common::string data)
            {
                uint32_t length = 0;
                for(const char* i = data; *i != '\0'; i++)
                    length++;
                backend->Send(this, (uint8_t*)data, length);
            }

            void UserDatagramProtocolSocket::Send(uint8_t* data, uint32_t length)
            {
                backend->Send(this, data, length);
            }

            void UserDatagramProtocolSocket::ConnectUserDatagramProtocolPayloadHandler(UserDatagramProtocolPayloadHandler* handler)
            {
                handlers.push_back(handler);
            }




            UserDatagramProtocolPort UserDatagramProtocolHandler::freePorts = 0x8000;

            UserDatagramProtocolHandler::UserDatagramProtocolHandler(InternetProtocolHandler* backend, common::OutputStream* errorMessages, MemoryManager* memoryManager)
                : InternetProtocolPayloadHandler(backend, 0x11)
            {
                this->memoryManager = memoryManager;
                this->errorMessages = errorMessages;
            }

            UserDatagramProtocolHandler::~UserDatagramProtocolHandler()
            {
            }


            
#define big_endian_word(x)  ((((x)&0xFF00)>>8) | (((x)&0x00FF)<<8))

            bool UserDatagramProtocolHandler::HandleInternetProtocolPayload(InternetProtocolAddress remoteAddress, InternetProtocolAddress localAddress,
                                                                            uint8_t* data, uint32_t length)
            {
                if(length > sizeof(UserDatagramProtocolHeader))
                {
                    UserDatagramProtocolHeader* header = (UserDatagramProtocolHeader*)data;
                    UserDatagramProtocolPort localPort  = big_endian_word(header->destinationPortNumber);
                    UserDatagramProtocolPort remotePort = big_endian_word(header->sourcePortNumber);

                    UserDatagramProtocolSocket* socket = 0;
                    for(common::Vector<UserDatagramProtocolSocket*>::iterator i = sockets.begin(); i != sockets.end(); i++)
                        if((*i)->remoteInternetProtocolAddress == remoteAddress && (*i)->remotePort == remotePort
                        && (*i)->localInternetProtocolAddress == localAddress && (*i)->localPort == localPort)
                        {
                            socket = *i;
                            break;
                        }

                    if(socket != 0)
                        socket->HandleUserDatagramProtocolPayload(data + sizeof(UserDatagramProtocolHeader), length - sizeof(UserDatagramProtocolHeader));
                }
                return false;
            }

            UserDatagramProtocolSocket* UserDatagramProtocolHandler::Connect(InternetProtocolAddress remoteAddress, UserDatagramProtocolPort port)
            {
                UserDatagramProtocolSocket* result = memoryManager->Instantiate<UserDatagramProtocolSocket>();
                result->backend = this;
                result->remoteInternetProtocolAddress = remoteAddress;
                result->remotePort = port;
                result->localInternetProtocolAddress = backend->GetInternetProtocolAddress();
                result->localPort = freePorts++;

                sockets.push_back(result);
                return result;
            }

            UserDatagramProtocolSocket* UserDatagramProtocolHandler::Connect(common::string remoteAddressAndPort)
            {
                char* colonPosition = (char*)remoteAddressAndPort;
                for(; *colonPosition != '\0'; colonPosition++)
                    if(*colonPosition == ':')
                        break;

                if(*colonPosition != ':')
                    return 0;

                InternetProtocolAddress remoteAddress = InternetProtocolHandler::Parse(remoteAddressAndPort);

                UserDatagramProtocolPort port = 0;
                for(colonPosition++; *colonPosition != '\0'; colonPosition++)
                    if('0' <= *colonPosition && *colonPosition <= '9')
                        port = port*10 + (*colonPosition-'0');

                return Connect(remoteAddress, port);
            }

            void UserDatagramProtocolHandler::Disconnect(UserDatagramProtocolSocket* socket)
            {
                sockets.erase(socket);
            }

            void UserDatagramProtocolHandler::Send(UserDatagramProtocolSocket* socket, uint8_t* data, uint32_t payloadLength)
            {
                uint32_t totalLength = payloadLength + sizeof(UserDatagramProtocolHeader);
                uint8_t* buffer = memoryManager->AllocateMemory(totalLength);
                UserDatagramProtocolHeader* header = (UserDatagramProtocolHeader*)buffer;
                header->sourcePortNumber      = big_endian_word( socket->localPort );
                header->destinationPortNumber = big_endian_word( socket->remotePort );
                header->totalLength           = big_endian_word(totalLength);

                if(data != 0)
                    for(uint8_t *src = data+payloadLength-1, *dst = buffer+totalLength-1; src >= data; --src, --dst)
                        *dst = *src;

                header->checksum = 0;
                //header->checksum = Checksum(header, &pseudoHeader,
                //                ((uint8_t*)header) + 4*(uint32_t)header->dataOffset, payloadLength);

                InternetProtocolPayloadHandler::Send(socket->remoteInternetProtocolAddress, buffer, totalLength);
            }

            uint16_t UserDatagramProtocolHandler::Checksum(UserDatagramProtocolHeader* header, UserDatagramProtocolPseudoHeader* pseudoHeader,
                                        uint8_t* data, uint32_t length)
            {
                return 0;
                /*
                uint8_t* buffer = memoryManager->AllocateMemory(sizeof(TransmissionControlProtocolHeader) + sizeof(TransmissionControlProtocolPseudoHeader) + payloadLength);

                uint8_t* headerOffset = &buffer[0] + sizeof(TransmissionControlProtocolPseudoHeader);
                for(uint8_t* src = (uint8_t*)pseudoHeader+sizeof(TransmissionControlProtocolPseudoHeader)-1, *dst = headerOffset-1; src >= (uint8_t*)pseudoHeader; --src, --dst)
                    *dst = *src;

                uint8_t* dataOffset = headerOffset + 4*(uint32_t)header->dataOffset;
                for(uint8_t* src = (uint8_t*)header+4*(uint32_t)header->dataOffset-1, *dst = dataOffset-1; src >= (uint8_t*)header; --src, --dst)
                    *dst = *src;

                if(data != 0)
                    for(uint8_t* src = data+payloadLength-1, *dst = dataOffset+payloadLength-1; src >= (uint8_t*)data; --src, --dst)
                        *dst = *src;

                uint32_t size = sizeof(TransmissionControlProtocolPseudoHeader) + 4*(uint32_t)header->dataOffset + payloadLength;

                return InternetProtocolHandler::Checksum((uint16_t*)&buffer[0], size);
                */
            }

        }
    }
}

