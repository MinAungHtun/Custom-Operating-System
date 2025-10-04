
#ifndef __GRAPHICALOBJECTS__GRAPHICALOBJECT_H
    #define __GRAPHICALOBJECTS__GRAPHICALOBJECT_H

    #include<common/types.h>
    #include<common/rectangle.h>
    #include<common/vector.h>
    #include<common/coordinates.h>
    #include<common/graphicscontext.h>
    #include<drivers/keyboard/keyboardinterpreter.h>
    #include<drivers/mouse/mouse.h>


    namespace MYYOS
    {
        namespace graphicalobjects
        {


            class GraphicalObject : public drivers::keyboard::KeyboardEventHandler
            {
                template<int L, int T, int W, int H> friend class GraphicalObjectMoverResizer;
                friend class GraphicalObjectContainer;
                private:
                    common::Rectangle<int> position;
                protected:
                    bool valid;
                    GraphicalObject* parent;
                    virtual void SetFocus(GraphicalObject* focussedObject);
                    virtual void BringToFront(GraphicalObject* frontObject);
                    uint32_t minimumWidth;
                    uint32_t maximumWidth;
                    uint32_t minimumHeight;
                    uint32_t maximumHeight;

                public:

                    GraphicalObject();
                    GraphicalObject(int32_t left, int32_t top, uint32_t width, uint32_t height);
                    ~GraphicalObject();
                    virtual common::Coordinates AbsoluteCoordinates(common::Coordinates coordinates);
                    virtual void Draw(common::GraphicsContext* context, common::Rectangle<int>& area);
                    void Invalidate();
                    virtual void Invalidate(common::Rectangle<int>& area);
                    virtual void AddChild(GraphicalObject* child);
                    common::Rectangle<int> GetPosition();
                    void Move(int left, int top);
                    void Resize(int width, int height);
                    void Focus();
                    void BringToFront();

                    virtual bool ContainsCoordinate(uint32_t x, uint32_t y);

                    virtual void OnMouseEnter(uint32_t toX, uint32_t toY);
                    virtual void OnMouseLeave(uint32_t fromX, uint32_t fromY);
                    virtual void OnMouseMoved(uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY);
                    virtual void OnFocus();
                    virtual void OnFocusLost();
                    virtual drivers::mouse::MouseEventHandler* OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button);
                    virtual void OnMouseButtonReleased(uint32_t X, uint32_t Y, uint8_t button);
            };

            class GraphicalObjectContainer : public GraphicalObject
            {
                protected:
                    common::Vector<GraphicalObject*> Children;
                    void Draw(GraphicsContext* context, Rectangle<int>& area, Vector<GraphicalObject*>::iterator start);
                    virtual void DrawSelf(GraphicsContext* context, Rectangle<int>& area);

                public:
                    GraphicalObjectContainer();
                    GraphicalObjectContainer(int32_t left, int32_t top, uint32_t width, uint32_t height);
                    ~GraphicalObjectContainer();
                    void Draw(common::GraphicsContext* context, common::Rectangle<int>& area);
                    void AddChild(GraphicalObject* child);

                    void OnMouseEnter(uint32_t toX, uint32_t toY);
                    void OnMouseLeave(uint32_t fromX, uint32_t fromY);
                    void OnMouseMoved(uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY);
                    drivers::mouse::MouseEventHandler* OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button);
                    void OnMouseButtonReleased(uint32_t X, uint32_t Y, uint8_t button);
            };


            template<int L, int T, int W, int H> class GraphicalObjectMoverResizer : public drivers::mouse::MouseEventHandler
            {
                protected:
                    GraphicalObject* target;
                public:
                    GraphicalObjectMoverResizer(GraphicalObject* target);
                    ~GraphicalObjectMoverResizer();
                    void OnMouseMoved(int8_t x, int8_t y);
            };
            typedef GraphicalObjectMoverResizer<1, 1, 0, 0> GraphicalObjectMover;
            typedef GraphicalObjectMoverResizer<1, 1,-1,-1> GraphicalObjectResizerTopLeft;
            typedef GraphicalObjectMoverResizer<0, 1, 0,-1> GraphicalObjectResizerTop;
            typedef GraphicalObjectMoverResizer<0, 1, 1,-1> GraphicalObjectResizerTopRight;
            typedef GraphicalObjectMoverResizer<1, 0,-1, 0> GraphicalObjectResizerLeft;
            typedef GraphicalObjectMoverResizer<0, 0, 1, 0> GraphicalObjectResizerRight;
            typedef GraphicalObjectMoverResizer<1, 0,-1, 1> GraphicalObjectResizerBottomLeft;
            typedef GraphicalObjectMoverResizer<0, 0, 0, 1> GraphicalObjectResizerBottom;
            typedef GraphicalObjectMoverResizer<0, 0, 1, 1> GraphicalObjectResizerBottomRight;




        }
    }

    #include "../../src/graphicalobjects/graphicalobject_templates.cpp"

#endif

