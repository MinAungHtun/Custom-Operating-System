
#include <drivers/console/console.h>

namespace MYYOS
{
    namespace drivers
    {
        namespace console
        {

            Console::Console()
            {
            }
            
            Console::~Console()
            {
            }

            uint16_t Console::GetWidth()
            {
                return 1;
            }
            
            uint16_t Console::GetHeight()
            {
                return 1;
            }
            
            char Console::GetChar(uint16_t, uint16_t)
            {
                return ' ';
            }
            
            color::ConsoleColor Console::GetForegroundColor(uint16_t, uint16_t)
            {
                return color::LightGray;
            }
            
            color::ConsoleColor Console::GetBackgroundColor(uint16_t, uint16_t)
            {
                return color::Black;
            }
                        
            void Console::PutChar(uint16_t x, uint16_t y, char c, color::ConsoleColor foreground, color::ConsoleColor background)
            {
                SetBackgroundColor(x,y,background);
                SetForegroundColor(x,y,foreground);
                PutChar(x,y,c);
            }

            void Console::PutString(uint16_t x, uint16_t y, common::string s, color::ConsoleColor foreground, color::ConsoleColor background)
            {
                
                for(const char* si = s; x < GetWidth() && *si != '\0'; si++, x++)
                    PutChar(x,y,*si,foreground,background);
            }            
            
            void Console::ScrollUp()
            {
                ScrollUp(0, 0, GetWidth(), GetHeight());
            }
            
            void Console::ScrollUp(uint16_t left, uint16_t top, uint16_t width, uint16_t height, color::ConsoleColor foreground, color::ConsoleColor background, char fill)
            {
                for(uint16_t y = top; y < top+height-1; y++)
                    for(uint16_t x = left; x < left+width; x++)
                        PutChar(x,y,GetChar(x,y+1), GetForegroundColor(x,y+1), GetBackgroundColor(x,y+1));
                for(uint16_t x = left; x < left+width; x++)
                    PutChar(x, top+height-1, fill, foreground, background);
            }
            
            void Console::Clear()
            {
                Clear(0,0,GetWidth(), GetHeight());
            }
            
            void Console::Clear(uint16_t left, uint16_t top, uint16_t width, uint16_t height, color::ConsoleColor foreground, color::ConsoleColor background, char fill)
            {
                for(uint16_t y = top; y < top+height; y++)
                    for(uint16_t x = left; x < left+width; x++)
                        PutChar(x,y,fill,foreground,background);
            }
            
            void Console::InvertColors(uint16_t x, uint16_t y)
            {
                color::ConsoleColor temp = GetForegroundColor(x,y);
                SetForegroundColor(x,y,GetBackgroundColor(x,y));
                SetBackgroundColor(x,y,temp);
            }
                        
            void Console::PutChar(uint16_t, uint16_t, char)
            {
            }
            
            void Console::SetForegroundColor(uint16_t, uint16_t, color::ConsoleColor)
            {
            }
            
            void Console::SetBackgroundColor(uint16_t, uint16_t, color::ConsoleColor)
            {
            }


        } // namespace console
    } // namespace drivers
}   