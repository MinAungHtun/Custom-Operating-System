
#include <drivers/keyboard/keyboard.h>

using namespace MYYOS::common;
using namespace MYYOS::hardwarecommunication;

namespace MYYOS
{
    namespace drivers
    {
        namespace keyboard
        {


            Keyboard::Keyboard(InterruptManager* interruptManager, OutputStream* errorMessageStream)
                : Driver(errorMessageStream),
                InterruptHandler(interruptManager->HardwareInterruptOffset()+1, interruptManager),
                dataport(0x60),
                commandport(0x64)
            {
            }

            Keyboard::~Keyboard()
            {
            }

            string Keyboard::GetDeviceName()
            {
                return "Keyboard";
            }

            void Keyboard::Initialize()
            {
                while(commandport.Read() & 0x1)
                    dataport.Read();
            }

            void Keyboard::Activate()
            {
                // activate keyboard controller
                WaitForSignal();
                commandport.Write(0xae);

                // activate interrupts
                WaitForSignal();
                commandport.Write(0x20); // command 0x20 = read controller command byte
                WaitForData();
                uint8_t status = (dataport.Read() | 1) & ~0x10;
                WaitForSignal();
                commandport.Write(0x60); // command 0x60 = set controller command byte
                WaitForSignal();
                dataport.Write(status);

                dataport.Write(0xf4);
            }

            void Keyboard::WaitForData()
            {
                //while((commandport.Read() & 1)!=1);
            }

            void Keyboard::WaitForSignal()
            {
                //while((commandport.Read() & 2)!=0);
            }

            void Keyboard::Deactivate()
            {
                //SendCommand(0xad);
            }

            void Keyboard::HandleInterrupt()
            {
                uint8_t scancode = dataport.Read();
                for(Vector<InputStreamEventHandler<uint8_t>*>::iterator handler = handlers.begin(); handler != handlers.end(); ++handler)
                    (*handler)->OnRead(scancode);
            }


        }
    }
}

