

#include <drivers/mouse/mouse.h>

using namespace MYYOS::common;
using namespace MYYOS::hardwarecommunication;


namespace MYYOS
{
    namespace drivers
    {
        namespace mouse
        {

            MouseEventHandler::MouseEventHandler()
            {
            }

            MouseEventHandler::~MouseEventHandler()
            {
            }

            void MouseEventHandler::OnMouseMoved(int8_t, int8_t)
            {
            }

            void MouseEventHandler::OnMouseButtonPressed(uint8_t)
            {
            }

            void MouseEventHandler::OnMouseButtonReleased(uint8_t)
            {
            }





            Mouse::Mouse(InterruptManager* interruptManager)
                : Driver(),
                InterruptHandler(interruptManager->HardwareInterruptOffset()+12, interruptManager),
                dataport(0x60),
                commandport(0x64)
            {
                offset = 2;
                buttons = 0;
            }

            Mouse::~Mouse()
            {
            }

            string Mouse::GetDeviceName()
            {
                return "Mouse";
            }

            void Mouse::Activate()
            {
                //Mousecontroller activation
                WaitForSignal();
                commandport.Write(0xA8);

                //Interrupts activation
                WaitForSignal();
                commandport.Write(0x20);
                WaitForData();
                uint8_t status = dataport.Read() | 2;
                WaitForSignal();
                commandport.Write(0x60); // command 0x60 = set controller command byte
                WaitForSignal();
                dataport.Write(status);

                //Select default mouse settings
                //Send(0xF6);
                //Receive();

                //Mouse Activation
                Send(0xF4);
                Receive();
            }

            void Mouse::HandleInterrupt()
            {
                uint8_t status = commandport.Read();
                if (!(status & 0x20))
                    return;

                buffer[offset] = dataport.Read();
                offset = (offset + 1) % 3;

                if(offset == 0)
                {
                    for(Vector<MouseEventHandler*>::iterator handler = handlers.begin(); handler != handlers.end(); ++handler)
                    {
                        if(buffer[1] != 0 || buffer[2] != 0)
                            (*handler)->OnMouseMoved(buffer[1], -buffer[2]);

                        for(uint8_t i = 0; i < 3; i++)
                        {
                            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
                            {
                                if(buttons & (0x1<<i))
                                    (*handler)->OnMouseButtonReleased(i+1);
                                else
                                    (*handler)->OnMouseButtonPressed(i+1);
                            }
                        }
                        buttons = buffer[0];
                    }
                }
            }

            void Mouse::WaitForData()
            {
                //while((commandport.Read() & 1)!=1);
            }

            void Mouse::WaitForSignal()
            {
                //while((commandport.Read() & 2)!=0);
            }

            void Mouse::Send(uint8_t data)
            {
                WaitForSignal();
                commandport.Write(0xD4);
                WaitForSignal();
                dataport.Write(data);
            }

            uint8_t Mouse::Receive()
            {
                WaitForData();
                return dataport.Read();
            }

            void Mouse::ConnectEventHandler(MouseEventHandler* handler)
            {
                handlers.push_back(handler);
            }

            void Mouse::DisconnectEventHandler(MouseEventHandler* handler)
            {
                handlers.erase(handler);
            }


        }
    }
}

