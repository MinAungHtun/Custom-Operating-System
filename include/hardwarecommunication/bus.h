
#ifndef __HARDWARECOMMUNICATION__BUS_H
    #define __HARDWARECOMMUNICATION__BUS_H

    #include <common/types.h>
    #include <common/vector.h>
    #include <hardwarecommunication/interrupts.h>

    namespace MYYOS
    {
        namespace hardwarecommunication
        {

            class BusReadEventHandler
            {
                public:
                    BusReadEventHandler();
                    ~BusReadEventHandler();
                    virtual void OnRead(const uint8_t* buffer, uint32_t size);
            };


            class Bus
            {
                protected:
                    common::Vector<BusReadEventHandler*> readEventHandlers;
                public:
                    Bus();
                    ~Bus();

                    virtual uint32_t Write(const uint8_t* buffer, uint32_t size);
                    void ConnectEventHandler(BusReadEventHandler* handler);
                    void DisconnectEventHandler(BusReadEventHandler* handler);
                    void OnRead(const uint8_t* buffer, uint32_t size);
            };


            class PassiveBus : public Bus
            {
                protected:
                    virtual uint32_t Read(uint8_t* buffer, uint32_t size);
                public:
                    void Poll();
            };


            class PassiveBusInterruptConnector : public InterruptHandler
            {
                protected:
                    PassiveBus* bus;
                public:
                    PassiveBusInterruptConnector(InterruptManager* manager, int interrupt, PassiveBus* bus);
                    ~PassiveBusInterruptConnector();
                    void HandleInterrupt();
            };

        }
    }

#endif
