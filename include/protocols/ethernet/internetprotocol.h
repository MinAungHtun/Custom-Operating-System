
#ifndef __PROTOCOLS__ETHERNET__INTERNETPROTOCOL_H
    #define __PROTOCOLS__ETHERNET__INTERNETPROTOCOL_H

    #include<common/types.h>
    #include<common/outputstream.h>
    #include<protocols/ethernet/ethernetframe.h>
    #include<memorymanagement.h>

    namespace MYYOS
    {
        namespace protocols
        {
            namespace ethernet
            {

                typedef uint32_t InternetProtocolAddress;
                typedef uint32_t SubnetMask;

                struct InternetProtocolHeader
                {
                    // Felder wegen Big Endian Kodierung vertauscht
                    unsigned headerLength : 4;   // (Length of header in bytes)/4
                    unsigned version : 4;        // Version of IP
                    uint8_t  typeOfService;      // Type of service
                    uint16_t totalLength;        // (Length of packet in bytes)/4
                    uint16_t identification;     // unique identifier
                    uint16_t flagsAndOffset;     // Flags
                    uint8_t  timeToLive;         // Time to live
                    uint8_t  protocol;           // Protocol number (ICMP=1, TCP=6, UDP=17 etc)
                                                 // see http://iana.org/assignments/protocol-numbers
                    uint16_t headerChecksum;     // IP checksum
                    uint32_t sourceAddress;      //
                    uint32_t destinationAddress; //
                } __attribute__((packed));



                class InternetProtocolHandler;
                class InternetProtocolAddressResolver
                {
                    public:
                        InternetProtocolAddressResolver(InternetProtocolHandler* handler);
                        ~InternetProtocolAddressResolver();
                        virtual MYYOS::drivers::ethernet::MediaAccessControlAddress Resolve(InternetProtocolAddress address);
                        virtual void Store(InternetProtocolAddress internetProtocolAddress, MYYOS::drivers::ethernet::MediaAccessControlAddress mediaAccessControlAddress);
                };


                class InternetProtocolPayloadHandler
                {
                    friend class InternetProtocolHandler;
                    protected:
                        uint8_t handledInternetProtocolProtocolNumber;
                        InternetProtocolHandler* backend;

                    public:
                        InternetProtocolPayloadHandler(InternetProtocolHandler* backend, uint8_t handledInternetProtocolProtocolNumber);
                        ~InternetProtocolPayloadHandler();

                        virtual bool HandleInternetProtocolPayload(InternetProtocolAddress remoteInternetProtocolAddress,
                                                                   InternetProtocolAddress localInternetProtocolAddress, uint8_t* data, uint32_t length);
                        void Send(InternetProtocolAddress destination, uint8_t* data, uint32_t length);
                };


                class InternetProtocolHandler : public EthernetFramePayloadHandler
                {
                    friend class InternetProtocolAddressResolver;
                    protected:
                        common::Map<uint8_t, InternetProtocolPayloadHandler*> handlers;
                        common::OutputStream* errorMessages;
                        InternetProtocolAddressResolver* resolver;
                        MemoryManager* memoryManager;

                        InternetProtocolAddress ownInternetProtocolAddress;
                        InternetProtocolAddress defaultGatewayInternetProtocolAddress;
                        SubnetMask subnetMask;

                        void RegisterInternetProtocolAddressResolver(InternetProtocolAddressResolver* cache);

                    public:
                        InternetProtocolHandler(EthernetFrameHandler* backend, InternetProtocolAddress ownInternetProtocolAddress,
                            InternetProtocolAddress defaultGatewayInternetProtocolAddress, SubnetMask subnetMask,
                            common::OutputStream* errorMessages, MemoryManager* memoryManager);
                        ~InternetProtocolHandler();

                        //                       e.g. CreateInternetProtocolAddress(192,168,0,1) for the IP 192.168.0.1
                        static InternetProtocolAddress CreateInternetProtocolAddress(uint8_t, uint8_t, uint8_t, uint8_t);
                        static InternetProtocolAddress Parse(common::string address);
                        static SubnetMask CreateSubnetMask(uint8_t, uint8_t, uint8_t, uint8_t);
                        InternetProtocolAddress GetInternetProtocolAddress();
                        MYYOS::drivers::ethernet::MediaAccessControlAddress GetMediaAccessControlAddress();

                        bool HandleEthernetFramePayload(uint8_t* data, uint32_t length);
                        void ConnectInternetProtocolPayloadHandler(InternetProtocolPayloadHandler* handler);
                        void Send(InternetProtocolAddress destination, uint8_t protocol, uint8_t* data, uint32_t length);

                        static uint16_t Checksum(uint16_t* data, uint32_t length);
                };

            }
        }
    }

#endif
