#ifndef __COMMON__OUTPUTSTREAM_H
    #define __COMMON__OUTPUTSTREAM_H

    #include <common/vector.h>
    #include <common/string.h>
    #include <common/inputstream.h>

    namespace MYYOS
    {
        namespace common
        {

            template<class T> class BasicOutputStream : public InputStreamProcessor<T,T>
            {
                public:
                    BasicOutputStream();
                    
                    // FIXME: must be virtual (currently isnt because the kernel has no memory management yet)
                    
                    ~BasicOutputStream();
                    virtual string GetName();
                    void OnRead(T c);
                    void OnEndOfStream(BasicInputStream<T>* stream);

                    virtual void Write(T);
                    virtual void Close();
                    virtual BasicOutputStream<T>& operator<<(T);
            };



            class OutputStream : public BasicOutputStream<string>
            {
                public:
                    OutputStream();
                    ~OutputStream();
                    virtual string GetName();

                    virtual void Write(string s);
                    virtual void WriteChar(char c);
                    virtual void WriteInt(int i);
                    virtual void WriteIntHex(unsigned int i);

                    virtual void LineFeed();
                    virtual void CarriageReturn();
                    virtual void Clear();
                    
                    OutputStream& operator<<(int i);
                    OutputStream& operator<<(string s);
                    OutputStream& operator<<(char c);
            };



            template<class T> class NullStream : public BasicOutputStream<T>
            {
                public:
                    NullStream();
                    ~NullStream();
                    string GetName();
            };

        }
    }

    #include "../../src/common/outputstream_templates.cpp"

#endif