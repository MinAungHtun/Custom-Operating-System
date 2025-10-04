
#include<drivers/video/videographicsarray.h>

// thanks a lot to
// http://files.osdev.org/mirrors/geezer/osd/graphics/modes.c

namespace MYYOS
{
    namespace drivers
    {
        namespace video
        {

            VideoGraphicsArray::VideoGraphicsArray()
                : VideoDriver(),
                  miscellaneousPort(0x3c2),
                  cathodeRayTubeControllerIndexPort(0x3d4),
                  cathodeRayTubeControllerDataPort(0x3d5),
                  sequencerIndexPort(0x3c4),
                  sequencerDataPort(0x3c5),
                  graphicsControllerIndexPort(0x3ce),
                  graphicsControllerDataPort(0x3cf),
                  attributeControllerIndexPort(0x3c0),
                  attributeControllerWritePort(0x3c0),
                  attributeControllerReadPort(0x3c1),
                  attributeControllerResetPort(0x3da)
            {
            }

            VideoGraphicsArray::~VideoGraphicsArray()
            {
            }

            common::string VideoGraphicsArray::GetDeviceName()
            {
                return "VGA compatible graphics card";
            }

            bool VideoGraphicsArray::SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth)
            {
                return width == 320 && height == 200 && colordepth == 8;
            }

            void VideoGraphicsArray::WriteRegisters(uint8_t* registers)
            {
                // Miscellaneous
                miscellaneousPort.Write(*(registers++));

                // Sequencer
                for(uint8_t i = 0; i < 5; ++i)
                {
                    sequencerIndexPort.Write(i);
                    sequencerDataPort.Write(*(registers++));
                }

                // Cathode Ray Tube Controller
                // unlock
                cathodeRayTubeControllerIndexPort.Write(0x03);
                cathodeRayTubeControllerDataPort.Write(cathodeRayTubeControllerDataPort.Read() | 0x80);
                cathodeRayTubeControllerIndexPort.Write(0x11);
                cathodeRayTubeControllerDataPort.Write(cathodeRayTubeControllerDataPort.Read() & ~0x80);
                // make sure they remain unlocked
                registers[0x03] |= 0x80;
                registers[0x11] &= ~0x80;
                // write data
                for(uint8_t i = 0; i < 25; ++i)
                {
                    cathodeRayTubeControllerIndexPort.Write(i);
                    cathodeRayTubeControllerDataPort.Write(*(registers++));
                }

                // Graphics Controller
                for(uint8_t i = 0; i < 9; ++i)
                {
                    graphicsControllerIndexPort.Write(i);
                    graphicsControllerDataPort.Write(*(registers++));
                }

                // Attribute Controller
                for(uint8_t i = 0; i < 21; ++i)
                {
                    attributeControllerResetPort.Read();
                    attributeControllerIndexPort.Write(i);
                    attributeControllerWritePort.Write(*(registers++));
                }

                // lock 16-color palette and unblank display
                attributeControllerResetPort.Read();
                attributeControllerIndexPort.Write(0x20);
            }

            uint8_t* VideoGraphicsArray::GetFramebufferSegment()
            {
                graphicsControllerIndexPort.Write(6);
                uint8_t segment = graphicsControllerDataPort.Read() & (3<<2);
                switch(segment)
                {
                    default: // unnecessary (because of the &3, no other values are possible)
                    case 0<<2: return (uint8_t*)0x00000;
                    case 1<<2: return (uint8_t*)0xA0000;
                    case 2<<2: return (uint8_t*)0xB0000;
                    case 3<<2: return (uint8_t*)0xB8000;
                }
            }

            bool VideoGraphicsArray::DoSetMode(uint32_t width, uint32_t height, uint32_t colordepth)
            {
                if(!SupportsMode(width, height, colordepth))
                    return false;

                uint8_t g_320x200x256[] =
                {
                    // Miscellaneous
                        0x63,
                    // Sequencer
                        0x03, 0x01, 0x0F, 0x00, 0x0E,
                    // Cathode Ray Tube Controller
                        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
                        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
                        0xFF,
                    // Graphics Controller
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
                        0xFF,
                    // Attribute controller
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                        0x41, 0x00, 0x0F, 0x00, 0x00
                };
                WriteRegisters(g_320x200x256);
                return true;
            }

            void VideoGraphicsArray::DoPutPixel8Bit(uint32_t x, uint32_t y, uint8_t color)
            {
                uint8_t* pixelAddress = GetFramebufferSegment() + y*320 + x;
                *pixelAddress = color;
            }

            uint8_t VideoGraphicsArray::DoGetPixel8Bit(uint32_t x, uint32_t y)
            {
                uint8_t* pixelAddress = GetFramebufferSegment() + y*320 + x;
                return *pixelAddress;
            }

        }
    }
}
