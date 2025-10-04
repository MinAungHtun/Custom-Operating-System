
#include<common/color.h>

namespace MYYOS
{
    namespace common
    {

        Color::Color()
        {
            Red = 0;
            Green = 0;
            Blue = 0;
            Alpha = 0;
        }

        Color::Color(uint8_t Red, uint8_t Green, uint8_t Blue)
        {
            this->Red = Red;
            this->Green = Green;
            this->Blue = Blue;
            this->Alpha = 0;
        }

        Color::Color(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha)
        {
            this->Red = Red;
            this->Green = Green;
            this->Blue = Blue;
            this->Alpha = Alpha;
        }

        Color::~Color()
        {
        }

    }
}
