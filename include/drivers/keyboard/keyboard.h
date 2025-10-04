
#ifndef __DRIVERS__KEYBOARD__KEYBOARD_H
    #define __DRIVERS__KEYBOARD__KEYBOARD_H

    #include <common/vector.h>
    #include <common/inputstream.h>
    #include <drivers/driver.h>
    #include <hardwarecommunication/port.h>
    #include <hardwarecommunication/interrupts.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace keyboard
            {

                class Keyboard : public Driver, public hardwarecommunication::InterruptHandler, public common::BasicInputStream<uint8_t>
                {
                    protected:
                        hardwarecommunication::Port8Bit dataport;
                        hardwarecommunication::Port8Bit commandport;
                        void HandleInterrupt();
                        void WaitForData();
                        void WaitForSignal();

                    public:
                        Keyboard(hardwarecommunication::InterruptManager* interruptManager, common::OutputStream* errorMessageStream = 0);
                        ~Keyboard();
                        void Initialize();
                        void Activate();
                        void Deactivate();
                        common::string GetDeviceName();
                };

            }
        }
    }

#endif