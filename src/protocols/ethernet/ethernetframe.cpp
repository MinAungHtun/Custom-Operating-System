
#include<protocols/ethernet/ethernetframe.h>

using namespace MYYOS::drivers::ethernet;

namespace MYYOS
{
    namespace protocols
    {
        namespace ethernet
        {



            EthernetFramePayloadHandler::EthernetFramePayloadHandler(EthernetFrameHandler* backend, uint16_t handledEthernetFrameType)
            {
                this->handledEthernetFrameType = handledEthernetFrameType;
                this->backend = backend;
                backend->ConnectEthernetFramePayloadHandler(this);
            }

            EthernetFramePayloadHandler::~EthernetFramePayloadHandler()
            {
            }

            bool EthernetFramePayloadHandler::HandleEthernetFramePayload(uint8_t*, uint32_t)
            {
                return false;
            }

            void EthernetFramePayloadHandler::Send(MediaAccessControlAddress destination, uint8_t* data, uint32_t length)
            {
                backend->Send(destination, handledEthernetFrameType, data, length);
            }





            EthernetFrameHandler::EthernetFrameHandler(MYYOS::drivers::ethernet::EthernetDriver* backend, common::OutputStream* errorMessages, MemoryManager* memoryManager)
              : EthernetDriverEventHandler()
            {
                this->backend = backend;
                this->errorMessages = errorMessages;
                this->memoryManager = memoryManager;
                backend->ConnectEventHandler(this);
            }

            EthernetFrameHandler::~EthernetFrameHandler()
            {
            }


            bool EthernetFrameHandler::DataReceived(uint8_t* data, uint32_t length)
            {
                // the footer is cut off by the driver
                if(length < sizeof(EthernetFrameHeader))
                    return false; // message too short, cannot handle

                EthernetFrameHeader* header = (EthernetFrameHeader*)data;
                bool sendBack = false;

                if(header->destination == 0xFFFFFFFFFFFF                      // broadcast
                    || header->destination == GetMediaAccessControlAddress()) // directed to me
                {
                    common::Map<uint16_t, EthernetFramePayloadHandler*>::iterator handlerIterator = handlers.find(header->type);
                    if(handlerIterator != handlers.end())
                        sendBack = handlerIterator->second->HandleEthernetFramePayload(data+sizeof(EthernetFrameHeader), length-sizeof(EthernetFrameHeader));
                }

                if(sendBack)
                {
                    header->destination = header->source;
                    header->source = GetMediaAccessControlAddress();
                }
                return sendBack;
            }

            void EthernetFrameHandler::ConnectEthernetFramePayloadHandler(EthernetFramePayloadHandler* payloadhandler)
            {
                uint16_t frameTypeBigEndian = ((payloadhandler->handledEthernetFrameType & 0x00ff) << 8 )
                                            | ((payloadhandler->handledEthernetFrameType & 0xff00) >> 8 );
                handlers.insert(frameTypeBigEndian, payloadhandler);
            }

            MYYOS::drivers::ethernet::MediaAccessControlAddress EthernetFrameHandler::GetMediaAccessControlAddress()
            {
                return backend->GetMediaAccessControlAddress();
            }

            void EthernetFrameHandler::Send(MediaAccessControlAddress destination, uint16_t frameType, uint8_t* data, uint32_t length)
            {
                uint8_t* buffer = memoryManager->AllocateMemory(sizeof(EthernetFrameHeader) + length);
                EthernetFrameHeader* header = (EthernetFrameHeader*)(buffer);
                header->destination = destination;
                header->source = GetMediaAccessControlAddress();
                header->type = ((frameType & 0x00ff) << 8 ) | ((frameType & 0xff00) >> 8 );
                for(uint8_t *src = data+length-1, *dst = buffer+sizeof(EthernetFrameHeader)+length-1; src >= data; --src, --dst)
                    *dst = *src;
                backend->Send(buffer, length+sizeof(EthernetFrameHeader));
            }

        }
    }
}