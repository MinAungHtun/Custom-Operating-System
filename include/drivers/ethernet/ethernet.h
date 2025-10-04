

#ifndef __DRIVERS__ETHERNET__ETHERNET_H
    #define __DRIVERS__ETHERNET__ETHERNET_H

    #include <common/vector.h>
    #include <common/string.h>
    #include <common/inputstream.h>
    #include <drivers/driver.h>
    #include <hardwarecommunication/port.h>
    #include <hardwarecommunication/bus.h>
    #include <hardwarecommunication/interrupts.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace ethernet
            {

                typedef uint64_t MediaAccessControlAddress;

                class EthernetDriverEventHandler
                {
                    public:
                        EthernetDriverEventHandler();
                        ~EthernetDriverEventHandler();

                        virtual void BeforeSend(uint8_t* buffer, uint32_t size);
                        virtual void DataSent(uint8_t* buffer, uint32_t size);
                        virtual bool DataReceived(uint8_t* buffer, uint32_t size);
                };

                class EthernetDriver : public Driver
                {
                    protected:
                        common::Vector<EthernetDriverEventHandler*> handlers;
                        virtual void DoSend(uint8_t* buffer, uint32_t size);
                        void FireDataReceived(uint8_t* buffer, uint32_t size);
                        void FireDataSent(uint8_t* buffer, uint32_t size);

                    public:
                        EthernetDriver(common::OutputStream* errorMessageStream);
                        ~EthernetDriver();

                        common::string GetDeviceName();
                        static MediaAccessControlAddress CreateMediaAccessControlAddress(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4, uint8_t digit5, uint8_t digit6);
                        virtual MediaAccessControlAddress GetMediaAccessControlAddress();

                        void Send(uint8_t* buffer, uint32_t size);
                        void ConnectEventHandler(EthernetDriverEventHandler* handler);
                };

            }
        }
    }

#endif
