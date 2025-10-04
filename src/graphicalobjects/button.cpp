
#include<graphicalobjects/button.h>

using namespace MYYOS::common;

namespace MYYOS
{
    namespace graphicalobjects
    {

        ButtonEventHandler::ButtonEventHandler()
        {
        }

        ButtonEventHandler::~ButtonEventHandler()
        {
        }

        void ButtonEventHandler::OnClick(Button*)
        {
        }





        Button::Button(int32_t left, int32_t top, uint32_t width, uint32_t height, string text)
            : GraphicalObject(left, top, width, height),
            font()
        {
            this->text = text;
            color = Color(0xff,0xff,0xff);
            borderColor = Color(0x57, 0x57, 0x57);
            font.backgroundColor = color;
        }

        Button::~Button()
        {
        }

        drivers::mouse::MouseEventHandler* Button::OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button)
        {
            for(Vector<ButtonEventHandler*>::iterator i = handlers.begin(); i != handlers.end(); i++)
                (*i)->OnClick(this);

            return GraphicalObject::OnMouseButtonPressed(X, Y, button);
        }

        void Button::Draw(common::GraphicsContext* context, Rectangle<int>& area)
        {
            GraphicalObject::Draw(context, area);

            Coordinates absoluteCoordinates = AbsoluteCoordinates(Coordinates(0,0));
            int32_t x = absoluteCoordinates.first;
            int32_t y = absoluteCoordinates.second;
            Rectangle<int> position = GetPosition();

            context->FillRectangle(x+area.left, y+area.top, x+area.left+area.width, y+area.top+area.height, color);
            if(area.Intersects(Rectangle<int>(0,0,position.width,1)))
                context->Line(x+area.left, y, x+area.left+area.width-1, y,borderColor);                                       // top
            if(area.Intersects(Rectangle<int>(0,0,1,position.height)))
                context->Line(x, y+area.top, x, y+area.top+area.height-1,borderColor);                                        // left
            if(area.Intersects(Rectangle<int>(0,position.height-1,position.width,1)))
                context->Line(x+area.left, y+position.height-1, x+area.left+area.width-1, y+position.height-1,borderColor);   // bottom
            if(area.Intersects(Rectangle<int>(position.width-1,0,1,position.height)))
                context->Line(x+position.width-1, y+area.top, x+position.width-1, y+area.top+area.height-1,borderColor);      // right

            common::Rectangle<int> textArea(area.left-1, area.top-1, area.width, area.height);
            font.WriteText(x+1,y+1,context,text,textArea);
        }

        void Button::ConnectEventHandler(ButtonEventHandler* handler)
        {
            handlers.push_back(handler);
        }

        void Button::DisconnectEventHandler(ButtonEventHandler* handler)
        {
            handlers.erase(handler);
        }


    }
}
