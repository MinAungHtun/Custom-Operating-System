
#include <protocols/ethernet/internetcontrolmessageprotocol.h>

namespace MYYOS
{
    namespace protocols
    {
        namespace ethernet
        {


            InternetControlMessageProtocolHandler::InternetControlMessageProtocolHandler(InternetProtocolHandler* backend)
             : InternetProtocolPayloadHandler(backend, 0x01)
            {
            }

            InternetControlMessageProtocolHandler::~InternetControlMessageProtocolHandler()
            {
            }

            bool InternetControlMessageProtocolHandler::HandleInternetProtocolPayload(InternetProtocolAddress, InternetProtocolAddress, uint8_t* data, uint32_t length)
            {
                if(length < sizeof(InternetControlMessageProtocolHeader))
                    return false;

                InternetControlMessageProtocolHeader* header = (InternetControlMessageProtocolHeader*)data;
                switch(header->type)
                {
                    case 0:
                        // ping reply received... do something
                        break;
                    case 8:
                        header->type = 0;
                        header->checksum = Checksum((uint16_t*)data, sizeof(InternetControlMessageProtocolHeader));
                        return true;
                }
                return false;
            }

            void InternetControlMessageProtocolHandler::RequestEchoReply(InternetProtocolAddress internetProtocolAddress)
            {
                InternetControlMessageProtocolHeader icmp;
                icmp.type = 8; // ping
                icmp.code = 0;
                icmp.data = 0x3713; // 0x1337 in big endian :-)
                icmp.checksum = 0;
                icmp.checksum = InternetControlMessageProtocolHandler::Checksum((uint16_t*)&icmp, sizeof(InternetControlMessageProtocolHeader));
                InternetProtocolPayloadHandler::Send(internetProtocolAddress, (uint8_t*)&icmp, sizeof(InternetControlMessageProtocolHeader));
            }

            uint16_t InternetControlMessageProtocolHandler::Checksum(uint16_t* buffer, uint32_t length)
            {
                return InternetProtocolHandler::Checksum(buffer, length);
            }

        }
    }
}

