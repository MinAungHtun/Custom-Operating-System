
#include <drivers/clock/clock.h>

using namespace MYYOS::common;
using namespace MYYOS::hardwarecommunication;

namespace MYYOS
{
    namespace drivers
    {
        namespace clock
        {

            ClockEventHandler::ClockEventHandler()
            {
            }

            ClockEventHandler::~ClockEventHandler()
            {
            }

            void ClockEventHandler::OnTime(const Time&)
            {
            }





            Clock::Clock(InterruptManager* interruptManager, uint16_t TimeBetweenEventsInTenthOfSeconds)
                : Driver(),
                InterruptHandler(interruptManager->HardwareInterruptOffset(), interruptManager),
                dataport(0x71),
                commandport(0x70)
            {
                ticks = 0;
                TicksBetweenEvents = TimeBetweenEventsInTenthOfSeconds;
                TicksUntilNextEvent = 1;
            }

            Clock::~Clock()
            {
            }

            string Clock::GetDeviceName()
            {
                return "Clock";
            }

            uint8_t Clock::BinaryRepresentation(uint8_t number)
            {
                if(!BinaryCodedDecimalRepresentation)
                    return number;
                return (number & 0xf) + ((number >> 4) & 0xf) * 10;
            }

            uint8_t Clock::ReadHardwareClock(uint8_t address)
            {
                //There should be no update progress should the date time registers have to be read
                if(address < 10)
                {
                    commandport.Write(0xa);
                    while((dataport.Read() & (1 << 7)) != 0)
                        asm volatile("nop");
                }

                commandport.Write(address);
                return dataport.Read();
            }


            void Clock::Activate()
            {
                BinaryCodedDecimalRepresentation = (ReadHardwareClock(0xb) & 4) == 0;
            }

            void Clock::HandleInterrupt()
            {
                ++ticks;
                if(--TicksUntilNextEvent)
                    return;
                TicksUntilNextEvent = TicksBetweenEvents;

                Time t;
                t.year =   2000 + BinaryRepresentation(ReadHardwareClock(0x9));
                t.month =  BinaryRepresentation(ReadHardwareClock(0x8));
                t.day =    BinaryRepresentation(ReadHardwareClock(0x7));

                t.hour =   BinaryRepresentation(ReadHardwareClock(0x4));
                t.minute = BinaryRepresentation(ReadHardwareClock(0x2));
                t.second = BinaryRepresentation(ReadHardwareClock(0x0));

                for(Vector<ClockEventHandler*>::iterator handler = handlers.begin(); handler != handlers.end(); ++handler)
                    (*handler)->OnTime(t);
            }

            void Clock::Delay(uint32_t milliseconds)
            {
                uint64_t EndTicks = ticks + ((milliseconds+99)/100);
                while(ticks < EndTicks);
            }

            void Clock::ConnectEventHandler(ClockEventHandler* handler)
            {
                handlers.push_back(handler);
            }

            void Clock::DisconnectEventHandler(ClockEventHandler* handler)
            {
                handlers.erase(handler);
            }


        }
    }
}


