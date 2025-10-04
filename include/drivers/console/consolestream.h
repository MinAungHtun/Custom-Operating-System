 
#ifndef __DRIVERS__CONSOLE__CONSOLESTREAM_H
    #define __DRIVERS__CONSOLE__CONSOLESTREAM_H

    #include <common/types.h>
    #include <common/outputstream.h>
    #include <drivers/console/console.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace console
            {

                class ConsoleStream : public OutputStream
                {
                    protected:
                        Console* console;
                        uint16_t cursorX;
                        uint16_t cursorY;
                        
                    public:
                        ConsoleStream(Console* console);
                        ~ConsoleStream();
                        void WriteChar(char c);
                        void SetCursor(uint16_t x, uint16_t y);
                };
                
            }
        }
    }
    
#endif
