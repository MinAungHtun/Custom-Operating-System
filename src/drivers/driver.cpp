
#include <drivers/driver.h>

using namespace MYYOS::common;

namespace MYYOS
{
    namespace drivers
    {

        Driver::Driver(OutputStream* errorMessageStream)
        {
            this->errorMessageStream = errorMessageStream;
        }

        Driver::~Driver()
        {
            this->errorMessageStream = 0;
        }

        void Driver::ErrorMessage(string message)
        {
            if(errorMessageStream != 0)
                errorMessageStream->Write(message);
        }

        void Driver::ErrorMessage(int i)
        {
            if(errorMessageStream != 0)
                errorMessageStream->WriteInt(i);
        }        

        void Driver::ErrorMessage(char c)
        {
            if(errorMessageStream != 0)
                errorMessageStream->WriteChar(c);
        }        
        
        
        uint32_t Driver::Reset()
        {
            return 0;
        }

        void Driver::Initialize()
        {
        }
        
        void Driver::Activate()
        {
        }

        void Driver::Deactivate()
        {
        }


        string Driver::GetVendorName()
        {
            return "Generic";
        }

        string Driver::GetDeviceName()
        {
            return "Unknown Driver";
        }



        DriverSelectorEventHandler::DriverSelectorEventHandler()
        {
        }

        DriverSelectorEventHandler::~DriverSelectorEventHandler()
        {
        }

        void DriverSelectorEventHandler::OnDriverSelected(Driver*)
        {
        }



        DriverSelector::DriverSelector()
        {
        }

        DriverSelector::~DriverSelector()
        {
        }

        void DriverSelector::SelectDrivers(DriverSelectorEventHandler*, MemoryManager*, hardwarecommunication::InterruptManager*, OutputStream*)
        {
        }



        DriverManager::DriverManager()
        {
        }

        DriverManager::~DriverManager()
        {
            while(!drivers.empty())
                RemoveDriver(*drivers.begin());
        }

        void DriverManager::AddDriver(Driver* driver)
        {
            drivers.push_back(driver);
        }

        void DriverManager::RemoveDriver(Driver* driver)
        {
            driver->Deactivate();
            drivers.erase(driver);
        }

        void DriverManager::OnDriverSelected(Driver* driver)
        {
            AddDriver(driver);
        }

    }
}
