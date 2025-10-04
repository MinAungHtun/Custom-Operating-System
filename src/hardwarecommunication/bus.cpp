
#include <hardwarecommunication/bus.h>

namespace MYYOS
{
    namespace hardwarecommunication
    {

        BusReadEventHandler::BusReadEventHandler()
        {
        }

        BusReadEventHandler::~BusReadEventHandler()
        {
        }

        void BusReadEventHandler::OnRead(const uint8_t*, uint32_t)
        {
        }



        Bus::Bus()
        {
        }

        Bus::~Bus()
        {
        }

        uint32_t Bus::Write(const uint8_t*, uint32_t)
        {
            return 0;
        }

        void Bus::ConnectEventHandler(BusReadEventHandler* handler)
        {
            readEventHandlers.push_back(handler);
        }

        void Bus::DisconnectEventHandler(BusReadEventHandler* handler)
        {
            readEventHandlers.erase(handler);
        }

        void Bus::OnRead(const uint8_t* buffer, uint32_t size)
        {
            for(Vector<BusReadEventHandler*>::iterator i = readEventHandlers.begin(); i != readEventHandlers.end(); i++)
                (*i)->OnRead(buffer, size);
        }



        uint32_t PassiveBus::Read(uint8_t*, uint32_t)
        {
            return 0;
        }

        void PassiveBus::Poll()
        {
            uint8_t buffer[256];
            uint32_t bytesread = Read(&buffer[0], 256);
            if( bytesread > 0 )
                OnRead(&buffer[0], bytesread);
        }



        PassiveBusInterruptConnector::PassiveBusInterruptConnector(InterruptManager* manager, int interrupt, PassiveBus* bus)
            : InterruptHandler(interrupt, manager)
        {
            this->bus = bus;
        }

        PassiveBusInterruptConnector::~PassiveBusInterruptConnector()
        {
        }

        void PassiveBusInterruptConnector::HandleInterrupt()
        {
            // This causes a SEGFAULT, if the interrupt occurs between base-class
            // initialization and setting this->bus.
            // Therefore, busses should only be connected while interrupts are disabled
            bus->Poll();
        }

    }
}


