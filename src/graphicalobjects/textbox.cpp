

#include<graphicalobjects/textbox.h>

using namespace MYYOS::common;
using namespace MYYOS::drivers::keyboard;

namespace MYYOS
{
    namespace graphicalobjects
    {

        TextBox::TextBox(int32_t left, int32_t top, uint32_t width, uint32_t height)
            : GraphicalObject(left, top, width, height),
            font()
        {
            borderColor = Color(0x00, 0x00, 0x00);
            cursorPosition = 0;
            fakeBuffer[0] = '\0';
        }

        TextBox::~TextBox()
        {
        }

        void TextBox::Draw(common::GraphicsContext* context, common::Rectangle<int>& area)
        {
            GraphicalObject::Draw(context, area);

            Coordinates absoluteCoordinates = AbsoluteCoordinates(Coordinates(0,0));
            Rectangle<int> position = GetPosition();
            int32_t x = absoluteCoordinates.first;
            int32_t y = absoluteCoordinates.second;

            context->FillRectangle(x+area.left, y+area.top, x+area.left+area.width, y+area.top+area.height, font.backgroundColor);

            if(area.Intersects(Rectangle<int>(0,0,position.width,1)))
                context->Line(x+area.left, y, x+area.left+area.width-1, y,borderColor);                                       // top
            if(area.Intersects(Rectangle<int>(0,0,1,position.height)))
                context->Line(x, y+area.top, x, y+area.top+area.height-1,borderColor);                                        // left
            if(area.Intersects(Rectangle<int>(0,position.height-1,position.width,1)))
                context->Line(x+area.left, y+position.height-1, x+area.left+area.width-1, y+position.height-1,borderColor);   // bottom
            if(area.Intersects(Rectangle<int>(position.width-1,0,1,position.height)))
                context->Line(x+position.width-1, y+area.top, x+position.width-1, y+area.top+area.height-1,borderColor);      // right

            common::Rectangle<int> textArea(area.left-1, area.top-1, area.width, area.height);
            font.WriteText(x+1, y+1, context, &fakeBuffer[0], textArea);
        }

        void TextBox::SetText(string text)
        {
            cursorPosition = 0;
            for(char* c = (char*)text, *buffer = &fakeBuffer[0]; *c != '\0'; ++c, buffer++)
            {
                cursorPosition++;
                *buffer = *c;
            }
            fakeBuffer[cursorPosition] = '\0';
            Invalidate();
        }

        string TextBox::GetText()
        {
            return &fakeBuffer[0];
        }

        void TextBox::OnFocus()
        {
            borderColor = Color(255,0,0);
        }

        void TextBox::OnFocusLost()
        {
            borderColor = Color(0,0,0);
        }

        void TextBox::OnKeyboardKeyPressed(KeyCode keyCode, KeyboardState)
        {
            switch(keyCode)
            {
                case KeyCode::Backspace:
                {
                    if(cursorPosition == 0)
                        break;
                    cursorPosition--;
                    // no break - we move the cursor to the left and use the <Delete> code
                }
                case KeyCode::Delete:
                {
                    for(uint32_t i = cursorPosition; fakeBuffer[i] != '\0'; ++i)
                        fakeBuffer[i] = fakeBuffer[i+1];
                    break;
                }
                case KeyCode::ArrowLeft:
                {
                    if(cursorPosition > 0)
                        cursorPosition--;
                    break;
                }
                case KeyCode::ArrowRight:
                {
                    if(fakeBuffer[cursorPosition] != '\0')
                        cursorPosition++;
                    break;
                }
                default:
                {
                    if(31 < keyCode && keyCode < 127)
                    {
                        uint32_t length = cursorPosition;
                        for(; fakeBuffer[length] != '\0'; ++length);
                        for(; length > cursorPosition; --length)
                            fakeBuffer[length+1] = fakeBuffer[length];
                        fakeBuffer[cursorPosition+1] = fakeBuffer[cursorPosition];
                        fakeBuffer[cursorPosition++] = (uint8_t)keyCode;
                    }
                    break;
                }
            }
            Invalidate();
        }

    }
}

