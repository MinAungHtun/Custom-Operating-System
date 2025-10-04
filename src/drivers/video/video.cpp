
#include<drivers/video/video.h>

namespace MYYOS
{
    namespace drivers
    {
        namespace video
        {

            VideoDriver::VideoDriver()
                : Driver(),
                common::GraphicsContext()
            {
            }

            VideoDriver::~VideoDriver()
            {
            }

            bool VideoDriver::SetTextMode()
            {
                return false;
            }

            bool VideoDriver::DoSetMode(uint32_t, uint32_t, uint32_t)
            {
                return false;
            }

            bool VideoDriver::SupportsMode(uint32_t, uint32_t, uint32_t)
            {
                return false;
            }

            bool VideoDriver::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
            {
                if(!SupportsMode(width, height, colordepth))
                    return false;

                if(DoSetMode(width, height, colordepth))
                {
                    this->width = width;
                    this->height = height;
                    this->colordepth = colordepth;
                    return true;
                }
                return false;
            }


        }
    }
}
