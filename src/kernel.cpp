
#include <common/inputstream.h>
#include <common/string.h>
#include <multiboot.h>
#include <globaldescriptortable.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/peripheralcomponentinterconnect.h>
#include <drivers/console/console.h>
#include <drivers/console/textmodeconsole.h>
#include <drivers/console/consolearea.h>
#include <drivers/console/consolestream.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/mouse/mouse.h>
#include <drivers/clock/clock.h>
#include <drivers/keyboard/keyboardinterpreter.h>

#include<drivers/video/video.h>
#include<drivers/video/videoelectronicsstandardsassociation.h>
#include<drivers/video/videographicsarray.h>

#include <protocols/ethernet/ethernetframe.h>
#include <protocols/ethernet/internetprotocol.h>
#include <protocols/ethernet/addressresolutionprotocol.h>
#include <protocols/ethernet/internetcontrolmessageprotocol.h>
#include <protocols/ethernet/transmissioncontrolprotocol.h>
#include <protocols/ethernet/userdatagramprotocol.h>

#include <graphicalobjects/desktop.h>
#include <graphicalobjects/window.h>
#include <graphicalobjects/textbox.h>
#include <graphicalobjects/button.h>

#include <applications/myynet.h>

using namespace MYYOS;
using namespace MYYOS::hardwarecommunication;
using namespace MYYOS::drivers;
using namespace MYYOS::drivers::console;
using namespace MYYOS::drivers::keyboard;
using namespace MYYOS::drivers::mouse;
using namespace MYYOS::drivers::clock;
using namespace MYYOS::drivers::ethernet;
using namespace MYYOS::drivers::video;
using namespace MYYOS::protocols::ethernet;
using namespace MYYOS::graphicalobjects;
using namespace MYYOS::applications;

extern "C" void kernelMain(const multiboot_info& multiboot_structure, uint32_t multiboot_magic);





class MouseToConsole : public MouseEventHandler
{
        Console* console;
        int x;
        int y;
        uint8_t buttons;

    public:
        MouseToConsole(Console* console)
        {
            this->console = console;
            buttons = 0;
            x = console->GetWidth()*2;
            y = console->GetHeight()*2;
            console->InvertColors(x/4,y/4);
        }

        void OnMouseButtonPressed(uint8_t button)
        {
            buttons |= 1 << button;
            console->InvertColors(this->x/4,this->y/4);
        }
        void OnMouseButtonReleased(uint8_t button)
        {
            buttons &= ~(1 << button);
            console->InvertColors(this->x/4,this->y/4);
        }

        void OnMouseMoved(int8_t x, int8_t y)
        {
            bool ButtonsPressed = (buttons != 0);
            if(!ButtonsPressed)
                console->InvertColors(this->x/4,this->y/4);

            this->x += x;
            if(this->x < 0)
                this->x = 0;
            if(this->x >= console->GetWidth()*4)
                this->x = console->GetWidth()*4-1;

            this->y += y;
            if(this->y < 0)
                this->y = 0;
            if(this->y >= console->GetHeight()*4)
                this->y = console->GetHeight()*4-1;

            if(!ButtonsPressed)
                console->InvertColors(this->x/4,this->y/4);
        }
};


class KeyboardToStream : public KeyboardEventHandler
{
        OutputStream* stream;
    public:
        KeyboardToStream(OutputStream* stream)
        {
            this->stream = stream;
        }

        void OnKeyboardKeyPressed(KeyCode keyCode, KeyboardState)
        {
            if(31 < keyCode && keyCode < 127)
                stream->WriteChar((char)keyCode);
        }
};


class ClockToStream : public ClockEventHandler
{
        OutputStream* stream;

    public:
        ClockToStream(OutputStream* stream)
        {
            this->stream = stream;
        }

        void OnTime(const Time& t)
        {
            *stream
            << "\r" << (t.hour >= 10 ? "" : " ") << (int)t.hour
            << ":" << (t.minute >= 10 ? "" : "0") << (int)t.minute
            << ":" << (t.second >= 10 ? "" : "0") << (int)t.second;
        }
};


class TCPtoStream : public TransmissionControlProtocolPayloadHandler
{
        OutputStream* stream;

    public:
        TCPtoStream(OutputStream* stream)
        : TransmissionControlProtocolPayloadHandler()
        {
            this->stream = stream;
        }

        void HandleTransmissionControlProtocolPayload(TransmissionControlProtocolConnection* connection, uint8_t* buffer, uint32_t length)
        {
            buffer[length] = '\0';
            if(stream != 0)
                (*stream)
                    << (int)((connection->remoteAddress & 0x000000FF)) << "."
                    << (int)((connection->remoteAddress & 0x0000FF00) >> 8) << "."
                    << (int)((connection->remoteAddress & 0x00FF0000) >> 16) << "."
                    << (int)((connection->remoteAddress & 0xFF000000) >> 24)
                    << ": " << (string)buffer;
        }

        void Connected(TransmissionControlProtocolConnection* connection)
        {
            if(stream != 0)
                (*stream)
                    << (int)((connection->remoteAddress & 0x000000FF)) << "."
                    << (int)((connection->remoteAddress & 0x0000FF00) >> 8) << "."
                    << (int)((connection->remoteAddress & 0x00FF0000) >> 16) << "."
                    << (int)((connection->remoteAddress & 0xFF000000) >> 24)
                    << " connected\n";
        }

        void Disconnected(TransmissionControlProtocolConnection* connection)
        {
            if(stream != 0)
                (*stream)
                    << (int)((connection->remoteAddress & 0x000000FF)) << "."
                    << (int)((connection->remoteAddress & 0x0000FF00) >> 8) << "."
                    << (int)((connection->remoteAddress & 0x00FF0000) >> 16) << "."
                    << (int)((connection->remoteAddress & 0xFF000000) >> 24)
                    << " disconnected\n";
        }

};

class UDPtoStream : public UserDatagramProtocolPayloadHandler
{
        OutputStream* stream;

    public:
        UDPtoStream(OutputStream* stream)
        : UserDatagramProtocolPayloadHandler()
        {
            this->stream = stream;
        }

        void HandleUserDatagramProtocolPayload(UserDatagramProtocolSocket*, uint8_t* buffer, uint32_t length)
        {
            buffer[length] = '\0';
            if(stream != 0)
                (*stream) << (string)buffer;
        }
};


void kernelMain(const multiboot_info& multibootHeader, uint32_t /*multiboot_magic*/)
{
    #define GRAPHICSMODE
    #define NETWORK
    //#define TEST_TCP
    //#define TEST_UDP

    TextmodeConsole console;
    console.PutString(0,0,"                                Welcome to MYYOS                                ",color::Black, color::LightGray);
    ConsoleArea ethernetRawDumpArea(&console, 0, 1, 24, 22);
    ConsoleStream ethernetRawDumpStream(&ethernetRawDumpArea);
    ConsoleArea kernelMessageArea(&console, 24, 1, console.GetWidth()-24, 22);
    ConsoleStream kernelMessageStream(&kernelMessageArea);







    kernelMessageStream << "Scanning Hardware.";
    GlobalDescriptorTable globalDescriptorTable(multibootHeader);
    kernelMessageStream << ".";
    MemoryManager memoryManager((uint8_t*)(1024*(multibootHeader.mem_upper - 1024*16)), 1024*1024*16);  // 16 MiB Heap at the end of the RAM

    kernelMessageStream << ".";
    InterruptManager interruptManager(0x20, &globalDescriptorTable, &kernelMessageStream);
    kernelMessageStream << ".";
    DriverManager driverManager;

        kernelMessageStream << ".";
        driverManager.AddDriver(&console);

        kernelMessageStream << ".";                                                                                                    // Keyboard
        Keyboard keyboard(&interruptManager);
        KeyboardInterpreterDE keyboardinterpreter;
        keyboard.ConnectEventHandler(&keyboardinterpreter);
        #ifndef GRAPHICSMODE
            KeyboardToStream keyboardToStream(&kernelMessageStream);
            keyboardinterpreter.ConnectEventHandler(&keyboardToStream);
        #endif
        driverManager.AddDriver(&keyboard);

        kernelMessageStream << ".";                                                                                                    // Mouse
        Mouse mouse(&interruptManager);
        #ifndef GRAPHICSMODE
            ConsoleArea consoleMouseArea(&console, 0,1,console.GetWidth(), console.GetHeight()-2);
            MouseToConsole mousetoconsole(&consoleMouseArea);
            mouse.ConnectEventHandler(&mousetoconsole);
        #endif
        driverManager.AddDriver(&mouse);

        kernelMessageStream << ".";                                                                                                    // Hardware Clock
        Clock clock(&interruptManager, 1/*15*/);
        #ifndef GRAPHICSMODE
            ConsoleArea consoleClockArea(&console, console.GetWidth()-8, console.GetHeight()-1, 8, 1);
            ConsoleStream clockStream(&consoleClockArea);
            ClockToStream clocktostream(&clockStream);
            clock.ConnectEventHandler(&clocktostream);
        #endif
        driverManager.AddDriver(&clock);

        Vector<DriverSelector*> driverSelectors;
            PeripheralComponentInterconnectController pcicontroller;                                                                   // PCI Devices
            driverSelectors.push_back(&pcicontroller);

            //UniversalSerialBusController usbcontroller;                                                                              // USB Devices
            //driverSelectors.push_back(&usbcontroller);

        for(Vector<DriverSelector*>::iterator i = driverSelectors.begin(); i != driverSelectors.end(); i++)
        {
            kernelMessageStream << ".";
            #ifdef GRAPHICSMODE
                (*i)->SelectDrivers(&driverManager, &memoryManager, &interruptManager, 0);
            #else
                (*i)->SelectDrivers(&driverManager, &memoryManager, &interruptManager, &ethernetRawDumpStream);
            #endif
        }

    kernelMessageStream << "                     [ OK ]\n";







    kernelMessageStream << "Initializing Hardware, Stage 1";
    uint32_t PauseTime = 0;
    for(Vector<Driver*>::iterator i = driverManager.drivers.begin(); i != driverManager.drivers.end(); i++)
    {
        kernelMessageStream << ".";
        uint32_t DevicePauseTime = (*i)->Reset();
        if(DevicePauseTime > PauseTime)
            PauseTime = DevicePauseTime;
    }
    kernelMessageStream << "           [ OK ]\n";

    kernelMessageStream << "Activating Interrupts";
    interruptManager.Activate();
    clock.Delay(PauseTime+100);  // must be after interruptManager.Activate() because this never terminates without the timer interrupt
    kernelMessageStream << "                          [ OK ]\n";

    kernelMessageStream << "Initializing Hardware, Stage 2";
    for(Vector<Driver*>::iterator i = driverManager.drivers.begin(); i != driverManager.drivers.end(); i++)
    {
        kernelMessageStream << ".";
        (*i)->Initialize();
    }
    kernelMessageStream << "           [ OK ]\n";

    kernelMessageStream << "Initializing Hardware, Stage 3";
    for(Vector<Driver*>::iterator i = driverManager.drivers.begin(); i != driverManager.drivers.end(); i++)
    {
        kernelMessageStream << ".";
        (*i)->Activate();
    }
    kernelMessageStream << "           [ OK ]\n";





    #ifdef NETWORK

        kernelMessageStream << "\nInitializing Network Subsystem\n";
         // Build the Open Systems Interconnection (OSI)-Model Layers

        kernelMessageStream << "  1: Hardware                                  ";                                               // 1 Ethernet-Chip Driver
        EthernetDriver* eth0 = (EthernetDriver*)(driverManager.drivers[5]);
        kernelMessageStream << "[ OK ]\n";

        kernelMessageStream << "  2: Ethernet Frame                            ";                                               // 2 Ethernet Frame
        EthernetFrameHandler ethernetFrameHandler(eth0, &kernelMessageStream, &memoryManager);
        kernelMessageStream << "[ OK ]\n";

        kernelMessageStream << "  3: Internet Protocol (IPv4)                  ";                                               // 3 IP
        SubnetMask subnetMask = InternetProtocolHandler::CreateSubnetMask(255,255,255,0);
        InternetProtocolAddress defaultGatewayInternetProtocolAddress = InternetProtocolHandler::CreateInternetProtocolAddress(10,0,2,2);
        InternetProtocolAddress ownInternetProtocolAddress = InternetProtocolHandler::CreateInternetProtocolAddress(10,0,2,15);
        InternetProtocolHandler internetProtocolHandler(&ethernetFrameHandler, ownInternetProtocolAddress, defaultGatewayInternetProtocolAddress,
                                                        subnetMask, &kernelMessageStream, &memoryManager);
        kernelMessageStream << "[ OK ]\n";

        kernelMessageStream << "     Address Resolution Protocol (ARP)         ";                                               // 3 ARP
        AddressResolutionProtocolHandler addressResolutionProtocolHandler(&ethernetFrameHandler,
                                                                        &internetProtocolHandler, &kernelMessageStream);
        kernelMessageStream << "[ OK ]\n";

        kernelMessageStream << "  4: Internet Control Message Protocol (ICMP)  ";                                               // 4 ICMP
        InternetControlMessageProtocolHandler internetControlMessageProtocolHandler(&internetProtocolHandler);
        internetControlMessageProtocolHandler.RequestEchoReply(defaultGatewayInternetProtocolAddress);
        kernelMessageStream << "[ OK ]\n";

        kernelMessageStream << "     Transmission Control Protocol (TCP)       ";                                               // 4 TCP
        TransmissionControlProtocolHandler transmissionControlProtocolHandler(&internetProtocolHandler, &kernelMessageStream, &memoryManager);
        TransmissionControlProtocolPortListener(&transmissionControlProtocolHandler, 0x1337, &kernelMessageStream, &memoryManager);//   Listening Socket
        kernelMessageStream << "[ OK ]\n";

        kernelMessageStream << "     User Datagram Protocol (UDP)              ";                                               // 4 UDP
        UserDatagramProtocolHandler userDatagramProtocolHandler(&internetProtocolHandler, &kernelMessageStream, &memoryManager);
        kernelMessageStream << "[ OK ]\n";

        //kernelMessageStream << "  5: Hypertext Transfer Protocol (HTTP)        ";                                             // 5 HTTP
        //HypertextTransferProtocolHandler hypertextTransferProtocolHandler(&transmissionControlProtocolHandler,
        //                                                                    &kernelMessageStream);
        //kernelMessageStream << "[ OK ]\n";

        //kernelMessageStream << "     Dynamic Host Configuration Protocol (DHCP)";                                             // 5 DHCP
        //DynamicHostConfigurationProtocolHandler dynamicHostConfigurationProtocolHandler(&userDatagramProtocolHandler)
        //kernelMessageStream << "[ OK ]\n";

        //kernelMessageStream << "     Domain Name System (DNS)                  ";                                             // 5 DNS
        //DomainNameSystemHandler domainNameSystemHandler(&userDatagramProtocolHandler);
        //kernelMessageStream << "[ OK ]\n";

    #endif // NETWORK


    #ifndef GRAPHICSMODE
        #ifdef NETWORK
            kernelMessageStream << "\n";
            InternetProtocolAddress hostIP;
            #define atwork
            #ifdef atwork
                hostIP = InternetProtocolHandler::CreateInternetProtocolAddress(130,149,165,40);
            #else
                hostIP = InternetProtocolHandler::CreateInternetProtocolAddress(192,168,178,23);
            #endif

            #ifdef TEST_TCP
                ethernetRawDumpStream << "Testing TCP\n";
                TransmissionControlProtocolConnection* connection = transmissionControlProtocolHandler.Connect(hostIP, 1234);
                TCPtoStream tcpToStream(&kernelMessageStream);
                connection->ConnectTransmissionControlProtocolPayloadHandler(&tcpToStream);
                while(!connection->IsConnected());
                connection->Send("HALLO\n");
            #endif

            #ifdef TEST_UDP
                ethernetRawDumpStream << "Testing UDP\n";
                UserDatagramProtocolSocket* udpSocket = userDatagramProtocolHandler.Connect(hostIP, 1235);
                UDPtoStream udpToStream(&kernelMessageStream);
                udpSocket->ConnectUserDatagramProtocolPayloadHandler(&udpToStream);
                udpSocket->Send("HALLO\n");
            #endif
        #endif
    #endif






    #ifdef GRAPHICSMODE
        kernelMessageStream << "\nInitializing Graphics Mode";
        VideoDriver* video = (VideoDriver*)(driverManager.drivers[4]);
        video->SetMode(320, 200, 8);
        Desktop desktop(video);
        mouse.ConnectEventHandler(&desktop);
        clock.ConnectEventHandler(&desktop);
        keyboardinterpreter.ConnectEventHandler(&desktop);

        #ifdef NETWORK
            myynetClient myynet(200,150, &transmissionControlProtocolHandler);
        #else
            myynetClient myynet(200,150, 0);
        #endif
        Window myynetWindow(&myynet, "myynet");
        desktop.AddChild(&myynetWindow);
    #endif

    while(1);
}







