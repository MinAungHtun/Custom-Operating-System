#ifndef __DRIVERS__CLOCK__CLOCK_H
    #define __DRIVERS__CLOCK__CLOCK_H

    #include <common/inputstream.h>
    #include <common/vector.h>
    #include <common/time.h>
    #include <drivers/driver.h>
    #include <hardwarecommunication/port.h>
    #include <hardwarecommunication/interrupts.h>

    using namespace MYYOS::common;

    namespace MYYOS
    {
        namespace drivers
        {
            namespace clock
            {

                class ClockEventHandler
                {
                    public:
                        ClockEventHandler();
                        ~ClockEventHandler();

                        virtual void OnTime(const common::Time& time);
                };

                class Clock : public Driver, public hardwarecommunication::InterruptHandler
                {
                    private:
                        volatile uint64_t ticks;
                    protected:
                        common::Vector<ClockEventHandler*> handlers;
                        uint16_t TicksBetweenEvents;
                        uint16_t TicksUntilNextEvent;
                        hardwarecommunication::Port8Bit dataport;
                        hardwarecommunication::Port8Bit commandport;
                        void HandleInterrupt();
                        uint8_t ReadHardwareClock(uint8_t address);
                        bool BinaryCodedDecimalRepresentation;
                        uint8_t BinaryRepresentation(uint8_t);

                    public:
                        Clock(hardwarecommunication::InterruptManager* interruptManager, uint16_t TimeBetweenEventsInTenthOfSeconds = 10);
                        ~Clock();
                        void Activate();
                        common::string GetDeviceName();
                        void ConnectEventHandler(ClockEventHandler* handler);
                        void DisconnectEventHandler(ClockEventHandler* handler);
                        void Delay(uint32_t milliseconds);
                };

            }
        }
    }

#endif
