
#include <drivers/console/consolearea.h>
 
namespace MYYOS
{
    namespace drivers
    {
        namespace console
        {
                        
            ConsoleArea::ConsoleArea(Console* console, uint16_t left, uint16_t top, uint16_t width, uint16_t height)
             :  console(console),
                left(left),
                top(top),
                width(width),
                height(height)
            {
            }

            ConsoleArea::~ConsoleArea()
            {
            }
            
            
            uint16_t ConsoleArea::GetWidth()
            {
                return width;
            }
            
            uint16_t ConsoleArea::GetHeight()
            {
                return height;
            }
            
            void ConsoleArea::PutChar(uint16_t x, uint16_t y, char c)
            {
                if(x >= width || y >= height)
                    return;
                console->PutChar(x+left, y+top, c);
            }
            
            void ConsoleArea::SetForegroundColor(uint16_t x, uint16_t y, color::ConsoleColor foreground)
            {
                if(x >= width || y >= height)
                    return;
                console->SetForegroundColor(x+left, y+top, foreground);
            }
            
            void ConsoleArea::SetBackgroundColor(uint16_t x, uint16_t y, color::ConsoleColor background)
            {
                if(x >= width || y >= height)
                    return;
                console->SetBackgroundColor(x+left, y+top, background);
            }
        
            char ConsoleArea::GetChar(uint16_t x, uint16_t y)
            {
                if(x >= width || y >= height)
                    return ' ';
                return console->GetChar(x+left, y+top);
            }
            
            color::ConsoleColor ConsoleArea::GetForegroundColor(uint16_t x, uint16_t y)
            {
                if(x >= width || y >= height)
                    return color::LightGray;
                return console->GetForegroundColor(x+left, y+top);
            }
            
            color::ConsoleColor ConsoleArea::GetBackgroundColor(uint16_t x, uint16_t y)
            {
                if(x >= width || y >= height)
                    return color::Black;
                return console->GetBackgroundColor(x+left, y+top);
            }
            
                        
        }        
    }
}
