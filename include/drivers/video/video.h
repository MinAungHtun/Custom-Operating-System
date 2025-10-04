#ifndef __DRIVERS__VIDEO__VIDEO_H
    #define __DRIVERS__VIDEO__VIDEO_H

    #include<common/graphicscontext.h>
    #include<common/color.h>
    #include<drivers/driver.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace video
            {

                class VideoDriver : public Driver, public common::GraphicsContext
                {
                    protected:
                        virtual bool DoSetMode(uint32_t width, uint32_t height, uint32_t colordepth);

                    public:
                        VideoDriver();
                        ~VideoDriver();

                        virtual bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);
                        bool SetMode(uint32_t width, uint32_t height, uint32_t colordepth);
                        virtual bool SetTextMode();
                };

            }
        }
    }

#endif