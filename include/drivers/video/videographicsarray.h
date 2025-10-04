#ifndef __DRIVERS__VIDEO__VIDEOGRAPHICSARRAY_H
    #define __DRIVERS__VIDEO__VIDEOGRAPHICSARRAY_H

    #include<hardwarecommunication/port.h>
    #include<drivers/video/video.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace video
            {

                class VideoGraphicsArray : public VideoDriver
                {
                    protected:
                        bool DoSetMode(uint32_t width, uint32_t height, uint32_t colordepth);
                        void DoPutPixel8Bit(uint32_t x, uint32_t y, uint8_t color);
                        uint8_t DoGetPixel8Bit(uint32_t x, uint32_t y);
                        void WriteRegisters(uint8_t* registers);
                        uint8_t* GetFramebufferSegment();

                        hardwarecommunication::Port8Bit miscellaneousPort;
                        hardwarecommunication::Port8Bit cathodeRayTubeControllerIndexPort;
                        hardwarecommunication::Port8Bit cathodeRayTubeControllerDataPort;
                        hardwarecommunication::Port8Bit sequencerIndexPort;
                        hardwarecommunication::Port8Bit sequencerDataPort;
                        hardwarecommunication::Port8Bit graphicsControllerIndexPort;
                        hardwarecommunication::Port8Bit graphicsControllerDataPort;
                        hardwarecommunication::Port8Bit attributeControllerIndexPort;
                        hardwarecommunication::Port8Bit attributeControllerWritePort;
                        hardwarecommunication::Port8Bit attributeControllerReadPort;
                        hardwarecommunication::Port8Bit attributeControllerResetPort;

                    public:
                        VideoGraphicsArray();
                        ~VideoGraphicsArray();

                        common::string GetDeviceName();
                        bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);

                        inline void* operator new(uint32_t, VideoGraphicsArray* p)
                        {
                            return p;
                        }

                };

            }
        }
    }

#endif

