
#ifndef __COMMON__VECTOR_TEMPLATES_CPP
    #define __COMMON__VECTOR_TEMPLATES_CPP

    namespace MYYOS
    {
        namespace common
        {

            template<class T> Vector<T>::Vector()
            {
                MaxSize = 100;
                Size = 0;
            }

            template<class T> Vector<T>::Vector(int Size, T element)
            {
                MaxSize = 100;

                if(Size >= MaxSize)
                    Size = MaxSize;

                this->Size = Size;
                for(iterator i = begin(); i != end(); i++)
                    *i = element;
            }


            template<class T> Vector<T>::~Vector()
            {
            }

            template<class T> T& Vector<T>::operator[](int index)
            {
                //if(index <= Size)
                    return elements[index];
                //else

            }

            template<class T> uint32_t Vector<T>::size()
            {
                return Size;
            }

            template<class T> typename Vector<T>::iterator Vector<T>::begin()
            {
                return &elements[0];
            }

            template<class T> typename Vector<T>::iterator Vector<T>::end()
            {
                return &elements[0] + Size;
            }

            template<class T> typename Vector<T>::iterator Vector<T>::find(T element)
            {
                for(iterator i = begin(); i != end(); ++i)
                    if(*i == element)
                        return i;
                return end();
            }

            template<class T> bool Vector<T>::empty()
            {
                return begin() == end();
            }



            template<class T> typename Vector<T>::iterator Vector<T>::push_back(T element)
            {
                if(Size >= MaxSize)
                    return end();

                elements[Size++] = element;
                return end()-1;
            }

            template<class T> void Vector<T>::pop_back()
            {
                if(Size > 0)
                    --Size;
            }

            template<class T> typename Vector<T>::iterator Vector<T>::push_front(T element)
            {
                if(Size >= MaxSize)
                    return end();

                for(iterator i = end(); i > begin(); --i)
                    *i = *(i-1);
                elements[0] = element;
                ++Size;
                return begin();
            }

            template<class T> void Vector<T>::pop_front()
            {
                if(Size > 0)
                {
                    for(iterator i = begin()+1; i != end(); ++i)
                        *(i-1) = *i;
                    --Size;
                }
            }

            template<class T> void Vector<T>::erase(T element)
            {
                int hits = 0;
                for(iterator i = begin(); i != end(); ++i)
                {
                    if(*i == element)
                        ++hits;
                    else
                        if(hits > 0)
                            *(i-hits) = *i;
                }
                Size -= hits;
            }

            template<class T> void Vector<T>::erase(typename Vector<T>::iterator position)
            {
                // element not in this vector
                if(position < begin() || position >= end())
                    return;

                for(++position; position != end(); ++position)
                    *(position-1) = *position;

                --Size;
            }

            template<class T> void Vector<T>::clear()
            {
                Size = 0;
            }



            template<class T> void Vector<T>::Iterate(VectorIterationHandler<T>* handler)
            {
                for(iterator i = begin(); i != end(); ++i)
                    handler->OnRead(*i);
                handler->OnEndOfStream();
            }

            template<class T> void Vector<T>::Iterate(void callback(T&))
            {
                for(iterator i = begin(); i != end(); ++i)
                    callback(*i);
            }

        }
    }

#endif