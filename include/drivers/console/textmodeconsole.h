#ifndef __DRIVERS__CONSOLE__TEXTMODECONSOLE_H
    #define __DRIVERS__CONSOLE__TEXTMODECONSOLE_H

    #include <common/types.h>
    #include <drivers/console/console.h>
    #include <drivers/driver.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace console
            {
                
                class TextmodeConsole : public Driver, public Console
                {
                    public:
                        TextmodeConsole();
                        ~TextmodeConsole();
                        common::string GetDeviceName();

                        uint16_t GetWidth();
                        uint16_t GetHeight();
                        
                        void PutChar(uint16_t x, uint16_t y, char c);
                        void SetForegroundColor(uint16_t x, uint16_t y, color::ConsoleColor foreground);
                        void SetBackgroundColor(uint16_t x, uint16_t y, color::ConsoleColor background);
                        
                        char GetChar(uint16_t x, uint16_t y);
                        color::ConsoleColor GetForegroundColor(uint16_t x, uint16_t y);
                        color::ConsoleColor GetBackgroundColor(uint16_t x, uint16_t y);

                    protected:
                        //Points to the video memory
                        uint16_t* videoMemory;
                };



            } // namespace console
        } // namespace drivers
    }   


#endif