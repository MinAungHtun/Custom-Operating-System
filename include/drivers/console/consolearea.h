 
#ifndef __DRIVERS__CONSOLE__CONSOLEAREA_H
    #define __DRIVERS__CONSOLE__CONSOLEAREA_H

    #include <common/types.h>
    #include <drivers/console/console.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace console
            {

                class ConsoleArea : public Console
                {
                    protected:
                        Console* console;
                        uint16_t left;
                        uint16_t top;
                        uint16_t width;
                        uint16_t height;
                    public:
                        ConsoleArea(Console* console, uint16_t left, uint16_t top, uint16_t width, uint16_t height);
                        ~ConsoleArea();
                        
                        uint16_t GetWidth();
                        uint16_t GetHeight();
                        
                        void PutChar(uint16_t x, uint16_t y, char c);
                        void SetForegroundColor(uint16_t x, uint16_t y, color::ConsoleColor foreground);
                        void SetBackgroundColor(uint16_t x, uint16_t y, color::ConsoleColor background);
                  
                        char GetChar(uint16_t x, uint16_t y);
                        color::ConsoleColor GetForegroundColor(uint16_t x, uint16_t y);
                        color::ConsoleColor GetBackgroundColor(uint16_t x, uint16_t y);
                };
             
            }
        }
    }
    
#endif
