
#ifndef __COMMON__FONT_H
    #define __COMMON__FONT_H

    #include<common/string.h>
    #include<common/rectangle.h>
    #include<common/graphicscontext.h>

    namespace MYYOS
    {
        namespace common
        {

            class Font
            {
                public:
                    Font();
                    ~Font();

                    bool bold;
                    bool italic;
                    uint16_t size;
                    common::Color color;
                    common::Color backgroundColor;

                    virtual void WriteText(int32_t x, int32_t y, GraphicsContext* context, string text);
                    virtual void WriteText(int32_t x, int32_t y, GraphicsContext* context, string text, Rectangle<int> limitArea);
                    virtual uint32_t TextHeight(string text);
                    virtual uint32_t TextWidth(string text);
            };

        }
    }

#endif