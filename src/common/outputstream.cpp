
#include <common/outputstream.h>

namespace MYYOS
{
    namespace common
    {

        OutputStream::OutputStream()
            : BasicOutputStream<string>::BasicOutputStream()
        {
        }

        OutputStream::~OutputStream()
        {
        }

        string OutputStream::GetName()
        {
            return "Unknown OutputStream";
        }

        void OutputStream::WriteChar(char)
        {
        }

        void OutputStream::LineFeed()
        {
            WriteChar('\n');
        }

        void OutputStream::CarriageReturn()
        {
            WriteChar('\r');
        }

        void OutputStream::Clear()
        {
        }

        void OutputStream::Write(string s)
        {
            while (true)
            {
                switch(*s)
                {
                    case '\n':
                        LineFeed();
                        break;
                    case '\r':
                        CarriageReturn();
                        break;
                    case '\0':
                        return;
                    default:
                        WriteChar(*s);
                        break;
                }
                s++;
            }
        }

        void OutputStream::WriteInt(int i)
        {
            if(i == 0)
            {
                WriteChar('0');
            }
            else
            {
                int absi = i >= 0 ? i : -i;
                int length = 0;
                while(absi > 0)
                {
                    absi /= 10;
                    length++;
                }
                if(i < 0)
                    WriteChar('-');
                for(; length > 0; length--)
                {
                    int absi = i >= 0 ? i : -i;
                    for(int j = 1; j < length; j++)
                        absi /= 10;
                    WriteChar('0' + absi % 10);
                }
            }
        }

        void OutputStream::WriteIntHex(unsigned int i)
        {
            if(i == 0)
            {
                Write("0x0");
            }
            else
            {
                int length = 0;
                unsigned int ibak = i;
                while(ibak > 0)
                {
                    ibak /= 16;
                    length++;
                }
                Write("0x");
                for(; length > 0; length--)
                {
                    ibak = i;
                    for(int j = 1; j < length; j++)
                        ibak /= 16;
                    const char* Hex = "0123456789ABCDEF";
                    WriteChar(Hex[ibak % 16]);
                }
            }
        }

        OutputStream& OutputStream::operator<<(int i)
        {
            WriteInt(i);
            return *this;
        }

        OutputStream& OutputStream::operator<<(char c)
        {
            WriteChar(c);
            return *this;
        }
        
        OutputStream& OutputStream::operator<<(string s)
        {
            Write(s);
            return *this;
        }        
        
    }
}
