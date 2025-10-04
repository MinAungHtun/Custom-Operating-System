
#include<common/graphicscontext.h>

using namespace MYYOS::common;

namespace MYYOS
{
    namespace common
    {

        GraphicsContext::GraphicsContext()
        {
            width = 0;
            height = 0;
            colordepth = 0;
            MirrorYAxis = false; // set to true in derived classes for graphics contexts

            // Palette of VirtualBox VGA
            
            Palette[0x00] = Color(0x00,0x00,0x00);
            Palette[0x01] = Color(0x00,0x00,0xA8);
            Palette[0x02] = Color(0x00,0xA8,0x00);
            Palette[0x03] = Color(0x00,0xA8,0xA8);
            Palette[0x04] = Color(0xA8,0x00,0x00);
            Palette[0x05] = Color(0xA8,0x00,0xA8);
            Palette[0x06] = Color(0xA8,0xA8,0x00);
            Palette[0x07] = Color(0xA8,0xA8,0xA8);
            Palette[0x08] = Color(0x00,0x00,0x57);
            Palette[0x09] = Color(0x00,0x00,0xFF);
            Palette[0x0A] = Color(0x00,0xA8,0x57);
            Palette[0x0B] = Color(0x00,0xA8,0xFF);
            Palette[0x0C] = Color(0xA8,0x00,0x57);
            Palette[0x0D] = Color(0xA8,0x00,0x57);
            Palette[0x0E] = Color(0xA8,0xA8,0x57);
            Palette[0x0F] = Color(0xA8,0xA8,0xFF);

            Palette[0x10] = Color(0x00,0x57,0x00);
            Palette[0x11] = Color(0x00,0x57,0xA8);
            Palette[0x12] = Color(0x00,0xFF,0x00);
            Palette[0x13] = Color(0x00,0xFF,0xA8);
            Palette[0x14] = Color(0xA8,0x57,0x00);
            Palette[0x15] = Color(0xA8,0x57,0xA8);
            Palette[0x16] = Color(0xA8,0xFF,0x00);
            Palette[0x17] = Color(0xA8,0xFF,0xA8);
            Palette[0x18] = Color(0x00,0x57,0x57);
            Palette[0x19] = Color(0x00,0x57,0xFF);
            Palette[0x1A] = Color(0x00,0xFF,0x57);
            Palette[0x1B] = Color(0x00,0xFF,0xFF);
            Palette[0x1C] = Color(0xA8,0x57,0x57);
            Palette[0x1D] = Color(0xA8,0x57,0xFF);
            Palette[0x1E] = Color(0xA8,0xFF,0x57);
            Palette[0x1F] = Color(0xA8,0xFF,0xFF);

            Palette[0x20] = Color(0x57,0x00,0x00);
            Palette[0x21] = Color(0x57,0x00,0xA8);
            Palette[0x22] = Color(0x57,0xA8,0x00);
            Palette[0x23] = Color(0x57,0xA8,0xA8);
            Palette[0x24] = Color(0xFF,0x00,0x00);
            Palette[0x25] = Color(0xFF,0x00,0xA8);
            Palette[0x26] = Color(0xFF,0xA8,0x00);
            Palette[0x27] = Color(0xFF,0xA8,0xA8);
            Palette[0x28] = Color(0x57,0x00,0x57);
            Palette[0x29] = Color(0x57,0x00,0xFF);
            Palette[0x2A] = Color(0x57,0xA8,0x57);
            Palette[0x2B] = Color(0x57,0xA8,0xFF);
            Palette[0x2C] = Color(0xFF,0x00,0x57);
            Palette[0x2D] = Color(0xFF,0x00,0xFF);
            Palette[0x2E] = Color(0xFF,0xA8,0x57);
            Palette[0x2F] = Color(0xFF,0xA8,0xFF);

            Palette[0x30] = Color(0x57,0x57,0x00);
            Palette[0x31] = Color(0x57,0x57,0xA8);
            Palette[0x32] = Color(0x57,0xFF,0x00);
            Palette[0x33] = Color(0x57,0xFF,0xA8);
            Palette[0x34] = Color(0xFF,0x57,0x00);
            Palette[0x35] = Color(0xFF,0x57,0xA8);
            Palette[0x36] = Color(0xFF,0xFF,0x00);
            Palette[0x37] = Color(0xFF,0xFF,0xA8);
            Palette[0x38] = Color(0x57,0x57,0x57);
            Palette[0x39] = Color(0x57,0x57,0xFF);
            Palette[0x3A] = Color(0x57,0xFF,0x57);
            Palette[0x3B] = Color(0x57,0xFF,0xFF);
            Palette[0x3C] = Color(0xFF,0x57,0x57);
            Palette[0x3D] = Color(0xFF,0x57,0xFF);
            Palette[0x3E] = Color(0xFF,0xFF,0x57);
            Palette[0x3F] = Color(0xFF,0xFF,0xFF);

            for(uint8_t colorcode = 255; colorcode >= 0x40; --colorcode)
                Palette[colorcode] = Color(0,0,0);
        }

        GraphicsContext::~GraphicsContext()
        {
        }

        void GraphicsContext::DoPutPixel8Bit(uint32_t, uint32_t, uint8_t)
        {
        }

        void GraphicsContext::DoPutPixel16Bit(uint32_t, uint32_t, uint16_t)
        {
        }

        void GraphicsContext::DoPutPixel24Bit(uint32_t, uint32_t, uint32_t)
        {
        }

        void GraphicsContext::DoPutPixel32Bit(uint32_t, uint32_t, uint32_t)
        {
        }

        void GraphicsContext::DoPutPixel(uint32_t x, uint32_t y, uint32_t color)
        {
            switch(colordepth)
            {
                case 8:
                    DoPutPixel8Bit(x,y,color);
                    break;
                case 16:
                    DoPutPixel16Bit(x,y,color);
                    break;
                case 24:
                    DoPutPixel24Bit(x,y,color);
                    break;
                case 32:
                    DoPutPixel32Bit(x,y,color);
                    break;
            }
        }

        uint32_t GraphicsContext::TranslateColor(Color color)
        {
            switch(colordepth)
            {
                case 8:
                {
                    uint32_t result = 0;
                    int mindistance = 0xfffffff;
                    for(uint32_t i = 0; i <= 255; ++i)
                    {
                        Color* c = &Palette[i];
                        int distance =
                            ((int)color.Red-(int)c->Red)*((int)color.Red-(int)c->Red)
                            +((int)color.Green-(int)c->Green)*((int)color.Green-(int)c->Green)
                            +((int)color.Blue-(int)c->Blue)*((int)color.Blue-(int)c->Blue);
                        if(distance < mindistance)
                        {
                            mindistance = distance;
                            result = i;
                        }
                    }
                    return result;
                }
                case 16:
                {
                    // 16-Bit Colors RRRRRGGGGGGBBBBB
                    return ((uint16_t)(color.Red   & 0xF8)) << 8
                         | ((uint16_t)(color.Green & 0xFC)) << 3
                         | ((uint16_t)(color.Blue  & 0xF8) >> 3);
                }
                case 24:
                {
                    return (uint32_t)color.Red   << 16
                         | (uint32_t)color.Green << 8
                         | (uint32_t)color.Blue;
                }
                default:
                case 32:
                {
                    return (uint32_t)color.Red   << 24
                         | (uint32_t)color.Green << 16
                         | (uint32_t)color.Blue  <<  8
                         | (uint32_t)color.Alpha;
                }
            }
        }

        uint8_t GraphicsContext::DoGetPixel8Bit(uint32_t, uint32_t)
        {
            return 0;
        }

        uint16_t GraphicsContext::DoGetPixel16Bit(uint32_t, uint32_t)
        {
            return 0;
        }

        uint32_t GraphicsContext::DoGetPixel24Bit(uint32_t, uint32_t)
        {
            return 0;
        }

        uint32_t GraphicsContext::DoGetPixel32Bit(uint32_t, uint32_t)
        {
            return 0;
        }

        uint32_t GraphicsContext::DoGetPixel(uint32_t x, uint32_t y)
        {
            switch(colordepth)
            {
                case 8:
                    return DoGetPixel8Bit(x,y);
                case 16:
                    return DoGetPixel16Bit(x,y);
                case 24:
                    return DoGetPixel24Bit(x,y);
                default:
                case 32:
                    return DoGetPixel32Bit(x,y);
            }
        }

        Color GraphicsContext::TranslateColorBack(uint32_t color)
        {
            switch(colordepth)
            {
                case 8:
                {
                    return Palette[color & 0xff];
                }
                case 16:
                {
                    // 16-Bit Colors RRRRRGGGGGGBBBBB
                    return Color( (color & 0xF800) >> 8,
                                  (color & 0x07E0) >> 3,
                                  (color & 0x001F) << 3);
                }
                case 24:
                {
                    return Color((color & 0xFF0000) >> 16,
                                 (color & 0x00FF00) >> 8,
                                 (color & 0x0000FF));
                }
                default:
                case 32:
                {
                    return Color((color & 0xFF000000) >> 24,
                                 (color & 0x00FF0000) >> 16,
                                 (color & 0x0000FF00) >> 8,
                                 (color & 0x000000FF));
                }
            }
        }

        uint32_t GraphicsContext::GetWidth()
        {
            return width;
        }

        uint32_t GraphicsContext::GetHeight()
        {
            return height;
        }

        // =====================================================================
        // =====================================================================

        void GraphicsContext::PutPixel(int32_t x, int32_t y, uint32_t color)
        {
            if(0 <= x && (uint32_t)x < width && 0 <= y && (uint32_t)y < height)
                DoPutPixel(x, MirrorYAxis ? height-y-1 : y, color);
        }
        void GraphicsContext::PutPixel(int32_t x, int32_t y, Color color)
        {
            PutPixel(x,y,TranslateColor(color));
        }

        Color GraphicsContext::GetPixel(int32_t x, int32_t y)
        {
            if(0 <= x && (uint32_t)x < width && 0 <= y && (uint32_t)y < height)
            {
                uint32_t translatedColor = DoGetPixel(x, MirrorYAxis ? height-y-1 : y);
                return TranslateColorBack(translatedColor);
            }
            else
                return Color(0,0,0);
        }

        void GraphicsContext::InvertPixel(int32_t x, int32_t y)
        {
            Color temp = GetPixel(x,y);
            PutPixel(x,y,Color(255-temp.Red, 255-temp.Green, 255-temp.Blue));
        }


        void GraphicsContext::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
        {
            int32_t ymin = y1 < y2 ? y1 : y2;
            int32_t ymax = y1 < y2 ? y2 : y1;

            if(x2 < x1)
                Line(x2,y2,x1,y1,color);
            else if(x1 == x2)
            {
                if(ymin < 0) ymin = 0;
                if((uint32_t)ymax >= height) ymax = height-1;

                if(MirrorYAxis)
                {
                    int32_t temp = ymax; // mirror y-axis (because we call DoPutPixel directly)
                    ymax = height - ymin - 1;
                    ymin = height - temp - 1;
                }

                if(0 <= x1 && (uint32_t)x1 < width)
                    for(int32_t y = ymin; y <= ymax; ++y)
                        DoPutPixel(x1,y,color);
            }
            else if(y1 == y2)
            {
                if(x1 < 0) x1 = 0;
                if((uint32_t)x2 >= width) x2 = width-1;

                if(MirrorYAxis)
                    y1 = height-y1-1;

                if(0 <= y1 && (uint32_t)y1 < height)
                    for(int32_t x = x1; x <= x2; ++x)
                        DoPutPixel(x,y1,color);
            }
            else
            {
                float slope = ((float)(y2-y1))/(x2-x1);
                if(-1 <= slope && slope <= 1)
                {
                    float y = y1;
                    for(int32_t x = x1; x <= x2; x++, y+=slope)
                        PutPixel(x, (int32_t)y, color);
                }
                else
                {
                    slope = 1.0f/slope;
                    float x = x1;
                    for(int32_t y = ymin; y <= ymax; x+=slope, y++)
                        PutPixel((int32_t)x, y, color);
                }
            }
        }
        void GraphicsContext::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color)
        {
            Line(x1,y1,x2,y2,TranslateColor(color));
        }



        void GraphicsContext::Rectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
        {
            --y2;
            --x2;
            Line(x1,y1,x2,y1,color); // top
            Line(x1,y1,x1,y2,color); // left
            Line(x1,y2,x2,y2,color); // bottom
            Line(x2,y1,x2,y2,color); // right
        }
        void GraphicsContext::Rectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color)
        {
            Rectangle(x1,y1,x2,y2,TranslateColor(color));
        }



        void GraphicsContext::FillRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
        {
            if(y2 < y1)
                FillRectangle(x2,y2,x1,y1,color);
            else
            {
                if(y1 < 0) y1 = 0;
                if((uint32_t)y2 > height) y2 = height;

                int32_t xmin = x1 < x2 ? x1 : x2;
                int32_t xmax = x1 < x2 ? x2 : x1;
                if(xmin < 0) xmin = 0;
                if((uint32_t)xmax > width) xmax = width;

                if(MirrorYAxis)
                {
                    uint32_t temp = y2; // mirror y-axis (because we call DoPutPixel directly)
                    y2 = height - y1 - 1;
                    y1 = height - temp - 1;
                }

                for(int32_t y = y1; y < y2; ++y)
                    for(int32_t x = xmin; x < xmax; ++x)
                        DoPutPixel(x,y,color);
            }
        }
        void GraphicsContext::FillRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color)
        {
            FillRectangle(x1,y1,x2,y2,TranslateColor(color));
        }



    }
}
