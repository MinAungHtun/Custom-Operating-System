#ifndef __DRIVERS__CONSOLE__CONSOLE_H
    #define __DRIVERS__CONSOLE__CONSOLE_H

    #include <common/types.h>
    #include<common/string.h>
    
    namespace MYYOS
    {
        namespace drivers
        {
            namespace console
            {

                namespace color
                {
                    enum ConsoleColor
                    {
                        Black        = 0x00,
                        Blue         = 0x01,
                        Green        = 0x02,
                        Cyan         = 0x03,
                        Red          = 0x04,
                        Magenta      = 0x05,
                        Brown        = 0x06,
                        LightGray    = 0x07,
                        DarkGray     = 0x08,
                        LightBlue    = 0x09,
                        LightGreen   = 0x0A,
                        LightCyan    = 0x0B,
                        LightRed     = 0x0C,
                        LightMagenta = 0x0D,
                        Yellow       = 0x0E,
                        White        = 0x0F
                    };
                }

                class Console
                {
                    public:
                        Console();
                        ~Console();

                        virtual uint16_t GetWidth();
                        virtual uint16_t GetHeight();
                        
                        virtual void PutChar(uint16_t x, uint16_t y, char c);
                        virtual void SetForegroundColor(uint16_t x, uint16_t y, color::ConsoleColor foreground);
                        virtual void SetBackgroundColor(uint16_t x, uint16_t y, color::ConsoleColor background);
                  
                        virtual char GetChar(uint16_t x, uint16_t y);
                        virtual color::ConsoleColor GetForegroundColor(uint16_t x, uint16_t y);
                        virtual color::ConsoleColor GetBackgroundColor(uint16_t x, uint16_t y);
                        
                        
                        virtual void PutChar(uint16_t x, uint16_t y, char c, color::ConsoleColor foreground, color::ConsoleColor background);
                        virtual void PutString(uint16_t x, uint16_t y, common::string s, color::ConsoleColor foreground = color::LightGray, color::ConsoleColor background = color::Black);
                        virtual void ScrollUp();
                        virtual void ScrollUp(uint16_t left, uint16_t top, uint16_t width, uint16_t height, color::ConsoleColor foreground = color::LightGray, color::ConsoleColor background = color::Black, char fill=' ');
                        virtual void Clear();
                        virtual void Clear(uint16_t left, uint16_t top, uint16_t width, uint16_t height, color::ConsoleColor foreground = color::LightGray, color::ConsoleColor background = color::Black, char fill=' ');
                        virtual void InvertColors(uint16_t x, uint16_t y);
                };
                
            } // namespace console
        } // namespace drivers
    }   


#endif