
#ifndef __DRIVERS__DRIVER_H
    #define __DRIVERS__DRIVER_H

    #include<common/vector.h>
    #include<common/string.h>
    #include<common/outputstream.h>
    #include<memorymanagement.h>
    #include<hardwarecommunication/interrupts.h>

    namespace MYYOS
    {
        namespace drivers
        {

            class Driver
            {
                protected:
                    common::OutputStream* errorMessageStream;
                    void ErrorMessage(common::string message);
                    void ErrorMessage(int i);
                    void ErrorMessage(char c);

                public:
                    Driver(common::OutputStream* errorMessageStream = 0);
                    
                    // FIXME: Must be virtual (currently isnt because the kernel has no memory management yet)
                    
                    ~Driver();

                    virtual uint32_t Reset();
                    virtual void Initialize();
                    virtual void Activate();
                    virtual void Deactivate();
                    virtual common::string GetVendorName();
                    virtual common::string GetDeviceName();
                   
                    // "normal" syntax for connecting event handlers:
                    // void ConnectEventHandler(EventHandler*);
                    // void DisconnectEventHandler(EventHandler*);
            };



            class DriverSelectorEventHandler
            {
                public:
                    DriverSelectorEventHandler();
                    ~DriverSelectorEventHandler();
                    virtual void OnDriverSelected(Driver* driver);
            };

            class DriverSelector
            {
                public:
                    DriverSelector();
                    ~DriverSelector();
                    virtual void SelectDrivers(DriverSelectorEventHandler* handler, MemoryManager* memoryManager, hardwarecommunication::InterruptManager* interruptManager, common::OutputStream* errorMessageStream);
            };

            class DriverManager : public DriverSelectorEventHandler
            {
                public:
                    common::Vector<Driver*> drivers;
                public:
                    DriverManager();
                    ~DriverManager();

                    void AddDriver(Driver* driver);
                    void RemoveDriver(Driver* driver);
                    void OnDriverSelected(Driver* driver);
            };

        }
    }

#endif