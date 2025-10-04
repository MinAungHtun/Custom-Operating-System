

#ifndef __COMMON__RECTANGLE_H
    #define __COMMON__RECTANGLE_H

    #include<common/vector.h>

    namespace MYYOS
    {
        namespace common
        {

            template<class T> class Rectangle
            {
                public:
                    T left;
                    T top;
                    T width;
                    T height;

                    Rectangle();
                    Rectangle(T left, T top, T width, T height);
                    ~Rectangle();

                    bool Intersects(const Rectangle<T>& other);
                    Rectangle<T> Intersection(const Rectangle<T>& other);
                    Vector<Rectangle<T> > Subtract(const Rectangle<T>& other);
                    bool Contains(const Rectangle<T>& other);
                    bool Contains(T x, T y);
            };



        }
    }

    #include "../../src/common/rectangle_templates.cpp"

#endif
