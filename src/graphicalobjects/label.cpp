
#include<graphicalobjects/label.h>

using namespace MYYOS::common;

namespace MYYOS
{
    namespace graphicalobjects
    {

        Label::Label(int32_t left, int32_t top, uint32_t width, uint32_t height, string text)
            : GraphicalObject(left, top, width, height),
            font()
        {
            SetText(text);
        }

        Label::~Label()
        {
        }

        void Label::Draw(common::GraphicsContext* context, common::Rectangle<int>& area)
        {
            GraphicalObject::Draw(context, area);

            Coordinates absoluteCoordinates = AbsoluteCoordinates(Coordinates(0,0));
            Rectangle<int> position = GetPosition();
            int32_t x = absoluteCoordinates.first;
            int32_t y = absoluteCoordinates.second;

            context->FillRectangle(x+area.left, y+area.top, x+area.left+area.width, y+area.top+area.height, font.backgroundColor);
            font.WriteText(x, y, context, &text[0], area);
        }

        void Label::SetText(string text)
        {
            for(uint32_t i = 0; ; i++)
            {
                this->text[i] = text[i];
                if(text[i] == '\0')
                    break;
            }
            Invalidate();
        }

    }
}

