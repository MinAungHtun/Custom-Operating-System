
#include <drivers/console/textmodeconsole.h>

using namespace MYYOS::common;
using namespace MYYOS::drivers;

namespace MYYOS
{
    namespace drivers
    {
        namespace console
        {
            
            TextmodeConsole::TextmodeConsole()
                : Driver(),
                Console(),
                videoMemory((uint16_t*) 0xb8000)
            {
            }

            TextmodeConsole::~TextmodeConsole()
            {
            }

            string TextmodeConsole::GetDeviceName()
            {
                return "Console";
            }


            uint16_t TextmodeConsole::GetWidth()
            {
                return 80;
            }

            uint16_t TextmodeConsole::GetHeight()
            {
                return 25;
            }

            void TextmodeConsole::PutChar(uint16_t x, uint16_t y, char c)
            {
                if(x >= 80 || y >= 25)
                    return;
                int offset = (y*80 + x);
                videoMemory[offset] = (videoMemory[offset] & 0xFF00) | (uint16_t)c;
            }
            
            void TextmodeConsole::SetForegroundColor(uint16_t x, uint16_t y, color::ConsoleColor foreground)
            {
                if(x >= 80 || y >= 25)
                    return;
                int offset = (y*80 + x);
                videoMemory[offset] = (videoMemory[offset] & 0xF0FF) | ((uint16_t)foreground << 8);
            }
            
            void TextmodeConsole::SetBackgroundColor(uint16_t x, uint16_t y, color::ConsoleColor background)
            {
                if(x >= 80 || y >= 25)
                    return;
                int offset = (y*80 + x);
                videoMemory[offset] = (videoMemory[offset] & 0x0FFF) | ((uint16_t)background << 12);
            }
            

            char TextmodeConsole::GetChar(uint16_t x, uint16_t y)
            {
                if(x >= 80 || y >= 25)
                    return ' ';
                int offset = (y*80 + x);
                return (char)(videoMemory[offset] & 0x00FF);
            }
            
            color::ConsoleColor TextmodeConsole::GetForegroundColor(uint16_t x, uint16_t y)
            {
                if(x >= 80 || y >= 25)
                    return color::LightGray;
                int offset = (y*80 + x);
                return (color::ConsoleColor)((videoMemory[offset] & 0x0F00) >> 8);
            }
            
            color::ConsoleColor TextmodeConsole::GetBackgroundColor(uint16_t x, uint16_t y)
            {
                if(x >= 80 || y >= 25)
                    return color::Black;
                int offset = (y*80 + x);
                return (color::ConsoleColor)((videoMemory[offset] & 0xF000) >> 12);
            }            
            
        } // namespace console
    } // namespace drivers
}   