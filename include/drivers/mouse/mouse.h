#ifndef __DRIVERS__MOUSE__MOUSE_H
    #define __DRIVERS__MOUSE__MOUSE_H

    #include <common/inputstream.h>
    #include <common/vector.h>
    #include <drivers/driver.h>
    #include <hardwarecommunication/port.h>
    #include <hardwarecommunication/interrupts.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace mouse
            {

                class MouseEventHandler
                {
                    public:
                        MouseEventHandler();
                        ~MouseEventHandler();

                        virtual void OnMouseMoved(int8_t x, int8_t y);
                        virtual void OnMouseButtonPressed(uint8_t button);
                        virtual void OnMouseButtonReleased(uint8_t button);
                };

                class Mouse : public Driver, public hardwarecommunication::InterruptHandler
                {
                    protected:
                        common::Vector<MouseEventHandler*> handlers;
                        hardwarecommunication::Port8Bit dataport;
                        hardwarecommunication::Port8Bit commandport;
                        void HandleInterrupt();

                        uint8_t buffer[3];
                        uint8_t offset;
                        uint8_t buttons;

                        void WaitForSignal();
                        void WaitForData();
                        void Send(uint8_t data);
                        uint8_t Receive();

                    public:
                        Mouse(hardwarecommunication::InterruptManager* interruptManager);
                        ~Mouse();
                        void Activate();
                        common::string GetDeviceName();
                        void ConnectEventHandler(MouseEventHandler* handler);
                        void DisconnectEventHandler(MouseEventHandler* handler);
                };

            }
        }
    }

#endif