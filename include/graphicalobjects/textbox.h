
#ifndef __GRAPHICALOBJECTS__TEXTBOX_H
    #define __GRAPHICALOBJECTS__TEXTBOX_H

    #include<common/font.h>
    #include<graphicalobjects/graphicalobject.h>

    namespace MYYOS
    {
        namespace graphicalobjects
        {


            class TextBox : public GraphicalObject
            {
                protected:
                    char fakeBuffer[256];
                public:
                    TextBox(int32_t left, int32_t top, uint32_t width, uint32_t height);
                    ~TextBox();

                    void Draw(common::GraphicsContext* context, common::Rectangle<int>& area);
                    void SetText(common::string text);
                    common::string GetText();
                    void OnFocus();
                    void OnFocusLost();
                    void OnKeyboardKeyPressed(drivers::keyboard::KeyCode keyCode, drivers::keyboard::KeyboardState state);

                    Color borderColor;
                    Font font;
                    uint32_t cursorPosition;
            };


        }
    }


#endif
