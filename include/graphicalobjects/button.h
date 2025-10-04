
#ifndef __GRAPHICALOBJECTS__BUTTON_H
    #define __GRAPHICALOBJECTS__BUTTON_H

    #include<common/string.h>
    #include<common/font.h>
    #include<graphicalobjects/graphicalobject.h>

    namespace MYYOS
    {
        namespace graphicalobjects
        {

            class Button;

            class ButtonEventHandler
            {
                public:
                    ButtonEventHandler();
                    ~ButtonEventHandler();

                    virtual void OnClick(Button* sender);
            };

            class Button : public GraphicalObject
            {
                protected:
                    common::Vector<ButtonEventHandler*> handlers;
                public:
                    Button(int32_t left, int32_t top, uint32_t width, uint32_t height, string text);
                    ~Button();

                    void Draw(common::GraphicsContext* context, common::Rectangle<int>& area);
                    void ConnectEventHandler(ButtonEventHandler* handler);
                    void DisconnectEventHandler(ButtonEventHandler* handler);
                    drivers::mouse::MouseEventHandler* OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button);

                    common::Color color;
                    common::Color borderColor;
                    common::Font font;
                    common::string text;
            };


        }
    }


#endif
