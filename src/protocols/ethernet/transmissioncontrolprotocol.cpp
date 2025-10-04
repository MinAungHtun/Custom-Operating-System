
#include <protocols/ethernet/transmissioncontrolprotocol.h>

namespace MYYOS
{
    namespace protocols
    {
        namespace ethernet
        {






            TransmissionControlProtocolPayloadHandler::TransmissionControlProtocolPayloadHandler()
            {
            }

            TransmissionControlProtocolPayloadHandler::~TransmissionControlProtocolPayloadHandler()
            {
            }

            void TransmissionControlProtocolPayloadHandler::HandleTransmissionControlProtocolPayload(TransmissionControlProtocolConnection*, uint8_t*, uint32_t)
            {
            }

            void TransmissionControlProtocolPayloadHandler::Connected(TransmissionControlProtocolConnection*)
            {
            }

            void TransmissionControlProtocolPayloadHandler::Disconnected(TransmissionControlProtocolConnection*)
            {
            }




            TransmissionControlProtocolConnection::TransmissionControlProtocolConnection()
            :   handlers()
            {
            }

            TransmissionControlProtocolConnection::~TransmissionControlProtocolConnection()
            {
            }

            void TransmissionControlProtocolConnection::Disconnect()
            {
                backend->Disconnect(this);
            }

            void TransmissionControlProtocolConnection::Send(uint8_t* buffer, uint32_t length)
            {
                const uint8_t ACK = 0x10;
                const uint8_t PSH = 0x08;
                backend->Send(this, buffer, length, ACK+PSH);
            }

            void TransmissionControlProtocolConnection::Send(common::string message)
            {
                uint32_t length = 0;
                for(; message[length] != '\0'; length++);
                const uint8_t ACK = 0x10;
                const uint8_t PSH = 0x08;
                backend->Send(this, (uint8_t*)message, length, ACK+PSH);
            }

            void TransmissionControlProtocolConnection::ConnectTransmissionControlProtocolPayloadHandler(TransmissionControlProtocolPayloadHandler* handler)
            {
                handlers.push_back(handler);
            }

            bool TransmissionControlProtocolConnection::HandleTransmissionControlProtocolPayload(uint8_t* data, uint32_t length)
            {
                for(common::Vector<TransmissionControlProtocolPayloadHandler*>::iterator i = handlers.begin(); i != handlers.end(); i++)
                    (*i)->HandleTransmissionControlProtocolPayload(this, data, length);

                if(length > 5
                    && data[0] == 'H'
                    && data[1] == 'A'
                    && data[2] == 'L'
                    && data[3] == 'L'
                    && data[4] == 'O'
                    && data[5] == 10)
                    Send("welt\n");

                if(length > 4
                    && data[0] == 'q'
                    && data[1] == 'u'
                    && data[2] == 'i'
                    && data[3] == 't'
                    && data[4] == 10)
                    Disconnect();

                if(length > 9
                    && data[0] == 'G'
                    && data[1] == 'E'
                    && data[2] == 'T'
                    && data[3] == ' '
                    && data[4] == '/'
                    && data[5] == ' '
                    && data[6] == 'H'
                    && data[7] == 'T'
                    && data[8] == 'T'
                    && data[9] == 'P')
                {
                    Send("HTTP/1.1 200 OK\r\nServer: MYYOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>Hallo Welt</title></head><body><b>Hallo</b> <i>Welt</i>!</body></html>\r\n");
                    Disconnect();
                }

                return true;
            }

            bool TransmissionControlProtocolConnection::Connected()
            {
                for(common::Vector<TransmissionControlProtocolPayloadHandler*>::iterator i = handlers.begin(); i != handlers.end(); i++)
                    (*i)->Connected(this);
                return true;
            }

            bool TransmissionControlProtocolConnection::IsConnected()
            {
                return state == TransmissionControlProtocolConnectionState::Connected;
            }

            bool TransmissionControlProtocolConnection::Disconnected()
            {
                for(common::Vector<TransmissionControlProtocolPayloadHandler*>::iterator i = handlers.begin(); i != handlers.end(); i++)
                    (*i)->Disconnected(this);
                return true;
            }





            TransmissionControlProtocolPortListener::TransmissionControlProtocolPortListener(TransmissionControlProtocolHandler* backend,
                                                                                             TransmissionControlProtocolPort port,
                                                                                             common::OutputStream* stream,
                                                                                             MemoryManager* memoryManager)
            {
                this->stream = stream;
                this->port = port;
                this->memoryManager = memoryManager;
                this->backend = backend;
                backend->ConnectTransmissionControlProtocolPortListener(this);
            }

            TransmissionControlProtocolPortListener::~TransmissionControlProtocolPortListener()
            {
            }

            TransmissionControlProtocolConnection* TransmissionControlProtocolPortListener::Connect(InternetProtocolAddress remoteAddress,
                                                                                                    TransmissionControlProtocolPort remotePort,
                                                                                                    InternetProtocolAddress localAddress,
                                                                                                    TransmissionControlProtocolPort localPort)
            {
                TransmissionControlProtocolConnection* result = memoryManager->Instantiate<TransmissionControlProtocolConnection>();
                result->backend = this->backend;
                result->remoteAddress = remoteAddress;
                result->remotePort = remotePort;
                result->localAddress = localAddress;
                result->localPort = localPort;

                return result;
            }





            TransmissionControlProtocolPort TransmissionControlProtocolHandler::freePorts = 0x8000;

            TransmissionControlProtocolHandler::TransmissionControlProtocolHandler(InternetProtocolHandler* backend, common::OutputStream* errorMessages, MemoryManager* memoryManager)
             : InternetProtocolPayloadHandler(backend, 0x06)
            {
                this->memoryManager = memoryManager;
                this->errorMessages = errorMessages;
            }

            TransmissionControlProtocolHandler::~TransmissionControlProtocolHandler()
            {
            }

            void TransmissionControlProtocolHandler::ConnectTransmissionControlProtocolPortListener(TransmissionControlProtocolPortListener* listener)
            {
                listeners.insert(listener->port, listener);
            }



#define big_endian_dword(x)  ((((x)&0x000000FF)<<24) | (((x)&0x0000FF00)<<8) | (((x)&0x00FF0000)>>8) | (((x)&0xFF000000)>>24))
#define big_endian_word(x)  ((((x)&0xFF00)>>8) | (((x)&0x00FF)<<8))

            bool TransmissionControlProtocolHandler::HandleInternetProtocolPayload(InternetProtocolAddress remoteAddress,
                                                                                   InternetProtocolAddress localAddress, uint8_t* data, uint32_t totalLength)
            {
                if(totalLength < 13)
                    return false;
                if(totalLength < 4*(data[12]/16)) // the lower 4 bits of the 13th byte are the header size. this many bytes must be there.
                    return false;

                TransmissionControlProtocolHeader* header = (TransmissionControlProtocolHeader*)data;
                uint32_t payloadLength = totalLength - 4*(uint32_t)header->dataOffset;
                uint8_t* payload = data + 4*(uint32_t)header->dataOffset;
                uint16_t localPort = big_endian_word(header->destinationPortNumber);
                uint16_t remotePort = big_endian_word(header->sourcePortNumber);

                const uint8_t FIN = 0x01;
                const uint8_t SYN = 0x02;
                const uint8_t RST = 0x04;
                const uint8_t ACK = 0x10;



                TransmissionControlProtocolConnection* connection = 0;
                for(common::Vector<TransmissionControlProtocolConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
                    if((*i)->remoteAddress == remoteAddress && (*i)->remotePort == remotePort
                    && (*i)->localAddress == localAddress && (*i)->localPort == localPort)
                    {
                        connection = *i;
                        break;
                    }


                TransmissionControlProtocolPortListener* listener = 0;
                if(connection == 0)
                {
                    common::Map<TransmissionControlProtocolPort, TransmissionControlProtocolPortListener*>::iterator listenerIterator = listeners.find(localPort);
                    if(listenerIterator != listeners.end())
                        listener = listenerIterator->second;
                }


                bool rejectPacket = false;


                if(connection != 0 && (header->flags & RST) != 0)
                {
                    connection->Disconnected();
                    connections.erase(connection);
                    return false;
                }

                switch( header->flags & (SYN | ACK | FIN) )
                {
                    case SYN:
                    {
                        if(connection != 0)
                        {
                            // unless someone spoofs his IP, this should only happen
                            // in case of "simultaneous SYN". Ignored for now
                            rejectPacket = true;
                        }
                        else if(listener != 0)
                        {
                            connection = listener->Connect(remoteAddress, remotePort, localAddress, localPort);
                            if(connection != 0)
                            {
                                connection->state = ConnectAcknowledged;
                                connection->acknowledgementNumber = big_endian_dword(header->sequenceNumber)+1;
                                connection->sequenceNumber = 0xbeefcafe; // FIXME should be random for security reasons
                                connections.push_back(connection);
                                Send(connection, 0,0, SYN|ACK);
                                connection->sequenceNumber++;

                                return false;
                            }
                            else
                                rejectPacket = true;
                        }
                        else
                            rejectPacket = true;
                    }
                    break;

                    case FIN | ACK:
                    case FIN:
                    {
                        if(connection != 0)
                        {
                            if(connection->state == Connected)
                            {
                                // remote host disconnect
                                connection->state = DisconnectAcknowledged;
                                connection->acknowledgementNumber++;
                                Send(connection, 0,0, ACK);
                                connection->Disconnected();
                                Send(connection, 0,0, FIN|ACK);
                                return false;
                            }
                            else if(connection->state == DisconnectAcknowledged)
                            {
                                // remote host disconnect
                                // this is the final ACK (should be only an ACK, but we also accept this)
                                connection->state = Disconnected;
                                connections.erase(connection);
                                return false;
                            }
                            else if(connection->state == DisconnectRequested || connection->state == DisconnectRequestedStage2)
                            {
                                // local host disconnect, we already have the ack to that (or it's also
                                // in this packet, now we also have the final FIN)
                                connection->state = Disconnected;
                                connection->acknowledgementNumber++;
                                Send(connection, 0,0, ACK);
                                connection->Disconnected();
                                connections.erase(connection);
                                return false;
                            }
                            else
                            {
                                // FIXME
                            }
                        }
                        else
                            rejectPacket = true; // not supposed to happen.
                    }
                    break;

                    case SYN | ACK:
                    {
                        if(connection->state == ConnectRequested)
                        {
                            // connect by local host, send final ACK of the handshake
                            connection->state = Connected;
                            connection->acknowledgementNumber = big_endian_dword(header->sequenceNumber);
                            connection->sequenceNumber++;
                            connection->acknowledgementNumber++;
                            Send(connection, 0,0, ACK);
                        }
                        else
                            rejectPacket = true; // not supposed to happen.
                    }
                    break;

                    case SYN | FIN:                                          // illegal combinations
                    case SYN | FIN | ACK:
                        rejectPacket = true;
                        break;

                    case ACK:
                    {
                        if(connection->state == ConnectAcknowledged)
                        {
                            // connect by remote host, handshake finished
                            connection->state = Connected;
                            connection->Connected();
                            return false;
                        }
                        else if(connection->state == DisconnectRequested)
                        {
                            // disconnect by local host - wait for final FIN
                            connection->state = DisconnectRequestedStage2;
                            return false;
                        }
                        else if(connection->state == DisconnectAcknowledged)
                        {
                            // final ACK to disconnect by remote host
                            connection->state = Disconnected;
                            connections.erase(connection);
                            return false;
                        }
                        if(header->flags == ACK)
                            break;
                    }
                    // no break because the Acknowledge-Bit might also be set in regular packets (piggybacking),
                    // so we want these to be given to the handler also (unless its clearly a reply to a SYN-ACK
                    // or a FIN, in which case the above breaks apply)

                    default:
                    {
                        if(big_endian_dword(header->sequenceNumber) == connection->acknowledgementNumber)
                        {
                            rejectPacket = !connection->HandleTransmissionControlProtocolPayload(payload, payloadLength);
                            if(!rejectPacket)
                            {
                                connection->acknowledgementNumber += payloadLength;
                                Send(connection, 0,0, ACK);
                                return false;
                            }
                        }
                        else
                        {
                            rejectPacket = true; // ... data arrived in wrong order. cannot handle yet
                        }
                    }
                    break;
                }


                if(rejectPacket)
                {
                    header->flags = RST | ACK; // Reset Flag

                    uint32_t temp32 = header->acknowledgementNumber;
                    header->acknowledgementNumber = big_endian_dword( header->sequenceNumber )+1; // fucking endianness...
                    header->acknowledgementNumber = big_endian_dword( header->acknowledgementNumber );
                    header->sequenceNumber = temp32;

                    uint16_t temp = header->sourcePortNumber;
                    header->sourcePortNumber = header->destinationPortNumber;
                    header->destinationPortNumber = temp;
                    //header->receiveWindow = 0;

                    TransmissionControlProtocolPseudoHeader pseudoHeader;
                    pseudoHeader.sourceInternetProtocolAddress = localAddress;
                    pseudoHeader.destinationInternetProtocolAddress = remoteAddress;
                    pseudoHeader.protocolNumber = big_endian_word(6);
                    pseudoHeader.totalLength = big_endian_word( totalLength );

                    if(header->dataOffset > 5)
                        header->options = 0;

                    header->checksum = 0;
                    header->checksum = Checksum(header, &pseudoHeader, payload, payloadLength);

                    return true;
                }
                return false;
            }

            void TransmissionControlProtocolHandler::Send(TransmissionControlProtocolConnection* connection, uint8_t* data, uint32_t payloadLength, uint8_t flags)
            {
                uint8_t* buffer = memoryManager->AllocateMemory(payloadLength + sizeof(TransmissionControlProtocolHeader));
                TransmissionControlProtocolHeader* header = (TransmissionControlProtocolHeader*)buffer;
                header->dataOffset = sizeof(TransmissionControlProtocolHeader)/4;
                uint32_t totalLength = 4*(uint32_t)header->dataOffset + payloadLength;
                header->sourcePortNumber =      big_endian_word( connection->localPort );
                header->destinationPortNumber = big_endian_word( connection->remotePort );
                header->sequenceNumber =        big_endian_dword( connection->sequenceNumber );
                header->acknowledgementNumber = big_endian_dword( connection->acknowledgementNumber );
                header->reserved = 0;
                header->flags = flags;
                header->receiveWindow = 0xffff;
                header->urgentPointer = 0;
                header->options = ((flags & 0x02)!=0) ? 0xb4050402 : 0;

                connection->sequenceNumber += payloadLength;

                if(data != 0)
                    for(uint8_t *src = data+payloadLength-1, *dst = buffer+totalLength-1; src >= data; --src, --dst)
                        *dst = *src;

                TransmissionControlProtocolPseudoHeader pseudoHeader;
                pseudoHeader.sourceInternetProtocolAddress = connection->localAddress;
                pseudoHeader.destinationInternetProtocolAddress = connection->remoteAddress;
                pseudoHeader.protocolNumber = big_endian_word( 6 );
                pseudoHeader.totalLength = big_endian_word( totalLength );

                header->checksum = 0;
                header->checksum = Checksum(header, &pseudoHeader,
                                ((uint8_t*)header) + 4*(uint32_t)header->dataOffset, payloadLength);

                InternetProtocolPayloadHandler::Send(connection->remoteAddress, buffer, totalLength);
            }


            TransmissionControlProtocolConnection* TransmissionControlProtocolHandler::Connect(common::string internetProtocolAddressAndPort)
            {
                char* colonPosition = (char*)internetProtocolAddressAndPort;
                for(; *colonPosition != '\0'; colonPosition++)
                    if(*colonPosition == ':')
                        break;

                if(*colonPosition != ':')
                    return 0;

                InternetProtocolAddress remoteAddress = InternetProtocolHandler::Parse(internetProtocolAddressAndPort);

                TransmissionControlProtocolPort port = 0;
                for(colonPosition++; *colonPosition != '\0'; colonPosition++)
                    if('0' <= *colonPosition && *colonPosition <= '9')
                        port = port*10 + (*colonPosition-'0');

                return Connect(remoteAddress, port);
            }

            TransmissionControlProtocolConnection* TransmissionControlProtocolHandler::Connect(InternetProtocolAddress remoteAddress, TransmissionControlProtocolPort remotePort)
            {
                TransmissionControlProtocolConnection* connection = memoryManager->Instantiate<TransmissionControlProtocolConnection>();
                connection->state = ConnectRequested;
                connection->backend = this;
                connection->remoteAddress = remoteAddress;
                connection->remotePort = remotePort;
                connection->localAddress = backend->GetInternetProtocolAddress();
                connection->localPort = freePorts++;
                connection->sequenceNumber = 0xbeefcafe; // FIXME should be random for security reasons
                connections.push_back(connection);
                const uint8_t SYN = 0x02;
                Send(connection, 0,0, SYN);
                return connection;
            }

            void TransmissionControlProtocolHandler::Disconnect(TransmissionControlProtocolConnection* connection)
            {
                const uint8_t ACK = 0x10;
                const uint8_t FIN = 0x01;
                connection->state = DisconnectRequested;
                Send(connection, 0, 0, FIN+ACK);
                connection->sequenceNumber++;
            }

            uint16_t TransmissionControlProtocolHandler::Checksum(TransmissionControlProtocolHeader* header,
                                                                  TransmissionControlProtocolPseudoHeader* pseudoHeader,
                                                                  uint8_t* data, uint32_t payloadLength)
            {
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
            }

        }
    }
}

