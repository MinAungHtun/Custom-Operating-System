
#include <drivers/console/consolestream.h>
 
namespace MYYOS
{
    namespace drivers
    {
        namespace console
        {
                        
            ConsoleStream::ConsoleStream(Console* console)
             : OutputStream()
            {
                cursorX = 0;
                cursorY = 0;
                this->console = console;
            }
            
            ConsoleStream::~ConsoleStream()
            {
            }
            
            void ConsoleStream::SetCursor(uint16_t x, uint16_t y)
            {
                cursorX = x;
                cursorY = (y < console->GetHeight() ? y : (console->GetHeight()-1));
            }
            
            void ConsoleStream::WriteChar(char c)
            {
                if(cursorX >= console->GetWidth())
                {
                    cursorX = 0;
                    if(++cursorY >= console->GetHeight())
                    {
                        console->ScrollUp();
                        cursorY = console->GetHeight() - 1;
                    }
                }
                
                switch(c)
                {
                    case '\n':
                        if(++cursorY >= console->GetHeight())
                        {
                            console->ScrollUp();
                            cursorY = console->GetHeight() - 1;
                        }

                        // no break here, we want to fall through to case \r ("\n" is implicitly "\n\r")
                    case '\r':
                        cursorX = 0;
                        break;
                        
                    case '\0':
                        return;
                        
                    default:
                        console->PutChar(cursorX++, cursorY, c);
                }

                // As it gets lower, the scroll gets faster
            }

           
            
        }        
    }
}
