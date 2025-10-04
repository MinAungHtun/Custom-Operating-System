
#ifndef __PROTOCOLS__ETHERNET__TRANSMISSIONCONTROLPROTOCOL_H
    #define __PROTOCOLS__ETHERNET__TRANSMISSIONCONTROLPROTOCOL_H

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

                typedef uint16_t TransmissionControlProtocolPort;

                enum TransmissionControlProtocolConnectionState
                {
                    ConnectRequested,
                    ConnectAcknowledged,
                    Connected,
                    DisconnectRequested,
                    DisconnectRequestedStage2,
                    DisconnectAcknowledged,
                    Disconnected,
                };

                struct TransmissionControlProtocolHeader
                {
                    TransmissionControlProtocolPort sourcePortNumber;
                    TransmissionControlProtocolPort destinationPortNumber;
                    uint32_t sequenceNumber;
                    uint32_t acknowledgementNumber;
                    uint8_t reserved : 4;
                    uint8_t dataOffset : 4;
                    uint8_t flags;
                    uint16_t receiveWindow;
                    uint16_t checksum;
                    uint16_t urgentPointer;
                    uint32_t options;
                } __attribute__((packed));


                struct TransmissionControlProtocolPseudoHeader
                {
                    InternetProtocolAddress sourceInternetProtocolAddress;
                    InternetProtocolAddress destinationInternetProtocolAddress;
                    uint16_t protocolNumber;
                    uint16_t totalLength;
                } __attribute__((packed));






                class TransmissionControlProtocolHandler;
                class TransmissionControlProtocolConnection;
                class TransmissionControlProtocolPayloadHandler
                {
                    public:
                        TransmissionControlProtocolPayloadHandler();
                        ~TransmissionControlProtocolPayloadHandler();

                        virtual void Connected(TransmissionControlProtocolConnection* connection);
                        virtual void Disconnected(TransmissionControlProtocolConnection* connection);
                        virtual void HandleTransmissionControlProtocolPayload(TransmissionControlProtocolConnection* connection, uint8_t* data, uint32_t length);
                };


                class TransmissionControlProtocolConnection
                {
                    friend class TransmissionControlProtocolPortListener;
                    friend class TransmissionControlProtocolHandler;
                    protected:
                        TransmissionControlProtocolHandler* backend;
                        common::Vector<TransmissionControlProtocolPayloadHandler*> handlers;
                        TransmissionControlProtocolConnectionState state;

                    public:
                        InternetProtocolAddress localAddress;
                        InternetProtocolAddress remoteAddress;
                        TransmissionControlProtocolPort localPort;
                        TransmissionControlProtocolPort remotePort;
                        uint32_t sequenceNumber;
                        uint32_t acknowledgementNumber;
                        //uint16_t window;
                        //uint16_t mss; ???

                        TransmissionControlProtocolConnection();
                        ~TransmissionControlProtocolConnection();

                        void Disconnect();
                        void Send(uint8_t* buffer, uint32_t length);
                        void Send(common::string message);

                        bool Disconnected();
                        bool Connected();
                        bool IsConnected();

                        void ConnectTransmissionControlProtocolPayloadHandler(TransmissionControlProtocolPayloadHandler* handler);
                        bool HandleTransmissionControlProtocolPayload(uint8_t* data, uint32_t length);

                        inline void* operator new(uint32_t, TransmissionControlProtocolConnection* p)
                        {
                            return p;
                        }
                };


                class TransmissionControlProtocolPortListener
                {
                    friend class TransmissionControlProtocolHandler;
                    protected:
                        TransmissionControlProtocolPort port;
                        common::OutputStream* stream;
                        TransmissionControlProtocolHandler* backend;
                        MemoryManager* memoryManager;
                    public:
                        TransmissionControlProtocolPortListener(TransmissionControlProtocolHandler* backend, TransmissionControlProtocolPort port, common::OutputStream* stream,
                                                                MemoryManager* memoryManager);
                        ~TransmissionControlProtocolPortListener();

                        virtual TransmissionControlProtocolConnection* Connect(InternetProtocolAddress remoteAddress, TransmissionControlProtocolPort remotePort,
                                                                               InternetProtocolAddress localAddress, TransmissionControlProtocolPort localPort);
                };


                class TransmissionControlProtocolHandler : public InternetProtocolPayloadHandler
                {
                    friend class TransmissionControlProtocolConnection;
                    protected:
                        common::OutputStream* errorMessages;
                        MemoryManager* memoryManager;
                        common::Map<TransmissionControlProtocolPort, TransmissionControlProtocolPortListener*> listeners;
                        common::Vector<TransmissionControlProtocolConnection*> connections;

                        static TransmissionControlProtocolPort freePorts;
                        void Send(TransmissionControlProtocolConnection* connection, uint8_t* data, uint32_t length, uint8_t flags = 0);
                        void SendBuffered(TransmissionControlProtocolConnection* connection, TransmissionControlProtocolHeader* header, uint32_t length, uint8_t flags = 0);

                    public:
                        TransmissionControlProtocolHandler(InternetProtocolHandler* backend, common::OutputStream* errorMessages, MemoryManager* memoryManager);
                        ~TransmissionControlProtocolHandler();

                        bool HandleInternetProtocolPayload(InternetProtocolAddress remoteAddress, InternetProtocolAddress localAddress,
                                                           uint8_t* data, uint32_t length);
                        TransmissionControlProtocolConnection* Connect(InternetProtocolAddress remoteAddress, TransmissionControlProtocolPort port);
                        TransmissionControlProtocolConnection* Connect(common::string remoteAddressAndPort);
                        void Disconnect(TransmissionControlProtocolConnection* connection);

                        void ConnectTransmissionControlProtocolPortListener(TransmissionControlProtocolPortListener* listener);
                        uint16_t Checksum(TransmissionControlProtocolHeader* header,
                                                 TransmissionControlProtocolPseudoHeader* pseudoHeader, uint8_t* data, uint32_t length);
                };

            }
        }
    }

#endif
