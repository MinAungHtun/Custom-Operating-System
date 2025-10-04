

#include<graphicalobjects/window.h>

using namespace MYYOS::common;

namespace MYYOS
{
    namespace graphicalobjects
    {

        Window::Window(int32_t left, int32_t top, uint32_t width, uint32_t height, string text)
            : GraphicalObjectContainer(left, top, width, height),
            graphicalObjectMover(this),
            graphicalObjectResizerTopLeft(this),
            graphicalObjectResizerTop(this),
            graphicalObjectResizerTopRight(this),
            graphicalObjectResizerLeft(this),
            graphicalObjectResizerRight(this),
            graphicalObjectResizerBottomLeft(this),
            graphicalObjectResizerBottom(this),
            graphicalObjectResizerBottomRight(this),
            title(0,-(10+5)+1,width-2*5,10+5-2,text)
        {
            frameWidth = 5;
            titleWidth = 10;
            minimumWidth = 2*frameWidth;
            minimumHeight = 2*frameWidth+titleWidth;

            color = Color(0xff, 0xff, 0xff);
            frameColor = Color(0x57,0xa8,0xff);
            title.font.color = Color(255,255,255);
            title.font.backgroundColor = frameColor;
            frameBorderColor = Color(0x00,0xa8,0xff);
            Window::AddChild(&title);
        }

        Window::Window(GraphicalObject* containedObject, string text)
            : GraphicalObjectContainer(0,0,containedObject->GetPosition().width+2*5+2, containedObject->GetPosition().height+2*5+10+2),
            graphicalObjectMover(this),
            graphicalObjectResizerTopLeft(this),
            graphicalObjectResizerTop(this),
            graphicalObjectResizerTopRight(this),
            graphicalObjectResizerLeft(this),
            graphicalObjectResizerRight(this),
            graphicalObjectResizerBottomLeft(this),
            graphicalObjectResizerBottom(this),
            graphicalObjectResizerBottomRight(this),
            title(0,-(10+5)+1,containedObject->GetPosition().width,10+5-2,text)
        {
            frameWidth = 5;
            titleWidth = 10;
            minimumWidth = 2*frameWidth;
            minimumHeight = 2*frameWidth+titleWidth;

            color = Color(0xff, 0xff, 0xff);
            frameColor = Color(0x57,0xa8,0xff);
            title.font.color = Color(255,255,255);
            title.font.backgroundColor = frameColor;
            frameBorderColor = Color(0x00,0xa8,0xff);
            Window::AddChild(&title);
            Window::AddChild(containedObject);
        }


        Window::~Window()
        {
        }

        void Window::AddChild(GraphicalObject* child)
        {
            if(child != 0)
            {
                Rectangle<int> childPosition = child->GetPosition();
                child->Move(childPosition.left + frameWidth + 1, childPosition.top + frameWidth + titleWidth + 1);
            }
            GraphicalObjectContainer::AddChild(child);
        }

        void Window::DrawSelf(common::GraphicsContext* context, common::Rectangle<int>& area)
        {
            Coordinates absoluteCoordinates = GraphicalObjectContainer::AbsoluteCoordinates(Coordinates(0,0));
            Rectangle<int> position = GetPosition();
            int32_t x = absoluteCoordinates.first;
            int32_t y = absoluteCoordinates.second;

            // draw the children-area
            Rectangle<int> childrenRectangle(frameWidth, frameWidth+titleWidth, position.width-2*frameWidth, position.height-2*frameWidth-titleWidth);
            if(childrenRectangle.Intersects(area))
            {
                Rectangle<int> intersection = childrenRectangle.Intersection(area);
                context->FillRectangle(x+intersection.left, y+intersection.top, x+intersection.left+intersection.width, y+intersection.top+intersection.height, color);
            }


            // draw the window-frame and the window-title
            Rectangle<int> titleRectangle(frameWidth,0, position.width-2*frameWidth, frameWidth+titleWidth);
            if(titleRectangle.Intersects(area))
            {
                Rectangle<int> intersection = titleRectangle.Intersection(area);
                context->FillRectangle(x+intersection.left, y+intersection.top, x+intersection.left+intersection.width, y+intersection.top+intersection.height, frameColor);

                if(area.Intersects(Rectangle<int>(0,0,position.width,1)))
                    context->Line(x+intersection.left, y, x+intersection.left+intersection.width-1, y,frameBorderColor);
            }

            // draw the left part of the frame
            Rectangle<int> leftRectangle(0,0, frameWidth, position.height);
            if(leftRectangle.Intersects(area))
            {
                Rectangle<int> intersection = leftRectangle.Intersection(area);
                context->FillRectangle(x+intersection.left, y+intersection.top, x+intersection.left+intersection.width, y+intersection.top+intersection.height, frameColor);

                if(area.Intersects(Rectangle<int>(0,0,1,position.height)))
                    context->Line(x, y+intersection.top, x, y+intersection.top+intersection.height-1,frameBorderColor);
            }

            // draw the right part of the frame
            Rectangle<int> rightRectangle(position.width-frameWidth, 0, frameWidth, position.height);
            if(rightRectangle.Intersects(area))
            {
                Rectangle<int> intersection = rightRectangle.Intersection(area);
                context->FillRectangle(x+intersection.left, y+intersection.top, x+intersection.left+intersection.width, y+intersection.top+intersection.height, frameColor);

                if(area.Intersects(Rectangle<int>(position.width-1,0,1,position.height)))
                    context->Line(x+position.width-1, y+intersection.top, x+position.width-1, y+intersection.top+intersection.height-1,frameBorderColor);
            }

            // draw the bottom part of the frame
            Rectangle<int> bottomRectangle(frameWidth, position.height-frameWidth, position.width-2*frameWidth, frameWidth);
            if(bottomRectangle.Intersects(area))
            {
                Rectangle<int> intersection = bottomRectangle.Intersection(area);
                context->FillRectangle(x+intersection.left, y+intersection.top, x+intersection.left+intersection.width, y+intersection.top+intersection.height, frameColor);

                if(area.Intersects(Rectangle<int>(0,position.height-1,position.width,1)))
                    context->Line(x+intersection.left, y+position.height-1, x+intersection.left+intersection.width-1, y+position.height-1,frameBorderColor);
            }
        }

        drivers::mouse::MouseEventHandler* Window::OnMouseButtonPressed(uint32_t X, uint32_t Y, uint8_t button)
        {
            drivers::mouse::MouseEventHandler* childrenResult = GraphicalObjectContainer::OnMouseButtonPressed(X,Y,button);
            Rectangle<int> position = GetPosition();

            if(X <= frameWidth)
            {
                if(Y <= frameWidth)
                    return &graphicalObjectResizerTopLeft;
                else if(Y < position.height-frameWidth)
                    return &graphicalObjectResizerLeft;
                else
                    return &graphicalObjectResizerBottomLeft;
            }
            else if(X < position.width-frameWidth)
            {
                if(Y <= frameWidth)
                    return &graphicalObjectResizerTop;
                else if(Y < frameWidth+titleWidth)
                    return &graphicalObjectMover;
                else if(Y >= position.height-frameWidth)
                    return &graphicalObjectResizerBottom;
            }
            else
            {
                if(Y <= frameWidth)
                    return &graphicalObjectResizerTopRight;
                else if(Y < position.height-frameWidth)
                    return &graphicalObjectResizerRight;
                else
                    return &graphicalObjectResizerBottomRight;
            }

            return childrenResult;
        }

    }
}
