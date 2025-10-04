
#include<drivers/video/videoelectronicsstandardsassociation.h>

namespace MYYOS
{
    namespace drivers
    {
        namespace video
        {

            VideoElectronicsStandardsAssociationDriver::VideoElectronicsStandardsAssociationDriver(MemoryManager* memoryManager)
                : VideoDriver()
            {
                framebufferAddress = 0;
                this->memoryManager = memoryManager;
            }

            VideoElectronicsStandardsAssociationDriver::~VideoElectronicsStandardsAssociationDriver()
            {
            }

            common::string VideoElectronicsStandardsAssociationDriver::GetDeviceName()
            {
                return "VESA compatible graphics card";
            }


            bool VideoElectronicsStandardsAssociationDriver::SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth)
            {
                if(width == 640 && height == 480 && colordepth == 16) return true;
                if(width == 800 && height == 600 && colordepth == 16) return true;
                if(width == 1024 && height == 768 && colordepth == 32) return true;
                return false;
            }

            bool VideoElectronicsStandardsAssociationDriver::DoSetMode(uint32_t width, uint32_t height, uint32_t colordepth)
            {
                framebufferAddress = memoryManager->AllocateMemory(width*height*(colordepth/8));
                return true;
            }

            void VideoElectronicsStandardsAssociationDriver::DoPutPixel32Bit(uint32_t x, uint32_t y, uint32_t color)
            {
                if(framebufferAddress == 0)
                    return;

                uint32_t* pixelAddress = (uint32_t*)(framebufferAddress
                                                     + y*width*(colordepth/8)
                                                     + x*(colordepth/8));
                *pixelAddress = color;
            }


        }
    }
}
