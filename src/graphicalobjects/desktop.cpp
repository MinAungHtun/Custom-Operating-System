
#include<graphicalobjects/desktop.h>

using namespace MYYOS::drivers::keyboard;

namespace MYYOS
{
    namespace graphicalobjects
    {

        Desktop::Desktop(GraphicsContext* context)
            : GraphicalObjectContainer(0,0,context->GetWidth(),context->GetHeight()),
            MouseEventHandler(),
            ClockEventHandler()
        {
            focussedObject = 0;
            draggedObject = 0;
            this->context = context;
            mousePositionX = context->GetWidth()/2;
            mousePositionY = context->GetHeight()/2;
            color = common::Color(0x00,0x57,0xff);

            InvertMouseCursor(); // draw an initial mouse cursor
            GraphicalObject::Invalidate(); // without this, the desktop would not be drawn at startup
        }

        Desktop::~Desktop()
        {
        }


        void Desktop::Invalidate(common::Rectangle<int>& area, Vector<Rectangle<int> >::iterator start, Vector<Rectangle<int> >::iterator stop)
        {
            for(Vector<Rectangle<int> >::iterator invalid = start; invalid != stop; invalid++)
                if(area.Intersects(*invalid))
                {
                    Vector<Rectangle<int> > Parts = area.Subtract(*invalid);
                    for(Vector<Rectangle<int> >::iterator p = Parts.begin(); p != Parts.end(); p++)
                        Invalidate(*p, invalid+1, stop);
                    return;
                }

            Vector<Rectangle<int> >::iterator insertposition = invalidAreas.push_back(area);
            // if the invalidation-buffer is full, redraw the whole screen
            if(insertposition == invalidAreas.end())
            {
                invalidAreas.clear();
                GraphicalObject::Invalidate();
            }
        }

        void Desktop::Invalidate(common::Rectangle<int>& area)
        {
            Invalidate(area, invalidAreas.begin(), invalidAreas.end());
        }


        void Desktop::DrawSelf(common::GraphicsContext* context, common::Rectangle<int>& area)
        {
            context->FillRectangle(area.left,area.top,area.left+area.width,area.top+area.height, color);
        }


        void Desktop::InvertMouseCursor()
        {
            for(int32_t x = mousePositionX-3; x <= (int32_t)mousePositionX+3; ++x)
                context->InvertPixel(x,mousePositionY);
            for(int32_t y = mousePositionY-3; y <= (int32_t)mousePositionY+3; ++y)
                context->InvertPixel(mousePositionX,y);
        }


        void Desktop::OnMouseMoved(int8_t offsetX, int8_t offsetY)
        {
            Rectangle<int> position = GetPosition();
            int32_t newX = mousePositionX + offsetX;
            int32_t newY = mousePositionY + offsetY;
            if(newX < 0) newX = 0;
            else if((uint32_t)newX >= position.width) newX = position.width-1;
            if(newY < 0) newY = 0;
            else if((uint32_t)newY >= position.height) newY = position.height-1;

            // erase the old mouse cursor
            InvertMouseCursor();

            // do the event handling
            if(draggedObject != 0)
                draggedObject->OnMouseMoved(newX-mousePositionX, newY-mousePositionY);
            GraphicalObjectContainer::OnMouseMoved(mousePositionX, mousePositionY, newX, newY);
            mousePositionX = newX;
            mousePositionY = newY;

            // draw the new mouse cursor
            InvertMouseCursor();
        }

        void Desktop::OnMouseButtonPressed(uint8_t button)
        {
            draggedObject = GraphicalObjectContainer::OnMouseButtonPressed(mousePositionX, mousePositionY, button);
        }

        void Desktop::OnMouseButtonReleased(uint8_t button)
        {
            GraphicalObjectContainer::OnMouseButtonReleased(mousePositionX, mousePositionY, button);
            draggedObject = 0;
        }

        void Desktop::OnKeyboardKeyPressed(KeyCode keyCode, KeyboardState state)
        {
            if(focussedObject != 0)
                focussedObject->OnKeyboardKeyPressed(keyCode, state);
        }

        void Desktop::OnKeyboardKeyReleased(KeyCode keyCode, KeyboardState state)
        {
            if(focussedObject != 0)
                focussedObject->OnKeyboardKeyReleased(keyCode, state);
        }

        void Desktop::AddChild(GraphicalObject* child)
        {
            bool newChildIsUnderMouseCursor = child->ContainsCoordinate(mousePositionX, mousePositionY);

            if(newChildIsUnderMouseCursor)
                OnMouseLeave(mousePositionX, mousePositionY);

            GraphicalObjectContainer::AddChild(child);

            if(newChildIsUnderMouseCursor)
                OnMouseEnter(mousePositionX, mousePositionY);
        }

        void Desktop::OnTime(const Time&)
        {
            if(invalidAreas.empty())
                return;

            // erase the mouse cursor
            InvertMouseCursor();

            // do the drawing
            while(!invalidAreas.empty())
            {
                Rectangle<int> first = *(invalidAreas.begin());
                invalidAreas.pop_front();
                Draw(context, first);
            }

            // draw the mouse cursor
            InvertMouseCursor();
        }

        void Desktop::SetFocus(GraphicalObject* focussedObject)
        {
            if(this->focussedObject != 0)
                this->focussedObject->OnFocusLost();
            this->focussedObject = focussedObject;
            if(focussedObject != 0)
                focussedObject->OnFocus();
        }

        void Desktop::BringToFront(GraphicalObject* frontObject)
        {
            Children.erase(frontObject);
            Children.push_front(frontObject);
        }


    }
}