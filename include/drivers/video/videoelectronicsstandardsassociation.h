#ifndef __DRIVERS__VIDEO__VIDEOELECTRONICSSTANDARDSASSOCIATION_H
    #define __DRIVERS__VIDEO__VIDEOELECTRONICSSTANDARDSASSOCIATION_H

    #include<drivers/video/video.h>
    #include<memorymanagement.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace video
            {

                class VideoElectronicsStandardsAssociationDriver : public VideoDriver
                {
                    protected:
                        bool DoSetMode(uint32_t width, uint32_t height, uint32_t colordepth);
                        void DoPutPixel32Bit(uint32_t x, uint32_t y, uint32_t color);
                        MemoryManager* memoryManager;
                        uint8_t* framebufferAddress;

                    public:
                        VideoElectronicsStandardsAssociationDriver(MemoryManager* memoryManager);
                        ~VideoElectronicsStandardsAssociationDriver();

                        common::string GetDeviceName();
                        bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);
                };

            }
        }
    }

#endif
