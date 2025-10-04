
#ifndef __COMMON__RECTANGLE_TEMPLATES_CPP
    #define __COMMON__RECTANGLE_TEMPLATES_CPP

    namespace MYYOS
    {
        namespace common
        {

            template<class T> Rectangle<T>::Rectangle()
            {
                left = 0;
                top = 0;
                height = 0;
                width = 0;
            }

            template<class T> Rectangle<T>::Rectangle(T left, T top, T width, T height)
            {
                if(width < 0)
                {
                    left += width;
                    width *= -1;
                }
                if(height < 0)
                {
                    top += height;
                    height *= -1;
                }
                this->left = left;
                this->top = top;
                this->width = width;
                this->height = height;
            }

            template<class T> Rectangle<T>::~Rectangle()
            {
            }

            template<class T> bool Rectangle<T>::Intersects(const Rectangle<T>& other)
            {
                if(    other.top+other.height <= top
                    || other.left+other.width <= left
                    || left+width <= other.left
                    || top+height <= other.top)
                    return false;
                return true;
            }

            template<class T> Rectangle<T> Rectangle<T>::Intersection(const Rectangle<T>& other)
            {
                T maxLeft = left >= other.left ? left : other.left;
                T maxTop = top >= other.top ? top : other.top;
                T minRight = left+width <= other.left+other.width ? left+width : other.left+other.width;
                T minBottom = top+height <= other.top+other.height ? top+height : other.top+other.height;

                return Rectangle<T>(maxLeft, maxTop, minRight-maxLeft, minBottom-maxTop);
            }

            template<class T> Vector<Rectangle<T> > Rectangle<T>::Subtract(const Rectangle<T>& other)
            {
                Vector<Rectangle<T> > result;

                if(Intersects(other))
                {
                    T maxLeft = left > other.left ? left : other.left;
                    T minRight = (left+width < other.left+other.width ? left+width : other.left+other.width);
                    T minBottom = (top+height < other.top+other.height ? top+height : other.top+other.height);

                    if(top < other.top) // block above "other"
                        result.push_back(Rectangle<T>(maxLeft, top, minRight-maxLeft, other.top-top));
                    if(left < other.left) // block on the left of "other"
                        result.push_back(Rectangle<T>(left, top, other.left-left,height));
                    if(other.left+other.width < left+width) // block on the right of "other"
                        result.push_back(Rectangle<T>(other.left+other.width, top, (left+width) - (other.left+other.width) + 1,height));
                    if(other.top+other.height < top+height) // block below "other"
                        result.push_back(Rectangle<T>(maxLeft, minBottom, minRight-maxLeft, top+height-minBottom + 1));
                }
                else
                {
                    result.push_back(*this);
                }

                return result;
            }

            template<class T> bool Rectangle<T>::Contains(const Rectangle<T>& other)
            {
                return left <= other.left
                    && other.left+other.width <= left+width
                    && top <= other.top
                    && other.top+other.height <= top+height;
            }

            template<class T> bool Rectangle<T>::Contains(T x, T y)
            {
                return left <= x && x < left + width
                    &&  top <= y && y < top + height;
            }

        }
    }

#endif
