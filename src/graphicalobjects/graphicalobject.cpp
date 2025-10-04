
#include<graphicalobjects/graphicalobject.h>

using namespace MYYOS::common;
using namespace MYYOS::drivers::keyboard;

namespace MYYOS
{
    namespace graphicalobjects
    {

        GraphicalObject::GraphicalObject()
            : KeyboardEventHandler()
        {
            minimumWidth = 5;
            minimumHeight = 5;
            maximumWidth = 0x8FFFFFFF;
            maximumHeight = 0x8FFFFFFF;
            parent = 0;
        }

        GraphicalObject::GraphicalObject(int32_t left, int32_t top, uint32_t width, uint32_t height)
            : KeyboardEventHandler(),
            position(left,top,width,height)
        {
            minimumWidth = 5;
            minimumHeight = 5;
            maximumWidth = 0x8FFFFFFF;
            maximumHeight = 0x8FFFFFFF;
            parent = 0;
        }


        GraphicalObject::~GraphicalObject()
        {
        }

        Coordinates GraphicalObject::AbsoluteCoordinates(Coordinates coordinates)
        {
            if(parent == 0)
                return Coordinates(coordinates.first+position.left, coordinates.second+position.top);
            return parent->AbsoluteCoordinates(Coordinates(coordinates.first+position.left,coordinates.second+position.top));
        }

        void GraphicalObject::Draw(common::GraphicsContext*, common::Rectangle<int>&)
        {
        }

        void GraphicalObject::AddChild(GraphicalObject* child)
        {
            child->parent = this;
        }


        bool GraphicalObject::ContainsCoordinate(uint32_t x, uint32_t y)
        {
            return position.Contains(x, y);
        }
        common::Rectangle<int> GraphicalObject::GetPosition()
        {
            return position;
        }
        void GraphicalObject::Move(int left, int top)
        {
            Invalidate();
            position.left = left;
            position.top = top;
            Invalidate();
        }
        void GraphicalObject::Resize(int width, int height)
        {
            if(width < minimumWidth) width = minimumWidth;
            if(width > maximumWidth) width = maximumWidth;
            if(height < minimumHeight) height = minimumHeight;
            if(height > maximumHeight) height = maximumHeight;

            Rectangle<int> oldPosition = position;
            position.width = width;
            position.height = height;

            Vector<Rectangle<int> > invalid1 = oldPosition.Subtract(position);
            Vector<Rectangle<int> > invalid2 = position.Subtract(oldPosition);
            for(Vector<Rectangle<int> >::iterator i = invalid1.begin(); i != invalid1.end(); i++)
                Invalidate(*i);
            for(Vector<Rectangle<int> >::iterator i = invalid2.begin(); i != invalid2.end(); i++)
                Invalidate(*i);
        }
        void GraphicalObject::Invalidate()
        {
            Coordinates absoluteCoordinates = AbsoluteCoordinates(Coordinates(0, 0));
            Rectangle<int> invalidArea(absoluteCoordinates.first, absoluteCoordinates.second, position.width, position.height);
            Invalidate(invalidArea);
        }
        void GraphicalObject::Invalidate(Rectangle<int>& area)
        {
            if(parent != 0)
                parent->Invalidate(area);
        }



        void GraphicalObject::OnMouseEnter(uint32_t, uint32_t) { }
        void GraphicalObject::OnMouseLeave(uint32_t, uint32_t) { }
        void GraphicalObject::OnMouseMoved(uint32_t, uint32_t, uint32_t, uint32_t) { }
        void GraphicalObject::OnFocus() { }
        void GraphicalObject::OnFocusLost() { }
        drivers::mouse::MouseEventHandler* GraphicalObject::OnMouseButtonPressed(uint32_t, uint32_t, uint8_t)
        {
            BringToFront();
            Focus();
            return 0;
        }
        void GraphicalObject::OnMouseButtonReleased(uint32_t, uint32_t, uint8_t) { }

        void GraphicalObject::Focus()
        {
            SetFocus(this);
        }

        void GraphicalObject::SetFocus(GraphicalObject* focussedObject)
        {
            if(parent != 0)
                parent->SetFocus(focussedObject);
        }

        void GraphicalObject::BringToFront()
        {
            BringToFront(this);
        }

        void GraphicalObject::BringToFront(GraphicalObject*)
        {
            if(parent != 0)
                parent->BringToFront(this);
        }




        GraphicalObjectContainer::GraphicalObjectContainer()
            : GraphicalObject()
        {
        }

        GraphicalObjectContainer::GraphicalObjectContainer(int32_t left, int32_t top, uint32_t width, uint32_t height)
            : GraphicalObject(left, top, width, height)
        {
        }

        GraphicalObjectContainer::~GraphicalObjectContainer()
        {
        }


        void GraphicalObjectContainer::Draw(GraphicsContext* context, Rectangle<int>& area)
        {
            Draw(context, area, Children.begin());
        }
        void GraphicalObjectContainer::Draw(GraphicsContext* context, Rectangle<int>& area, Vector<GraphicalObject*>::iterator start)
        {
            GraphicalObject::Draw(context, area);
            Rectangle<int> ownArea = GetPosition();

            for(Vector<GraphicalObject*>::iterator i = start; i != Children.end(); ++i)
            {
                Rectangle<int> childPosition = (*i)->GetPosition();
                if(area.Intersects(childPosition))
                {
                    // draw the child
                    Rectangle<int> localArea = area.Intersection(childPosition);
                    localArea.left -= childPosition.left;
                    localArea.top -= childPosition.top;
                    (*i)->Draw(context, localArea);

                    // continue drawing your own parts
                    Vector<Rectangle<int> > rest = area.Subtract(childPosition);
                    for(Vector<Rectangle<int> >::iterator j = rest.begin(); j != rest.end(); j++)
                        Draw(context, *j, i+1);

                    return;
                }
            }

            DrawSelf(context, area);
        }
        void GraphicalObjectContainer::DrawSelf(GraphicsContext*, Rectangle<int>&)
        {
        }

        void GraphicalObjectContainer::AddChild(GraphicalObject* child)
        {
            Children.push_front(child);
            GraphicalObject::AddChild(child);
        }




        void GraphicalObjectContainer::OnMouseMoved(uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY)
        {
            GraphicalObject* enterObject = 0;
            GraphicalObject* leaveObject = 0;

            for(common::Vector<GraphicalObject*>::iterator i = Children.begin(); i != Children.end(); ++i)
            {
                bool containsFrom = (*i)->ContainsCoordinate(fromX,fromY);
                bool containsTo = (*i)->ContainsCoordinate(toX,toY);
                if(containsFrom)
                {
                    if(containsTo)
                    {
                        (*i)->OnMouseMoved(fromX, fromY, toX, toY);
                        return;
                    }
                    else
                        leaveObject = *i;
                }
                else
                    if(containsTo)
                        enterObject = *i;
            }

            if(leaveObject != 0)
                leaveObject->OnMouseLeave(fromX, fromY);
            if(enterObject != 0)
                enterObject->OnMouseEnter(toX,toY);
        }

        drivers::mouse::MouseEventHandler* GraphicalObjectContainer::OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button)
        {
            drivers::mouse::MouseEventHandler* result = 0;
            for(common::Vector<GraphicalObject*>::iterator i = Children.begin(); i != Children.end(); ++i)
                if((*i)->ContainsCoordinate(X,Y))
                {
                    result = (*i)->OnMouseButtonPressed(X-(*i)->position.left, Y-(*i)->position.top, button);
                    break;
                }
            return result;
        }

        void GraphicalObjectContainer::OnMouseButtonReleased(uint32_t X, uint32_t Y, uint8_t button)
        {
            for(common::Vector<GraphicalObject*>::iterator i = Children.begin(); i != Children.end(); ++i)
                if((*i)->ContainsCoordinate(X,Y))
                {
                    (*i)->OnMouseButtonReleased(X-(*i)->position.left, Y-(*i)->position.top, button);
                    break;
                }
        }

        void GraphicalObjectContainer::OnMouseEnter(uint32_t X, uint32_t Y)
        {
            for(common::Vector<GraphicalObject*>::iterator i = Children.begin(); i != Children.end(); ++i)
                if((*i)->ContainsCoordinate(X,Y))
                {
                    (*i)->OnMouseEnter(X-(*i)->position.left, Y-(*i)->position.top);
                    break;
                }
        }

        void GraphicalObjectContainer::OnMouseLeave(uint32_t X, uint32_t Y)
        {
            for(common::Vector<GraphicalObject*>::iterator i = Children.begin(); i != Children.end(); ++i)
                if((*i)->ContainsCoordinate(X,Y))
                {
                    (*i)->OnMouseLeave(X-(*i)->position.left, Y-(*i)->position.top);
                    break;
                }
        }


    }
}