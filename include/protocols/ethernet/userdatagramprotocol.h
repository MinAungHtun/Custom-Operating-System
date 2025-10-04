
#ifndef __PROTOCOLS__ETHERNET__USERDATAGRAMPROTOCOL_H
    #define __PROTOCOLS__ETHERNET__USERDATAGRAMPROTOCOL_H

    #include<common/types.h>
    #include<common/string.h>
    #include<common/map.h>
    #include<common/outputstream.h>
    #include<protocols/ethernet/internetprotocol.h>
    #include<memorymanagement.h>

    namespace MYYOS
    {
        namespace protocols
        {
            namespace ethernet
            {

                typedef uint16_t UserDatagramProtocolPort;

                struct UserDatagramProtocolHeader
                {
                    UserDatagramProtocolPort sourcePortNumber;
                    UserDatagramProtocolPort destinationPortNumber;
                    uint16_t totalLength;
                    uint16_t checksum;
                } __attribute__((packed));


                struct UserDatagramProtocolPseudoHeader
                {
                    InternetProtocolAddress sourceInternetProtocolAddress;
                    InternetProtocolAddress destinationInternetProtocolAddress;
                    uint16_t protocolNumber;
                    uint16_t totalLength;
                } __attribute__((packed));


                class UserDatagramProtocolSocket;
                class UserDatagramProtocolHandler;

                class UserDatagramProtocolPayloadHandler
                {
                    public:
                        UserDatagramProtocolPayloadHandler();
                        ~UserDatagramProtocolPayloadHandler();

                        virtual void HandleUserDatagramProtocolPayload(UserDatagramProtocolSocket* socket, uint8_t* data, uint32_t length);
                };


                class UserDatagramProtocolSocket
                {
                    friend class UserDatagramProtocolHandler;
                    protected:
                        InternetProtocolAddress remoteInternetProtocolAddress;
                        UserDatagramProtocolPort remotePort;
                        InternetProtocolAddress localInternetProtocolAddress;
                        UserDatagramProtocolPort localPort;

                        UserDatagramProtocolHandler* backend;
                        common::Vector<UserDatagramProtocolPayloadHandler*> handlers;

                    public:
                        UserDatagramProtocolSocket();
                        ~UserDatagramProtocolSocket();

                        virtual void HandleUserDatagramProtocolPayload(uint8_t* data, uint32_t length);
                        void Send(uint8_t* data, uint32_t length);
                        void Send(common::string data);
                        void Disconnect();
                        void ConnectUserDatagramProtocolPayloadHandler(UserDatagramProtocolPayloadHandler* handler);

                        inline void* operator new(uint32_t, UserDatagramProtocolSocket* p) { return p; }
                };


                class UserDatagramProtocolHandler : public InternetProtocolPayloadHandler
                {
                    protected:
                        common::OutputStream* errorMessages;
                        MemoryManager* memoryManager;
                        common::Vector<UserDatagramProtocolSocket*> sockets;

                        static UserDatagramProtocolPort freePorts;

                    public:
                        UserDatagramProtocolHandler(InternetProtocolHandler* backend, common::OutputStream* errorMessages, MemoryManager* memoryManager);
                        ~UserDatagramProtocolHandler();

                        bool HandleInternetProtocolPayload(InternetProtocolAddress remoteAddress, InternetProtocolAddress localAddress,
                                                           uint8_t* data, uint32_t length);

                        UserDatagramProtocolSocket* Connect(InternetProtocolAddress remoteAddress, UserDatagramProtocolPort port);
                        UserDatagramProtocolSocket* Connect(common::string remoteAddressAndPort);
                        void Disconnect(UserDatagramProtocolSocket* socket);
                        void Send(UserDatagramProtocolSocket* socket, uint8_t* data, uint32_t length);

                        uint16_t Checksum(UserDatagramProtocolHeader* header, UserDatagramProtocolPseudoHeader* pseudoHeader,
                                          uint8_t* data, uint32_t length);
                };


            }
        }
    }

#endif
