
#ifndef __GRAPHICALOBJECTS__DESKTOP_H
    #define __GRAPHICALOBJECTS__DESKTOP_H

    #include<graphicalobjects/graphicalobject.h>
    #include<drivers/mouse/mouse.h>
    #include<drivers/clock/clock.h>


    namespace MYYOS
    {
        namespace graphicalobjects
        {


            class Desktop : public GraphicalObjectContainer,
                            public drivers::mouse::MouseEventHandler,
                            public drivers::clock::ClockEventHandler
            {
                protected:
                    uint32_t mousePositionX;
                    uint32_t mousePositionY;
                    common::GraphicsContext* context;
                    GraphicalObject* focussedObject;
                    drivers::mouse::MouseEventHandler* draggedObject;

                    void SetFocus(GraphicalObject* focussedObject);
                    void BringToFront(GraphicalObject* frontObject);
                    void InvertMouseCursor();

                    common::Vector<Rectangle<int> > invalidAreas;
                    void Invalidate(common::Rectangle<int>& area, common::Vector<common::Rectangle<int> >::iterator start, common::Vector<common::Rectangle<int> >::iterator stop);
                    void DrawSelf(common::GraphicsContext* context, common::Rectangle<int>& area);

                public:
                    Desktop(common::GraphicsContext* context);
                    ~Desktop();
                    void AddChild(GraphicalObject* child);

                    void OnMouseMoved(int8_t x, int8_t y);
                    void OnMouseButtonPressed(uint8_t button);
                    void OnMouseButtonReleased(uint8_t button);

                    void OnKeyboardKeyPressed(drivers::keyboard::KeyCode keyCode, drivers::keyboard::KeyboardState state);
                    void OnKeyboardKeyReleased(drivers::keyboard::KeyCode keyCode, drivers::keyboard::KeyboardState state);

                    common::Color color;
                    void Invalidate(common::Rectangle<int>& area);
                    void OnTime(const common::Time&);
            };


        }
    }


#endif


