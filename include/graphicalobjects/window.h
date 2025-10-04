
#ifndef __GRAPHICALOBJECTS__WINDOW_H
    #define __GRAPHICALOBJECTS__WINDOW_H

    #include<common/string.h>
    #include<common/font.h>
    #include<graphicalobjects/graphicalobject.h>
    #include<graphicalobjects/label.h>

    namespace MYYOS
    {
        namespace graphicalobjects
        {


            class Window : public GraphicalObjectContainer
            {
                protected:
                    GraphicalObjectMover graphicalObjectMover;
                    GraphicalObjectResizerTopLeft graphicalObjectResizerTopLeft;
                    GraphicalObjectResizerTop graphicalObjectResizerTop;
                    GraphicalObjectResizerTopRight graphicalObjectResizerTopRight;
                    GraphicalObjectResizerLeft graphicalObjectResizerLeft;
                    GraphicalObjectResizerRight graphicalObjectResizerRight;
                    GraphicalObjectResizerBottomLeft graphicalObjectResizerBottomLeft;
                    GraphicalObjectResizerBottom graphicalObjectResizerBottom;
                    GraphicalObjectResizerBottomRight graphicalObjectResizerBottomRight;

                    Label title;
                public:
                    Window(int32_t left, int32_t top, uint32_t width, uint32_t height, common::string text);
                    Window(GraphicalObject* containedObject, common::string text);
                    ~Window();

                    void DrawSelf(common::GraphicsContext* context, common::Rectangle<int>& area);
                    void AddChild(GraphicalObject* child);
                    drivers::mouse::MouseEventHandler* OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button);

                    uint8_t frameWidth;
                    uint8_t titleWidth;

                    common::Color color;
                    common::Color frameColor;
                    common::Color frameBorderColor;
            };


        }
    }


#endif
